#ifndef SYMBOLTABLEHASH_H
#define SYMBOLTABLEHASH_H

typedef struct varHashTable{
    variableTable* array[VARIABLES_SIZE];
} varHashTable;

typedef struct funcHashTable{
    funcTable* array[FUNCTIONS_SIZE];
} funcHashTable;

typedef struct recHashTable{
    recTable* array[RECORDS_SIZE];
} recHashTable;

typedef struct variableTable{
    char *name
    int type;
    struct variableTable *next;
} variableTable;

variableTable* tempVar;
int key;

void addVariable(varHashTable* varTable , char *name, int type);
int findVariableType(varHashTable* varTable, char *name);//returns -1 if no variable, else variable type
varTable* getVarHashTableEntry(varHashTable* varTable,char* name);
void removeVariableTable(varHashTable* varTable);
int typeIntOrReal(int type);

typedef struct recTable {
    char *name;
    int type;
    varHashTable* recFields;
    struct recTable *next;
} recTable;

recTable* tempRec;

void addRec(recHashTable* recordTable, char *name, variableTable *fields, int type);
int findRecType(recHashTable* recordTable, char *name);
varHashTable* getRecFields(recHashTable* recordTable, char *name);
char* getRecordName(recHashTable* recordTable, int type);
void removeRecTable(recHashTable* recordTable);

typedef struct funcTable {
    char *name;
    varHashTable* inputList;
    varHashTable* outputList;
    varHashTable* localVariables;
    struct funcTable *next;
} funcTable;

funcTable* tempFunc;
void addFunc(funcHashTable* functionTable, char *name, varHashTable *inputList, varHashTable *outputList);
bool findFunc(funcHashTable* functionTable, char *name); // searches for 'functionName'
varHashTable getFuncInputList(funcHashTable* functionTable, char *name);
varHashTable getFuncOutputList(funcHashTable* functionTable, char *name);
varHashTable getFuncLocalVariables(funcHashTable* functionTable, char *name);
bool setFuncLocalVariables(funcHashTable* functionTable, char *name);
void removeFuncTable(funcHashTable* functionTable);

int getKeyVariable(char *name);
int getKeyRecord(char *name);
int getKeyFunction(char *name);

//variableTable* varHashTable[MAX_VARIABLES];
//recTable* recHashTable[MAX_RECORDS];
//funcTable* funcHashTable[MAX_FUNCTIONS];

#endif // SYMBOLTABLEHASH_H
