/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
stack_struct * top;
int nameToId(char *name) {
	if(strcmp(name, "program") == 0) return program;
	if(strcmp(name, "mainFunction") == 0) return mainFunction;
	if(strcmp(name, "otherFunctions") == 0) return otherFunctions;
	if(strcmp(name, "function") == 0) return function;
	if(strcmp(name, "input_par") == 0) return input_par;
	if(strcmp(name, "output_par") == 0) return output_par;
	if(strcmp(name, "parameter_list") == 0) return parameter_list;
	if(strcmp(name, "dataType") == 0) return dataType;
	if(strcmp(name, "primitiveDatatype") == 0) return primitiveDatatype;
	if(strcmp(name, "constructedDatatype") == 0) return constructedDatatype;
	if(strcmp(name, "remaining_list") == 0) return remaining_list;
	if(strcmp(name, "stmts") == 0) return stmts;
	if(strcmp(name, "typeDefinitions") == 0) return typeDefinitions;
	if(strcmp(name, "typeDefinition") == 0) return typeDefinition;
	if(strcmp(name, "fieldDefinitions") == 0) return fieldDefinitions;
	if(strcmp(name, "fieldDefinition") == 0) return fieldDefinition;
	if(strcmp(name, "moreFields") == 0) return moreFields;
	if(strcmp(name, "declarations") == 0) return declarations;
	if(strcmp(name, "declaration") == 0) return declaration;
	if(strcmp(name, "global_or_not") == 0) return global_or_not;
	if(strcmp(name, "otherStmts") == 0) return otherStmts;
	if(strcmp(name, "stmt") == 0) return stmt;
	if(strcmp(name, "assignmentStmt") == 0) return assignmentStmt;
	if(strcmp(name, "singleOrRecId") == 0) return singleOrRecId;
    if(strcmp(name, "singleOrRecIdPrime") == 0) return singleOrRecIdPrime;
	if(strcmp(name, "funCallStmt") == 0) return funCallStmt;
	if(strcmp(name, "outputParameters") == 0) return outputParameters;
	if(strcmp(name, "inputParameters") == 0) return inputParameters;
	if(strcmp(name, "iterativeStmt") == 0) return iterativeStmt;
	if(strcmp(name, "conditionalStmt") == 0) return conditionalStmt;
	if(strcmp(name, "elsePart") == 0) return elsePart;
	if(strcmp(name, "ioStmt") == 0) return ioStmt;
	if(strcmp(name, "allVar") == 0) return allVar;
	if(strcmp(name, "arithmeticExpression") == 0) return arithmeticExpression;
	if(strcmp(name, "expPrime") == 0) return expPrime;
	if(strcmp(name, "term") == 0) return term;
	if(strcmp(name, "termPrime") == 0) return termPrime;
	if(strcmp(name, "factor") == 0) return factor;
	if(strcmp(name, "highPrecedenceOperators") == 0) return highPrecedenceOperators;
	if(strcmp(name, "lowPrecedenceOperators") == 0) return lowPrecedenceOperators;
	if(strcmp(name, "all") == 0) return all;
	if(strcmp(name, "temp") == 0) return temp;
	if(strcmp(name, "booleanExpression") == 0) return booleanExpression;
	if(strcmp(name, "var") == 0) return var;
	if(strcmp(name, "logicalOp") == 0) return logicalOp;
	if(strcmp(name, "relationalOp") == 0) return relationalOp;
	if(strcmp(name, "returnStmt") == 0) return returnStmt;
	if(strcmp(name, "optionalReturn") == 0) return optionalReturn;
	if(strcmp(name, "idList") == 0) return idList;
	if(strcmp(name, "more_ids") == 0) return more_ids;
	if(strcmp(name, "TK_ASSIGNOP") == 0) return TK_ASSIGNOP ;
	if(strcmp(name, "TK_COMMENT") == 0) return TK_COMMENT ;
	if(strcmp(name, "TK_FIELDID") == 0) return TK_FIELDID ;
	if(strcmp(name, "TK_ID") == 0) return TK_ID ;
	if(strcmp(name, "TK_NUM") == 0) return TK_NUM ;
	if(strcmp(name, "TK_RNUM") == 0) return TK_RNUM ;
	if(strcmp(name, "TK_FUNID") == 0) return TK_FUNID ;
	if(strcmp(name, "TK_RECORDID") == 0) return TK_RECORDID ;
	if(strcmp(name, "TK_WITH") == 0) return TK_WITH ;
	if(strcmp(name, "TK_PARAMETERS") == 0) return TK_PARAMETERS ;
	if(strcmp(name, "TK_END") == 0) return TK_END ;
	if(strcmp(name, "TK_WHILE") == 0) return TK_WHILE ;
	if(strcmp(name, "TK_INT") == 0) return TK_INT ;
	if(strcmp(name, "TK_REAL") == 0) return TK_REAL ;
	if(strcmp(name, "TK_TYPE") == 0) return TK_TYPE ;
	if(strcmp(name, "TK_MAIN") == 0) return TK_MAIN ;
	if(strcmp(name, "TK_GLOBAL") == 0) return TK_GLOBAL ;
	if(strcmp(name, "TK_PARAMETER") == 0) return TK_PARAMETER ;
	if(strcmp(name, "TK_LIST") == 0) return TK_LIST ;
	if(strcmp(name, "TK_SQL") == 0) return TK_SQL ;
	if(strcmp(name, "TK_SQR") == 0) return TK_SQR ;
	if(strcmp(name, "TK_INPUT") == 0) return TK_INPUT ;
	if(strcmp(name, "TK_OUTPUT") == 0) return TK_OUTPUT  ;
	if(strcmp(name, "TK_SEM") == 0) return TK_SEM ;
	if(strcmp(name, "TK_COLON") == 0) return TK_COLON ;
	if(strcmp(name, "TK_DOT") == 0) return TK_DOT;
	if(strcmp(name, "TK_COMMA") == 0) return TK_COMMA;
	if(strcmp(name, "TK_ENDWHILE") == 0) return TK_ENDWHILE ;
	if(strcmp(name, "TK_OP") == 0) return TK_OP ;
	if(strcmp(name, "TK_CL") == 0) return TK_CL ;
	if(strcmp(name, "TK_IF") == 0) return TK_IF ;
	if(strcmp(name, "TK_THEN") == 0) return TK_THEN ;
	if(strcmp(name, "TK_ENDIF") == 0) return TK_ENDIF ;
	if(strcmp(name, "TK_READ") == 0) return TK_READ ;
	if(strcmp(name, "TK_WRITE") == 0) return TK_WRITE ;
	if(strcmp(name, "TK_RETURN") == 0) return TK_RETURN ;
	if(strcmp(name, "TK_PLUS") == 0) return TK_PLUS ;
	if(strcmp(name, "TK_MINUS") == 0) return TK_MINUS ;
	if(strcmp(name, "TK_MUL") == 0) return TK_MUL ;
	if(strcmp(name, "TK_DIV") == 0) return TK_DIV ;
	if(strcmp(name, "TK_CALL") == 0) return TK_CALL ;
	if(strcmp(name, "TK_RECORD") == 0) return TK_RECORD ;
	if(strcmp(name, "TK_ENDRECORD") == 0) return TK_ENDRECORD ;
	if(strcmp(name, "TK_ELSE") == 0) return TK_ELSE ;
	if(strcmp(name, "TK_AND") == 0) return TK_AND ;
	if(strcmp(name, "TK_OR") == 0) return TK_OR ;
	if(strcmp(name, "TK_NOT") == 0) return TK_NOT ;
	if(strcmp(name, "TK_LT") == 0) return TK_LT;
	if(strcmp(name, "TK_LE") == 0) return TK_LE;
	if(strcmp(name, "TK_EQ") == 0) return TK_EQ ;
	if(strcmp(name, "TK_GT") == 0) return TK_GT ;
	if(strcmp(name, "TK_GE") == 0) return TK_GE ;
	if(strcmp(name, "TK_NE") == 0) return TK_NE;
    if(strcmp(name, "TK_EOF") == 0) return TK_EOF;
	if(strcmp(name, "eps") == 0) return eps;
	if(strcmp(name, "TK_ERROR") == 0) return TK_ERROR;
    return -1;
}

