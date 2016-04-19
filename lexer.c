/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include "utils.h"
#include "lexerDef.h"

typedef char* buffer;
typedef unsigned long long buffersize; 
typedef unsigned long long big;
// #define EOF 99
//TODO: init k =-1
big lineNumber = 1; // the "current line" in the source file
//char buffer[SIZE_MAX_BUFFER]; // the input buffer
int currentPointer; // indicates the current character to be read from the buffer
int bufferSize = -1; // the number of characters stored in the buffer (-1 indicates first use)

FILE* getStream(FILE *fp, buffer B, buffersize k){
	memset(B, 0, k);
	bufferSize = fread(B, sizeof(char),(size_t)k,fp);
	currentPointer=0;
	return fp;

}
void clean(tokenInfo *t){
	memset(t->lexeme,0,SIZE_MAX_LEXEME);
	memset(t->tokenType,0,SIZE_MAX_TOKEN);
	memset(t->errorMsg,0,SIZE_MAX_ERROR_MSG);
	t->isError=false;
}


char getNext(FILE *fp, buffer B, buffersize k) {
	/* fetches the next character from the input stream
	 * uses buffering for efficiency
	 */
	char ch;
	if(bufferSize == -1 || currentPointer == k){
        	getStream(fp, B, k);
	}
        if(bufferSize == 0 || (bufferSize == currentPointer +1 && bufferSize<k) )
            return -1; // eof
        else
		ch=B[currentPointer];
		currentPointer++;
            	return ch;
   
}

