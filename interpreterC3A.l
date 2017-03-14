/* https://services.emi.u-bordeaux.fr/projet/git/pcla/blob/HEAD:/src/c3a.l */

%s ETIQ OPER ARG1 ARG2 DEST
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "bilquad.h"
	#include "environment.h"

	BILQUAD bilquad = bilquad_vide();
	char* etiq, arg1, arg2, dest
	int oper = 0;
%}

%%
<ETIQ>:[A-Za-z][A-Za-z0-9]* 			{
											BEGIN OPER;
											etiq = strdup(yytext+1);
										}

<OPER>:(Pl|Mo|Mu|Af|Afc|Sk|Jp|Jz|St)	{
											BEGIN ARG1;
											for(int op = 257; op < 266; op++){
												if(strcmp(nomop(op), yytext+1) == 0)
													oper = op;
											}
										}

<ARG1>:([A-Za-z][A-Za-z0-9]*|[0-9]+)	{
											BEGIN ARG2;
											arg1 = strdup(yytext+1);
										}

<ARG2>:[A-Za-z][A-Za-z0-9]*				{
											BEGIN DEST;
											arg2 = strdup(yytext+1);
										}

<DEST>[A-Za-z][A-Za-z0-9]* 				{
											BEGIN ETIQ;
											dest = strdup(yytext);
											/* Creating a bilquad if it does not exist yet, otherwise concatenating a new one */
											bilquad = (bilquad.first == NULL) ? creer_bilquad(creer_quad(etiq, oper, arg1, arg2, dest)) : concatq(bilquad, creer_bilquad(creer_quad(etiq, oper, arg1, arg2, dest)));
										}

.|\n 		{}
%%

void interpretC3A(Environment environment, QUAD cursor){
	while(cursor != NULL){
		switch (cursor->OPER) {
			case Pl:
			case Mo:
			case Mu: 	initEnvironment(&environment, cursor->DEST);
						affect(environment, cursor->DEST, evaluate(cursor->OPER, getValue(environment, cursor->ARG1), getValue(environment, cursor->ARG1)));
						break;

			case Af:	initEnvironment(&environment, cursor->ARG1);
						affect(environment, cursor->ARG1, getValue(environment, cursor->ARG2));
						break;

			case Afc:	initEnvironment(&environment, cursor->DEST);
						affect(environment, cursor->DEST, atoi(cursor->ARG1));
						break;

			case Sk:

			case Jp:

			case Jz:

			case St:
						printEnvironment(environment);
						return;

			default: break;
		}

		cursor = cursor->NEXT;
	}
}
