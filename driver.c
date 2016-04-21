/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

bool semantic_flag = true;
int main(int argc, char *argv[]){

    FILE* fpSource;

    buffersize k = 1000; //arbitary buffer size
    buffer B= (char*)malloc(k*sizeof(char));
    int astNodes=0, parseTreeNodes=0;
    unsigned long astMem, pTreeMem;

    if(argc != 3) {
        printf("Please run command as: $./stage1exe testcase.txt parsetreeOutFile.txt\n");
        return 0;
    }

    fpSource = fopen(argv[1], "r");
    if(fpSource== NULL) {
        printf("File open error: %s\n",argv[1]);
        return 0;
    }

    grammar G;
    getGrammar(G,"grammar.txt");
    printf("Both lexical and syntax analysis modules implemented\n");
    printf("Grammar stored in grammar.txt\n");

    printf("Grammar OBTAINED\n");
    firstset fi;
    followset f;
    parseTable PT;
    int parseReturn;
    parseTree root;
    astNode astRoot;
    varHashTable *temp,*global;
    recHashTable *recs;
    funcHashTable *funcs;
    tokenInfo tokenCurrent;

    int i,j;
    for(i=0;i<50;i++){
        for(j=0;j<105;j++){
            fi[i][j]=0;
        }
    }
    for(i=0;i<50;i++){
        for(j=0;j<106;j++){
            f[i][j]=0;
        }
    }
    for(i=0;i<50;i++){
        for(j=0;j<55;j++){
            PT[i][j]=-1;
        }
    }
    first(fi,G);
    printf(" AUTOMATED\n");
    follow(f,fi,G);
    printf("Both follow set and first set AUTOMATED\n");
    createParseTable(fi,f,G,PT);
    printf("Parse tree CREATED\n");

    int z=0;
    int flag=0;
    int astFlag =0,symFlag=0,recordTableFlag = 0;

    do{
        printf("\nType option:\n\n");
        printf("0:Quit\n");
        printf("1:print list of tokens\n");
        printf("2:parse, verify syntactic correctness and display parse tree\n");
        printf("3:print AST to console\n");
        printf("4:display memory usage of parse tree and AST\n");
        printf("5:display symbol table\n");
        printf("6:verify syntactic and semantic correctness\n");
        printf("7:produce assembly code in .asm file\n");
        scanf("%d",&z);
        switch(z){
            case 0:
                break;
//			case 1:
//				commentRemover(fpSource);
//				printf("1 done");
//				break;
            case 1:
                printAllTokens(fpSource, B,k);
                printf("1 done");
                break;
            case 2:
                updateFilePointer();
                getNextTokenHelper(fpSource, B, k, &tokenCurrent);
                parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
                updateFilePointer();
                printf("Parsing routine complete\n");

                fseek(fpSource,0,SEEK_SET);
                flag=1;
                printParseTree(root,"console");
                if(parseReturn!=0)
                    printf("compilation failed, syntactic errors displayed. Cannot generate AST\n");
                printf("2 done");
                break;
            case 3:
                if(flag==0){
                    updateFilePointer();
                    getNextTokenHelper(fpSource, B, k, &tokenCurrent);
                    parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
                    updateFilePointer();
                    printf("Parsing routine complete\n");
                    fseek(fpSource,0,SEEK_SET);
                    flag=1;
                }
                if(parseReturn!=0){
                    printf("compilation failed, syntactic errors displayed. Cannot generate AST\n");
                    break;
                }
                else{
                    buildAST(&root,&astRoot);
                    printAST(astRoot,"console");
                    astFlag=1;
                }
                printf("3 done");
                break;
            case 4:
                if(astFlag==0){
                    if(flag==0){
                        updateFilePointer();
                        getNextTokenHelper(fpSource, B, k, &tokenCurrent);
                        parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
                        updateFilePointer();
                        printf("Parsing routine complete\n");
                        fseek(fpSource,0,SEEK_SET);
                        flag=1;
                    }
                    if(parseReturn!=0){
                        printf("compilation failed, syntactic errors displayed. Cannot generate AST\n");
                        break;
                    }
                    else{
                        buildAST(&root,&astRoot);
                        astFlag=1;
                    }
                }
                getNumNodesAST(astRoot, &astNodes);
                getNumNodesParseTree(root, &parseTreeNodes);
                astMem=sizeof(astNode) * astNodes;
                pTreeMem=sizeof(parseTree) * parseTreeNodes;
                printf("Parse Tree  Number of Nodes=%d  Allocated Memory=%lu Bytes",parseTreeNodes,pTreeMem);
                printf("AST         Number of Nodes=%d  Allocated Memory=%lu Bytes",astNodes,astMem);
        break;
            case 5:
                if(astFlag==0){
                    if(flag==0){
                        updateFilePointer();
                        getNextTokenHelper(fpSource, B, k, &tokenCurrent);
                        parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
                        updateFilePointer();
                        printf("Parsing routine complete\n");
                        fseek(fpSource,0,SEEK_SET);
                        flag=1;
                    }
                    if(parseReturn!=0){
                        printf("compilation failed, syntactic errors displayed. Cannot generate AST\n");
                        break;
                    }
                    else{
                        buildAST(&root,&astRoot);
                        astFlag=1;
                    }
                }
        createVarTable(temp);
    if(recordTableFlag == 0){
            createRecTable(recs);
            populateRecordTable(astRoot,recs);
        recordTableFlag = 1;
    }
        //printSymbolTable(astRoot,temp,recs);
        //removeVariableTable(temp);
            case 6:
            if(symFlag == 0){
            if(astFlag==0){
                        if(flag==0){
                            updateFilePointer();
                            getNextTokenHelper(fpSource, B, k, &tokenCurrent);
                            parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
                            updateFilePointer();
                            printf("Parsing routine complete\n");
                            fseek(fpSource,0,SEEK_SET);
                            flag=1;
                        }
                        if(parseReturn!=0){
                            printf("compilation failed, syntactic errors displayed. Cannot generate AST\n");
                            break;
                        }
                        else{
                            buildAST(&root,&astRoot);
                            astFlag=1;
                        }
                    }
    if(recordTableFlag == 0){
            createRecTable(recs);
            populateRecordTable(astRoot,recs);
        recordTableFlag = 1;
    }
            createFuncTable(funcs);
            populateFunctionTable(astRoot,funcs,recs);
            createVarTable(global);
            populateGlobalTable(astRoot,global,recs);
            semantic(astRoot,funcs,recs,global,funcs);
            symFlag = 1;
        }else{
            semantic(astRoot,funcs,recs,global,funcs);
        }
        break;

        }
    }while(z);
    return 0;
}