char *idToName(int id) {
	// returns the printable representation of an id
	if(id == program) return "program";
	if(id == mainFunction) return "mainFunction";
	if(id == otherFunctions) return "otherFunctions";
	if(id == function) return "function";
	if(id == input_par) return "input_par";
	if(id == output_par) return "output_par";
	if(id == parameter_list) return "parameter_list";
	if(id == dataType) return "dataType";
	if(id == primitiveDatatype) return "primitiveDatatype";
	if(id == constructedDatatype) return "constructedDatatype";
	if(id == remaining_list) return "remaining_list";
	if(id == stmts) return "stmts";
	if(id == typeDefinitions) return "typeDefinitions";
	if(id == typeDefinition) return "typeDefinition";
	if(id == fieldDefinitions) return "fieldDefinitions";
	if(id == fieldDefinition) return "fieldDefinition";
	if(id == moreFields) return "moreFields";
	if(id == declarations) return "declarations";
	if(id == declaration) return "declaration";
	if(id == global_or_not) return "global_or_not";
	if(id == otherStmts) return "otherStmts";
	if(id == stmt) return "stmt";
	if(id == assignmentStmt) return "assignmentStmt";
	if(id == singleOrRecId) return "singleOrRecId";
	if(id == singleOrRecIdPrime) return "singleOrRecIdPrime";
	if(id == funCallStmt) return "funCallStmt";
	if(id == outputParameters) return "outputParameters";
	if(id == inputParameters) return "inputParameters";
	if(id == iterativeStmt) return "iterativeStmt";
	if(id == conditionalStmt) return "conditionalStmt";
	if(id == elsePart) return "elsePart";
	if(id == ioStmt) return "ioStmt";
	if(id == allVar) return "allVar";
	if(id == arithmeticExpression) return "arithmeticExpression";
	if(id == expPrime) return "expPrime";
	if(id == term) return "term";
	if(id == termPrime) return "termPrime";
	if(id == factor) return "factor";
	if(id == highPrecedenceOperators) return "highPrecedenceOperators";
	if(id == lowPrecedenceOperators) return "lowPrecedenceOperators";
	if(id == all) return "all";
	if(id == temp) return "temp";
	if(id == booleanExpression) return "booleanExpression";
	if(id == var) return "var";
	if(id == logicalOp) return "logicalOp";
	if(id == relationalOp) return "relationalOp";
	if(id == returnStmt) return "returnStmt";
	if(id == optionalReturn) return "optionalReturn";
	if(id == idList) return "idList";
	if(id == more_ids) return "more_ids";
	if(id == TK_ASSIGNOP) return "TK_ASSIGNOP";
	if(id == TK_COMMENT) return "TK_COMMENT";
	if(id == TK_FIELDID) return "TK_FIELDID";
	if(id == TK_ID) return "TK_ID";
	if(id == TK_NUM) return "TK_NUM";
	if(id == TK_RNUM) return "TK_RNUM";
	if(id == TK_FUNID) return "TK_FUNID";
	if(id == TK_RECORDID) return "TK_RECORDID";
	if(id == TK_WITH) return "TK_WITH";
	if(id == TK_PARAMETERS) return "TK_PARAMETERS";
	if(id == TK_END) return "TK_END";
	if(id == TK_WHILE) return "TK_WHILE";
	if(id == TK_INT) return "TK_INT";
	if(id == TK_REAL) return "TK_REAL";
	if(id == TK_TYPE) return "TK_TYPE";
	if(id == TK_MAIN) return "TK_MAIN";
	if(id == TK_GLOBAL) return "TK_GLOBAL";
	if(id == TK_PARAMETER) return "TK_PARAMETER";
	if(id == TK_LIST) return "TK_LIST";
	if(id == TK_SQL) return "TK_SQL";
	if(id == TK_SQR) return "TK_SQR";
	if(id == TK_INPUT) return "TK_INPUT";
	if(id == TK_OUTPUT) return "TK_OUTPUT";
	if(id == TK_SEM) return "TK_SEM";
	if(id == TK_COLON) return "TK_COLON";
	if(id == TK_DOT) return "TK_DOT";
	if(id == TK_COMMA) return "TK_COMMA";
	if(id == TK_ENDWHILE) return "TK_ENDWHILE";
	if(id == TK_OP) return "TK_OP";
	if(id == TK_CL) return "TK_CL";
	if(id == TK_IF) return "TK_IF";
	if(id == TK_THEN) return "TK_THEN";
	if(id == TK_ENDIF) return "TK_ENDIF";
	if(id == TK_READ) return "TK_READ";
	if(id == TK_WRITE) return "TK_WRITE";
	if(id == TK_RETURN) return "TK_RETURN";
	if(id == TK_PLUS) return "TK_PLUS";
	if(id == TK_MINUS) return "TK_MINUS";
	if(id == TK_MUL) return "TK_MUL";
	if(id == TK_DIV) return "TK_DIV";
	if(id == TK_CALL) return "TK_CALL";
	if(id == TK_RECORD) return "TK_RECORD";
	if(id == TK_ENDRECORD) return "TK_ENDRECORD";
	if(id == TK_ELSE) return "TK_ELSE";
	if(id == TK_AND) return "TK_AND";
	if(id == TK_OR) return "TK_OR";
	if(id == TK_NOT) return "TK_NOT";
	if(id == TK_LT) return "TK_LT";
	if(id == TK_LE) return "TK_LE";
	if(id == TK_EQ) return "TK_EQ";
	if(id == TK_GT) return "TK_GT";
	if(id == TK_GE) return "TK_GE";
	if(id == TK_NE) return "TK_NE";
	if(id == TK_EOF) return "TK_EOF";
	if(id == eps) return "eps";
	return "";
}


