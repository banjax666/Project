#ifndef SYMBOLTABLEHASH_H
#define SYMBOLTABLEHASH_H

#define VARIABLES_SIZE 37
#define RECORDS_SIZE 37
#define FUNCTIONS_SIZE 37

typedef struct variableTable{
    char *name;
    int type;
    struct variableTable *next;
} variableTable;

typedef struct varHashTable{
    variableTable* array[VARIABLES_SIZE];
} varHashTable;

variableTable* tempVar;
int key;

void createVarTable(varHashTable* varTable);
void addVariable(varHashTable* varTable , char *name, int type);
int findVariableType(varHashTable* varTable, char *name);//returns -1 if no variable, else variable type
void removeVariableTable(varHashTable* varTable);


typedef struct recTable {
    char *name;
    int type;
    varHashTable* recFields;
    struct recTable *next;
} recTable;

typedef struct recHashTable{
    recTable* array[RECORDS_SIZE];
} recHashTable;

void createRecTable(recHashTable* recordTable);
void addRec(recHashTable* recordTable, char *name, varHashTable *fields, int type);
int findRecType(recHashTable* recordTable, char *name);
varHashTable* getRecFields(recHashTable* recordTable, char *name);
void removeRecTable(recHashTable* recordTable);

typedef struct funcTable {
    char *name;
    varHashTable* inputList;
    varHashTable* outputList;
    varHashTable* localVariables;
    struct funcTable *next;
} funcTable;

typedef struct funcHashTable{
    funcTable* array[FUNCTIONS_SIZE];
} funcHashTable;

funcTable* tempFunc;

void createFuncTable(funcHashTable* functionTable);
void addFunc(funcHashTable* functionTable, char *name, varHashTable *inputList, varHashTable *outputList);
bool findFunc(funcHashTable* functionTable, char *name); // searches for 'functionName'
varHashTable *getFuncInputList(funcHashTable* functionTable, char *name);
varHashTable *getFuncOutputList(funcHashTable* functionTable, char *name);
varHashTable *getFuncLocalVariables(funcHashTable* functionTable, char *name);
bool setFuncLocalVariables(funcHashTable* functionTable, char *name, varHashTable* varTable);
void removeFuncTable(funcHashTable* functionTable);

int getKeyFunction(char *name);
int getKeyVariable(char *name);
int getKeyRecord(char *name);

//variableTable* varHashTable[MAX_VARIABLES];
//recTable* recHashTable[MAX_RECORDS];
//funcTable* funcHashTable[MAX_FUNCTIONS];

#endif // SYMBOLTABLEHASH_H
