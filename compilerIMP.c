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

int numET = 0;
int numCT = 0;
int numVA = 0;
int is_oper = 0;
int is_seq = 0;

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
    char *etiqJz;
    int op;
    int ret;
    int jmp;
    int numTmp;
    QUAD Quad;
    QUAD quadJz;
    BILQUAD tmp;

    if(!lexeme) return 0;
    switch (lexeme->type) {
        case typeCons:  sprintf(str, "%d", lexeme->cons.value);
                        return strdup(str);

        case typeIdent: sprintf(str, "%s", lexeme->ident.value); // Doit tout simplement renvoyer l'identifiant
                        return strdup(str);

        case typeOper:
            switch (lexeme->oper.operT) {
                case Wh:    ret = numET;
                            val1 = compileIMP(environment, lexeme->oper.operN[0]); // Expression
                            if(is_oper == 0){ // while (X) 
                              sprintf(str, "ET%d", numET++);
                              etiq = strdup(str);
                              oper = SK;
                              arg1 = strdup("");
                              arg2 = strdup("");
                              dest = strdup(val1);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            }
                            
                            etiq = "Jz";//strdup(str);
                            oper = JZ;
                            arg1 = strdup(val1);
                            arg2 = strdup("");


                            dest = strdup("Jz");
    		                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
    		                    tmp = creer_bilquad(Quad);
    		                    bilquad = concatq(bilquad, tmp);

                            compileIMP(environment, lexeme->oper.operN[1]); // Commands in Do bloc
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


                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            quadJz = rechbq("Jz", bilquad);
                            quadJz->ETIQ = etiq;
                            sprintf(str, "ET%d", numET);
                            etiq = strdup(str);
                            quadJz->DEST = etiq;

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

                case If:    val1 = compileIMP(environment, lexeme->oper.operN[0]); // Expression
                            if(is_oper == 0){ // if (X) 
                              sprintf(str, "ET%d", numET++);
                              etiq = strdup(str);
                              oper = SK;
                              arg1 = strdup("");
                              arg2 = strdup("");
                              dest = strdup(val1);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            }
                            
                            etiq = "Jz";//strdup(str);
                            oper = JZ;
                            arg1 = strdup(val1);
                            arg2 = strdup("");

                            dest = strdup("Jz");
                            Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                            tmp = creer_bilquad(Quad);
                            bilquad = concatq(bilquad, tmp);

                            compileIMP(environment, lexeme->oper.operN[1]); // Commands in Then bloc

                  
                            jmp = numET++;
                            
                                // Jump if always true
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            etiqJz = strdup(str);
                            oper = JP;
                            arg1 = strdup("");
                            arg2 = strdup("");
                            dest = strdup("Jp");
                            Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                            tmp = creer_bilquad(Quad);
                            bilquad = concatq(bilquad, tmp);


                            sprintf(str, "ET%d", jmp);
                            etiq = strdup(str);
                            quadJz = rechbq("Jz", bilquad);
                            quadJz->ETIQ = etiq;
                            sprintf(str, "ET%d", numET+1);
                            etiq = strdup(str);
                            quadJz->DEST = etiq;

                                // Else
                            compileIMP(environment, lexeme->oper.operN[2]); // Commands in Else bloc

                            sprintf(str, "ET%d", numET);
                            etiq = strdup(str);
                            quadJz = rechbq(etiqJz, bilquad);
                            quadJz->DEST = etiq;

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


                case Af:    numTmp = numET++; //On range le sk dans la liste et tester si on a AF ou AFc
                            val1 = strdup(compileIMP(environment, lexeme->oper.operN[0]));
                            val2 = strdup(compileIMP(environment, lexeme->oper.operN[1]));
                            if((strncmp(val2, "0", 1) >= 0) && (strncmp(val2, "9", 1) <= 0)){ // Si le premier caractère est un chiffre
                              sprintf(str, "ET%d", (numET++));
                              etiq = strdup(str);
                              oper = AFc;
                              arg1 = strdup(val2);
                              arg2 = strdup("");
                              sprintf(str, "CT%d", numCT);
                              dest = strdup(str);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);

                              sprintf(str, "ET%d", numTmp);
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
                                    sprintf(str, "ET%d", numET++);
                                    etiq = strdup(str);
                                    oper = SK;
                                    arg1 = strdup("");
                                    arg2 = strdup("");
                                    dest = strdup(val2);
                                    Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                    tmp = creer_bilquad(Quad);
                                    bilquad = concatq(bilquad, tmp);
                                }

                              sprintf(str, "ET%d", numTmp);
                              numET+= (is_oper != 0) ? 0 : 1;
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


                case Se:    is_seq = 1;
                            compileIMP(environment, lexeme->oper.operN[0]); compileIMP(environment, lexeme->oper.operN[1]);
                            is_seq = 0;
                            break;
                case Sk:    is_seq = 1;
                            sprintf(str, "ET%d", (numET++));
                            etiq = strdup(str);
                            oper = SK;
                            arg1 = strdup("");
                            arg2 = strdup("");
                            dest = strdup("");
                            Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                            tmp = creer_bilquad(Quad);
                            bilquad = concatq(bilquad, tmp);
                            is_seq = 0;
                            return NULL;

                case Mo:    
                case Mu:    

                case Pl:    is_oper = 1;
                            numTmp = numET + 1;
                            val1 = strdup(compileIMP(environment, lexeme->oper.operN[0]));
                            val2 = strdup(compileIMP(environment, lexeme->oper.operN[1]));
                            if((strncmp(val1, "0", 1) < 0) || (strncmp(val1, "9", 1) > 0)){ // Si le premier caractère n'est pas un chiffre
                                sprintf(str, "ET%d", (numTmp++));
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
                                sprintf(str, "ET%d", (numTmp++));
                                etiq = strdup(str);
                                oper = SK;
                                arg1 = strdup("");
                                arg2 = strdup("");
                                dest = strdup(val2);
                                Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                                tmp = creer_bilquad(Quad);
                                bilquad = concatq(bilquad, tmp);
                            }

                            if((strncmp(val1, "0", 1) >= 0) && (strncmp(val1, "9", 1) <= 0)){ // Si le premier caractère est un chiffre
                                sprintf(str, "ET%d", (numTmp++));
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
                            if((strncmp(val2, "0", 1) >= 0) && (strncmp(val2, "9", 1) <= 0)){ // Si le premier caractère est un chiffre
                                sprintf(str, "ET%d", (numTmp++));
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
                              numET += 2;
                              etiq = strdup(str);
                              oper = op;
                              arg1 = val1; // Le résultat de l'instruction AFc
                              arg2 = strdup(dest);
                              sprintf(str, "VA%d", numVA++);
                              dest = strdup(str);
                              Quad = creer_quad(etiq, oper, arg1, arg2, dest);
                              tmp = creer_bilquad(Quad);
                              bilquad = concatq(bilquad, tmp);
                            return dest;
                default: break;
            }
        default: break;
    }
    
    
    return 0;
}

int executeCOM(Environment *environment, nodeType *lexeme) {
    QUAD Quad = bilquad.first;
    compileIMP(environment, lexeme);
    // The last instruction which stops the execution
    char str[20];
    sprintf(str, "ET%d", (numET++));
    char *etiq = strdup(str);
    Quad = creer_quad(etiq, ST, "", "", "");
    BILQUAD tmp = creer_bilquad(Quad);
    bilquad = concatq(bilquad, tmp);
    printf("\n");
    printBilquad(bilquad);
    printf("\n");
    return 1;
}