void traverse(){

	struct stack_struct *var=top;
     if(var!=NULL)
     { 
          printf("\nElements are as:\n");
          while(var!=NULL)
          {
               printf("\t%s\n",idToName(var->data));
               var=var->below;
          } 
     printf("\n");
     }
     else
     printf("\nStack is Empty");

}

void pop(){

	
	struct stack_struct *tempnode, *var=top;
    if(var==top)
    {
        top = top->below;
        free(var);
    }
    else
    printf("\nStack Empty");
	// if(top==NULL){
	// 	printf("Stack empty,cannot pop");
	// 	return;
	// }
	// top=top->below;
}
void push(IDS element){
	stack_struct* tempnode;
    tempnode=(stack_struct*)malloc(sizeof(stack_struct));
    tempnode->data=element;
    if (top == NULL)
    {
         top=tempnode;
         top->below=NULL;
    }
    else
    {
        tempnode->below=top;
        top=tempnode;
    }

	// if(top==NULL){
	// 	top=(stack_struct*)malloc(sizeof(stack_struct));
	// 	top->data=element;
	// 	top->below=NULL;
	// 	printf("stack:");
	// 	traverse(top);
	// 	printf("Press Enter to Continue");

	// }
	// else{
	// 	tempNode=(stack_struct*)malloc(sizeof(stack_struct));
	// 	tempNode->data=element;
	// 	tempNode->below=top;
	// 	top=tempNode;	
	// }
}
IDS topElement(){

	return top->data;
}



