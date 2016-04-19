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
        case eps:
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

void printAST(parseTree *p, FILE *outfile)
{
    int i,j;

    totalAllocatedMemory = totalAllocatedMemory+1;
    
    for(i = 0; i < p->noOfChildren; i++){
        if(p->children[i].isTerminal == true)
        {
            totalAllocatedMemory = totalAllocatedMemory+1;
            if(p->children[i].terminal.tokenClass == TK_FIELDID || p->children[i].terminal.tokenClass == TK_ID ||
               p->children[i].terminal.tokenClass == TK_NUM || p->children[i].terminal.tokenClass == TK_RNUM ||
               p->children[i].terminal.tokenClass == TK_FUNID || p->children[i].terminal.tokenClass == TK_FIELDID)
            {
                fprintf(outfile, "%s", p->children[i].terminal.lexeme);
                for(j=strlen(p->children[i].terminal.lexeme);j<=20;j++)
                    fprintf(outfile, " ");
            }
            else        
            {
                fprintf(outfile, "%s", idRepr(p->children[i].terminal.tokenClass));
                for(j=strlen(idRepr(p->children[i].terminal.tokenClass));j<=20;j++)
                    fprintf(outfile, " ");
            }
            
            fprintf(outfile, "   ");
            if(p->children[i].terminal.tokenClass == eps)
                fprintf(outfile,"---\t");
            else
            {
                fprintf(outfile, "%llu\t", p->children[i].terminal.lineNo);
            }
            
            fprintf(outfile, "   ");
            fprintf(outfile, "%s", tokenRepr(p->children[i].terminal.tokenClass));
            for(j=strlen(tokenRepr(p->children[i].terminal.tokenClass));j<=10;j++)
                fprintf(outfile, " ");
            
            fprintf(outfile, "   ");
            if(p->children[i].terminal.tokenClass == TK_RNUM || p->children[i].terminal.tokenClass == TK_NUM)
            {
                fprintf(outfile, "%s", p->children[i].terminal.lexeme);
                for(j=strlen(p->children[i].terminal.lexeme);j<=4;j++)
                    fprintf(outfile, " ");
            }
            else
            {
                for(j=0;j<=4;j++)
                    fprintf(outfile, "-");
            }
            
            fprintf(outfile, "   ");
            fprintf(outfile, "YES");
            fprintf(outfile, "   ");
            
            fprintf(outfile, "%s", idRepr(p->nonTerminal));
            for(j=strlen(idRepr(p->nonTerminal));j<=10;j++)
                fprintf(outfile, " ");
            fprintf(outfile, "   ");
            fprintf(outfile, "----------");
            fprintf(outfile, "\n");
        }
        else {
            fprintf(outfile, "----                   ");
            
            fprintf(outfile, "---   ");
            
            fprintf(outfile, "    --       ");
            
            fprintf(outfile, "  --     ");            
            
            fprintf(outfile, "   NO    ");
            
            fprintf(outfile, "%s\t", idRepr(p->nonTerminal));
            for(j=strlen(idRepr(p->nonTerminal));j<=10;j++)
                fprintf(outfile, " ");
            fprintf(outfile, "   ");
            fprintf(outfile, "%s\t", idRepr(p->children[i].nonTerminal));
            for(j=strlen(idRepr(p->children[i].nonTerminal));j<=10;j++)
                fprintf(outfile, " ");
            fprintf(outfile,"\n");
            printAST(&p->children[i], outfile);
        }
    }
}
