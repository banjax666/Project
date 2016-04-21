#ifndef AST_H
#define AST_H
void printASTreeHelper(astNode* pTree, FILE* fp);
void printAST(astNode PT, char *outfile);
void buildAST(parseTree *pTree, astNode *ast);
void setSTPointers(astNode *ast,varHashTable *varTable);
void getNumNodesAST(astNode ast, int *nodes);

#endif // AST_H