tokenInfo getNextTokenHelper(FILE *fp, buffer B, buffersize k,tokenInfo *t) { 
	clean(t);
	char ch;
	int dfaState=1;	
        big lexemeIndex=0;
        int flag=0;
        
        while(1){
		ch=getNext(fp,B,k);
		t->lexeme[lexemeIndex++] = ch; // store the character (to be used for identifiers etc.)
        	t->lineNumber = lineNumber; // update the line number in the token
		
		switch(dfaState){
			case 1: 
				switch(ch) {
                    			//TODO: Make all tokens in double quotes 
                   			case '[': // TK_SQLfound
                      				strcpy(t->tokenType,"TK_SQL");
                        			return *t;
                    			case ']': // TK_SQR found
                        			strcpy(t->tokenType,"TK_SQR");
                        			return *t;
                    			case ';': // TK_SEM found
                        			strcpy(t->tokenType,"TK_SEM");
                        			return *t;
                    			case ':': // TK_COLON found
                        			strcpy(t->tokenType,"TK_COLON");
                        			return *t;
                   			case '.': // TK_DOT found
                   			    	strcpy(t->tokenType,"TK_DOT");
                        			return *t;
                    			case '(': // TK_OP found
                       				strcpy(t->tokenType,"TK_OP");
                        			return *t;
                    			case ')': // TK_CL found
                        			strcpy(t->tokenType,"TK_CL");
                        			return *t;
                    			case '+': // TK_PLUS found
                        			strcpy(t->tokenType,"TK_PLUS");
                        			return *t;
                   			case '-': // TK_MINUS found
                        			strcpy(t->tokenType,"TK_MINUS");
                        			return *t;
                    			case '*': // TK_MUL found
                        			strcpy(t->tokenType,"TK_MUL");
                        			return *t;
                    			case '/': // TK_DIV found
                        			strcpy(t->tokenType,"TK_DIV");
                       				return *t;
                    			case '~': // TK_NOT found
                        			strcpy(t->tokenType,"TK_NOT");
                        			return *t;
					case ',': // TK_COMMA found
                        			strcpy(t->tokenType,"TK_COMMA");
                        			return *t;
                    			case '\n':
						t->lexeme[lexemeIndex-1]='\0';
						lexemeIndex--;
                        			lineNumber++;
						break;
					case '%':
						memset(t->lexeme,0,SIZE_MAX_LEXEME);
						lexemeIndex=0;
						char c;
						c=getNext(fp,B,k);
						while(c!='\n'){
							c=getNext(fp,B,k);
						}
						lineNumber++;
						break;
					case '#':
						dfaState = 2;
						break;
                    			case 'b':
					case 'c':
					case 'd':
						dfaState =3;
						break;
					case '_':
						dfaState=5;
						break;
					case '<':
						dfaState=7;
						break;
					case '>':
						dfaState=8;
						break;
					case '=':
						dfaState=9;
						break;
					case '!':
						dfaState=25;
						break;
					case '&':
						dfaState=11;
						break;
					case '@':
						dfaState=12;
						break;
					case ' ' :
					case '\t':
						t->lexeme[lexemeIndex-1]='\0';
						lexemeIndex--;
						break;
					case EOF: // EOF
                        			strcpy(t->tokenType,"TK_EOF");
                        			return *t;
					default :
						if(ch >= 'a' && ch<='z'){
						dfaState =4;
						break;
						}
						else if(ch >= '0' && ch<='9'){ 
						dfaState=6;
						break;	
						}
						else{
						strcpy(t->tokenType,"TK_ERROR");
						t->isError=true;
			                        sprintf(t->errorMsg, "ERROR_2: Unknown Symbol <%c> at line %llu", ch,lineNumber);
			                        return *t;
			     			} 
					}  
			break;
			case 2:
				if(ch>='z'||ch<='a'){
					if(ch == '\n' || ch == '\r' || ch == ' ' || ch == '\v' || ch == '\f' || ch == '\t' || ch == EOF){
                            			sprintf(t->errorMsg, "ERROR_3:  Unknown pattern # at <%llu>",lineNumber);
						t->isError= true;	
					}
					else
                            			sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
				strcpy(t->tokenType,"TK_ERROR");
                        	return *t;				
				}                       		
				else{
					ch=getNext(fp,B,k);					
					while(ch>='a'&&ch<='z'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);
					}					
					if(ch!=EOF){
					currentPointer--;
					}
					strcpy(t->tokenType,"TK_RECORDID");							
					return *t;
				}
			break;
			//TODO: CHECK THIS SHIT			
			case 3:
				if(ch>='2'&& ch<='7'){
					ch=getNext(fp,B,k); 
					if(ch>='b'&& ch<='d'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);						
						while(ch>='b'&& ch<='d'){
							t->lexeme[lexemeIndex++]=ch;
							ch=getNext(fp,B,k);
						}	                                        	
						if(ch>='2'&& ch<='7'){
							t->lexeme[lexemeIndex++]=ch;
							ch=getNext(fp,B,k);						
							while(ch>='2'&& ch<='7'){
								t->lexeme[lexemeIndex++]=ch;
								ch=getNext(fp,B,k);
							}
							if(ch!=EOF){
								currentPointer--;
							}
                                                        if(lexemeIndex>20){
								strcpy(t->tokenType,"TK_ERROR");
								t->isError=true;
                            					sprintf(t->errorMsg, "ERROR_1 : Identifier at line <%llu>is longer than the prescribed length of 20 characters",lineNumber);
                            					return *t;
							}
							else{
							strcpy(t->tokenType,"TK_ID");							
							return *t;

							}	
							
						}
						else{
							if(ch!=EOF){
								currentPointer--;
							}
							if(lexemeIndex>20){
								strcpy(t->tokenType,"TK_ERROR");
								t->isError=true;
                            					sprintf(t->errorMsg, "ERROR_1 : Identifier at line <%llu>is longer than the prescribed length of 20 characters",lineNumber);
                            					return *t;
							}
							else{
							strcpy(t->tokenType,"TK_ID");							
							return *t;

							}	
							
							
						}
                                        
					}
					else if(ch>='2'&& ch<='7'){
						t->lexeme[lexemeIndex++]=ch;
							ch=getNext(fp,B,k);						
							while(ch>='2'&& ch<='7'){
								t->lexeme[lexemeIndex++]=ch;
								ch=getNext(fp,B,k);
							}
							if(ch!=EOF){
								currentPointer--;
							}
							if(lexemeIndex>20){
								strcpy(t->tokenType,"TK_ERROR");
								t->isError=true;
                            					sprintf(t->errorMsg, "ERROR_1 : Identifier at line <%llu>is longer than the prescribed length of 20 characters",lineNumber);
                            					return *t;
							}
							else{
							strcpy(t->tokenType,"TK_ID");							
							return *t;

							}	
					}
					else{
						if(ch!=EOF){
							currentPointer--;
						}
						if(lexemeIndex>20){
							strcpy(t->tokenType,"TK_ERROR");
							t->isError=true;
                            				sprintf(t->errorMsg, "ERROR_1 : Identifier at line <%llu>is longer than the prescribed length of 20 characters",lineNumber);       					return *t;
							}
						else{
							strcpy(t->tokenType,"TK_ID");							
							return *t;

						}	
					}					
				}

				else if(ch>='a'&& ch<='z'){
					dfaState=4;
					break;				
				}
				
				else{
					if(ch!=EOF){
						currentPointer--;
					}
					t->lexeme[lexemeIndex-1]='\0';
					strcpy(t->tokenType,"TK_FIELDID");							
					return *t;				
				}
				 
			break; //breaks case 3 
			case 4:
				flag=0;
				if(ch >= 'a' && ch<='z'){
					flag =1;
					ch=getNext(fp,B,k);					
					while(ch >= 'a' && ch<='z'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);					
					}
			 	}//TODO: double quotes
				if(ch!=EOF){
					currentPointer--;
				}
				if(!flag){
					t->lexeme[lexemeIndex-1]='\0';
				}
				if(strcmp(t->lexeme, "with") == 0)
                               		strcpy(t->tokenType,"TK_WITH");
                        	else if(strcmp(t->lexeme, "parameters") == 0)
                        	    	strcpy(t->tokenType,"TK_PARAMETERS");
                       	 	else if(strcmp(t->lexeme, "end") == 0)
                            		strcpy(t->tokenType,"TK_END");
                        	else if(strcmp(t->lexeme, "while") == 0)
                            		strcpy(t->tokenType,"TK_WHILE");
                        	else if(strcmp(t->lexeme, "int") == 0)
                            		strcpy(t->tokenType,"TK_INT");
                        	else if(strcmp(t->lexeme, "real") == 0)
                            		strcpy(t->tokenType,"TK_REAL");
                        	else if(strcmp(t->lexeme, "type") == 0)
                            		strcpy(t->tokenType,"TK_TYPE");
                        	else if(strcmp(t->lexeme, "global") == 0)
                            		strcpy(t->tokenType,"TK_GLOBAL");
                        	else if(strcmp(t->lexeme, "parameter") == 0)
                            		strcpy(t->tokenType,"TK_PARAMETER");
                        	else if(strcmp(t->lexeme, "list") == 0)
                            		strcpy(t->tokenType,"TK_LIST");
                        	else if(strcmp(t->lexeme, "input") == 0)
                            		strcpy(t->tokenType,"TK_INPUT");
                        	else if(strcmp(t->lexeme, "output") == 0)
                            		strcpy(t->tokenType,"TK_OUTPUT");
                        	else if(strcmp(t->lexeme, "endwhile") == 0)
                            		strcpy(t->tokenType,"TK_ENDWHILE");
                        	else if(strcmp(t->lexeme, "if") == 0)
                            		strcpy(t->tokenType,"TK_IF");
                        	else if(strcmp(t->lexeme, "then") == 0)
                            		strcpy(t->tokenType,"TK_THEN");
                        	else if(strcmp(t->lexeme, "endif") == 0)
                            		strcpy(t->tokenType,"TK_ENDIF");
                        	else if(strcmp(t->lexeme, "read") == 0)
                            		strcpy(t->tokenType,"TK_READ");
                       		else if(strcmp(t->lexeme, "write") == 0)
                            		strcpy(t->tokenType,"TK_WRITE");
                        	else if(strcmp(t->lexeme, "return") == 0)
                            		strcpy(t->tokenType,"TK_RETURN");
                        	else if(strcmp(t->lexeme, "call") == 0)
                            		strcpy(t->tokenType,"TK_CALL");
                        	else if(strcmp(t->lexeme, "record") == 0)
                            		strcpy(t->tokenType,"TK_RECORD");
                        	else if(strcmp(t->lexeme, "endrecord") == 0)
                            		strcpy(t->tokenType,"TK_ENDRECORD");
                        	else if(strcmp(t->lexeme, "else") == 0)
                            		strcpy(t->tokenType,"TK_ELSE");
				else{
					strcpy(t->tokenType,"TK_FIELDID");
				}				
				return *t;
							
			break;
			case 5:
				if((ch >= 'a' && ch<='z' ) || (ch >= 'A' && ch<='Z') ){
					ch=getNext(fp,B,k);					
					while(ch >= 'a' && ch<='z' || ch >= 'A' && ch<='Z' ){
						
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);					
					}
					while(ch >= '0' && ch<='9'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);							
					}
					if(ch!=EOF){
						currentPointer--;
					}
				        if(lexemeIndex>30){
						strcpy(t->tokenType,"TK_ERROR");
						t->isError=true;
                            			sprintf(t->errorMsg, "ERROR_1 : Identifier at line <%llu>is longer than the prescribed length of 30 characters",lineNumber);
                            			return *t;
					}
					if(strcmp(t->lexeme, "_main") == 0){
                            			strcpy(t->tokenType,"TK_MAIN");
						return *t;
					}					
					strcpy(t->tokenType,"TK_FUNID");
					return *t;	
												
				}
				else{
					if(ch!=EOF){
						currentPointer--;
					}				
					strcpy(t->tokenType,"TK_ERROR");
					t->isError=true;
                            		sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            		return *t;
									
				}
			break;
			case 6:
				flag=0;
				while(ch>='0' && ch <='9'){
					flag=1;
					t->lexeme[lexemeIndex++]=ch;
					ch=getNext(fp,B,k);								
				}
				if(ch=='.'){			
										
					ch=getNext(fp,B,k);						
					if(ch>='0' && ch <='9'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);
						if(ch>='0' && ch <='9'){
							t->lexeme[lexemeIndex++]=ch;
							strcpy(t->tokenType,"TK_RNUM");
							return *t;	
						}
						else{
							if(ch!=EOF){
								currentPointer--;
							}					
							strcpy(t->tokenType,"TK_ERROR");
							t->isError=true;
							t->lexeme[lexemeIndex++]=ch;                       				
							sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            				return *t;					
						}
					
					}
					else{
							if(ch!=EOF){
								currentPointer--;
							}					
							strcpy(t->tokenType,"TK_ERROR");
							t->isError=true;
							t->lexeme[lexemeIndex++]=ch;
                            				sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            				return *t;					
					}	
				
				}
				else{
					if(!flag){
						t->lexeme[lexemeIndex-1]='\0';						
					}
					if(ch!=EOF){
						currentPointer--;
					}				
					strcpy(t->tokenType,"TK_NUM");
					return *t;				
				}
					
			break;
			case 7:
				if(ch=='='){
					strcpy(t->tokenType,"TK_LE");
					return *t;					
				}
				else if(ch=='-'){
					ch=getNext(fp,B,k);
					if(ch=='-'){
						t->lexeme[lexemeIndex++]=ch;
						ch=getNext(fp,B,k);
						if(ch=='-'){
							t->lexeme[lexemeIndex++]=ch;							
							strcpy(t->tokenType,"TK_ASSIGNOP");
							return *t;
						}
						else{
							if(ch!=EOF){
								currentPointer--;
							}
							t->lexeme[lexemeIndex++]=ch;					
							strcpy(t->tokenType,"TK_ERROR");
							t->isError=true;
                            				sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            				return *t;
						}
					}
					else{
						if(ch!=EOF){
							currentPointer--;
						}
						t->lexeme[lexemeIndex++]=ch;					
						strcpy(t->tokenType,"TK_ERROR");
						t->isError=true;
                            			sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            			return *t;
					}
				}
				else{
					if(ch!=EOF){
						currentPointer--;
					}
					t->lexeme[lexemeIndex-1]='\0';
					strcpy(t->tokenType,"TK_LT");
					return *t;	
				}
			break;
			case 8:
				if(ch=='='){
					strcpy(t->tokenType,"TK_GE");
					return *t;					
				}
				else{
					if(ch!=EOF){
						currentPointer--;
					}
					t->lexeme[lexemeIndex-1]='\0';
					strcpy(t->tokenType,"TK_GT");
					return *t;	
				}
			break;
			case 9:
				if(ch=='='){
					strcpy(t->tokenType,"TK_EQ");
					return *t;					
				}
				else{
					if(ch!=EOF){
						currentPointer--;
					}
					t->lexeme[lexemeIndex++]=ch;					
					strcpy(t->tokenType,"TK_ERROR");
					t->isError=true;
                            		sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            	}	return *t;
				
			break;
			case 25:
				if(ch=='='){
					strcpy(t->tokenType,"TK_NE");
					return *t;					
				}
				else{
					if(ch!=EOF){
						currentPointer--;
						}
					t->lexeme[lexemeIndex++]=ch;					
					strcpy(t->tokenType,"TK_ERROR");
					t->isError=true;
                            		sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            	}	return *t;
			break;
			case 11:
				if(ch=='&'){
					ch=getNext(fp,B,k);
					if(ch=='&'){
						t->lexeme[lexemeIndex++]=ch;							
						strcpy(t->tokenType,"TK_AND");
						return *t;
					}
					else{
						if(ch!=EOF){
							currentPointer--;
						}
						t->lexeme[lexemeIndex++]=ch;					
						strcpy(t->tokenType,"TK_ERROR");
						t->isError=true;
                            			sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            			return *t;
					}
				}else{
					if(ch!=EOF){
						currentPointer--;
						}
					t->lexeme[lexemeIndex++]=ch;					
					strcpy(t->tokenType,"TK_ERROR");
					t->isError=true;
                            		sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            		return *t;
				}
			break;
			case 12:
				if(ch=='@'){
					ch=getNext(fp,B,k);
					if(ch=='@'){
						t->lexeme[lexemeIndex++]=ch;							
						strcpy(t->tokenType,"TK_OR");
						return *t;
					}
					else{
						if(ch!=EOF){
							currentPointer--;
							}
						t->lexeme[lexemeIndex++]=ch;					
						strcpy(t->tokenType,"TK_ERROR");
						t->isError=true;
                            			sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            			return *t;
					}
				}else{
					if(ch!=EOF){
						currentPointer--;
					}
					t->lexeme[lexemeIndex++]=ch;					
					strcpy(t->tokenType,"TK_ERROR");
					t->isError=true;
                            		sprintf(t->errorMsg, "ERROR_3:  Unknown pattern <%s> at <%llu>",t->lexeme,lineNumber);
                            		return *t;
				}
			break;
		}
	}      
}