int isTerm(int x){
	if(x>49){
		return 1;
	}
	return 0;
}

int hasEPS(grammar G,int x){
	int i=0;
	while(i<89){
		if(G[i].LHS==x){
			if(G[i].RHS[0]==eps){
				return 1;
			}	
		}
		i++;
	}
	return 0;
}

void getGrammar(grammar G,char* grammarFile){
	FILE *fp;
	fp = fopen(grammarFile,"r");
	int i=0,j;
	char x[100],f[10];
	char *z;
	while(i<89){
		fscanf(fp,"%d %s %s",&j,x,f);
		G[i].LHS=nameToId(x);
		G[i].numberOfRHS=j;
		while(j>0){
			fscanf(fp,"%s",x);
			G[i].RHS[G[i].numberOfRHS-j]=nameToId(x);
			j--;
		}
	i++;
	}
	fclose(fp);
}

void firstHelper(firstset f,grammar G,int lhs,int *c){
	int i=0;
	c[lhs]=1;
	while(i<89){
		if(G[i].LHS==lhs){
			if(isTerm(G[i].RHS[0])){
				int k=0;
				while(k<50){
					if(c[k]==1){
						f[k][G[i].RHS[0]]=1;
						//c[k]=0; fail
					}
					c[lhs]=1;
					k++;
				}
			}else{
				int j=0;
				do{
					if(!isTerm(G[i].RHS[j])){
					firstHelper(f,G,G[i].RHS[j],c);
					c[G[i].RHS[j]]=0;
					}else{
						f[lhs][G[i].RHS[j]]=1;
					}
					j++;
				}while(hasEPS(G,G[i].RHS[j-1]));
			}
		}
		
		i++;
	}
}

