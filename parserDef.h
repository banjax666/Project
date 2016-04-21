/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/
#ifndef HEADERPARSERDEF
#define HEADERPARSERDEF

#include "lexerDef.h"
#include "symbolTableHash.h"

typedef struct grammar1 {
	int LHS;
	int numberOfRHS;
	int RHS[10];
} grammar1;
typedef grammar1 grammar[89];
typedef int firstset[50][105];
typedef int followset[50][106];
typedef int parseTable[50][55];
typedef parseTable table;

typedef enum {
	program,
	mainFunction,
	otherFunctions,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	primitiveDatatype,
	constructedDatatype,
	remaining_list,
	stmts,
	typeDefinitions,
	typeDefinition,
	fieldDefinitions,
	fieldDefinition,
	moreFields,
	declarations,
	declaration,
	global_or_not,
	otherStmts,
	stmt,
	assignmentStmt,
	singleOrRecId,
	singleOrRecIdPrime,
	funCallStmt,
	outputParameters,
	inputParameters,
	iterativeStmt,
	conditionalStmt,
	elsePart,
	ioStmt,
	allVar,
	arithmeticExpression,
	expPrime,
	term,
	termPrime,
	factor,
	highPrecedenceOperators,
	lowPrecedenceOperators,
	all,
	temp,
	booleanExpression,
	var,
	logicalOp,
	relationalOp,
	returnStmt,
	optionalReturn,
	idList,
	more_ids,
	TK_ASSIGNOP, 
	TK_COMMENT, 
	TK_FIELDID, 
	TK_ID, 
	TK_NUM, 
	TK_RNUM,
	TK_FUNID, 
	TK_RECORDID, 
	TK_WITH, 
	TK_PARAMETERS, 
	TK_END, 
	TK_WHILE, 
	TK_INT, 
	TK_REAL, 
	TK_TYPE, 
	TK_MAIN,
	TK_GLOBAL,
	TK_PARAMETER, 
	TK_LIST,
	TK_SQL, 
	TK_SQR, 
	TK_INPUT, 
	TK_OUTPUT,  
	TK_SEM, 
	TK_COLON, 
	TK_DOT,
	TK_COMMA,
	TK_ENDWHILE, 
	TK_OP, 
	TK_CL, 
	TK_IF, 
	TK_THEN, 
	TK_ENDIF, 
	TK_READ, 
	TK_WRITE, 
	TK_RETURN, 
	TK_PLUS, 
	TK_MINUS, 
	TK_MUL, 
	TK_DIV, 
	TK_CALL, 
	TK_RECORD, 
	TK_ENDRECORD, 
	TK_ELSE, 
	TK_AND,	 
	TK_OR,	 
	TK_NOT, 
	TK_LT,
	TK_LE,
	TK_EQ, 
	TK_GT, 
	TK_GE, 
	TK_NE,
	TK_EOF,
	eps,
	TK_ERROR
}IDS;

typedef struct parseTreeNode{
	int numChildren;
	IDS id;
	IDS parentId;
	tokenInfo token;
    struct parseTreeNode* children;//MAX_CHILDREN

} parseTreeNode;

typedef struct stack_struct{
	IDS data;
	struct stack_struct* below;
}stack_struct;

typedef struct astNode{
    int numChildren;
    IDS id;
    IDS parentId;
    tokenInfo token;
    struct astNode* children;
    struct astNode* collapseTo;
    variableTable* symbolTableEntry;
} astNode;


typedef parseTreeNode parseTree;

#endif
