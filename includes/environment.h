#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "iimp.h"

struct sEnvironment {
    char *identifier;
    int value;
    struct sEnvironment *next;
};
typedef struct sEnvironment* Environment;

extern Environment envirAlloc();

extern int initEnvironment(Environment environment, char* identifier);

extern Environment find(char *identifier, Environment environment);

extern int affect(Environment environment, char *identifier, int value);

extern int printEnvironment(Environment environment);

extern int getValue(Environment environment, char *identifier);

extern int interprete(Environment environment, nodeType *lexeme);

extern int execute(Environment environment, nodeType *lexeme);

#endif
