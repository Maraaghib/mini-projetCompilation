/* bilquad.h */
#ifndef BILQUAD_H
#define BILQUAD_H
/* ----------------------------types--------------------------------------------*/
/* biliste de quadruplets etiquetes (stocke C3A ou Y86 )*/
typedef struct cellquad{
  char *ETIQ;
  int  OPER;
  char *ARG1, *ARG2, *DEST;
  struct cellquad *NEXT;
} *QUAD;

typedef struct{
  QUAD first;
  QUAD last;
}BILQUAD;

/*---------------------fonctions ------------------------------------------------*/
extern QUAD creer_quad(char *etiq,int op,char *arg1,char *arg2,char *res);
extern BILQUAD bilquad_vide() ;                    /* retourne une biliste vide  */
extern BILQUAD creer_bilquad(QUAD qd); /* retourne une biliste  a un element     */
extern QUAD rechbq(char *chaine, BILQUAD bq);/*ret le quad etiquete par chaine   */
extern BILQUAD concatq(BILQUAD bq1, BILQUAD bq2);/* retourne la concatenation    */
extern char *nomop(int codop); /* traduit entier vers chaine (= nom operation)    */
extern void printQuad(QUAD qd); /* affiche le quadruplet                       */
extern void printBilquad(BILQUAD bq); /* affiche la biliste de quadruplets     */

#endif
