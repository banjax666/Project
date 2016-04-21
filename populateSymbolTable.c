#include "populateSymbolTable.h"

extern bool semantic_flag;

int childIdToIndex(parseTree *parent, int id){
    int i;
    for(i=0;i<parent->numChildren;++i){
        if(parent->children[i].id==id)
            return i;
    }
    return -1;
}
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

void populateFunctionTable(parseTree *t, funcHashTable *functionTable, recHashTable* recordTable){
    
    if(!isTerm(t->id))
    {
        if(t->id == function)
        {
            char* name = (char *)malloc(SIZE_MAX_FUNID*sizeof(char));
            int nameChild;
            childName=childIdToIndex(t,TK_FUNID);
            strcpy(name,t->children[nameChild].token.lexeme);
            varHashTable* inputList = (varHashTable*)malloc(sizeof(varHashTable));
            varHashTable* outputList = (varHashTable*)malloc(sizeof(varHashTable));

            if(findFunc(functionTable,name)==true)
            {
                printf("Line %lu : Redeclaration of function : %s\n",t->children[childName].token.lineNumber,name);
                semantic_flag = false;
            }

            parseTree parameterListNode = t->children[childToIndex(input_par)].children[childIdToIndex(parameter_list)];

            while(1)
            {
                int formalParameterType;
                
                if(parameterListNode.children[childToIndex(dataType)].children[0].id==primitiveDatatype)
                {
                    formalParameterType = parameterListNode.children[childToIndex(dataType)].children[childToIndex(primitiveDatatype)].children[0].id;
                    addVariable(inputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);

                }
                else
                {
                    formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(dataType)].children[childToIndex(constructedDatatype)].children[childToIndex(TK_RECORDID)].token.lexeme);
                    addVariable(inputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);
                }
                if(parameterListNode.children[childToIndex(remaining_list)].children[0].id == eps)
                    break;
                parameterListNode = parameterListNode.children[childToIndex(remaining_list)].children[childToIndex(parameter_list)];
            }
            
            parseTree outputParameterNode = t->children[childToIndex(output_par)];
            if(outputParameterNode.children[0].id == eps)
                outputList=NULL;
            else
            {
                parameterListNode = t->children[childToIndex(output_par)].children[childIdToIndex(parameter_list)];
                while(1)
                {
                    if(parameterListNode.children[childToIndex(dataType)].children[0].id==primitiveDatatype)
                    {
                        formalParameterType = parameterListNode.children[childToIndex(dataType)].children[childToIndex(primitiveDatatype)].children[0].id;
                        addVariable(outputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);

                    }
                    else
                    {
                        formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(dataType)].children[childToIndex(constructedDatatype)].children[childToIndex(TK_RECORDID)].token.lexeme);
                        addVariable(outputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);
                    }

                    if(parameterListNode.children[childToIndex(remaining_list)].children[0].id == eps)
                        break;
                    parameterListNode = parameterListNode.children[childToIndex(remaining_list)].children[childToIndex(parameter_list)];
                }
            }

            addFunc(functionTable, name, inputList, outputList);
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateFunctionTable(&t->children[i], functionTable, recordTable);
        }
    }
}



