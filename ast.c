#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTableHash.h"

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

void buildAST(parseTree *pTree, astNode *ast) {

    int parseTreeCounter=0, astCounter=0;
    ast->numChildren=0;
    for(parseTreeCounter = 0; parseTreeCounter < pTree->numChildren; parseTreeCounter++)
        if( (isTerm(pTree->children[parseTreeCounter].id)==0) || used(pTree->children[parseTreeCounter].id))
            ast->numChildren++;

    ast->children = (astNode *)malloc(ast->numChildren * sizeof(astNode));
    for(parseTreeCounter=0;parseTreeCounter<pTree->numChildren;++parseTreeCounter) {

        ast->children[astCounter].parentId = ast->id;

        if(isTerm(pTree->children[parseTreeCounter].id) && used(pTree->children[parseTreeCounter].id)){

            ast->children[astCounter].id = pTree->children[parseTreeCounter].id;
            strcpy(ast->children[astCounter].token.tokenType, pTree->children[parseTreeCounter].token.tokenType);
            strcpy(ast->children[astCounter].token.lexeme, pTree->children[parseTreeCounter].token.lexeme);
            ast->children[astCounter].token.lineNumber = pTree->children[parseTreeCounter].token.lineNumber;
            astCounter++;

        }
        else if(isTerm(pTree->children[parseTreeCounter].id)==0 ){

            ast->children[astCounter].id = pTree->children[parseTreeCounter].id;
            buildAST(&pTree->children[parseTreeCounter], &ast->children[astCounter]);
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

void printASTHelper(astNode* pTree, FILE* fp){
    int i,len,templen;
    char ch;


    if(isTerm(pTree->id)==1){

        if(pTree->id==eps){
            // printf("eps");
            // while(getchar()!='\n');
            fprintf(fp,"eps");
            for(i=0;i<30- strlen("eps");++i){
                fprintf(fp," ");
            }

            fprintf(fp,"----");
            for(i=0;i<10-strlen("----");++i){
                fprintf(fp," ");
            }
            fprintf(fp,"----");
            for(i=0;i<20-strlen("----");++i){
                fprintf(fp," ");
            }
            fprintf(fp,"----");
            for(i=0;i<10-strlen("----");++i){
                fprintf(fp," ");
            }

            fprintf(fp,"%s",idToName(pTree->parentId));
            for(i=0;i<30 - strlen(idToName(pTree->parentId));++i){
                fprintf(fp," ");
            }
            //leaf node. Hence, yes.
            fprintf(fp,"Yes");
            for(i=0;i<20- strlen("Yes");++i){
                fprintf(fp," ");
            }
            fprintf(fp,"----");
            for(i=0;i<30- strlen("----");++i){
                fprintf(fp," ");
            }
            fprintf(fp,"\n");

        }
        else{

            // printf("%s",pTree->token.lexeme);
            // while(getchar()!='\n');
            fprintf(fp,"%s",pTree->token.lexeme);
            for(i=0;i<30- strlen(pTree->token.lexeme);++i){
                fprintf(fp," ");
            }

            fprintf(fp,"%lu",pTree->token.lineNumber);
            char str[10];
            sprintf(str, "%lu", pTree->token.lineNumber);
            for(i=0;i<10-strlen(str);++i){
                fprintf(fp," ");
            }

            fprintf(fp,"%s",pTree->token.tokenType);
            for(i=0;i<20- strlen(pTree->token.tokenType);++i){
                fprintf(fp," ");
            }

            if(strcmp(pTree->token.tokenType,"TK_RNUM")==0 ||  strcmp(pTree->token.tokenType,"TK_NUM")==0){
                fprintf(fp,"%s",pTree->token.lexeme);
                for(i=0;i<10- strlen(pTree->token.lexeme);++i){
                    fprintf(fp," ");
                }
            }
            else{
                fprintf(fp,"----");
                for(i=0;i<10- strlen("----");++i){
                    fprintf(fp," ");
                }

            }

            fprintf(fp,"%s",idToName(pTree->parentId));
            len = strlen(idToName(pTree->parentId));
            templen= 30-len;
            for(i=0;i<templen;++i){
                fprintf(fp," ");
            }

            //leaf node. Hence, yes.
            fprintf(fp,"Yes");
            for(i=0;i<20- strlen("Yes");++i){
                fprintf(fp," ");
            }

            fprintf(fp,"----");
            for(i=0;i<30- strlen("----");++i){
                fprintf(fp," ");
            }


        }

        fprintf(fp,"\n");
    }
    else{
        // printf("%s",idToName(pTree->id));
        // 	while(getchar()!='\n');
        fprintf(fp,"----");
        for(i=0;i<30- strlen("----");++i){
            fprintf(fp," ");
        }

        fprintf(fp,"----");
        for(i=0;i<10-strlen("----");++i){
            fprintf(fp," ");
        }
        fprintf(fp,"----");
        for(i=0;i<20-strlen("----");++i){
            fprintf(fp," ");
        }
        fprintf(fp,"----");
        for(i=0;i<10-strlen("----");++i){
            fprintf(fp," ");
        }

        fprintf(fp,"%s",idToName(pTree->parentId));
        for(i=0;i<30- strlen(idToName(pTree->parentId));++i){
            fprintf(fp," ");
        }

        //Not leaf node. Hence, no.
        fprintf(fp,"No");
        for(i=0;i<20- strlen("No");++i){
            fprintf(fp," ");
        }
        fprintf(fp,"%s",idToName(pTree->id));

        len=strlen(idToName(pTree->id));
        for(i=0;i<30-len;++i){
            fprintf(fp," ");
        }
        fprintf(fp,"\n");

        for(i=0;i<pTree->numChildren;++i){
            printASTHelper(&pTree->children[i], fp);
        }

    }
    fflush(fp);
}

void printAST(astNode PT, char *parseTreeDisplayFile){
    printf("Outfile name: %s\n",parseTreeDisplayFile);
    int ch;
    FILE* fp;
    if(strcmp(parseTreeDisplayFile,"console")==0)
        fp=stdout;
    else
        fp= fopen(parseTreeDisplayFile, "w");
    if(fp==NULL){
        printf("File open error or console error: %s",parseTreeDisplayFile);
        return;
    }
    //changed, for aesthetic purposes (only number of spaces changed, for better formatting)
    fprintf(fp, "lexemeCurrentNode             lineno    token               valOfNum  parentNodeSymbol              isLeafNode(yes/no)  NodeSymbol\n\n\n");

    printASTHelper(&PT, fp);

}

void getNumNodesAST(astNode ast, int *nodes){
    *nodes= (*nodes) + 1;
    int i;
    for(i=0; i<ast.numChildren ; ++i){
        getNumNodesAST(ast.children[i], nodes);
    }
}
