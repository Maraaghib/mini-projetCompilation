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

<OPERATOR>:(PL|MO|MU|AF|AFc|SK|JP|JZ|ST)	{
												BEGIN ARGUMENT1;
												for(int opp = PL; opp <= ST; opp++){
													if(strcmp(nomop(opp), yytext+1) == 0)
														oper = opp;
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

%%

void echo(int n,char *etiq, char *oper, char *arg1, char *arg2) {
    char *tmmpEtiq = NULL;
    if(etiq == NULL) {
        tmmpEtiq = malloc(1);
        tmmpEtiq[0] = '\0';
    } else {
        tmmpEtiq = strdup(etiq);
        strcat(tmmpEtiq,":");
    }
    if(n == 1) {
        printf("%s\t%s\t%s\n", tmmpEtiq ,oper ? oper : "",arg1 ? arg1 : "");
    } else {
        printf("%s\t%s\t%s,\t\t%s\n", tmmpEtiq ,oper ? oper : "",arg1 ? arg1 : "",arg2 ? arg2 : "");
    }
    free(tmmpEtiq);
}

void printInstructs() {
    printf("MUL:\tnop\n");
    echo(2,NULL,"mrmovl","4(%esp)","%eax");
    echo(2,NULL,"mrmovl","8(%esp)","%ebx");
    echo(2,NULL,"rrmovl","%eax","%ecx");
    echo(2,NULL,"subl","%ebx","%ecx");
    echo(1,NULL,"jl","NEXT",NULL);
    echo(2,NULL,"rmmovl","%ebx","4(%esp)");
    echo(2,NULL,"rmmovl","%eax","8(%esp)");
    echo(2,NULL,"rrmovl","%eax","%ecx");
    echo(2,NULL,"rrmovl","%ebx","%eax");
    echo(2,NULL,"rrmovl","%ecx","%ebx");
    echo(2,"NEXT","andl","%eax","%eax");
    echo(1,NULL,"je","ENDMUL",NULL);
    echo(1,"SIGN","nop",NULL,NULL);
    echo(1,NULL,"jg","MULPLUS",NULL);
    echo(2,NULL,"irmovl","0","%ecx");
    echo(2,NULL,"subl","%eax","%ecx");
    echo(2,NULL,"rrmovl","%ecx","%eax");
    echo(2,NULL,"rmmovl","%eax","4(%esp)");
    echo(2,NULL,"irmovl","0","%ecx");
    echo(2,NULL,"subl","%ebx","%ecx");
    echo(2,NULL,"rrmovl","%ecx","%ebx");
    echo(2,NULL,"rmmovl","%ebx","8(%esp)");
    printf("MULPLUS:\tnop\n");
    echo(2,NULL,"mrmovl","4(%esp)","%eax");
    echo(2,NULL,"andl","%eax","%eax");
    echo(1,NULL,"je","ENDMUL",NULL);
    echo(2,NULL,"irmovl","1","%esi");
    echo(2,NULL,"subl","%esi","%eax");
    echo(2,NULL,"mrmovl","8(%esp)","%ebx");
    echo(1,NULL,"pushl","%ebx",NULL);
    echo(1,NULL,"pushl","%eax",NULL);
    echo(1,NULL,"call","MULPLUS",NULL);
    echo(1,NULL,"popl","%ebx",NULL);
    echo(1,NULL,"popl","%ebx",NULL);
    echo(2,NULL,"mrmovl","8(%esp)","%ebx");
    echo(2,NULL,"addl","%ebx","%eax");
    echo(1,NULL,"ret",NULL,NULL);
    echo(2,"ENDMUL","irmovl","0","%eax");
    echo(1,NULL,"ret",NULL,NULL);
}

int main(int argc,char *argv[]) {
	//BEGIN(ETIQUETTE);
    char buf[128];
    int current = 0;
	yylex();
	Environment environment = envirAlloc();
	QUAD quad = bilquad.first;
	//Init all variables with a memory case associated with it name
	while(quad != NULL) {
	    if(quad->OPER == AFc) {
	        if(strcmp(quad->DEST,"") != 0) {
	            if(initEnvironment(&environment,quad->DEST) == EXIT_SUCCESS) {
	                affect(environment,quad->DEST,++current);
	            }
	        }
	    } else if(quad->OPER == AF) {
	        if(strcmp(quad->ARG1,"") != 0) {
	            if(initEnvironment(&environment,quad->ARG1) == EXIT_SUCCESS) {
	                affect(environment,quad->ARG1,++current);
	            }
	        }
	        if(strcmp(quad->ARG2,"") != 0) {
	            if(initEnvironment(&environment,quad->ARG2) == EXIT_SUCCESS) {
	                affect(environment,quad->ARG2,++current);
	            }
	        }
	    } else if(quad->OPER == SK) {
	        if(strcmp(quad->ARG2,"") != 0) {
	            if(initEnvironment(&environment,quad->DEST) == EXIT_SUCCESS) {
	                affect(environment,quad->DEST,++current);
	            }
	        }
	    } else if(quad->OPER == PL || quad->OPER == MU || quad->OPER == MO) {
	        if(strcmp(quad->ARG1,"") != 0) {
	            if(initEnvironment(&environment,quad->ARG1) == EXIT_SUCCESS) {
	                affect(environment,quad->ARG1,++current);
	            }
	        }
	        if(strcmp(quad->ARG2,"") != 0) {
	            if(initEnvironment(&environment,quad->ARG2) == EXIT_SUCCESS) {
	                affect(environment,quad->ARG2,++current);
	            }
	        }
	        if(strcmp(quad->DEST,"") != 0) {
	            if(initEnvironment(&environment,quad->DEST) == EXIT_SUCCESS) {
	                affect(environment,quad->DEST,++current);
	            }
	        }

	    }
	    quad = quad->NEXT;
	}
	//Real compilation
	printf("\t\t.pos\t0\n");
	echo(2,"INIT","irmovl","Data","%edx");
	echo(2,NULL,"rrmovl","%edx","%eax");
	echo(2,NULL,"rrmovl","%eax","%esp");
	echo(2,NULL,"rrmovl","%eax","%ebp");
	quad = bilquad.first;
	while(quad != NULL) {
	    if(quad->OPER == AFc) {
            echo(2,quad->ETIQ,"irmovl",quad->ARG1,"%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->DEST));
            echo(2,NULL,"rmmovl","%eax", buf);
	    }
	    else if(quad->OPER == AF) {
	        snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG2));
            echo(2,quad->ETIQ,"mrmovl",buf,"%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG1));
            echo(2,NULL,"rmmovl","%eax", buf);
	    }
	    else if(quad->OPER == PL ) {
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG1));
            echo(2,quad->ETIQ,"mrmovl",buf,"%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG2));
            echo(2,NULL,"mrmovl", buf,"%ebx");
            echo(2,NULL,"addl", "%eax","%ebx");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->DEST));
            echo(2,NULL,"rmmovl", "%ebx",buf);
	    }
	    else if(quad->OPER == MO ) {
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG1));
            echo(2,quad->ETIQ,"mrmovl",buf,"%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG2));
            echo(2,NULL,"mrmovl", buf,"%ebx");
            echo(2,NULL,"subl", "%ebx","%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->DEST));
            echo(2,NULL,"rmmovl", "%eax",buf);
	    }
	    else if(quad->OPER == MU ) {
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG2));
            echo(2,quad->ETIQ,"mrmovl",buf,"%eax");
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG1));
            echo(1,NULL,"pushl","%eax",NULL);
            echo(2,NULL,"mrmovl", buf,"%eax");
            echo(1,NULL,"pushl","%eax",NULL);
            echo(1,NULL,"call", "MUL",NULL);
            snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->DEST));
            echo(1,NULL,"popl","%ebx",NULL);
            echo(1,NULL,"popl","%ebx",NULL);
            echo(2,NULL,"rmmovl", "%eax",buf);
	    }
	    else if(quad->OPER == JP) {
	        echo(1,quad->ETIQ,"jmp",quad->DEST,NULL);
	    }
    	    else if(quad->OPER == JZ) {
	        snprintf(buf,128,"%d(%%edx)",4*getValue(environment,quad->ARG1));
	        echo(2,quad->ETIQ,"mrmovl",buf,"%eax");
	        echo(2,NULL,"andl","%eax","%eax");
	        echo(1,NULL,"je",quad->DEST,NULL);
	    }
	    else if(quad->OPER == SK) {
	        echo(1,quad->ETIQ,"nop",NULL,NULL);
	    }else if(quad->OPER == ST) {
	        echo(1,quad->ETIQ,"halt",NULL,NULL);
	    }
	    quad = quad->NEXT;
	}
	printInstructs();
	printf("\t\t.align\t8\n");
	printf("\t\t.pos\t0xF00\n");
	printf("Data:\n");
	printf("\t\t.long 255\n");
	char **tmpArr = malloc(sizeof(char*) * 100);
	int ind=0;
	while(environment != NULL) {
	    if(environment->identifier != NULL && strcmp(environment->identifier,"(null)") != 0 )  {
	        tmpArr[ind++] = strdup(environment->identifier);
	    }
	    environment = environment->next;
	}
	for(int i = ind-1; i >= 0; i--) {
	    printf("%s:\t.long 0\n",tmpArr[i]);
	    free(tmpArr[i]);
	}
	printf("\n");

    free(tmpArr);


}