void first1(firstset f,grammar G,int i){
	int k=0;
	int check[50];
	while(k<50){
		check[k]=0;
		k++;
	}
	firstHelper(f,G,G[i].LHS,check);
}

void first(firstset f,grammar G){
	int i=0;
	while(i<89){
		first1(f,G,i);
		i++;
	}
}

void followhelper(followset f,firstset fi,grammar G,int prod,int pos){
	if(pos==G[prod].numberOfRHS-1){
		int i=0;
		while(i<106){
			f[G[prod].RHS[pos]][i]=f[G[prod].RHS[pos]][i]|f[G[prod].LHS][i];
			i++;
		}
	}else{
		int k=pos+1;
		while(k<G[prod].numberOfRHS){
			if(isTerm(G[prod].RHS[k])){
				f[G[prod].RHS[pos]][G[prod].RHS[k]]=1;
				break;
			}else{
				int i=0;
				while(i<104){
					f[G[prod].RHS[pos]][i]=f[G[prod].RHS[pos]][i]|fi[G[prod].RHS[k]][i];
					i++;
				}
			}
			if(fi[G[prod].RHS[k]][eps]==1){
				k++;
			}else{
				return;
			}
		}
		if(!isTerm(G[prod].RHS[G[prod].numberOfRHS-1])){
			int i=0;
			while(i<106){
			f[G[prod].RHS[pos]][i]=f[G[prod].RHS[pos]][i]|f[G[prod].LHS][i];
			i++;
			}
		}
	}
}

void follow1(followset f,firstset fi,grammar G,int id){
	int i=0,k=0;
	while(i<89){
		int j=0;
		while(j<G[i].numberOfRHS){
			if(id==G[i].RHS[j]){
				followhelper(f,fi,G,i,j);
			}
			j++;
		}
		i++;
	}
}

void follow(followset f,firstset fi,grammar G){
	f[0][eps+1]=1;
	int i=1;
	while(i<50){
		follow1(f,fi,G,i);
		i++;
	}
}

