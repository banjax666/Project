#ifndef _POPULATESYMBOLTABLE_H_
#define _POPULATESYMBOLTABLE_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "symbolTableHash.h"
#include "parser.h"
#include "lexer.h"
#include "lexerDef.h"
#include "parserDef.h"

void printSymbolTable(parseTree *t,varHashTable *symbolTable,recHashTable *recs);
int printSymbolTableHelper(parseTree *t,varHashTable *symbolTable,char *currFunc,int offset,recHashTable *recs);
void populateRecordTable(astNode *t, recHashTable *recordTable);
void populateGlobalTable(astNode *t, varHashTable *globals,recHashTable *recordTable);
void populateLocalTable(astNode *t, varHashTable *local,recHashTable *recordTable,varHashTable *global);
int functionOrder(funcHashTable *funcs, char *callee, char *caller);
int compareVarHashTables(varHashTable *t1,varHashTable *t2);
int childIdToIndex(astNode *parent, int id);
void addVarHashTable(varHashTable *dest,varHashTable *src);
int getFuncLineNumber(funcHashTable *funcs,char *name);
void populateFunctionTable(astNode *t, funcHashTable *functionTable, recHashTable* recordTable);

#endif
