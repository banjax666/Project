//Group 41
//Jayanth N. Koushik 2010A7PS021P
//Naresh Raghuraman 2010A7PS090P

/*
 * utils.c: defines various utility functions as declared in utils.h
 * 
 * this file contains the following functions:
 *     bool notPrintable(char c)
 *     int parseIdStr(char *idStr)
 *     char *idRepr(int id)   
 *     bool isUseful(int tokenClass)
 *
 * Group 41 - Jayanth N. Koushik, Naresh Raghuraman
 */

#include <string.h>
#include "lexerDef.h"
#include "parserDef.h"

bool notPrintable(char c) {
    /* returns true for characters which aren't easily displayed line newline, eof etc.
     * used when generating error messages
     */
    if(c == '\n' || c == '\r' || c == ' ' || c == '\v' || c == '\f' || c == '\t' || c == 26) // 26 is eof
        return true;
    else
        return false;
}

int parseIdStr(char *idStr) {
    /* convertes the identifier into its numeric equivalent
     * can be made more efficient using hashing
     */
	if(strcmp(idStr, "program") == 0) return program;
	if(strcmp(idStr, "mainFunction") == 0) return mainFunction;
	if(strcmp(idStr, "otherFunctions") == 0) return otherFunctions;
	if(strcmp(idStr, "function") == 0) return function;
	if(strcmp(idStr, "input_par") == 0) return input_par;
	if(strcmp(idStr, "output_par") == 0) return output_par;
	if(strcmp(idStr, "parameter_list") == 0) return parameter_list;
	if(strcmp(idStr, "dataType") == 0) return dataType;
	if(strcmp(idStr, "primitiveDatatype") == 0) return primitiveDatatype;
	if(strcmp(idStr, "constructedDatatype") == 0) return constructedDatatype;
	if(strcmp(idStr, "remaining_list") == 0) return remaining_list;
	if(strcmp(idStr, "stmts") == 0) return stmts;
	if(strcmp(idStr, "typeDefinitions") == 0) return typeDefinitions;
	if(strcmp(idStr, "typeDefinition") == 0) return typeDefinition;
	if(strcmp(idStr, "fieldDefinitions") == 0) return fieldDefinitions;
	if(strcmp(idStr, "fieldDefinition") == 0) return fieldDefinition;
	if(strcmp(idStr, "moreFields") == 0) return moreFields;
	if(strcmp(idStr, "declarations") == 0) return declarations;
	if(strcmp(idStr, "declaration") == 0) return declaration;
	if(strcmp(idStr, "global_or_not") == 0) return global_or_not;
	if(strcmp(idStr, "otherStmts") == 0) return otherStmts;
	if(strcmp(idStr, "stmt") == 0) return stmt;
	if(strcmp(idStr, "assignmentStmt") == 0) return assignmentStmt;
	if(strcmp(idStr, "singleOrRecId") == 0) return singleOrRecId;
    if(strcmp(idStr, "singleOrRecIdPrime") == 0) return singleOrRecIdPrime;
	if(strcmp(idStr, "funCallStmt") == 0) return funCallStmt;
	if(strcmp(idStr, "outputParameters") == 0) return outputParameters;
	if(strcmp(idStr, "inputParameters") == 0) return inputParameters;
	if(strcmp(idStr, "iterativeStmt") == 0) return iterativeStmt;
	if(strcmp(idStr, "conditionalStmt") == 0) return conditionalStmt;
	if(strcmp(idStr, "elsePart") == 0) return elsePart;
	if(strcmp(idStr, "ioStmt") == 0) return ioStmt;
	if(strcmp(idStr, "allVar") == 0) return allVar;
	if(strcmp(idStr, "arithmeticExpression") == 0) return arithmeticExpression;
	if(strcmp(idStr, "expPrime") == 0) return expPrime;
	if(strcmp(idStr, "term") == 0) return term;
	if(strcmp(idStr, "termPrime") == 0) return termPrime;
	if(strcmp(idStr, "factor") == 0) return factor;
	if(strcmp(idStr, "highPrecedenceOperators") == 0) return highPrecedenceOperators;
	if(strcmp(idStr, "lowPrecedenceOperators") == 0) return lowPrecedenceOperators;
	if(strcmp(idStr, "all") == 0) return all;
	if(strcmp(idStr, "temp") == 0) return temp;
	if(strcmp(idStr, "booleanExpression") == 0) return booleanExpression;
	if(strcmp(idStr, "var") == 0) return var;
	if(strcmp(idStr, "logicalOp") == 0) return logicalOp;
	if(strcmp(idStr, "relationalOp") == 0) return relationalOp;
	if(strcmp(idStr, "returnStmt") == 0) return returnStmt;
	if(strcmp(idStr, "optionalReturn") == 0) return optionalReturn;
	if(strcmp(idStr, "idList") == 0) return idList;
	if(strcmp(idStr, "more_ids") == 0) return more_ids;
	if(strcmp(idStr, "TK_ASSIGNOP") == 0) return TK_ASSIGNOP ;
	if(strcmp(idStr, "TK_COMMENT") == 0) return TK_COMMENT ;
	if(strcmp(idStr, "TK_FIELDID") == 0) return TK_FIELDID ;
	if(strcmp(idStr, "TK_ID") == 0) return TK_ID ;
	if(strcmp(idStr, "TK_NUM") == 0) return TK_NUM ;
	if(strcmp(idStr, "TK_RNUM") == 0) return TK_RNUM ;
	if(strcmp(idStr, "TK_FUNID") == 0) return TK_FUNID ;
	if(strcmp(idStr, "TK_RECORDID") == 0) return TK_RECORDID ;
	if(strcmp(idStr, "TK_WITH") == 0) return TK_WITH ;
	if(strcmp(idStr, "TK_PARAMETERS") == 0) return TK_PARAMETERS ;
	if(strcmp(idStr, "TK_END") == 0) return TK_END ;
	if(strcmp(idStr, "TK_WHILE") == 0) return TK_WHILE ;
	if(strcmp(idStr, "TK_INT") == 0) return TK_INT ;
	if(strcmp(idStr, "TK_REAL") == 0) return TK_REAL ;
	if(strcmp(idStr, "TK_TYPE") == 0) return TK_TYPE ;
	if(strcmp(idStr, "TK_MAIN") == 0) return TK_MAIN ;
	if(strcmp(idStr, "TK_GLOBAL") == 0) return TK_GLOBAL ;
	if(strcmp(idStr, "TK_PARAMETER") == 0) return TK_PARAMETER ;
	if(strcmp(idStr, "TK_LIST") == 0) return TK_LIST ;
	if(strcmp(idStr, "TK_SQL") == 0) return TK_SQL ;
	if(strcmp(idStr, "TK_SQR") == 0) return TK_SQR ;
	if(strcmp(idStr, "TK_INPUT") == 0) return TK_INPUT ;
	if(strcmp(idStr, "TK_OUTPUT") == 0) return TK_OUTPUT  ;
	if(strcmp(idStr, "TK_SEM") == 0) return TK_SEM ;
	if(strcmp(idStr, "TK_COLON") == 0) return TK_COLON ;
	if(strcmp(idStr, "TK_DOT") == 0) return TK_DOT;
	if(strcmp(idStr, "TK_COMMA") == 0) return TK_COMMA;
	if(strcmp(idStr, "TK_ENDWHILE") == 0) return TK_ENDWHILE ;
	if(strcmp(idStr, "TK_OP") == 0) return TK_OP ;
	if(strcmp(idStr, "TK_CL") == 0) return TK_CL ;
	if(strcmp(idStr, "TK_IF") == 0) return TK_IF ;
	if(strcmp(idStr, "TK_THEN") == 0) return TK_THEN ;
	if(strcmp(idStr, "TK_ENDIF") == 0) return TK_ENDIF ;
	if(strcmp(idStr, "TK_READ") == 0) return TK_READ ;
	if(strcmp(idStr, "TK_WRITE") == 0) return TK_WRITE ;
	if(strcmp(idStr, "TK_RETURN") == 0) return TK_RETURN ;
	if(strcmp(idStr, "TK_PLUS") == 0) return TK_PLUS ;
	if(strcmp(idStr, "TK_MINUS") == 0) return TK_MINUS ;
	if(strcmp(idStr, "TK_MUL") == 0) return TK_MUL ;
	if(strcmp(idStr, "TK_DIV") == 0) return TK_DIV ;
	if(strcmp(idStr, "TK_CALL") == 0) return TK_CALL ;
	if(strcmp(idStr, "TK_RECORD") == 0) return TK_RECORD ;
	if(strcmp(idStr, "TK_ENDRECORD") == 0) return TK_ENDRECORD ;
	if(strcmp(idStr, "TK_ELSE") == 0) return TK_ELSE ;
	if(strcmp(idStr, "TK_AND") == 0) return TK_AND ;
	if(strcmp(idStr, "TK_OR") == 0) return TK_OR ;
	if(strcmp(idStr, "TK_NOT") == 0) return TK_NOT ;
	if(strcmp(idStr, "TK_LT") == 0) return TK_LT;
	if(strcmp(idStr, "TK_LE") == 0) return TK_LE;
	if(strcmp(idStr, "TK_EQ") == 0) return TK_EQ ;
	if(strcmp(idStr, "TK_GT") == 0) return TK_GT ;
	if(strcmp(idStr, "TK_GE") == 0) return TK_GE ;
	if(strcmp(idStr, "TK_NE") == 0) return TK_NE;
    if(strcmp(idStr, "TK_EOF") == 0) return TK_EOF;
	if(strcmp(idStr, "eps") == 0) return eps;
    return -1;
}

