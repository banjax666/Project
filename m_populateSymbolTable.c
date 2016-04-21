#include "populateSymbolTable.h"

extern bool semantic_flag;
int recordTypeCounter = 1;
/*
void printSymbolTable(parseTree *t,varHashTable *symbolTable){

	if(!isTerm(t->id)){
		if(t->id == function || t->id == mainFunction){

			if(t->id == function){
				char *name;
				name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
				tokenInfo *tempToken;
				tempToken = t->children[0].token;
				strcpy(name,tempToken->lexeme);
				printSymbolTableHelper(t,symbolTable,name,0);
			}else{
				printSymbolTableHelper(t,symbolTable,"main",0);				
			}
		}else{
			int i;
            		for(i = 0; i < t->numChildren; i++){
                		printSymbolTable(&t->children[i],symbolTable);
			}
		}
	}
}

int printSymbolTableHelper(parseTree *t,varHashTable *symbolTable,char *currFunc,int offset){

	if(!isTerm(t->id)){
		if(t->id == declaration){
			tokenInfo *tempToken;
			tempToken = t->children[2].token;

                	if(findVariableType(symbolTable,tempToken->lexeme) == -1){
	                	if(t->children[3].children[0].id == TK_GLOBAL){
					if(t->children[1].children[0].id  ==primitiveDatatype){
						type = t->children[1].children[0].children[0].id - TK_INT;
						addVariable(symbolTable,tempToken->lexeme,type);
						if(t->children[1].children[0].children[0].id == TK_INT){
							printf("%s\tint\tglobal\t%d",tempToken->lexeme,offset);
							offset+=4;
							return offset;
						else{
							printf("%s\treal\tglobal\t%d",tempToken->lexeme,offset);
							offset+=2;
							return offset;
						}
					}else{
		    				tempToken = t->children[1].children[0].children[1].token;
                    				type = findRecType(recordTable,tempToken->lexeme);
						tempToken = t->children[2].token;
						addVariable(symbolTable,tempToken->lexeme,type);
						printf("%s\t",tempToken->lexeme);

						printf("%s\t%d",currFunc,offset);
					}
				}else{
					if(t->children[1].children[0].id  ==primitiveDatatype){
						type = t->children[1].children[0].children[0].id - TK_INT;
						addVariable(symbolTable,tempToken->lexeme,type);
						if(t->children[1].children[0].children[0].id == TK_INT){
							printf("%s\tint\t%s\t%d",tempToken->lexeme,currFunc,offset);
							offset+=4;
							return offset;
						else{
							printf("%s\treal\t%s\t%d",tempToken->lexeme,currFunc,offset);
							offset+=2;
							return offset;
						}
					}else{
						tempToken = t->children[1].children[0].children[1].token;
                				type = findRecType(recordTable,tempToken->lexeme);
						tempToken = t->children[2].token;
						addVariable(symbolTable,tempToken->lexeme,type);
						printf("%s\t",tempToken->lexeme);

						printf("%s\t%d",currFunc,offset);
						
					}
				}
        	        }			
		}else{
			int i;
			for(i=0;i<t->numChildren,i++){
				offset = printSymbolTableHelper(&t->children[i],symbolTable,currFunc,offset);
			}
		}
	}
	return offset;
}
*/

int childIdToIndex(astNode *parent, int id){
    int i;
    for(i=0;i<parent->numChildren;++i){
        if(parent->children[i].id==id)
            return i;
    }
    return -1;
}
int recordTypeCounter = 1;

