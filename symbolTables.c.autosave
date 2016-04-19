#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbolTables.h"
#include "lexerDef.h"
#include "parserDef.h"

variableTable *addVariable(variableTable *id,char *name,int type){

    variableTable *new = (variableTable *)malloc(sizeof(variableTable));
    new->name=(char *)malloc(sizeof(name));

    new->type = type;
    strcpy(new->name,name);
    new->next = id;

    return new;
}

int findvariableType(variableTable *id,char *name){

    variableTable *temp;
    temp = id;

    while(temp!= NULL){
        if(!strcmp(name,temp->name)){
            return temp->type;
        }
        temp = temp->next;
    }

    return -1;
}

void removeVariableTable(variableTable *varTable) {

    variableTable *temp;
    while(varTable != NULL) {
        temp = varTable;
        varTable = varTable->next;
        free(temp.name);
        free(temp);
    }
}



void printvariableTable(variableTable *id){

    variableTable *temp;
    temp = id;

    while(temp->next != NULL){
        printf("%s\n",temp->name);
        temp = temp->next;
    }
    printf("%s\n",temp->name);
}

recTable *addRec(recTable *r,char *name,variableTable *id,int type){

    recTable *new = (recTable *)malloc(sizeof(recTable));
    new->name=(char *)malloc(sizeof(name));

    new->type = type;
    strcpy(new->name,name);
    new->recFields = id;
    new->next = r;

    return new;
}

variableTable *getRecFields(recTable *r,char *name){

    recTable *tempRecord;
    tempRecord = r;

    while(tempRecord!= NULL){
        if(!strcmp(name,tempRecord->name)){
            return tempRecord->recFields;

        }
        tempRecord = tempRecord->next;
    }

    return NULL;
}


int findRecType(recTable *recordTable, char *name){
    recTable *temp;

    for(temp = recordTable; temp != NULL; temp = temp->next)
        if(strcmp(temp->name, name) == 0)
        {
            return temp->type;
        }

    return 0;
}



void removeRecTable(recTable *recordTable) {
    recTable *temp;
    while(recordTable != NULL) {
        temp = recordTable;
        recordTable = recordTable->next;

        free(temp->name);
        removeVariableTable(temp->recFields);
        free(temp);
    }
}

funcTable *addFunc(funcTable *functionTable, char *name, variableTable *inputList, variableTable *outputList){

    funcTable *new = (funcTable *)malloc(sizeof(funcTable));
    new->name = (char *)malloc(sizeof(char));

    new->inputList = inputList;
    new->outputList = outputList;
    new->localVariables = NULL;
    strcpy(new->name,name);
    new->next = functionTable;

    return new;
}
bool findFunc(funcTable *functionTable, char *name){

    funcTable *temp;
    temp = functionTable;

    while(temp->next != NULL){
        if(!strcmp(name,temp->name)){
            return 1;
        }
        temp=temp->next;
    }
    if(!strcmp(name,temp->name)){
            return 1;
    }
    return 0;
}
variableTable *getFuncInputList(funcTable *functionTable, char *name){

    funcTable *temp;
    temp = functionTable;

    while(temp->next != NULL){
        if(!strcmp(name,temp->name)){
            return temp->inputList;
        }
        temp=temp->next;
    }
    if(!strcmp(name,temp->name)){
        return temp->inputList;
    }
    return NULL;
}
variableTable *getFuncOutputList(funcTable *functionTable, char *name){

    funcTable *temp;
    temp = functionTable;

    while(temp->next != NULL){
        if(!strcmp(name,temp->name)){
            return temp->outputList;
        }
        temp=temp->next;
    }
    if(!strcmp(name,temp->name)){
        return temp->outputList;
    }
    return NULL;
}
variableTable *getFuncLocalVariables(funcTable *functionTable, char *name){

    funcTable *temp;
    temp = functionTable;

    while(temp->next != NULL){
        if(!strcmp(name,temp->name)){
            return temp->localVariables;
        }
        temp=temp->next;
    }
    if(!strcmp(name,temp->name)){
        return temp->localVariables;
    }
    return NULL;
}

void removeFuncTable(funcTable *functionTable) {
    funcTable *temp;
    while(functionTable != NULL) {
        temp = functionTable;
        functionTable = funcTable->next;

        free(temp->name);
        removeVariableTable(temp->inputParameterList);
        (temp->outputParameterList);
        free(temp);
    }
}
