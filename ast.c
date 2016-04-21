#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "utils.h"
#include "parser.h"

//COPIED DIRECTLY
bool used(int tokenType) {
   switch(tokenType) {
        case TK_MAIN:
        case TK_END:
        case TK_SEM:
        case TK_INPUT:
        case TK_PARAMETER:
        case TK_LIST:
        case TK_SQL:
        case TK_SQR:
        case TK_OUTPUT:
        case TK_COMMA:
        case TK_COLON:
        case TK_DOT:
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_ENDWHILE:
        case TK_IF:
        case TK_ENDIF:
        case TK_OP:
        case TK_CL:
        case TK_RETURN:
            return false;
            break;
        default:
            return true;
    }
}

void buildAST(parseTree *pTree, parseTree *ast) {

    int parseTreeCounter=0, astCounter=0;
    ast->numChildren=0;
    for(parseTreeCounter = 0; parseTreeCounter < p->numChildren; parseTreeCounter++)
        if( (isTerm(pTree->id)==0) || used(pTree->children[parseTreeCounter].id))
            ast->numChildren++;

    ast->children = (parseTree *)malloc(ast->numChildren * sizeof(parseTree));
    for(parseTreeCounter=0;parseTreeCounter<p->numChildren;++parseTreeCounter) {

        ast->children[astCounter].parentId = ast->id;

        if(isTerm(pTree->children[parseTreeCounter].id) && used(pTree->children[parseTreeCounter].id)){

            ast->children[astCounter].id = pTree->children[parseTreeCounter].id;
            strcpy(ast->children[astCounter].token.tokenType, pTree->children[parseTreeCounter].token.tokenType);
            strcpy(ast->children[astCounter].token.lexeme, pTree->children[parseTreeCounter].token.lexeme);
            ast->children[astCounter].token.lineNumber = pTre->children[parseTreeCounter].token.lineNumber;
            astCounter++;

        }
        else { 

            ast->children[astCounter].id = pTree->children[parseTreeCounter].id;
            createAbstractSyntaxTree(&pTree->children[parseTreeCounter], &ast->children[astCounter]);
            astCounter++;
        }

    }
}

void setSTPointers(astNode *ast,varHashTable *varTable){
    int i;
    if(isTerm(ast->id)){
        ast->symbolTableEntry= getVarHashTableEntry(varTable,ast->token.lexeme);
    }
    else{
        ast->symbolTableEntry=NULL;
    }
    for(i=0;i<ast->numChildren;++i)
        setSTPointers(&(ast->children[i]),varTable);
}
