#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
#include "iimp.h"
#include "bilquad.h"
#include "environment.h"

char *strdup(const char *);

BILQUAD bilquad;

char* int2Str(int val){
    char str[20];
    sprintf(str, "%d", val);
    return strdup(str);
}

void pre(char *etiq, int oper, char *arg1, char *arg2, char *dest){printf("\n%s  :%s :%s  :%s  :%s\n", etiq, nomop(oper), arg1, arg2, dest);}
void prc(char *s){printf("\nAffiche: %s\n",s);}
void pr(void){printf("\nJe suis là ! \n");}

/*
QUAD addEnd (QUAD qHead, char *etiq, int oper, char *arg1, char *arg2, char *dest){
   /* allocation du noeud
   QUAD qNew = malloc (sizeof *qNew);

   /* si tout s'est bien passe :
   if (qNew != NULL){
      /* mise a jour des champs : *

      /* donnees *
      qNew->ETIQ = etiq;
      qNew->OPER = oper;
      qNew->ARG1 = arg1;
      qNew->ARG2 = arg2;
      qNew->DEST = dest;


      /* chainage par defaut *
      qNew->NEXT = NULL;

      /* chainage *
      if (qHead == NULL)
      {
         /* c'est le premier : *
         qHead = qNew;
      }
      else
      {
         /* on cherche le dernichar *etiq, int oper, char *arg1, *arg2, *dester noeud *
         QUAD last = qHead;
		printf("last = NULL : %d\n",qHead == NULL);
         while (1){
         	if(last->NEXT == NULL){
         		last->NEXT = qNew;
         		prc("Ajout à la fin de liste");
         		break;
         	}
            /* pointer sur le suivant *
            last = last->NEXT;
         }

         /* modification du chainage *
         //(*last)->NEXT = qNew;
      }
   }
   return qHead;
}
*/

int numET = 0;
int numCT = 0;
int numVA = 0;
int is_oper = 0;

