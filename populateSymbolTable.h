#ifndef _POPULATESYMBOLTABLE_H_
#define _POPULATESYMBOLTABLE_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbolTables.h"
#include "parser.h"
#include "lexer.h"
#include "utils.h"
#include "lexerDef.h"
#include "parserDef.h"



FunctionTable *populateFunctionTable(parseTree *p, FunctionTable *funcs);
void printTheFunctionsTable(FunctionTable *funcs);
RecordTable *populateRecordTable(parseTree *p, RecordTable *recs);
void printTheRecordTable(RecordTable *recs);
IdentifierTable *populateGlobalTable(parseTree *p, IdentifierTable *globals);
IdentifierTable *populateLocalTable(parseTree *p, IdentifierTable *globals);
void printTheGlobalTable(IdentifierTable *globals, RecordTable *recs);
int functionOrder(FunctionTable *funcs, char *calledFunc, char *currFunc);
IdentifierTable *addContents(IdentifierTable *from, IdentifierTable *to, unsigned long long lineNo);
int semantic(parseTree *p, FunctionTable *funcs, RecordTable *recs, IdentifierTable *globals, char *currFunc);
void displaySymbolTable(IdentifierTable *globals, RecordTable *recs, FunctionTable *funcs, parseTree *p);

#endif
