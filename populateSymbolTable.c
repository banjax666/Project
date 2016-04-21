#include "populateSymbolTable.h"

extern bool semantic_flag;
int recordTypeCounter = 1;

void printSymbolTable(parseTree *t,varHashTable *symbolTable,recHashTable *recs){

    if(!isTerm(t->id)){
        if(t->id == function || t->id == mainFunction){

            if(t->id == function){
                char *name;
                name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                tokenInfo tempToken;
                tempToken = t->children[0].token;
                strcpy(name,tempToken.lexeme);
                printSymbolTableHelper(t,symbolTable,name,0,recs);
            }else{
                printSymbolTableHelper(t,symbolTable,"main",0,recs);
            }
        }else{
            int i;
                    for(i = 0; i < t->numChildren; i++){
                        printSymbolTable(&t->children[i],symbolTable,recs);
            }
        }
    }
}

int printSymbolTableHelper(parseTree *t,varHashTable *symbolTable,char *currFunc,int offset,recHashTable *recs){

    if(!isTerm(t->id)){
        if(t->id == declaration){
            tokenInfo tempToken;
            int type;
            tempToken = t->children[2].token;

                    if(findVariableType(symbolTable,tempToken.lexeme) == -1){
                        if(t->children[3].children[0].id == TK_GLOBAL){
                    if(t->children[1].children[0].id  ==primitiveDatatype){
                        type = t->children[1].children[0].children[0].id - TK_INT;
                        addVariable(symbolTable,tempToken.lexeme,type,0);
                        if(t->children[1].children[0].children[0].id == TK_INT){
                            printf("%s\tint\tglobal\t%d",tempToken.lexeme,offset);
                            offset+=4;
                            return offset;
                        }else{
                            printf("%s\treal\tglobal\t%d",tempToken.lexeme,offset);
                            offset+=2;
                            return offset;
                        }
                    }else{
                            tempToken = t->children[1].children[0].children[1].token;
                                    type = findRecType(recs,tempToken.lexeme);
                        tempToken = t->children[2].token;
                        addVariable(symbolTable,tempToken.lexeme,type,0);
                        printf("%s\t",tempToken.lexeme);

                        printf("%s\t%d",currFunc,offset);
                    }
                }else{
                    if(t->children[1].children[0].id  ==primitiveDatatype){
                        type = t->children[1].children[0].children[0].id - TK_INT;
                        addVariable(symbolTable,tempToken.lexeme,type,0);
                        if(t->children[1].children[0].children[0].id == TK_INT){
                            printf("%s\tint\t%s\t%d",tempToken.lexeme,currFunc,offset);
                            offset+=4;
                            return offset;
                        }else{
                            printf("%s\treal\t%s\t%d",tempToken.lexeme,currFunc,offset);
                            offset+=2;
                            return offset;
                        }
                    }else{
                        tempToken = t->children[1].children[0].children[1].token;
                                type = findRecType(recs,tempToken.lexeme);
                        tempToken = t->children[2].token;
                        addVariable(symbolTable,tempToken.lexeme,type,0);
                        printf("%s\t",tempToken.lexeme);

                        printf("%s\t%d",currFunc,offset);

                    }
                }
                    }
        }else{
            int i;
            for(i=0;i<t->numChildren;i++){
                offset = printSymbolTableHelper(&t->children[i],symbolTable,currFunc,offset,recs);
            }
        }
    }
    return offset;
}


int childIdToIndex(astNode *parent, int id){
    int i;
    char ch;
    printf("Child Node: %s\n",idToName(id));
    printf("Parent Node: %s\n",idToName(parent->id));
    printf("parent's children :%d\n",parent->numChildren);
    for(i=0;i<parent->numChildren;++i){
        if(parent->children[i].id==id){
            printf("position: %d\n",i);
            return i;
        }

    }
    printf("NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("Press ENTER key to Continue\n");

    scanf("%c",&ch);
    return -1;
}

void populateRecordTable(astNode *t, recHashTable *recordTable)
{
    tokenInfo tempToken;

    if(!isTerm(t->id))
    {
        if(t->id == typeDefinition)
        {
            tempToken = t->children[1].token;
            if(findRecType(recordTable,tempToken.lexeme) != -1){
                printf("Line no. %lu : Redeclaration of record with name %s.\n",tempToken.lineNumber,tempToken.lexeme);
                semantic_flag =false;
            }

            astNode temp = t->children[2];
            int type;
            varHashTable fields;
	    int i=0;
            type = temp.children[0].children[1].children[0].id - TK_INT;
            tempToken = temp.children[0].children[2].token;
            addVariable(&fields,tempToken.lexeme,type,i++);
            type = temp.children[1].children[1].children[0].id - TK_INT;
            tempToken = temp.children[1].children[2].token;
            addVariable(&fields,tempToken.lexeme,type,i++);

            temp = temp.children[2];
	    
            while(!isTerm(temp.children[0].id) && temp.children[0].id != eps)
            {
                type = temp.children[0].children[1].children[0].id - TK_INT;
                tempToken = temp.children[0].children[2].token;
                addVariable(&fields,tempToken.lexeme,type,i++);
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
    //return true;

}

void addVarHashTable(varHashTable *dest,varHashTable *src){
    int i;
    variableTable *temp;
    for(i=0;i<VARIABLES_SIZE;i++){
        temp = src->array[i];
        while(temp != NULL){
            if(findVariableType(dest,temp->name) != -1){
                semantic_flag = false;
            }else{
                addVariable(dest,temp->name,temp->type,temp->position);
            }
            temp=temp->next;
        }
    }
}

void populateGlobalTable(astNode *t, varHashTable *globals,recHashTable *recordTable)
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
                    semantic_flag = false;
                }
                addVariable(globals,tempToken.lexeme,type,0);
            }
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateGlobalTable(&t->children[i], globals,recordTable);
        }
    }
    //return true;

}

