#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/* ----------------------------types--------------------------------------------*/
#include "iimp.h"

struct sEnvironment {
    char *identifier;
    int value;
    struct sEnvironment *next;
};
typedef struct sEnvironment* Environment;

/* ------------------CONSTANTES -------------------------------------------------*/
#define MAXIDENT 16          /* long max d'un identificateur de variable         */
#define MAXQUAD  5*MAXIDENT  /* long max d'un quadruplet c3a                     */
#define PL 257
#define MO 258
#define MU 259
#define AF 260
#define AFc 261
#define SK 262
#define JP 263
#define JZ 264
#define ST 265

/*------------------FONCTIONS ---------------------------------------------------*/
extern char *indentAlloc();

extern Environment envirAlloc();

extern int initEnvironment(Environment *environment, char* identifier);

extern Environment find(char *identifier, Environment environment);

extern int affect(Environment environment, char *identifier, int value);

extern int printEnvironment(Environment environment);

extern int getValue(Environment environment, char *identifier);

extern int evaluate(int oper, int arg1, int arg2);

extern int interprete(Environment *environment, nodeType *lexeme);

extern int execute(Environment *environment, nodeType *lexeme);

#endif
