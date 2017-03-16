#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
#include "iimp.h"
#include "bilquad.h"
#include "environment.h"

char *strdup(const char *);

char* int2Str(int val){
    char str[20];
    sprintf(str, "%d", val);
    return strdup(str);
}

QUAD addEnd (QUAD qHead, char *etiq, int oper, char *arg1, char *arg2, char *dest){
   /* allocation du noeud */
   QUAD qNew = malloc (sizeof *qNew);

   /* si tout s'est bien passe : */
   if (qNew != NULL){
      /* mise a jour des champs : */

      /* donnees */
      qNew->ETIQ = etiq;
      qNew->OPER = oper;
      qNew->ARG1 = arg1;
      qNew->ARG2 = arg2;
      qNew->DEST = dest;

      /* chainage par defaut */
      qNew->NEXT = NULL;

      /* chainage */
      if (qHead == NULL)
      {
         /* c'est le premier : */
         qHead = qNew;
      }
      else
      {
         /* on cherche le dernichar *etiq, int oper, char *arg1, *arg2, *dester noeud */
         QUAD last = qHead;

         while (last->NEXT != NULL)
         {
            /* pointer sur le suivant */
            last = last->NEXT;
         }

         /* modification du chainage */
         last->NEXT = qNew;
      }
   }
   return qHead;
}

int numET = 0;
int numCT = 0;
int numVA = 0;

int compileIMP(Environment *environment, nodeType *lexeme, QUAD *instruction) {
    QUAD instruct;
    char str[15];
    char *etiq;
    int oper;
    char *arg1;
    char *arg2;
    char *dest;
    int ret;
    int jmp;

    if(!lexeme) return 0;
    switch (lexeme->type) {
        case typeCons:  sprintf(str, "ET%d", numET++);
                        etiq = strdup(str);
                        oper = AFc;
                        arg1 = int2Str(lexeme->cons.value);
                        arg2 = strdup("");
                        sprintf(str, "CT%d", numCT++);
                        dest = strdup(str);
                        *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                        break;

        case typeIdent: sprintf(str, "ET%d", numET++);
                        etiq = strdup(str);
                        oper = AF;
                        arg1 = strdup(lexeme->ident.value);
                        sprintf(str, "VA%d", numVA++);
                        arg2 = strdup(str);
                        dest = strdup("");
                        *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                        break;

        case typeOper:
            switch (lexeme->oper.operT) {
                case Wh:    ret = numET;
                            compileIMP(environment, lexeme->oper.operN[0], instruction); // Expression
                            // Jump if not
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = JZ;
                            arg1 = strdup(lexeme->ident.value);
                            arg2 = strdup("");
                            sprintf(str, "VA%d", numVA++);
                            sprintf(str, "ET%d", jmp);
                            dest = strdup(str);
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                            // Jump if always true
                            sprintf(str, "ET%d", numET++);
                            jmp = numET;
                            etiq = strdup(str);
                            oper = JP;
                            arg1 = strdup("");
                            arg2 = strdup("");
                            sprintf(str, "ET%d", ret);
                            dest = strdup(str);
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);

                            // Sk
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = SK;
                            arg1 = strdup("");
                            arg2 = strdup("");
                            dest = strdup("");
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                            break;

                case If:    compileIMP(environment, lexeme->oper.operN[0], instruction); // Expression
                            // Jump if not
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = JZ;
                            arg1 = strdup(lexeme->ident.value);
                            arg2 = strdup("");
                            sprintf(str, "ET%d", jmp);
                            dest = strdup(str);
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                            // Commandes
                            compileIMP(environment, lexeme->oper.operN[1], instruction);
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
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                            // Else
                            compileIMP(environment, lexeme->oper.operN[2], instruction);
                            *ret = numET;
                            // Sk
                            sprintf(str, "ET%d", numET++);
                            etiq = strdup(str);
                            oper = SK;
                            arg1 = strdup("");
                            arg2 = strdup("");
                            dest = strdup("");
                            *instruction = addEnd (*instruction, etiq, oper, arg1, arg2, dest);
                            break;

                case Af:    compileIMP(environment, lexeme->oper.operN[1], instruction);
                            compileIMP(environment, lexeme->oper.operN[0], instruction);
                            break;


                case Se:    compileIMP(environment, lexeme->oper.operN[0], instruction); return compileIMP(environment, lexeme->oper.operN[1], instruction);

                case Pl:    //return compileIMP(environment, lexeme->oper.operN[0]) + compileIMP(environment, lexeme->oper.operN[1]);
                case Mo:    //return compileIMP(environment, lexeme->oper.operN[0]) - compileIMP(environment, lexeme->oper.operN[1]);
                case Mu:    break;//return compileIMP(environment, lexeme->oper.operN[0]) * compileIMP(environment, lexeme->oper.operN[1]);
            }
    }
    return 0;
}

int executeCOM(Environment *environment, nodeType *lexeme) {
    BILQUAD Bilquad;
    QUAD *Quad;
    int result = compileIMP(environment, lexeme, &Bilquad.first);
    while ((*Quad)->NEXT != NULL) {
        printf("%s  :%s :%s  :%s  :%s\n", (*Quad)->ETIQ, nomop((*Quad)->OPER), (*Quad)->ARG1, (*Quad)->ARG2, (*Quad)->DEST);
    }
    return result;
}