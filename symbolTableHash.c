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
