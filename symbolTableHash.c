#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symboltablehash.h"
#include "lexerDef.h"
#include "parserDef.h"

int getKeyVariable(char *name){
    int sum = 0;
    int i;
    for(i=0;i<strlen(name);++i){
        sum = sum + (int)name[i];
    }
    sum=sum%VARIABLES_SIZE;
    return sum;
}

int getKeyRecord(char *name){
    int sum = 0;
    int i;
    for(i=0;i<strlen(name);++i){
        sum = sum + (int)name[i];
    }
    sum=sum%RECORDS_SIZE;
    return sum;
}

int getKeyFunction(char *name){
    int sum = 0;
    int i;
    for(i=0;i<strlen(name);++i){
        sum = sum + (int)name[i];
    }
    sum=sum%FUNCTIONS_SIZE;
    return sum;
}

void addVariable(varHashTable varTable , char *name, int type){
    key = getKeyVariable(name);
    variableTable* newNode = (variableTable *)malloc(sizeof(variableTable));
    newNode->name=(char *)malloc(sizeof(name));
    newNode->type = type;
    strcpy(newNode->name,name);
    newNode->next = NULL;
    if(varTable[key]==NULL){
        varTable[key]=newNode;
        return;
    }
    tempVar=varTable[key];
    while(tempVar->next!=NULL){
        tempVar=tempVar->next;
    }
    tempVar->next = newNode;
    return;
}

int findVariableType(varHashTable varTable, char *name){
    key = getKeyVariable(name);
    if(varTable[key]==NULL)
        return -1;
    else{
        tempVar=varTable[key];
        while(tempVar!=NULL){
            if(!strcmp(tempVar->name,name)){
                return tempVar->type;
            }
            tempVar= tempVar->next;
        }
        return -1;
    }
}

void removeVariableTable(varHashTable varTable){
    int i;
    for(i=0;i<VARIABLES_SIZE;++i){
        while(varTable[i] != NULL) {
           tempVar = varTable[i];
           varTable[i] = varTable[i]->next;
           free(tempVar->name);
           free(tempVar);
        }
    }
}

void addFunc(funcHashTable functionTable, char *name, variableTable *inputList, variableTable *outputList){
    key = getKeyFunction(name);
    funcTable* newNode = (funcTable *)malloc(sizeof(funcTable));
    newNode->name=(char *)malloc(sizeof(name));
    strcpy(newNode->name,name);
    newNode->next = NULL;
    if(functionTable[key]==NULL){
        functionTable[key]=newNode;
        return;
    }
    tempFunc=varTable[key];
    while(tempFunc->next!=NULL){
        tempFunc=tempFunc->next;
    }
    tempFunc->next = newNode;
    return;

}

bool findFunc(funcHashTable functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable[key]==NULL)
        return false;
    else{
        tempFunc=functionTable[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return true;
            }
            tempFunc= tempFunc->next;
        }
        return false;
    }
}

varHashTable getFuncInputList(funcHashTable functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->inputList;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

varHashTable getFuncOutputList(funcHashTable functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->outputList;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

varHashTable getFuncLocalVariables(funcHashTable functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->localVariables;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

bool setFuncLocalVariables(funcHashTable functionTable, char *name, varHashTable varTable){
    key = getKeyFunction(name);
    if(functionTable[key]==NULL)
        return false;
    else{
        tempFunc=functionTable[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                tempFunc->localVariables = varTable;
                return true;
            }
            tempFunc= tempFunc->next;
        }
        return false;
    }
}

void removeFuncTable(funcHashTable functionTable){
    int i;
    for(i=0;i<FUNCTIONS_SIZE;++i){
        while(functionTable[i] != NULL) {
           tempFunc = functionTable[i];
           functionTable[i] = functionTable[i]->next;
           free(tempFunc->name);
           removeVariableTable(tempFunc->inputList);
           removeVariableTable(tempFunc->outputList);
           removeVariableTable(tempFunc->localVariables);
           free(tempVar);
        }
    }
}
