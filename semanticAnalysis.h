#ifndef SEMANTICANALYSIS_H
#define SEMANTICANALYSIS_H

#include "populateSymbolTable.h"
int semantic(parseTree *ast, funcHashTable *functionTable, recHashTable *recordTable, varHashTable *globalTable, char *funcPresent);

#endif // SEMANTICANALYSIS_H