char *compileIMP(Environment *environment, nodeType *lexeme) {
    QUAD instruct;
    char str[15];
    char *etiq;
    int oper;
    char *arg1;
    char *arg2;
    char *val1;
    char *val2;
    char *dest;
    int op;
    int ret;
    int jmp;
    QUAD Quad;
    BILQUAD tmp;

    if(!lexeme) return 0;
    switch (lexeme->type) {
        case typeCons:  /*sprintf(str, "ET%d", (numET+1));
                        etiq = strdup(str);
                        oper = AFc;
                        sprintf(str, "%d", lexeme->cons.value);
                        arg1 = strdup(str);
                        arg2 = strdup("");
                        sprintf(str, "CT%d", numCT++);
                        dest = strdup(str);
                        Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                        tmp = creer_bilquad(Quad);
                        bilquad = concatq(bilquad, tmp); */

                        sprintf(str, "%d", lexeme->cons.value);
                        return strdup(str);

        case typeIdent: sprintf(str, "%s", lexeme->ident.value); // Doit tout simplement renvoyer l'identifiant
                        return strdup(str);

        case typeOper:
            switch (lexeme->oper.operT) {
                case Wh:    ret = numET;
                            compileIMP(environment, lexeme->oper.operN[0]); // Expression
                            // Jump if not
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = JZ;
                            arg1 = strdup(lexeme->ident.value);
                            arg2 = strdup("");
                            sprintf(str, "VA%d", numVA++);
                            sprintf(str, "ET%d", jmp);
                            dest = strdup(str);
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);
                                // Jump if always true
                                sprintf(str, "ET%d", numET++);
                                jmp = numET;
                                etiq = strdup(str);
                                oper = JP;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                sprintf(str, "ET%d", ret);
                                dest = strdup(str);
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);

                                // Sk
                                sprintf(str, "ET%d", numET++);
                                etiq = strdup(str);
                                oper = SK;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                dest = strdup("");
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);
                            break;

                case If:    compileIMP(environment, lexeme->oper.operN[0]); // Expression
                            // Jump if not
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = JZ;
                            arg1 = strdup(lexeme->ident.value);
                            arg2 = strdup("");
                            sprintf(str, "ET%d", jmp);
                            dest = strdup(str);
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);
                                // Commandes
                                compileIMP(environment, lexeme->oper.operN[1]);
                                // Jump after the else
                                sprintf(str, "ET%d", numET++);
                                jmp = numET;
                                etiq = strdup(str);
                                oper = JP;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                int *ret;
                                sprintf(str, "ET%d", *ret);
                                dest = strdup(str);
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);
                                // Else
                                compileIMP(environment, lexeme->oper.operN[2]);
                                *ret = numET;
                                // Sk
                                sprintf(str, "ET%d", numET++);
                                etiq = strdup(str);
                                oper = SK;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                dest = strdup("");
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);
                            break;

                case Af:    //On range le sk dans la liste et tester si on a AF ou AFc
                            val1 = strdup(compileIMP(environment, lexeme->oper.operN[0]));
                            val2 = strdup(compileIMP(environment, lexeme->oper.operN[1]));
                            if((strncmp(val2, "0", 1) >= 0) && (strncmp(val2, "9", 1) <= 0)){ // Si le premier caractère est un chiffre
                              sprintf(str, "ET%d", (numET+1));
                              etiq = strdup(str);
                              oper = AFc;
                              arg1 = strdup(val2);
                              arg2 = strdup("");
                              sprintf(str, "CT%d", numCT);
                              dest = strdup(str);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);

                              sprintf(str, "ET%d", numET++);
                              numET++;
                              etiq = strdup(str);
                              oper = AF;
                              arg1 = strdup(val1);
                              sprintf(str, "CT%d", numCT++);
                              arg2 = strdup(str);
                              dest = strdup("");
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            }
                            else{
                                if(is_oper == 0){
                                    sprintf(str, "ET%d", numET+1);
                                    etiq = strdup(str);
                                    oper = SK;
                                    arg1 = strdup("");
                                    arg2 = strdup("");
                                    dest = strdup(val2);
                                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                    tmp = creer_bilquad(Quad);
                                    bilquad = concatq(bilquad, tmp);
                                }

                              sprintf(str, "ET%d", numET++);
                              numET++;
                              etiq = strdup(str);
                              oper = AF;
                              arg1 = strdup(val1);
                              arg2 = strdup(val2);
                              dest = strdup("");
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            }

                            break;


                case Se:    compileIMP(environment, lexeme->oper.operN[0]); return compileIMP(environment, lexeme->oper.operN[1]);


                case Mo:    //return compileIMP(environment, lexeme->oper.operN[0]) - compileIMP(environment, lexeme->oper.operN[1]);
                case Mu:    //return compileIMP(environment, lexeme->oper.operN[0]) * compileIMP(environment, lexeme->oper.operN[1]);

                case Pl:    is_oper = 1;
                            val1 = strdup(compileIMP(environment, lexeme->oper.operN[0]));
                            val2 = strdup(compileIMP(environment, lexeme->oper.operN[1]));
                            if((strncmp(val1, "0", 1) < 0) || (strncmp(val1, "9", 1) > 0)){ // Si le premier caractère n'est pas un chiffre
                                sprintf(str, "ET%d", (numET+1));
                                etiq = strdup(str);
                                oper = SK;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                dest = strdup(val1);
                                Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                tmp = creer_bilquad(Quad);
                                bilquad = concatq(bilquad, tmp);
                            }
                            if((strncmp(val2, "0", 1) < 0) || (strncmp(val2, "9", 1) > 0)){ // Si le premier caractère n'est pas un chiffre
                                sprintf(str, "ET%d", (numET+1));
                                etiq = strdup(str);
                                oper = SK;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                dest = strdup(val2);
                                Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                tmp = creer_bilquad(Quad);
                                bilquad = concatq(bilquad, tmp);
                            }

                            if((strncmp(val1, "0", 1) >= 0) && (strncmp(val1, "9", 1) <= 0)){ // Si le premier caractère n'est pas un chiffre
                                sprintf(str, "ET%d", (numET+1));
                                etiq = strdup(str);
                                oper = AFc;
                                arg1 = strdup(val1);
                                arg2 = strdup("");
                                sprintf(str, "CT%d", numCT++);
                                dest = strdup(str);
                                Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                tmp = creer_bilquad(Quad);
                                bilquad = concatq(bilquad, tmp);
                            }
                            if((strncmp(val2, "0", 1) >= 0) && (strncmp(val2, "9", 1) <= 0)){ // Si le premier caractère n'est pas un chiffre
                                sprintf(str, "ET%d", (numET+1));
                                etiq = strdup(str);
                                oper = AFc;
                                arg1 = strdup(val2);
                                arg2 = strdup("");
                                sprintf(str, "CT%d", numCT++);
                                dest = strdup(str);
                                Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                tmp = creer_bilquad(Quad);
                                bilquad = concatq(bilquad, tmp);
                            }
                            if(lexeme->oper.operT == Pl)
                                op = PL;

                            if(lexeme->oper.operT == Mo)
                                op = MO;

                            if(lexeme->oper.operT == Mu)
                                op = MU;

                              sprintf(str, "ET%d", (numET++));
                              etiq = strdup(str);
                              oper = op;
                              arg1 = dest; // Le résultat de l'instruction AFc
                              arg2 = strdup(val1);
                              sprintf(str, "VA%d", numVA++);
                              dest = strdup(str);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            return strdup(str);
            }
    }

    sprintf(str, "ET%d", (numET++));
    etiq = strdup(str);
    oper = ST;
    arg1 = strdup("");
    arg2 = strdup("");
    dest = strdup("");
    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    tmp = creer_bilquad(Quad);
    bilquad = concatq(bilquad, tmp);
    return 0;
}

int executeCOM(Environment *environment, nodeType *lexeme) {
    QUAD Quad = bilquad.first;
    compileIMP(environment, lexeme);
    printf("\n");
    printBilquad(bilquad);
    printf("\n");
    /*while (Quad->NEXT != NULL) {
        printf("%s  :%s :%s  :%s  :%s\n", Quad->ETIQ, nomop(Quad->OPER), Quad->ARG1, Quad->ARG2, Quad->DEST);
    }*/
    return 1;
}
