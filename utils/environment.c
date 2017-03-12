#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

#define LENGTH 20



Environment initEnvironment(Environment environment, char* identifier) {
    if(find(identifier, environment) == NULL){
        if((headEnv = (Environment)malloc(sizeof(struct sEnvironment))) == NULL)
            yyerror("ERROR: Out of memory !\n");

        if((headEnv->identifier = (char*)malloc(LENGTH * sizeof(struct char))) == NULL)
            yyerror("ERROR: Out of memory !\n");

        strcpy(headEnv->identifier, identifier);
        headEnv->value = 0;
        headEnv->next = environment;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

Environment find(char *identifier, Environment environment) {
    if(environment != NULL){
        if (strcmp(environment->identifier, identifier) == 0) {
            return environment;
        }
        else{
            return find(identifier, environment->next);
        }
    }
    return NULL;
}

int affect(Environment environment, char *identifier, int value) {
    Environment currentEnv;
    if((currentEnv = find(identifier, environment)) != NULL){
        currentEnv->value = value;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int printEnvironment(Environment environment) {
    Environment tmp = environment; // Est-ce ça marche !?
    while(tmp != NULL){
        printf("Variable: %s || Valeur: %d \n", tmp->identifier, tmp->value);
        tmp = tmp->next
    }
    printf("End of the environment !\n");
    return EXIT_SUCCESS;
}

int getValue(Environment environment, char *identifier) {
    Environment currentEnv;
    if ((currentEnv = find(identifier, environment)) != NULL) {
        return currentEnv->value;
    }
    return 0;
}
