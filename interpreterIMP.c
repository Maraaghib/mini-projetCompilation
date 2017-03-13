#include <stdio.h>
#include "y.tab.h"
#include "mini-projet.h"
#include "environment.h"

int interprete(Environment environment, nodeType *lexeme) {
    if(!lexeme) return 0;
    switch (lexeme->type) {
        case typeCons:  return lexeme->cons.value;
        case typeIdent: return getValue(environment, lexeme->ident.value);
        case typeOper:
            switch (lexeme->oper.operT) {
                case Wh:    while (interprete(environment, lexeme->oper.operN[0])) {
                                interprete(environment, lexeme->oper.operN[1]);
                            }
                            return 0;
                case If:    if (interprete(environment, lexeme->oper.operN[0])) {
                                interprete(environment, lexeme->oper.operN[1]);
                            }
                            else{
                                interprete(environment, lexeme->oper.operN[2]);
                            }
                            return 0;
                case Af:    initEnvironment(environment, lexeme->oper.operN[0]->ident.value); return affect(environment, lexeme->oper.operN[0]->ident.value, interprete(environment, lexeme->oper.operN[1]));
                case Se:    interprete(environment, lexeme->oper.operN[0]); return interprete(environment, lexeme->oper.operN[1]);

                case Pl:    return interprete(environment, lexeme->oper.operN[0]) + interprete(environment, lexeme->oper.operN[1]);
                case Mo:    return interprete(environment, lexeme->oper.operN[0]) - interprete(environment, lexeme->oper.operN[1]);
                case Mu:    return interprete(environment, lexeme->oper.operN[0]) * interprete(environment, lexeme->oper.operN[1]);
            }
    }
    return 0;
}

int execute(Environment environment, nodeType *lexeme) {
    int result = interprete(environment, lexeme);
    printEnvironment(environment);
    return result;
}
