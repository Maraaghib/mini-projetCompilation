#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
#include "iimp.h"
#include "environment.h"

char *strdup(const char *);


#define LENGTH 20

Environment envirAlloc() {
    return (Environment)malloc(sizeof(struct sEnvironment));
}

int initEnvironment(Environment *environment, char* identifier) {
	 Environment headEnv;
    if(find(identifier, *environment) == NULL){
        if((headEnv = envirAlloc()) == NULL)
            perror("ERROR: Out of memory !\n");

        headEnv->identifier = strdup(identifier);
        headEnv->value = 0;
        headEnv->next = *environment;
        *environment = headEnv;
        /*environment->identifier = strdup(identifier);
        environment->value = headEnv->value;
        environment->next = NULL;*/
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;

}

Environment find(char *identifier, Environment environment) {
    if(environment != NULL){
      if(environment->identifier != NULL){
        if (strcmp(environment->identifier, identifier) == 0) {
            return environment;
        }
        else{
            return find(identifier, environment->next);
        }
      }
    }
    return NULL;
}

int affect(Environment environment, char *identifier, int value) {
    Environment currentEnv;

    if((currentEnv = find(identifier, environment)) != NULL){
        currentEnv->value = value;
        environment = currentEnv;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int printEnvironment(Environment environment) {
    Environment tmp = environment; // Est-ce ça marche !?
    int cpt = 0;
    printf("\n-----------------------------------------\n");
    printf("Number  | Identifier	|===>| Value	|\n");
    printf("-----------------------------------------\n");
    while(tmp != NULL && tmp->identifier != NULL){ // tmp->identifier = null
        cpt++;
        if(cpt<10){
        	printf("(0%d)	| %s	  	|===>| %d	|\n", cpt, tmp->identifier, tmp->value);
    		printf("-----------------------------------------\n");
    	}
        else{
        	printf("(%d) 	| %s	 	|===>| %d	|\n", cpt, tmp->identifier, tmp->value);
		    printf("-----------------------------------------\n");
		}
        tmp = tmp->next;
    }
    printf("\nEnd of the environment !\n\n");
    return EXIT_SUCCESS;
}

int getValue(Environment environment, char *identifier) {
    Environment currentEnv;
    if ((currentEnv = find(identifier, environment)) != NULL) { // Identifier's value is found
        return currentEnv->value;
    }
    return 0;
}

int interprete(Environment *environment, nodeType *lexeme) {
    if(!lexeme) return 0;
    switch (lexeme->type) {
        case typeCons:  return lexeme->cons.value;
        case typeIdent: return getValue(*environment, lexeme->ident.value);
        case typeOper:
            switch (lexeme->oper.operT) {
                case Wh:    while (interprete(environment, lexeme->oper.operN[0])) {
                                interprete(environment, lexeme->oper.operN[1]);
                            }
                            return 0;
                case If:    if (interprete(environment, lexeme->oper.operN[0])) {
                                interprete(environment, lexeme->oper.operN[1]);
                            }
                            else{
                                interprete(environment, lexeme->oper.operN[2]);
                            }
                            return 0;
                case Af:    initEnvironment(environment, lexeme->oper.operN[0]->ident.value); return affect(*environment, lexeme->oper.operN[0]->ident.value, interprete(environment, lexeme->oper.operN[1]));
                case Se:    interprete(environment, lexeme->oper.operN[0]); return interprete(environment, lexeme->oper.operN[1]);

                case Pl:    return interprete(environment, lexeme->oper.operN[0]) + interprete(environment, lexeme->oper.operN[1]);
                case Mo:    return interprete(environment, lexeme->oper.operN[0]) - interprete(environment, lexeme->oper.operN[1]);
                case Mu:    return interprete(environment, lexeme->oper.operN[0]) * interprete(environment, lexeme->oper.operN[1]);
            }
    }
    return 0;
}

int executeINT(Environment *environment, nodeType *lexeme) {
    int result = interprete(environment, lexeme);
    printEnvironment(*environment);
    return result;
}
