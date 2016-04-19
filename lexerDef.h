/*
-- Group Number: 68
-- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
-- Name: Shubham Sharma     ID: 2013A7PS110P
*/

#ifndef HEADERLEXERDEF
#define HEADERLEXERDEF

#include<stdbool.h> 

#define SIZE_MAX_LEXEME 1000 // larger than the allowed limits to accomodate large error messages
#define SIZE_MAX_TOKEN 20 // the largest size is 13 (tk_parameters)
#define SIZE_MAX_ERROR_MSG 1000

typedef char* buffer;
typedef unsigned long long buffersize; 
typedef unsigned long long big;

typedef struct tokenInfo {

    char lexeme[SIZE_MAX_LEXEME];
    char tokenType[SIZE_MAX_TOKEN];
    unsigned long lineNumber;
    bool isError;
    char errorMsg[SIZE_MAX_ERROR_MSG];
    
}tokenInfo;

#endif
