#include "populateSymbolTable.h"

int recordTypeCounter = 1;

bool populateRecordTable(parseTree *t, recHashTable *recordTable)
{
	tokenInfo *tempToken;

    if(!isTerm(t->id))
    {
        if(t->id == typeDefinition)
        {
	    tempToken = t->children[1].token;
            if(findRecType(recordTable,tempToken->lexeme) != -1){
                    printf("Line no. %lu : Redeclaration of record with name %s.\n",tempToken->lineNumber,tempToken->lexeme);
                    return false;
            }
            
            parseTree temp = t->children[2];
            int type;
	    varHashTable fields;
            
            type = temp.children[0].children[1].children[0].id - TK_NUM;
	    tempToken = temp.children[0].children[2].token;
            addVariable(&fields,tempToken->lexeme,type);
            type = temp.children[1].children[1].children[0].id - TK_NUM;
	    tempToken = temp.children[1].children[2].token;
            addVariable(&fields,tempToken->lexeme,type);
            
            temp = temp.children[2];
            while(!isTerm(temp.children[0].id) && temp.children[0].id != eps)
            {
                type = temp.children[0].children[1].children[0].id - TK_NUM;
		tempToken = temp.children[0].children[2].token;
                addVariable(&fields,tempToken->lexeme,type);
                temp = temp.children[1];
            }
	    recordTypeCounter++;
	    tempToken = t->children[1].token;
            addRec(recordTable,tempToken->lexeme,&fields,recordTypeCounter);
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateRecordTable(&t->children[i], recordTable);
        }
    }
    return true;

}


bool populateGlobalTable(parseTree *t, varHashTable *globals,recHashTable *recordTable)
{

	tokenInfo *tempToken;
    if(!isTerm(t->id))
    {
        if(t->id == declaration)
        {
            if(t->children[3].children[0].id == TK_GLOBAL)
            {
                
                int type;
                if(t->children[1].children[0].id  ==primitiveDatatype)
                {
                    type = t->children[1].children[0].children[0].id - TK_INT;
                }
                else
                {
		    tempToken = t->children[1].children[0].children[1].token;
                    type = findRecType(recordTable,tempToken->lexeme);
                }
                
		tempToken = t->children[2].token;
                if(findVariableType(globals,tempToken->lexeme) != -1)
                {
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken->lineNumber,tempToken->lexeme);
                    return false;
                }
		addVariable(globals,tempToken->lexeme,type);
            }
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateGlobalTable(&t->children[i], globals,recordTable);
        }
    }    
    return true;

}

bool populateLocalTable(parseTree *t, varHashTable *local,recHashTable *recordTable)
{
    tokenInfo *tempToken;
    if(!isTerm(t->id))
    {
        if(t->id == declaration)
        {
            if(t->children[3].children[0].id == eps)
            {
                
                int type;
                if(t->children[1].children[0].id  ==primitiveDatatype)
                {
                    type = t->children[1].children[0].children[0].id - TK_INT;
                }
                else
                {
		    tempToken = t->children[1].children[0].children[1].token;
                    type = findRecType(recordTable,tempToken->lexeme);
                }
                
		tempToken = t->children[2].token;
                if(findVariableType(local,tempToken->lexeme) != -1)
                {
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken->lineNumber,tempToken->lexeme);
                    return false;
                }
		addVariable(local,tempToken->lexeme,type);
            }
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateLocalTable(&t->children[i], local,recordTable);
        }
    }    
    return true;

}

