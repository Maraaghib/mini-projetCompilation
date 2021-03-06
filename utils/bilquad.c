/* arbre.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bilquad.h"
#include "environment.h"
/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/* QUAD,BILQUAD: definis dans bilquad.h                              */
/*-------------------------------------------------------------------*/
/*---------------------quadruplets-----------------------------------*/

/* retourne un quadruplet (avec etiquette etiq) */

QUAD creer_quad(char *etiq,int op,char *arg1,char *arg2,char *res)
{QUAD qd;int lres;
  qd=(struct cellquad *)malloc(sizeof(struct cellquad));
  if (etiq !=NULL)
    {qd->ETIQ=(char *)malloc(MAXIDENT*sizeof(char));
      strcpy(qd->ETIQ,etiq);}
  qd->OPER=op;
  if (arg1 !=NULL)
    {qd->ARG1=(char *)malloc(MAXIDENT*sizeof(char));
      strcpy(qd->ARG1,arg1);}
  if (arg2 !=NULL)
    {qd->ARG2=(char *)malloc(MAXIDENT*sizeof(char));
      strcpy(qd->ARG2, arg2);}
  if (res!= NULL)
    {lres=strlen(res);
      qd->DEST=(char *)malloc(lres*sizeof(char));
      strcpy(qd->DEST,res);}
  return(qd);
}

/* retourne une biliste vide  */
BILQUAD bilquad_vide()
{BILQUAD bq;
  bq.first=NULL;bq.last=NULL;
  return(bq);
}

/* retourne une biliste a un element  */
BILQUAD creer_bilquad(QUAD qd)
{BILQUAD bq;
  bq.first=qd;bq.last=qd;
  return(bq);
}

/* fonction aux  pour la fonction rechbq */
QUAD rechq(char *chaine, QUAD qd)
{QUAD qcour;
  qcour=qd;
  if (qcour!=NULL)
    {if (strcmp(qcour->ETIQ,chaine)==0)
        return qcour;
      else
	return rechq(chaine,qcour->NEXT);
    }
  else
    return NULL;
}

/*retourne le quad etiquete par chaine, NULL s'il n'y en a pas */
QUAD rechbq(char *chaine, BILQUAD bq)
{return(rechq(chaine,bq.first));}


BILQUAD concatq(BILQUAD bq1, BILQUAD bq2)
/* retourne la concatenation; ne detruit pas bqi; ne copie pas *bqi */
/* peut creer une boucle ! */
{BILQUAD bq;
  if (bq1.last!= NULL)
    if (bq2.first!=NULL)
       { bq1.last->NEXT=bq2.first;
        bq.first=bq1.first;
        bq.last=bq2.last;
        return(bq);}
    else
      return(bq1);
  else
    return(bq2);
}

/* traduit entier (= codop) vers chaine (= nom operation)  */
/* les codes d'op sont definis dans environ.h              */
char *nomop(int codop)
{switch(codop)
    {case(AF): return("Af");
    case(SK): return("Sk");
    case(PL): return("Pl");
    case(MO): return("Mo");
    case(MU): return("Mu");
    case(AFc): return("Afc");
    case(ST): return("St");
    case(JP): return("Jp");
    case(JZ): return("Jz");
    default:return(NULL);
    };
}

/* Displays the quadruplet (to generate code); Then jump to the line */
void printQuad(QUAD qd)
{ if(strcmp(qd->ETIQ,"") == 0)       /* Etiquette = empty word */
    {printf("%-10s ","");}
  else
    {printf("%-10s",qd->ETIQ);}
  printf(":%-6s ",nomop(qd->OPER));
  if (qd->ARG1!=NULL)
    {printf(":%-10s",qd->ARG1);}
  else
    {printf("%-10s","");}
  if (qd->ARG2!=NULL)
    {printf(":%-10s",qd->ARG2);}
  else
    {printf("%-10s","");}
  if (qd->DEST!=NULL)
    {printf(":%-10s\n",qd->DEST);}
  else
    {printf("\n");}
  }

/* affiche la biliste de quad */
void printBilquad(BILQUAD bq)
{QUAD qcour;
  qcour=bq.first;
  while(qcour!=NULL)
    {printQuad(qcour);
      qcour=qcour->NEXT;}
}
