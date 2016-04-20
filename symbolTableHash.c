#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "symbolTableHash.h"

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

void createVarTable(varHashTable* varTable){
	int i;
	for(i=0;i<VARIABLES_SIZE;i++){
		varTable->array[i]=NULL;
	}
}

void addVariable(varHashTable* varTable , char *name, int type){

    key = getKeyVariable(name);
    variableTable* newNode = (variableTable *)malloc(sizeof(variableTable));
    newNode->name=(char *)malloc(sizeof(name));
    newNode->type = type;
    strcpy(newNode->name,name);
    newNode->next = NULL;
    if(varTable->array[key]==NULL){
        varTable->array[key]=newNode;
        return;
    }
    tempVar=varTable->array[key];
    while(tempVar->next!=NULL){
        tempVar=tempVar->next;
    }
    tempVar->next = newNode;
    return;
}

int findVariableType(varHashTable* varTable, char *name){
    key = getKeyVariable(name);
    if(varTable->array[key]==NULL)
        return -1;
    else{
        tempVar=varTable->array[key];
        while(tempVar!=NULL){
            if(!strcmp(tempVar->name,name)){
                return tempVar->type;
            }
            tempVar= tempVar->next;
        }
        return -1;
    }
}

void removeVariableTable(varHashTable* varTable){
    int i;
    for(i=0;i<VARIABLES_SIZE;++i){
        while(varTable->array[i] != NULL) {
           tempVar = varTable->array[i];
           varTable->array[i] = varTable->array[i]->next;
           free(tempVar->name);
           free(tempVar);
        }
    }
    free(varTable);
}

void createRecTable(recHashTable* recordTable){
	int i;
	for(i=0;i<RECORDS_SIZE;i++){
		recordTable->array[i]=NULL;
	}
}

void addRec(recHashTable* recordTable, char *name, varHashTable *fields, int type){

    key = getKeyRecord(name);
    recTable *new = (recTable *)malloc(sizeof(recTable));
    new->name = (char *)malloc(sizeof(char));
    recTable *temp;

    new->type = type;
    strcpy(new->name,name);
    new->recFields = fields;
    new->next = NULL;

    if(recordTable->array[key] == NULL){
        recordTable->array[key] = new;
    }else{
        temp = recordTable->array[key];
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new;
    }
}

int findRecType(recHashTable* recordTable, char *name){

    key = getKeyRecord(name);
    recTable *temp;

    if(recordTable->array[key] == NULL){
        return -1;
    }else{
        temp = recordTable->array[key];
        while(temp != NULL){
            if(!strcmp(temp->name,name)){
                return temp->type;
            }
            temp = temp->next;
        }
    }

    return -1;
}

varHashTable* getRecFields(recHashTable* recordTable, char *name){

    key = getKeyRecord(name);
    recTable *temp;

    if(recordTable->array[key] == NULL){
        return NULL;
    }else{
        temp = recordTable->array[key];
        while(temp != NULL){
            if(!strcmp(temp->name,name)){
                return temp->recFields;
            }
            temp = temp->next;
        }
    }

    return NULL;
}

void removeRecTable(recHashTable* recordTable){

    int i=0;
    recTable *temp,*temp1;

    while(i<RECORDS_SIZE){
        if(recordTable->array[i] != NULL){
            temp = recordTable->array[i];

            while(temp != NULL){
                temp1 = temp;
                temp = temp->next;
                free(temp1->name);
                removeVariableTable(temp1->recFields);
                free(temp1);
            }
            recordTable->array[i] = NULL;
        }
        i++;
    }
}

void createFuncTable(funcHashTable* functionTable){
	int i;
	for(i=0;i<FUNCTIONS_SIZE;i++){
		functionTable->array[i]=NULL;
	}
}

void addFunc(funcHashTable* functionTable, char *name, varHashTable *inputList, varHashTable *outputList){

    key = getKeyFunction(name);
    funcTable* newNode = (funcTable *)malloc(sizeof(funcTable));
    newNode->name=(char *)malloc(sizeof(name));
    strcpy(newNode->name,name);
    newNode->inputList=inputList;
    newNode->outputList=outputList;
    newNode->next = NULL;
    if(functionTable->array[key]==NULL){
        functionTable->array[key]=newNode;
        return;
    }
    tempFunc=functionTable->array[key];
    while(tempFunc->next!=NULL){
        tempFunc=tempFunc->next;
    }
    tempFunc->next = newNode;
    return;

}

bool findFunc(funcHashTable *functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return false;
    else{
        tempFunc=functionTable->array[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return true;
            }
            tempFunc= tempFunc->next;
        }
        return false;
    }
}

varHashTable* getFuncInputList(funcHashTable *functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable->array[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->inputList;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

varHashTable* getFuncOutputList(funcHashTable *functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable->array[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->outputList;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

varHashTable* getFuncLocalVariables(funcHashTable *functionTable, char *name){
    key = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return NULL;
    else{
        tempFunc=functionTable->array[key];
        while(tempFunc!=NULL){
            if(!strcmp(tempFunc->name,name)){
                return tempFunc->localVariables;
            }
            tempFunc= tempFunc->next;
        }
        return NULL;
    }
}

bool setFuncLocalVariables(funcHashTable* functionTable, char *name, varHashTable* varTable){
    key = getKeyFunction(name);
    if(functionTable->array[key]==NULL)
        return false;
    else{
        tempFunc=functionTable->array[key];
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

void removeFuncTable(funcHashTable* functionTable){
    int i;
    for(i=0;i<FUNCTIONS_SIZE;++i){
        while(functionTable->array[i] != NULL) {
           tempFunc = functionTable->array[i];
           functionTable->array[i] = functionTable->array[i]->next;
           free(tempFunc->name);
           removeVariableTable(tempFunc->inputList);
           removeVariableTable(tempFunc->outputList);
           removeVariableTable(tempFunc->localVariables);
           free(tempFunc);
        }
    }
    free(functionTable);
}

int main(){

	
}
