%error-verbose
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "iimp.h"
#include "environment.h"

nodeType *constant(int value);
nodeType *identifier(char* val);
nodeType *operation(int lexeme, int nbOper, ...);

int yylex();
int yyerror(const char *s);
extern int executeCOM(Environment *environment, nodeType *lexeme);
%}

%union{
    int ival;
    char* ident;
    struct nodeTypeTag *nTPtr;
};


%token Af Sk Se If Th El Wh Do Pl Mo Mu /* liste des terminaux */
%token<ival> I
%token<ident> V
%nonassoc Th
%nonassoc El
%left Pl Mo
%left Mu
%left '('
%type<nTPtr> C0 C E T F

%%

/*
E = Expressoin
T = Terme
F = Facteur
*/

// Faire un test pour exécuter soit l'interpréteur soit le compilateur de IMP
program : C { Environment environment = (Environment)malloc(sizeof(struct sEnvironment)); executeCOM(&environment, $1); };

C0	: V Af E			{$$ = operation(Af, 2, identifier($1), $3); }
 	| Sk				{ $$ = operation(Sk, 2, NULL, NULL); }
 	| '(' C ')'			{ $$ = $2; }
 	| If E Th C El C0	{ $$ = operation(If, 3, $2, $4, $6); }
 	| Wh E Do C0			{ $$ = operation(Wh, 2, $2, $4); };



C   : C0                { $$ = $1; }
    | C Se C0           { $$ = operation(Se, 2, $1, $3); };


E	: E Pl T { $$ = operation(Pl, 2, $1, $3); } /* Une expression peut-être sous la forme d'une somme d'une expression à un terme */
 	| E Mo T { $$ = operation(Mo, 2, $1, $3); } /* Une expression peut-être sous la forme d'une différence d'une expression d'un terme */
 	| T 				{ $$ = $1; }; /* Une expressoin peut être simplement un terme */

T	: T Mu F 			{ $$ = operation(Mu, 2, $1, $3); } /* Un terme peut être un produit ou un quotien d'un terme et un facteur */
 	| F 				{ $$ = $1; }; /* Un terme peut être tout simplement un facteur */

F	: '(' E ')' 		{ $$ = $2; } /* Un facteur peu être une expression mise entre parenthèse (i.e (E) ) */
 	| I 				{ $$ = constant($1); } /* Un facteur peut être une suite de chiffres, non vide, commençant par un chiffre non-nul */
 	| V 				{ $$ = identifier($1); }; /* Un facteur peut être une variable */

%%

nodeType *constant(int val){
    nodeType *term;

    /* Allocate node */
    if((term = malloc(sizeof(nodeType))) == NULL)
        yyerror("ERROR: Out of memory !\n");

    /* Copy information */
    term->type = typeCons;
    term->cons.value = val;

    return term;
}

nodeType *identifier(char* val){
    nodeType *term;

    /* Allocate node */
    if((term = malloc(sizeof(nodeType))) == NULL)
        yyerror("ERROR: Out of memory !\n");

    /* Copy information */
    term->type = typeIdent;
    term->ident.value = val;

    return term;
}

nodeType *operation(int lexeme, int nbOper, ...){
    va_list args;
    nodeType *term;

    /* Allocate node, extending operN array */
    if((term = malloc(sizeof(nodeType))) == NULL)
        yyerror("ERROR: Out of memory !\n");
    if((term->oper.operN = malloc((nbOper) * sizeof(nodeType *))) == NULL)
        yyerror("ERROR: Out of memory !\n");


    /* Copy information */
    term->type = typeOper;
    term->oper.operT = lexeme;
    term->oper.nbOperN = nbOper; // Number of operands
    va_start(args, nbOper); // Initialization from the last defined and parameter known
    for(int i = 0; i < nbOper; i++)
        term->oper.operN[i] = va_arg(args, nodeType*); //The next nodeType* parameter is accessed
    va_end(args); // The end

    return term;
}

int yyerror(const char *s){
  fprintf( stderr, "%s\n", s );
  return 0;
}


int main(int argn, char **argv){
  yyparse(); //Il lance l'analyseur syntaxique qui lui appelle le yylex()
  return 0;
}