bool populateRecordTable(astNode *t, recHashTable *recordTable)
{
    tokenInfo tempToken;

    if(!isTerm(t->id))
    {
        if(t->id == typeDefinition)
        {
            tempToken = t->children[1].token;
            if(findRecType(recordTable,tempToken.lexeme) != -1){
                printf("Line no. %lu : Redeclaration of record with name %s.\n",tempToken.lineNumber,tempToken.lexeme);
                return false;
            }

            astNode temp = t->children[2];
            int type;
            varHashTable fields;

            type = temp.children[0].children[1].children[0].id - TK_NUM;
            tempToken = temp.children[0].children[2].token;
            addVariable(&fields,tempToken.lexeme,type);
            type = temp.children[1].children[1].children[0].id - TK_NUM;
            tempToken = temp.children[1].children[2].token;
            addVariable(&fields,tempToken.lexeme,type);

            temp = temp.children[2];
            while(!isTerm(temp.children[0].id) && temp.children[0].id != eps)
            {
                type = temp.children[0].children[1].children[0].id - TK_NUM;
                tempToken = temp.children[0].children[2].token;
                addVariable(&fields,tempToken.lexeme,type);
                temp = temp.children[1];
            }
            recordTypeCounter++;
            tempToken = t->children[1].token;
            addRec(recordTable,tempToken.lexeme,&fields,recordTypeCounter);
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


bool populateGlobalTable(astNode *t, varHashTable *globals,recHashTable *recordTable)
{

    tokenInfo tempToken;
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
                    type = findRecType(recordTable,tempToken.lexeme);
                }

                tempToken = t->children[2].token;
                if(findVariableType(globals,tempToken.lexeme) != -1)
                {
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken.lineNumber,tempToken.lexeme);
                    return false;
                }
                addVariable(globals,tempToken.lexeme,type);
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

bool populateLocalTable(astNode *t, varHashTable *local,recHashTable *recordTable)
{
    tokenInfo tempToken;
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
                    type = findRecType(recordTable,tempToken.lexeme);
                }

                tempToken = t->children[2].token;
                if(findVariableType(local,tempToken.lexeme) != -1)
                {
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken.lineNumber,tempToken.lexeme);
                    return false;
                }
                addVariable(local,tempToken.lexeme,type);
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

int getFuncLineNumber(funcHashTable *funcs,char *name){
    int k = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return -1;
    else{
        tempFunc=functionTable->array[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->lineNum;
            }
            tempFunc= tempFunc->next;
        }
        return -1;
    }
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

void populateFunctionTable(astNode *t, funcHashTable *functionTable, recHashTable* recordTable){
    
    if(!isTerm(t->id))
    {
        if(t->id == function)
        {
            char* name = (char *)malloc(SIZE_MAX_FUNID*sizeof(char));
            int nameChild;
            nameChild=childIdToIndex(t,TK_FUNID);
            strcpy(name,t->children[nameChild].token.lexeme);
            varHashTable* inputList = (varHashTable*)malloc(sizeof(varHashTable));
            varHashTable* outputList = (varHashTable*)malloc(sizeof(varHashTable));
	    int k = t->children[nameChild].token.lineNumber;

            if(findFunc(functionTable,name)==true)
            {
                printf("Line %lu : Redeclaration of function : %s\n",t->children[nameChild].token.lineNumber,name);
                semantic_flag = false;
            }

            astNode parameterListNode = t->children[childToIndex(t,input_par)].children[childIdToIndex(t,parameter_list)];

            while(1)
            {
                int formalParameterType;
                
                if(parameterListNode.children[childToIndex(t,dataType)].children[0].id==primitiveDatatype)
                {
                    formalParameterType = parameterListNode.children[childToIndex(t,dataType)].children[childToIndex(t,primitiveDatatype)].children[0].id;
                    addVariable(inputList, parameterListNode.children[childToIndex(t,TK_ID)].token.lexeme, formalParameterType);

                }
                else
                {
                    formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(t,dataType)].children[childToIndex(t,constructedDatatype)].children[childToIndex(t,TK_RECORDID)].token.lexeme);
                    addVariable(inputList, parameterListNode.children[childToIndex(t,TK_ID)].token.lexeme, formalParameterType);
                }
                if(parameterListNode.children[childToIndex(t,remaining_list)].children[0].id == eps)
                    break;
                parameterListNode = parameterListNode.children[childToIndex(t,remaining_list)].children[childToIndex(t,parameter_list)];
            }
            
            astNode outputParameterNode = t->children[childToIndex(t,output_par)];
            if(outputParameterNode.children[0].id == eps)
                outputList=NULL;
            else
            {
                parameterListNode = t->children[childToIndex(t,output_par)].children[childIdToIndex(t,parameter_list)];
                while(1)
                {
                    int formalParameterType;
                    if(parameterListNode.children[childToIndex(t,dataType)].children[0].id==primitiveDatatype)
                    {
                        formalParameterType = parameterListNode.children[childToIndex(t,dataType)].children[childToIndex(t,primitiveDatatype)].children[0].id;
                        addVariable(outputList, parameterListNode.children[childToIndex(t,TK_ID)].token.lexeme, formalParameterType);

                    }
                    else
                    {
                        formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(dataType)].children[childToIndex(constructedDatatype)].children[childToIndex(TK_RECORDID)].token.lexeme);
                        addVariable(outputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);
                    }

                    if(parameterListNode.children[childToIndex(t,remaining_list)].children[0].id == eps)
                        break;
                    parameterListNode = parameterListNode.children[childToIndex(t,remaining_list)].children[childToIndex(t,parameter_list)];
                }
            }

            addFunc(functionTable, name, inputList, outputList,k);
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateFunctionTable(&t->children[i], functionTable, recordTable);
        }
    }
}


