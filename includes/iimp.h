#ifndef MINI_H
#define MINI_H

typedef enum {typeCons, typeIdent, typeOper} nodeEnum;

// Constants
typedef struct {
	int value;
} consNodeType;

// Variables
typedef struct {
	char* value;
} identNodeType;

// Opération
typedef struct {
	int operT; // Opérateur
	int nbOperN; // Nombre d'opérandes
	struct nodeTypeTag **operN; // Tableau de symboles (opérateurs, variables, constntes, ...)
} operNodeType;

typedef struct nodeTypeTag {
	nodeEnum type;

	union {
		consNodeType cons;
		identNodeType ident;
		operNodeType oper;
	};
} nodeType;

#endif