void PTHelper(parseTable PT,grammar G,int lhs,int *c){
	int i=0;
	c[lhs]=1;
	while(i<89){
		if(G[i].LHS==lhs){
			c[lhs]=i;
			if(isTerm(G[i].RHS[0])){
				int k=0;
				while(k<50){
					if(c[k]>=0){
						if(G[i].RHS[0]!=eps){
						PT[k][G[i].RHS[0]-50]=c[k];
						}//c[k]=0; fail
					}
					c[lhs]=i;
					k++;
				}
			}else{
				int j=0;
				do{
					if(!isTerm(G[i].RHS[j])){
					PTHelper(PT,G,G[i].RHS[j],c);
					c[G[i].RHS[j]]=-1;
					}else{
						if(G[i].RHS[0]!=eps){
						PT[lhs][G[i].RHS[j]-50]=i;
						}
					}
					j++;
				}while(hasEPS(G,G[i].RHS[j-1]));
			}
		}
		
		i++;
	}
}

void PTHelper1(parseTable PT,grammar G,int i){
	int k=0;
	int check[50];
	while(k<50){
		check[k]=-1;
		k++;
	}
	PTHelper(PT,G,G[i].LHS,check);
}

void createParseTable(firstset fi,followset f,grammar G,parseTable PT){
	int i=0;
	while(i<89){
		PTHelper1(PT,G,i);
		if(G[i].RHS[0]==eps){
			int k=0;
			while(k<106){
				if(f[G[i].LHS][k]==1){
					PT[G[i].LHS][k-50]=i;
				}
				k++;
			}
		}
		i++;
	}
	
}
//(fpSource, B, k, &tokenCurrent, &root, G , PT)
int parseInputSourceCode(FILE *fpSource, buffer B, buffersize k, tokenInfo *tokenCurrent, parseTree* root, grammar G, table PT) {
    
    if(nameToId(tokenCurrent->tokenType) == TK_ERROR) {
        printf("%s\n",tokenCurrent->errorMsg); //ask monks how he is storing error msg
        
        return -1;
    }
    if(isTerm(root->id) == 1) {
        
        if(nameToId(root->token.tokenType) == eps)
            return 0;
        else if(nameToId(root->token.tokenType) != nameToId(tokenCurrent->tokenType)) {
            if(nameToId(tokenCurrent->tokenType)==TK_EOF){
            	printf("ERROR_4: Input is consumed while it is expected to have token <%s> at line number <%lu> \n", root->token.tokenType, tokenCurrent->lineNumber);
            }
            else{ 
            	printf("ERROR_5: The token<%s> for lexeme<%s> does not match at line <%lu>. The expected token here is <%s> \n", tokenCurrent->tokenType,tokenCurrent->lexeme, tokenCurrent->lineNumber,root->token.tokenType);	
	            //while(getchar()!='\n');
	            return -1;
        	}
        }
        else {
            // move the input pointer forward and set the lexeme for the current node
            strcpy(root->token.lexeme, tokenCurrent->lexeme);
            root->token.lineNumber = tokenCurrent->lineNumber;
            getNextTokenHelper(fpSource, B, k, tokenCurrent);
            return 0;
        }
    }
    else {
        // add a production for the root and proceed recursively
        if(PT[root->id][nameToId(tokenCurrent->tokenType)-50] == -1) {
            // generate an error message stating legal tokens at this point
            printf("ERROR_5: The token<%s> for lexeme<%s> does not match at line <%lu>. The expected token here is of type <%s> \n", tokenCurrent->tokenType,tokenCurrent->lexeme, tokenCurrent->lineNumber,idToName(root->id));
            //while(getchar()!='\n');
            return -1;
        }
        else {
            
            int i;

            root->numChildren = G[PT[root->id][nameToId(tokenCurrent->tokenType)-50]].numberOfRHS; // the rule length is the no. of children
            root->children = (parseTree *)malloc(root->numChildren * sizeof(parseTree));
            for(i = 0; i < root->numChildren; i++) {
                if(isTerm(G[PT[root->id][nameToId(tokenCurrent->tokenType)-50]].RHS[i]) == 1) {
                    
                    root->children[i].id= G[PT[root->id][nameToId(tokenCurrent->tokenType)-50]].RHS[i];
                    root->children[i].parentId= root->id;
                    strcpy(root->children[i].token.tokenType ,idToName(G[PT[root->id][nameToId(tokenCurrent->tokenType)-50]].RHS[i]));

                }
                else {
                    
                    root->children[i].id= G[PT[root->id][nameToId(tokenCurrent->tokenType)-50]].RHS[i];
                    root->children[i].parentId= root->id;
                }
                //printf("Child %d: %s \n",i,idToName(root->children[i].id));
            }

            int returnValueChild, flag = 0;
            for(i = 0; i < root->numChildren; i++) {
                
                returnValueChild = parseInputSourceCode(fpSource, B, k, tokenCurrent, &(root->children[i]), G , PT);
                if(returnValueChild == -1) {
                   
                    if(root->id == assignmentStmt ||root->id == iterativeStmt || 
                            root->id == conditionalStmt || root->id == ioStmt ||
                            root->id == funCallStmt || root->id == typeDefinition || 
                            root->id == declaration || root->id == returnStmt ||
                            root->id == mainFunction) {
                        flag = -1;
                        break;
                    }
                    else if(root->id == function) {
                        if(i >= 3) { 
                            flag = -1;
                            break;
                        }
                        else {
                            
                            while(nameToId(tokenCurrent->tokenType) != TK_SEM) {
                                getNextTokenHelper(fpSource, B, k,tokenCurrent);
                                if(nameToId(tokenCurrent->tokenType) == TK_ERROR){
                                    printf("%s\n",tokenCurrent->errorMsg);
                                    //while(getchar()!='\n');

                                }
                                else if(nameToId(tokenCurrent->tokenType) == TK_EOF)
                                    return -1;
                            }
                            getNextTokenHelper(fpSource, B, k,tokenCurrent);
                            flag = 1;
                            i = 3; // continue parsing
                        }
                    }
                    else
                        return -1;
                }
                else if(returnValueChild == 1)
                    flag = 1;
            }
            
            if(flag == -1) {
                
                int delimiterRecovery;
                if(root->id == iterativeStmt)
                    delimiterRecovery = TK_ENDWHILE;
                else if(root->id == conditionalStmt)
                    delimiterRecovery = TK_ENDIF;
                else if(root->id == function || root->id == mainFunction)
                    delimiterRecovery = TK_END;
                else
                    delimiterRecovery = TK_SEM;

                while(nameToId(tokenCurrent->tokenType)!= delimiterRecovery) {
                    getNextTokenHelper(fpSource, B, k,tokenCurrent);
                    if(nameToId(tokenCurrent->tokenType) == TK_ERROR){
                        printf("%s\n",tokenCurrent->errorMsg);
                        //while(getchar()!='\n');
                    }	
                    else if(nameToId(tokenCurrent->tokenType) == TK_EOF)
                        return -1;
                }

                flag = 1;
                getNextTokenHelper(fpSource, B, k,tokenCurrent);
            }
           
            return flag;
        }
    }
}

void printParseTreeHelper(parseTree* pTree, FILE* fp){
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
			printParseTreeHelper(&pTree->children[i], fp);
		}

	}
	fflush(fp);
}

void printParseTree(parseTree PT, char *parseTreeDisplayFile){
	printf("Outfile name: %s\n",parseTreeDisplayFile);
	int ch;
	FILE* fp  = fopen(parseTreeDisplayFile, "w");
	if(fp==NULL){
		printf("File open error: %s",parseTreeDisplayFile);
		return;
	}
	//changed, for aesthetic purposes (only number of spaces changed, for better formatting)
	fprintf(fp, "lexemeCurrentNode             lineno    token               valOfNum  parentNodeSymbol              isLeafNode(yes/no)  NodeSymbol\n\n\n");	
	
	printParseTreeHelper(&PT, fp);
}