char *idRepr(int id) {
	// returns the printable representation of an id
	if(id == program) return "program";
	if(id == mainFunction) return "mainFunction";
	if(id == otherFunctions) return "otherFunctions";
	if(id == function) return "function";
	if(id == input_par) return "input_par";
	if(id == output_par) return "output_par";
	if(id == parameter_list) return "parameter_list";
	if(id == dataType) return "dataType";
	if(id == primitiveDatatype) return "primitiveDatatype";
	if(id == constructedDatatype) return "constructedDatatype";
	if(id == remaining_list) return "remaining_list";
	if(id == stmts) return "stmts";
	if(id == typeDefinitions) return "typeDefinitions";
	if(id == typeDefinition) return "typeDefinition";
	if(id == fieldDefinitions) return "fieldDefinitions";
	if(id == fieldDefinition) return "fieldDefinition";
	if(id == moreFields) return "moreFields";
	if(id == declarations) return "declarations";
	if(id == declaration) return "declaration";
	if(id == global_or_not) return "global_or_not";
	if(id == otherStmts) return "otherStmts";
	if(id == stmt) return "stmt";
	if(id == assignmentStmt) return "assignmentStmt";
	if(id == singleOrRecId) return "singleOrRecId";
	if(id == singleOrRecIdPrime) return "singleOrRecIdPrime";
	if(id == funCallStmt) return "funCallStmt";
	if(id == outputParameters) return "outputParameters";
	if(id == inputParameters) return "inputParameters";
	if(id == iterativeStmt) return "iterativeStmt";
	if(id == conditionalStmt) return "conditionalStmt";
	if(id == elsePart) return "elsePart";
	if(id == ioStmt) return "ioStmt";
	if(id == allVar) return "allVar";
	if(id == arithmeticExpression) return "arithmeticExpression";
	if(id == expPrime) return "expPrime";
	if(id == term) return "term";
	if(id == termPrime) return "termPrime";
	if(id == factor) return "factor";
	if(id == highPrecedenceOperators) return "highPrecedenceOperators";
	if(id == lowPrecedenceOperators) return "lowPrecedenceOperators";
	if(id == all) return "all";
	if(id == temp) return "temp";
	if(id == booleanExpression) return "booleanExpression";
	if(id == var) return "var";
	if(id == logicalOp) return "logicalOp";
	if(id == relationalOp) return "relationalOp";
	if(id == returnStmt) return "returnStmt";
	if(id == optionalReturn) return "optionalReturn";
	if(id == idList) return "idList";
	if(id == more_ids) return "more_ids";
	if(id == TK_ASSIGNOP) return "'<---'";
	if(id == TK_COMMENT) return "TK_COMMENT";
	if(id == TK_FIELDID) return "field identifier";
	if(id == TK_ID) return "identifier";
	if(id == TK_NUM) return "numeric constant";
	if(id == TK_RNUM) return "real constant";
	if(id == TK_FUNID) return "function id";
	if(id == TK_RECORDID) return "record id";
	if(id == TK_WITH) return "'with'";
	if(id == TK_PARAMETERS) return "'parameters'";
	if(id == TK_END) return "'end'";
	if(id == TK_WHILE) return "'while'";
	if(id == TK_INT) return "'int'";
	if(id == TK_REAL) return "'real'";
	if(id == TK_TYPE) return "'type'";
	if(id == TK_MAIN) return "'_main'";
	if(id == TK_GLOBAL) return "'global'";
	if(id == TK_PARAMETER) return "'parameter'";
	if(id == TK_LIST) return "'list'";
	if(id == TK_SQL) return "'['";
	if(id == TK_SQR) return "']'";
	if(id == TK_INPUT) return "'input'";
	if(id == TK_OUTPUT) return "'output'";
	if(id == TK_SEM) return "';'";
	if(id == TK_COLON) return "':'";
	if(id == TK_DOT) return "'.'";
	if(id == TK_COMMA) return "','";
	if(id == TK_ENDWHILE) return "'endwhile'";
	if(id == TK_OP) return "'('";
	if(id == TK_CL) return "')'";
	if(id == TK_IF) return "'if'";
	if(id == TK_THEN) return "'then'";
	if(id == TK_ENDIF) return "'endif'";
	if(id == TK_READ) return "'read'";
	if(id == TK_WRITE) return "'write'";
	if(id == TK_RETURN) return "'return'";
	if(id == TK_PLUS) return "'+'";
	if(id == TK_MINUS) return "'-'";
	if(id == TK_MUL) return "'*'";
	if(id == TK_DIV) return "'/'";
	if(id == TK_CALL) return "'call'";
	if(id == TK_RECORD) return "'record'";
	if(id == TK_ENDRECORD) return "'endrecord'";
	if(id == TK_ELSE) return "'else'";
	if(id == TK_AND) return "'&&&'";
	if(id == TK_OR) return "'@@@'";
	if(id == TK_NOT) return "'~'";
	if(id == TK_LT) return "'<'";
	if(id == TK_LE) return "'<='";
	if(id == TK_EQ) return "'=='";
	if(id == TK_GT) return "'>'";
	if(id == TK_GE) return "'>='";
	if(id == TK_NE) return "'!='";
	if(id == TK_EOF) return "end of file";
	if(id == eps) return "eps";
	return "";
}

