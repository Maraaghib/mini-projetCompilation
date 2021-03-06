%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	char *strdup(const char *);
	int fileno(FILE *);
	#include "y.tab.h"
	#include "iimp.h"
%}

%%
":=" 		{return Af;}
"skip" 	{return Sk;}
";" 		{return Se;}
"if" 		{return If;}
"then" 	{return Th;}
"else" 	{return El;}
"while" 	{return Wh;}
"do" 		{return Do;}
"+" 		{return Pl;}
"-" 		{return Mo;}
"*" 		{return Mu;}
"("			{ return yytext[0]; }
")"			{ return yytext[0]; }
[A-Za-z](_|[A-Za-z]|[0-9])*	{yylval.ident = strdup(yytext); return V;}
([1-9][0-9]*)|0 	{yylval.ival = atoi(yytext); return I;}
.|\n 		{}
%%
