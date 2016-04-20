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


bool populateRecordTable(parseTree *t, recHashTable *recordTable);
bool populateGlobalTable(parseTree *t, varHashTable *globals,recHashTable *recordTable);
bool populateLocalTable(parseTree *t, varHashTable *local,recHashTable *recordTable);
int semantic(parseTree *p, funcHashTable *funcs, recHashTable *recs, varHashTable *globals, char *currFunc);
int functionOrder(funcHashTable *funcs, char *callee, char *caller);
int compareVarHashTables(varHashTable *t1,varHashTable *t2);

#endif
