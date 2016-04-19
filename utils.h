//Group 41
//Jayanth N. Koushik 2010A7PS021P
//Naresh Raghuraman 2010A7PS090P

/*
 * utils.h - declares various utility functions
 * used by different phases of the compiler
 * Group 41 - Jayanth N. Koushik, Naresh Raghuraman
 */

#ifndef UTILS
#define UTILS

#include <stdbool.h>
#include "lexerDef.h"

bool notPrintable(char c);
int parseIdStr(char *idStr);
char *idRepr(int id);
char *tokenRepr(int id);
bool isUseful(int tokenClass);

#endif