char *tokenRepr(int id) {
	// returns the printable representation of a token
	if(id == TK_ASSIGNOP) return "TK_ASSIGNOP";
	if(id == TK_COMMENT) return "TK_COMMENT";
	if(id == TK_FIELDID) return "TK_FIELDID";
	if(id == TK_ID) return "TK_ID";
	if(id == TK_NUM) return "TK_NUM";
	if(id == TK_RNUM) return "TK_RNUM";
	if(id == TK_FUNID) return "TK_FUNID";
	if(id == TK_RECORDID) return "TK_RECORDID";
	if(id == TK_WITH) return "TK_WITH";
	if(id == TK_PARAMETERS) return "TK_PARAMETERS";
	if(id == TK_END) return "TK_END";
	if(id == TK_WHILE) return "TK_WHILE";
	if(id == TK_INT) return "TK_INT";
	if(id == TK_REAL) return "TK_REAL";
	if(id == TK_TYPE) return "TK_TYPE";
	if(id == TK_MAIN) return "TK_MAIN";
	if(id == TK_GLOBAL) return "TK_GLOBAL";
	if(id == TK_PARAMETER) return "TK_PARAMETER";
	if(id == TK_LIST) return "TK_LIST";
	if(id == TK_SQL) return "TK_SQL";
	if(id == TK_SQR) return "TK_SQR";
	if(id == TK_INPUT) return "TK_INPUT";
	if(id == TK_OUTPUT) return "TK_OUTPUT";
	if(id == TK_SEM) return "TK_SEM";
	if(id == TK_COLON) return "TK_COLON";
	if(id == TK_DOT) return "TK_DOT";
	if(id == TK_COMMA) return "TK_COMMA";
	if(id == TK_ENDWHILE) return "TK_ENDWHILE";
	if(id == TK_OP) return "TK_OP";
	if(id == TK_CL) return "TK_CL";
	if(id == TK_IF) return "TK_IF";
	if(id == TK_THEN) return "TK_THEN";
	if(id == TK_ENDIF) return "TK_ENDIF";
	if(id == TK_READ) return "TK_READ";
	if(id == TK_WRITE) return "TK_WRITE";
	if(id == TK_RETURN) return "TK_RETURN";
	if(id == TK_PLUS) return "TK_PLUS";
	if(id == TK_MINUS) return "TK_MINUS";
	if(id == TK_MUL) return "TK_MUL";
	if(id == TK_DIV) return "TK_DIV";
	if(id == TK_CALL) return "TK_CALL";
	if(id == TK_RECORD) return "TK_RECORD";
	if(id == TK_ENDRECORD) return "TK_ENDRECORD";
	if(id == TK_ELSE) return "TK_ELSE";
	if(id == TK_AND) return "TK_AND";
	if(id == TK_OR) return "TK_OR";
	if(id == TK_NOT) return "TK_NOT";
	if(id == TK_LT) return "TK_LT";
	if(id == TK_LE) return "TK_LE";
	if(id == TK_EQ) return "TK_EQ";
	if(id == TK_GT) return "TK_GT";
	if(id == TK_GE) return "TK_GE";
	if(id == TK_NE) return "TK_NE";
	if(id == TK_EOF) return "TK_EOF";
	if(id == eps) return "eps";
	return "";
}

bool isUseful(int tokenClass) {
    /* returns false for tokens that don't serve any purpose
     * after the parse tree is created
     */
    switch(tokenClass) {
        case TK_MAIN:
        case TK_END:
        case TK_SEM:
        case TK_INPUT:
        case TK_PARAMETER:
        case TK_LIST:
        case TK_SQL:
        case TK_SQR:
        case TK_OUTPUT:
        case TK_COMMA:
        case TK_COLON:
        case TK_DOT:
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_ENDWHILE:
        case TK_IF:
        case TK_ENDIF:
        case TK_OP:
        case TK_CL:
        case TK_RETURN:
            return false;
            break;
        default:
            return true;
    }
}
