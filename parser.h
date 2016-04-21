/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/
#ifndef HEADERPARSER
#define HEADERPARSER

#include "parserDef.h"
#include "lexerDef.h"

void pop();
void push(IDS element);
IDS top();
int nameToId(char *name);
char *idToName(int id);
int isTerm(int x);
int hasEPS(grammar G,int x);
void getGrammar(grammar G, char* grammarFile);
void firstHelper(firstset f,grammar G,int lhs,int *c);
void first1(firstset f,grammar G,int i);
void first(firstset f,grammar G);
void followhelper(followset f,firstset fi,grammar G,int prod,int pos);
void follow1(followset f,firstset fi,grammar G,int id);
void follow(followset f,firstset fi,grammar G);
void PTHelper(parseTable PT,grammar G,int lhs,int *c);
void PTHelper1(parseTable PT,grammar G,int i);
void createParseTable(firstset fi,followset f,grammar G,parseTable PT);
int parseInputSourceCode(FILE *fpSource, buffer B, buffersize k, tokenInfo *tokenCurrent, parseTree* root, grammar G, table PT);
void printParseTreeHelper(parseTree* pTree, FILE* fp);
void printParseTree(parseTree PT, char *outfile);
void getNumNodesParseTree(parseTree p, int *nodes);
void traverse(stack_struct* stack);


#endif
