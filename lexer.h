/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/
#ifndef HEADERLEXER
#define HEADERLEXER

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

char getNext(FILE *fp, buffer B, buffersize k);
tokenInfo getNextTokenHelper(FILE *fp, buffer B, buffersize k,tokenInfo *t);
void getNextToken(FILE *fp, tokenInfo *t);
void clean(tokenInfo *t);
void commentRemover(FILE* fp);
void printAllTokens(FILE *fp, buffer B, buffersize k);
void updateFilePointer();

#endif