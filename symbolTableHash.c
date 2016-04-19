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

void addRec(recHashTable recordTable, char *name, variableTable *fields, int type){

    key = getKeyRecord(name);
    recTable *new = (recTable *)malloc(sizeof(recTable));
    new->name = (char *)malloc(sizeof(char));
    recTable *temp;

    new->type = type;
    strcpy(new->name,name);
    new->recFields = fields;
    new->next = NULL;

    if(recordTable[key] == NULL){
        recordTable[key] = new;
    }else{
        temp = recordTable[key];
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new;
    }
}

int findRecType(recHashTable recordTable, char *name){

    key = getKeyRecord(name);
    recTable *temp;

    if(recordTable[key] == NULL){
        return -1;
    }else{
        temp = recordTable[key];
        while(temp != NULL){
            if(!strcmp(temp->name,name)){
                return temp->type;
            }
            temp = temp->next;
        }
    }

    return -1;
}

varHashTable getRecFields(recHashTable recordTable, char *name){

    key = getKeyRecord(name);
    recTable *temp;

    if(recordTable[key] == NULL){
        return NULL;
    }else{
        temp = recordTable[key];
        while(temp != NULL){
            if(!strcmp(temp->name,name)){
                return temp->recFields;
            }
            temp = temp->next;
        }
    }

    return NULL;
}

void removeRecTable(recHashTable recordTable){

    int i=0;
    recTable *temp,*temp1;

    while(i<MAX_RECORDS){
        if(recordTable[i] != NULL){
            temp = recordTable[i];

            while(temp != NULL){
                temp1 = temp;
                temp = temp->next;
                free(temp1->name);
                removeVariableTable(temp1->recFields);
                free(temp1);
            }
            recordTable[i] = NULL;
        }
        i++;
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