void populateLocalTable(astNode *t, varHashTable *local,recHashTable *recordTable,varHashTable *global)
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
                    semantic_flag = false;
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken.lineNumber,tempToken.lexeme);

                }
                if(findVariableType(global,tempToken.lexeme) != -1)
                {
                    semantic_flag = false;
                    printf("Line No. %lu : Redeclaration of identifier with name %s.\n",tempToken.lineNumber,tempToken.lexeme);

                }
                addVariable(local,tempToken.lexeme,type,0);
            }
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateLocalTable(&t->children[i], local,recordTable, global);
        }
    }
    //return true;

}

int compareVarHashTables(varHashTable *t1,varHashTable *t2,int lineNum){
    int i,pos1,j,maxpos2=0,maxpos1=0;
    variableTable  *temp1,*temp2;
    for(i=0;i<VARIABLES_SIZE;i++){
        temp1 = t1->array[i];
        while(temp1 != NULL){
	    	pos1 = temp1->position;
	    	for(i=0;i<VARIABLES_SIZE;i++){
			temp2 = t2->array[i];
			while(temp2 != NULL){
				if(temp2->position > maxpos2){
					maxpos2 = temp2->position;
				}
				if(pos1 == temp2->position){
					if(temp1->type != temp2->type){
                    				printf("Line No. %lu : Type mismatch between %s and %s.\n",lineNum,temp1->name,temp2->name);
						return 0;
					}
				}
				temp2 = temp2->next;
			}
		}	
		if(temp1->position > maxpos1){
			maxpos1 = temp1->position;
		}
            	temp1 = temp1->next;
        }
    }

    if(maxpos1 != maxpos2){
	printf("Line No. %lu : Mismatch in number of parameters.\n",lineNum);
	return 0;
	}
    return 1;
}

int getFuncLineNumber(funcHashTable *functionTable,char *name){
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

    printf("Node: %s\n",idToName(t->id));

    if(!isTerm(t->id))
    {
        if(t->id == function)
        {
            char* name = (char *)malloc(SIZE_MAX_FUNID*sizeof(char));
            int nameChild;
            nameChild=0;
            strcpy(name,t->children[nameChild].token.lexeme);
            varHashTable* inputList = (varHashTable*)malloc(sizeof(varHashTable));
            varHashTable* outputList = (varHashTable*)malloc(sizeof(varHashTable));
            int k = t->children[nameChild].token.lineNumber;

            if(findFunc(functionTable,name)==true)
            {
                printf("Line %lu : Redeclaration of function : %s\n",t->children[nameChild].token.lineNumber,name);
                semantic_flag = false;
            }

            astNode parameterListNode = t->children[1].children[0];

            while(1)
            {
                int formalParameterType,i=0;

                if(parameterListNode.children[0].children[0].id==primitiveDatatype)
                {
                    formalParameterType = parameterListNode.children[0].children[0].children[0].id;
                    addVariable(inputList, parameterListNode.children[1].token.lexeme, formalParameterType,i++);

                }
                else
                {
                    formalParameterType = findRecType(recordTable, parameterListNode.children[0].children[0].children[1].token.lexeme);
                    addVariable(inputList, parameterListNode.children[1].token.lexeme, formalParameterType,i++);
                }
                if(parameterListNode.children[2].children[0].id == eps)
                    break;
                parameterListNode = parameterListNode.children[2].children[0];
            }

            astNode outputParameterNode = t->children[2];
            if(outputParameterNode.children[0].id == eps)
                outputList=NULL;
            else
            {
                parameterListNode = t->children[2].children[0];
                while(1)
                {
                    int formalParameterType,i=0;
                    if(parameterListNode.children[0].children[0].id==primitiveDatatype)
                    {
                        formalParameterType = parameterListNode.children[0].children[0].children[0].id;
                        addVariable(outputList, parameterListNode.children[1].token.lexeme, formalParameterType,i++);

                    }
                    else
                    {
                        formalParameterType = findRecType(recordTable, parameterListNode.children[0].children[0].children[1].token.lexeme);
                        addVariable(outputList, parameterListNode.children[1].token.lexeme, formalParameterType,i++);
                    }

                    if(parameterListNode.children[2].children[0].id == eps)
                        break;
                    parameterListNode = parameterListNode.children[2].children[0];
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

