%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
int yyerror(char *s);
%}

%token I V Af Sk Se If Th El Wh Do Pl Mo Mu /* liste des terminaux */
%nonassoc Th
%nonassoc El

%%

/*
E = Expressoin
T = Terme
F = Facteur
*/

E	: E Pl T 			{ $$ = $1 + $3; } /* Une expression peut-être sous la forme d'une somme d'une expression à un terme */
 	| E Mo T 			{ $$ = $1 - $3; } /* Une expression peut-être sous la forme d'une différence d'une expression d'un terme */
 	| T 				{ $$ = $1; }; /* Une expressoin peut être simplement un terme */

T	: T Mu F 			{ $$ = $1 * $3; } /* Un terme peut être un produit ou un quotien d'un terme et un facteur */
 	| F 				{ $$ = $1; }; /* Un terme peut être tout simplement un facteur */

F	: '(' E ')' 		{ $$ = $2; } /* Un facteur peu être une expression mise entre parenthèse (i.e (E) ) */
 	| I 				{ $$ = $1; } /* Un facteur peut être une suite de chiffres, non vide, commençant par un chiffre non-nul */
 	| V 				{ $$ = $1; }; /* Un facteur peut être une variable */

C0	: V Af E			{ $$ = operation(Af, 2, identifier($1), $3); }
 	| Sk				{ $$ = operation(Sk, 2, NULL, NULL); }
 	| '(' C ')'			{ $$ = $2; }
 	| If E Th C El C	{ $$ = operation(If, 3, $2, $4, $6); }
 	| Wh E Do C			{ $$ = operation(Wh, 2, $2, $4); }

C   : C Se C0           { $$ = operation(Se, 2, $1, $3); }
C   | C0                { $$ = $1; };

%%

nodeType *contant(int value){
    nodeType *term;

    /* Allocate node */
    if((term = malloc(sizeof(nodeType))) == NULL)
        yyerror("Out of memory !\n");

    /* Copy information */
    term->type = typeCons;
    term->cons.value = val;

    return term;
}

nodeType *identifier(int val){
    nodeType *term;

    /* Allocate node */
    if((term = malloc(sizeof(nodeType))) == NULL)
        yyerror("Out of memory !\n");

    /* Copy information */
    term->type = typeIdent;
    term->ident.value = val;

    return term;
}

nodeType *operation(int lexeme, int nbOper, ...){
    va_list args;
    nodeType *term;

    /* Allocate node, extending operN array */
    if((term = malloc(sizeof(nodeType)) + (nbOper-1) * sizeof(nodeType *)) == NULL)
        yyerror("Out of memory !\n");

    /* Copy information */
    term->type = typeOper;
    term->oper.operT = lexeme;
    term->oper.nbOperN = nbOper;
    va_start(args, nbOper); // Initialization from the last defined and parameter known
    for(int i = 0; i < nbOper; i++)
        term->oper.operN[i] = va_arg(args, nodeType*) //The next nodeType* parameter is accessed
    va_end(args); // The end

    return term;
}

int yyerror(char *s){
  fprintf( stderr, "*** ERROR: %s\n", s );
  return 0;
}

int main(int argn, char **argv){
  yyparse(); //Il lance l'analyseur syntaxique qui lui appelle le yylex()
  return 0;
}
