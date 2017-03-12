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
    return ;
}

int printEnvironment(Environment environment) {
    return ;
}

int getValue(Environment environment, char *identifier) {
    return ;
}
