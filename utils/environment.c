#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
#include "environment.h"
#include "mini-projet"


#define LENGTH 20

Environment envirAlloc() {
    return (Environment)malloc(sizeof(struct sEnvironment));
}

int initEnvironment(Environment environment, char* identifier) {
	 Environment headEnv;
    if(find(identifier, environment) == NULL){
        if((headEnv = envirAlloc()) == NULL)
            perror("ERROR: Out of memory !\n");

        if((headEnv->identifier = (char*)malloc(LENGTH * sizeof(char))) == NULL)
            perror("ERROR: Out of memory !\n");

        strcpy(headEnv->identifier, identifier);
        headEnv->value = 0;
        headEnv->next = environment;
        environment->identifier = strdup(identifier);
        environment->value = headEnv->value;
        environment->next = NULL;
        printf("\ninitEnvironment: identifier = %s\n", environment->identifier);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
     /*Environment headEnv;
     if(environment->identifier == NULL){
     printf("\ninitEnvironment: identifier = %s\n", environment->identifier);
         if((headEnv = envirAlloc()) == NULL)
             perror("ERROR: Out of memory !\n");

         headEnv->identifier = strdup(identifier);
         headEnv->value = 2017;
         headEnv->next = environment;
         environment = headEnv;
         printf("\ninitEnvironment: identifier = %s\n", environment->identifier);
         return EXIT_SUCCESS;
     }
     printf("\ninitEnvironment: identifier = %s\n", identifier);
    if(find(identifier, environment) == NULL){
        if((headEnv = envirAlloc()) == NULL)
            perror("ERROR: Out of memory !\n");

        if((headEnv->identifier = (char*)malloc(LENGTH * sizeof(char))) == NULL)
            perror("ERROR: Out of memory !\n");

        strcpy(headEnv->identifier, identifier);
        headEnv->value = 191;
        headEnv->next = environment;
        environment = headEnv->next;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;*/
}

Environment find(char *identifier, Environment environment) { // ERREUR: environment->identifier = (null)
    if(environment != NULL){
      if(environment->identifier != NULL){
      printf("\nfind: identifier = %s\n", identifier);
        if (strcmp(environment->identifier, identifier) == 0) {
            return environment;
        }
        else{
            return find(identifier, environment->next);
        }
      }
    }
    return NULL;
    /*if(environment != NULL){
       if(environment->identifier != NULL){
        if (strcmp(environment->identifier, identifier) == 0) {
            return environment;
        }
        else{
            return find(identifier, environment->next);
        }
      }
         printf("\nfind : Je suis là %s!\n\n", environment->identifier);
    }
    return NULL;*/
}

int affect(Environment environment, char *identifier, int value) {
    Environment currentEnv;

      printf("\nAffect: %s = %d\n", identifier, value );
    if((currentEnv = find(identifier, environment)) != NULL){
        currentEnv->value = value;
        environment = currentEnv;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int printEnvironment(Environment environment) {
    Environment tmp = environment; // Est-ce ça marche !?
    while(tmp != NULL){
        printf("Variable: %s || Valeur: %d \n", tmp->identifier, tmp->value);
        tmp = tmp->next;
    }
    printf("End of the environment !\n");
    return EXIT_SUCCESS;
}

int getValue(Environment environment, char *identifier) {
    Environment currentEnv;
      printf("\ngetValue: %s\n", environment->identifier);
    if ((currentEnv = find(identifier, environment)) != NULL) { // Identifier's value is found
        return currentEnv->value;
    }
    return 0;
}