int compareVarHashTables(varHashTable *t1,varHashTable *t2){
	int i;
	variableTable  *temp1,*temp2;
	for(i=0;i<VARIABLES_SIZE;i++){
		temp1 = t1->array[i];
		temp2 = t2->array[i];
		while(t1 != NULL && t2 != NULL){
			if(temp1->type != temp2->type){
				return 0;
			}
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		
		if(temp1 != NULL){
			return 0;
		}
		if(temp2 != NULL){
			return 0;
		}
	}
	return 1;
}

int functionOrder(funcHashTable *funcs, char *callee, char *caller){
	int f1,f2;
	f1 = getFuncLineNumber(funcs,callee);
	f2 = getFuncLineNumber(funcs,caller);

	if(f1==f2){
		return -1;
	}else if(f1 == -1){
		return 0;
	}else if(f1 > f2){
		return 1;
	}else{
		return 2;
	}
}

int semantic(parseTree *p, funcHashTable *funcs, recHashTable *recs, varHashTable *globals, char *currFunc){

	if(!isTerm(p->id)){
		varHashTable local,*input,*output;
		createVarTable(&local);
		int k,i;
		tokenInfo *tempToken;
		char* name;
		
		switch(p->id){
			case function:
				createVarTable(input);
				createVarTable(output);
				
				tempToken = p->children[0].token;
				input = getFuncInputList(funcs,tempToken->lexeme);
				output = getFuncOutputList(funcs,tempToken->lexeme);
				populateLocalTable(p,&local,recs);

				addVarHashTable(&local,&globals);
				addVarHashTable(&local,&input);
				addVarHashTable(&local,&output);

		                for(i = 0; i < p->numChildren; i++){
                    			k=semantic(&p->children[i], funcs, recs,&local,tempToken->lexeme);
                		}

				return k;
			
			case mainFunction:
				createVarTable(&local);

				addVarHashTable(&local,&globals);
				populateLocalTable(p,&local,recs);

		                for(i = 0; i < p->numChildren; i++){
                    			k=semantic(&p->children[i], funcs, recs,&local,"main");
                		}

				return k;

			case ioStmt:
                    		name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
				int type;
				varHashTable *temp;

				if(p->children[0].id == TK_READ){
					tempToken = p->children[1].children[0].token;
					type = findVariableType(globals,tempToken->lexeme);

					if(type == -1){
						//ERROR
					}else if(type >= 2){
						strcpy(name,getRecordName(recs,type));
						temp = getRecFields(recs,name);
						
						if(p->children[1].children[1].children[0].id != eps){
							tempToken = p->children[1].children[1].children[0].token;
							if(findVariableType(temp,tempToken->lexeme)==-1){
								//ERROR
							}
						}
					}
				}else if(p->children[0].id == TK_WRITE){
					if(p->children[1].children[0].id != TK_NUM && p->children[1].children[0].id != TK_RNUM){
						tempToken = p->children[1].children[0].token;
						type = findVariableType(globals,tempToken->lexeme);

						if(type == -1){
							//ERROR
						}else if(type >= 2){
							if(p->children[1].children[1].children[0].id != eps){
								tempToken = p->children[1].children[1].children[0].token;
								strcpy(name,getRecordName(recs,type));
								temp = getRecFields(recs,name);

								if(findVariableType(temp,tempToken->lexeme) == -1){
									//ERROR
								}
							}
						}
					}
				}
			case funCallStmt:
				tempToken = p->children[1].token;
				k = functionOrder(funcs,tempToken->lexeme,currFunc);

				if(k == -1){
					//recursion
				}else if(k == 0){
					//DNE
				}else if(k == 1){
					//outOfOrder
				}

				varHashTable *output,*input,outCall,inCall;
				output = getFuncOutputList(funcs,tempToken->lexeme);
				input = getFuncOutputList(funcs,tempToken->lexeme);
				createVarTable(&outCall);
				createVarTable(&inCall);
				parseTree tempTree;

				if(p->children[0].children[0].id == idList){
					tempTree = p->children[0].children[0];

					do{
						tempToken = tempTree.children[0].token;
						if(findVariableType(globals,tempToken->lexeme) == -1){
							//ERROR
						}else{
							int g;
							g = findVariableType(globals,tempToken->lexeme);
							addVariable(&outCall,tempToken->lexeme,g);
						}

						tempTree = tempTree.children[1].children[0];
					}while(tempTree.children[1].children[0].id != eps);
				}

				if(compareVarHashTables(output,&outCall) == 0){
					//ERROR
				}

				if(p->children[2].children[0].id == idList){
					tempTree = p->children[2].children[0];

					do{
						tempToken = tempTree.children[0].token;
						if(findVariableType(globals,tempToken->lexeme) == -1){
							//ERROR
						}else{
							int g;
							g = findVariableType(globals,tempToken->lexeme);
							addVariable(&inCall,tempToken->lexeme,g);
						}

						tempTree = tempTree.children[1].children[0];
					}while(tempTree.children[1].children[0].id != eps);

				}

				if(compareVarHashTables(input,&inCall) == 0){
					//ERROR
				}
			case returnStmt:
				if(strcmp(currFunc,"main")!=0){
					varHashTable *output,outCall;
					output = getFuncOutputList(funcs,currFunc);
					createVarTable(&outCall);
					parseTree tempTree;

					if(p->children[0].children[0].id == idList){
						tempTree = p->children[0].children[0];

						do{
							tempToken = tempTree.children[0].token;
							if(findVariableType(globals,tempToken->lexeme) == -1){
								//ERROR
							}else{
								int g;
								g = findVariableType(globals,tempToken->lexeme);
								addVariable(&outCall,tempToken->lexeme,g);
							}
	
							tempTree = tempTree.children[1].children[0];
						}while(tempTree.children[1].children[0].id != eps);
					}

					if(compareVarHashTables(output,&outCall) == 0){
						//ERROR
					}
					
				}
		}

	}

}


