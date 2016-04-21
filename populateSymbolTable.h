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


bool populateRecordTable(astNode *t, recHashTable *recordTable);
bool populateGlobalTable(astNode *t, varHashTable *globals,recHashTable *recordTable);
bool populateLocalTable(astNode *t, varHashTable *local,recHashTable *recordTable);
int functionOrder(funcHashTable *funcs, char *callee, char *caller);
int compareVarHashTables(varHashTable *t1,varHashTable *t2);
int childIdToIndex(astNode *parent, int id);
void populateFunctionTable(astNode *t, funcHashTable *functionTable, recHashTable* recordTable);

#endif
