#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

char *strdup(const char *s);

#define LENGTH 20

char *indentAlloc(){
	return((char *)malloc(MAXIDENT*sizeof(char)));
}

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

int evaluate(int oper, int arg1, int arg2){
	switch(oper) {
		case PL:
			return(arg1 + arg2);
		case MO:
		  	return(arg1 - arg2);
		case MU:
		  	return(arg1 * arg2);
		default:
      		return(0);
    }
  return(0);
}









