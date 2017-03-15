%s ETIQUETTE OPERATOR ARGUMENT1 ARGUMENT2 DESTINATION
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "bilquad.h"
	#include "environment.h"

	BILQUAD bilquad;
	char *etiq, *arg1, *arg2, *dest;
	int oper = 0;

	char *strdup(const char *s);
	int fileno(FILE *stream);
%}

%%
<ETIQUETTE>([a-zA-Z][A-Za-z0-9]*)*			{
												BEGIN OPERATOR;
												etiq = strdup(yytext);
											}

<OPERATOR>:(Pl|Mo|Mu|Af|Afc|Sk|Jp|Jz|St)	{
												BEGIN ARGUMENT1;
												for(int op = PL; op <= ST; op++){
													if(strcmp(nomop(op), yytext+1) == 0)
														oper = op;
												}
											}

<ARGUMENT1>:([A-Za-z][A-Za-z0-9]*|[0-9]+)*	{
												BEGIN ARGUMENT2;
												arg1 = strdup(yytext+1);
											}

<ARGUMENT2>:([A-Za-z][A-Za-z0-9]*)*			{
												BEGIN DESTINATION;
												arg2 = strdup(yytext+1);
											}

<DESTINATION>:([A-Za-z][A-Za-z0-9]*)* 		{
												dest = strdup(yytext+1);
											}



<DESTINATION>[ \t\n]*\n						{
												/* Creating a bilquad if it does not exist yet, otherwise concatenating a new one */
												BEGIN ETIQUETTE;
												QUAD Quad = creer_quad(etiq, oper, arg1, arg2, dest);
												BILQUAD tmp = creer_bilquad(Quad);
												//bilquad = (bilquad.first == NULL) ? creer_bilquad(Quad) : concatq(bilquad, creer_bilquad(Quad));
												bilquad = concatq(bilquad, tmp);
											}
[ \t\n]+ ; /* ignore whitespace */

. {printf("Expression error !\n"); return EXIT_FAILURE;}

%%

void interpretC3A(Environment *environment, QUAD cursor){
	while(cursor != NULL){
		switch (cursor->OPER) {
			// Note: Dans chaque bien vérifier si les arguments ou les champs ne sont pas vides si l'opérateur en question les utilise
			case PL:	// Go to the next case
			case MO:	// Go to the next case
			case MU: 	initEnvironment(environment, cursor->DEST);
						affect(*environment, cursor->DEST, evaluate(cursor->OPER, getValue(*environment, cursor->ARG1), getValue(*environment, cursor->ARG2)));
						break;

			case AF:	initEnvironment(environment, cursor->ARG1);
						affect(*environment, cursor->ARG1, getValue(*environment, cursor->ARG2));
						break;

			case AFc:	initEnvironment(environment, cursor->DEST);
						affect(*environment, cursor->DEST, atoi(cursor->ARG1));
						break;

			case SK:	// Nothing to do, go to he next instruction
						break;

			case JP:	cursor = rechbq(cursor->DEST, bilquad);
						break;

			case JZ:	cursor = (getValue(*environment, cursor->ARG1) == 0) ? rechbq(cursor->DEST, bilquad) : cursor->NEXT;
						break;

			case ST:	return;

			default: break;
		}

		cursor = cursor->NEXT;
	}
}

int main(int argc, char const *argv[]) {
	BEGIN(ETIQUETTE);
	yylex();
	Environment environment = envirAlloc();
	interpretC3A(&environment, bilquad.first);
	printEnvironment(environment);

	return EXIT_SUCCESS;
}
