%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(char *s);
%} 

%token NUMBER PLUS MOINS FOIS DIVIS OPEN_PAR CLOSE_PAR /* liste des terminaux */

%%
S: E {printf("Le résultat de l'opération est: %d\n", $1);};

/*
E = Expressoin
T = Terme
F = Facteur
*/
E	: E Pl T 			{$$ = $1 + $3;} /* Une expression peut-être sous la forme d'une somme d'une expression à un terme */
 	| E Mo T 			{$$ = $1 - $3;} /* Une expression peut-être sous la forme d'une différence d'une expression d'un terme */
 	| T 					{$$ = $1;}; /* Une expressoin peut être simplement un terme */
 
T	: T Mu F 			{$$ = $1 * $3;} /* Un terme peut être un produit ou un quotien d'un terme et un facteur */
 	| F 					{$$ = $1;}; /* Un terme peut être tout simplement un facteur */
 
F	: '(' E ')' 		{$$ = $2;} /* Un facteur peu être une expression mise entre parenthèse (i.e (E) ) */
 	| I 					{$$ = $1;} /* Un facteur peut être une suite de chiffres, non vide, commençant par un chiffre non-nul */
 	| V 					{$$ = $1;}; /* Un facteur peut être une variable */
 
C	: V af E				{ajoute($1,$3->valeur);}
 	| Sk					{}
 	| '(' C ')'			{}
 	| If E Th C El C	{}	
 	| Wh E Do C			{}
 	| C Se C				{};
%%

int yyerror(char *s){
  fprintf( stderr, "*** ERROR: %s\n", s );
  return 0;
}

int main(int argn, char **argv){
  yyparse(); //Il lance l'analyseur syntaxique qui lui appelle le yylex()
  return 0;
}
