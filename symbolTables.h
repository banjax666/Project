#ifndef _SYMBOL_TABLES_H_
#define _SYMBOL_TABLES_H_

typedef struct variableTable{
    char *name;
    int type;
    struct variableTable *next;
} variableTable;

variableTable *addVariable(variableTable *varTable, char *name, int type);
int findVariableType(variableTable *varTable, char *name);//returns -1 if no variable, else variable type
void removeVariableTable(variableTable *varTable);


typedef struct recTable {
    char *name;
    int type;
    variableTable *recFields;
    struct recTable *next;
} recTable;


recTable *addRec(recTable *recordTable, char *name, variableTable *fields, int type);
int findRecType(recTable *recordTable, char *name);
variableTable *getRecFields(recordTable *recTable, char *recName);
void removeRecTable(recTable *recordTable);


typedef struct funcTable {
    char *name;
    variableTable *inputList;
    variableTable *outputList;
    variableTable *localVariables;
    struct funcTable *next;
} funcTable;

funcTable *addFunc(funcTable *functionTable, char *name, variableTable *inputList, variableTable *outputList);
bool findFunc(funcTable *functionTable, char *name); // searches for 'functionName'
variableTable *getFuncInputList(functionTable *funcTable, char *name);
variableTable *getFuncOutputList(functionTable *funcTable, char *name);
variableTable *getFuncLocalVariables(functionTable *funcTable, char *name);
void removeFuncTable(funcTable *functionTable);

#endif
