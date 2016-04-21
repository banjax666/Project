/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

bool semantic_flag = true;
int main(int argc, char *argv[]){

	FILE* fpSource;
    
	buffersize k = 1000; //arbitary buffer size   
	buffer B= (char*)malloc(k*sizeof(char));
	
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
	
	do{
		printf("\nType option:\n\n");
		printf("0:Quit\n");
		printf("1:remove comments\n");
		printf("2:print list of tokens\n");
		printf("3:parse\n");
		printf("4:print parse tree\n");
		scanf("%d",&z);
		switch(z){
			case 0:
				break;
			case 1:
				commentRemover(fpSource);
				printf("1 done");
				break;
			case 2:
				printAllTokens(fpSource, B,k);
				printf("2 done");				
				break;
			case 3:
				updateFilePointer();
				getNextTokenHelper(fpSource, B, k, &tokenCurrent);
				parseReturn =  parseInputSourceCode(fpSource, B, k, &tokenCurrent, &root, G , PT);
				updateFilePointer();
				printf("Parsing routine complete\n");
				fseek(fpSource,0,SEEK_SET);
				flag=1;
				printf("3 done");
				break;
			case 4:
				if(flag==0){
					printf("Please run the parse option first\n");
					break;
				}

				printParseTree(root, argv[2]);
				printf("Printed to file: %s\n",argv[2]);
				printf("4 done");
				break;
		}
	}while(z);
	return 0;
}