void commentRemover(FILE *fp){
	
	buffer B;
	buffersize k;
	k=5;
	B=(char *) malloc(k);
	char ch;
	ch = getNext(fp,B,k);
	while(ch!=EOF){
		if(ch=='%'){
			ch = getNext(fp,B,k);
			while(ch!='\n'){
				ch = getNext(fp,B,k);
			}
		}else{
			printf("%c",ch);
			ch = getNext(fp,B,k);	
		}
	}
	bufferSize = -1;
	lineNumber=1;
	fseek(fp,0,SEEK_SET);
}

void printAllTokens(FILE *fp, buffer B, buffersize k){

	tokenInfo t;
	do{
		clean(&t);
		getNextTokenHelper(fp,B,k,&t);
		if(t.isError == 1){
			printf("\n%s %s",t.tokenType,t.errorMsg);
		}else{
			printf("\n%s %s %lu",t.tokenType,t.lexeme,t.lineNumber);
		}
	}while(strcmp(t.tokenType,"TK_EOF"));
	bufferSize = -1;
	lineNumber=1;
	fseek(fp,0,SEEK_SET);
}

void updateFilePointer(FILE *fp){
	bufferSize = -1;
	lineNumber=1;
}
// void main(){

// 	commentRemover("a.txt");
	
// 	FILE* fp;
// 	fp=fopen("a.txt","r");
// 	buffer B;
// 	buffersize k;
// 	k=5;
// 	B=(char *) malloc(k);
// 	tokenInfo t;
// 	do{
// 		clean(&t);
// 		getNextTokenHelper(fp,B,k,&t);
// 		if(t.isError == 1){
// 			printf("\n%s %s",t.tokenType,t.errorMsg);
// 		}else{
// 			printf("\n%s %s %lu",t.tokenType,t.lexeme,t.lineNumber);
// 		}
// 	}while(strcmp(t.tokenType,"TK_EOF"));
// 	bufferSize = -1;
// 	lineNumber=1;
// 	fclose(fp);
// 	commentRemover("a.txt");
// }

