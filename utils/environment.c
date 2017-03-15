#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"


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
    while(tmp != NULL && tmp->identifier != NULL){
        cpt++;
        printf("%d) Variable: %s || Valeur: %d \n", cpt, tmp->identifier, tmp->value);
        tmp = tmp->next;
    }
    printf("End of the environment !\n");
    return EXIT_SUCCESS;
}

int getValue(Environment environment, char *identifier) {
    Environment currentEnv;
    if ((currentEnv = find(identifier, environment)) != NULL) { // Identifier's value is found
        return currentEnv->value;
    }
    return 0;
}
