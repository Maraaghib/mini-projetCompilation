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
#define Pl 257
#define Mo 258
#define Mu 259
#define Af 260
#define Afc 261
#define Sk 262
#define Jp 263
#define Jz 264
#define St 265

/*------------------FONCTIONS ---------------------------------------------------*/
extern Environment envirAlloc();

extern int initEnvironment(Environment *environment, char* identifier);

extern Environment find(char *identifier, Environment environment);

extern int affect(Environment environment, char *identifier, int value);

extern int printEnvironment(Environment environment);

extern int getValue(Environment environment, char *identifier);

extern int interprete(Environment *environment, nodeType *lexeme);

extern int execute(Environment *environment, nodeType *lexeme);

#endif
