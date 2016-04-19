/* populateSymbolTables.c
 * Group 41 - Jayanth N. Koushik (2010A7PS021P), Naresh Raghuraman (2010A7PS090P)
 */
 
#include "populateSymbolTable.h"

int recordTypeCounter = 1;
extern int semantic_flag;

FunctionTable *populateFunctionTable(parseTree *p, FunctionTable *funcs) {
    
    if(p->isTerminal == false)
    {
        if(p->nonTerminal == function)
        {
            char *functionName;
            functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
            IdentifierTable *input_parameter_list = createNewIdentifierTable();
            IdentifierTable *output_parameter_list = createNewIdentifierTable();

            if(p->children[0].isTerminal == true)
            {
                strcpy(functionName,p->children[0].terminal.lexeme);
                
                if(findFunction(funcs,functionName)!=0)
                {
                    printf("Line No. %llu : Redeclaration of function with name %s\n",p->children[0].terminal.lineNo,functionName);
                    semantic_flag=1;
                    return funcs;
                    //exit(EXIT_FAILURE);
                } 
            }
            
            parseTree temp = p->children[1].children[4];
                        
            while(1)
            {
                int type;
                
                if(temp.children[0].children[0].nonTerminal==primitiveDatatype)
                {
                    type = temp.children[0].children[0].children[0].terminal.tokenClass;
                    input_parameter_list = addIdentifier(input_parameter_list, temp.children[1].terminal.lexeme, type, " ");
                }    
                else
                {
                    type = RECORD_DATATYPE;
                    input_parameter_list = addIdentifier(input_parameter_list, temp.children[1].terminal.lexeme, type, temp.children[0].children[0].children[1].terminal.lexeme);
                }
                if(temp.children[2].children[0].terminal.tokenClass == eps)
                    break;
                temp = temp.children[2].children[1];       
            }
            
            temp = p->children[2];
            if(temp.children[0].isTerminal==true && temp.children[0].terminal.tokenClass == eps)
                output_parameter_list = NULL;
            else
            {
                temp = temp.children[4];
                while(1)
                {
                    int type;
                
                    if(temp.children[0].children[0].nonTerminal==primitiveDatatype)
                    {
                        type = temp.children[0].children[0].children[0].terminal.tokenClass;
                        output_parameter_list = addIdentifier(output_parameter_list, temp.children[1].terminal.lexeme, type, " ");
                    }    
                    else
                    {
                        type = RECORD_DATATYPE;
                        output_parameter_list = addIdentifier(output_parameter_list, temp.children[1].terminal.lexeme, type, temp.children[0].children[0].children[1].terminal.lexeme);
                    }
                
                    if(temp.children[2].children[0].terminal.tokenClass == eps)
                        break;
                    temp = temp.children[2].children[1];       
                }
            }
            
            funcs = addFunction (funcs, functionName, input_parameter_list, output_parameter_list);   
        }
        else
        {
            int i;
            for(i = 0; i < p->noOfChildren; i++)
                funcs = populateFunctionTable(&p->children[i], funcs);
        }
    }    
    return funcs;
}

void printTheFunctionsTable(FunctionTable *funcs)
{
    printf("\nPrinting the function table details\n");
    FunctionTable *cur = createFuncTable();
    cur = funcs;
    while(cur!=NULL)
    {
        printf("%s\n",cur->functionName);
        printf("Input Parameters :\n");
        IdentifierTable *test = createNewIdentifierTable();
        test = cur->inputParameterList;
        while(test!=NULL)
        {
            if(test->identifier.type==RECORD_DATATYPE)
            {
                printf("%s %s %s\n",idRepr(test->identifier.type),test->identifier.nameOfRecord,test->identifier.name);    
            }
            else
                printf("%s %s\n",idRepr(test->identifier.type),test->identifier.name);
            test=test->next;
        }           
        printf("Output parameters :\n");
        test = cur->outputParameterList;
        while(test!=NULL)
        {
            if(test->identifier.type==RECORD_DATATYPE)
            {
                printf("%s %s %s\n",idRepr(test->identifier.type),test->identifier.nameOfRecord,test->identifier.name);    
            }
            else
                printf("%s %s\n",idRepr(test->identifier.type),test->identifier.name);
            test=test->next;
        }
        cur=cur->next;
    }
}

void populateRecordTable(parseTree *t, recHashTable *recordTable)
{
    if(!isTerm(t->id))
    {
        if(t->id == typeDefinition)
        {

            if(findRecType(recordTable,t->children[1].token.lexeme) != -1){
                    printf("Line no. %lu : Redeclaration of record with name %s.\n",t->children[0].token.lineNumber,t->children[0].token.lexeme);
                    return;
            }
            
            parseTree temp = t->children[2];
            int type;
	    varHashTable fields;
            
            type = temp.children[0].children[1].children[0].id - TK_NUM;
            addVariable(&fields,temp.children[0].children[2].token.lexeme,type);
            type = temp.children[1].children[1].children[0].id - TK_NUM;
            addVariable(&fields,temp.children[1].children[2].token.lexeme,type);
            
            temp = temp.children[2];
            while(!isTerm(temp.children[0].id) && temp.children[0].id != eps)
            {
                type = temp.children[0].children[1].children[0].id - TK_NUM;
                addVariable(&fields,temp.children[0].children[2].token.lexeme,type);
                temp = temp.children[1];
            }
	    recordTypeCounter++;
            addRec(recordTable,t->children[1].token.lexeme,fields,recordTypeCounter)
        }
        else
        {
            int i;
            for(i = 0; i < a->numChildren; i++)
                populateRecordTable(&a->children[i], recordTable);
        }
    }
    
}
  
void printTheRecordTable(RecordTable *recs)
{
    printf("\nPrinting the record table details\n");
    RecordTable *cur = createRecordTable();
    cur = recs;
    
    while(cur!=NULL)
    {
        printf("%s\n",cur->recordName);
        printf("Field Parameters :\n");
        IdentifierTable *test = createNewIdentifierTable();
        test = cur->fields;
        while(test!=NULL)
        {
            printf("%s %s\n",idRepr(test->identifier.type),test->identifier.name);
            test=test->next;
        }           
        cur=cur->next;
    }
}

IdentifierTable *populateGlobalTable(parseTree *p, IdentifierTable *globals)
{
    if(p->isTerminal == false)
    {
        if(p->nonTerminal == declaration)
        {
            if(p->children[4].children[0].terminal.tokenClass==TK_COLON)
            {
                char *name;
                name = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                char *nameOfRecord;
                nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                
                int type;
                if(p->children[1].children[0].nonTerminal==primitiveDatatype)
                {
                    type = p->children[1].children[0].children[0].terminal.tokenClass;
                    strcpy(nameOfRecord,"");
                }
                else
                {
                    type = RECORD_DATATYPE;
                    strcpy(nameOfRecord, p->children[1].children[0].children[1].terminal.lexeme);
                }
                    
                strcpy(name, p->children[3].terminal.lexeme);
                if(findIdentifier(globals,name)!=0)
                {
                    printf("Line No. %llu : Redeclaration of identifier with name %s.\n",p->children[3].terminal.lineNo,name);
                    semantic_flag=1;
                    //exit(EXIT_FAILURE);
                    return globals;
                }
                globals = addIdentifier(globals, name, type, nameOfRecord);
            }
            
        }
        else
        {
            int i;
            for(i = 0; i < p->noOfChildren; i++)
                globals = populateGlobalTable(&p->children[i], globals);
        }
    }    
    return globals;
    
}
  
IdentifierTable *populateLocalTable(parseTree *p, IdentifierTable *globals)
{
    if(p->isTerminal == false)
    {
        if(p->nonTerminal == declaration)
        {
            if(p->children[4].children[0].terminal.tokenClass==eps)
            {
                char *name;
                name = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                char *nameOfRecord;
                nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                
                int type;
                if(p->children[1].children[0].nonTerminal==primitiveDatatype)
                {
                    type = p->children[1].children[0].children[0].terminal.tokenClass;
                    strcpy(nameOfRecord,"");
                }
                else
                {
                    type = RECORD_DATATYPE;
                    strcpy(nameOfRecord, p->children[1].children[0].children[1].terminal.lexeme);
                }
                    
                strcpy(name, p->children[3].terminal.lexeme);
                if(findIdentifier(globals,name)!=0)
                {
                    printf("Line No. %llu : Redeclaration of identifier with name %s\n",p->children[3].terminal.lineNo,name);
                    semantic_flag=1;
                    //exit(EXIT_FAILURE);
                    return globals;
                }
                globals = addIdentifier(globals, name, type, nameOfRecord);
            }
            
        }
        
        else
        {
            int i;
            for(i = 0; i < p->noOfChildren; i++)
                globals = populateLocalTable(&p->children[i], globals);
        }
    }
    else if(p->terminal.tokenClass == TK_END)
        return globals;
        
    return globals;
}

void printTheGlobalTable(IdentifierTable *globals, RecordTable *recs)
{
    printf("\nPrinting the global identifiers table details\n");
    IdentifierTable *cur = createNewIdentifierTable();
    cur = globals;
    
    while(cur!=NULL)
    {
        if(cur->identifier.type == RECORD_DATATYPE)
        {
            if(findRecord(recs,cur->identifier.nameOfRecord)!=0)
            {
                IdentifierTable *var = createNewIdentifierTable();
                printf("%s %s %s\n",idRepr(cur->identifier.type),cur->identifier.nameOfRecord,cur->identifier.name);
                var = findRecordDetails(recs,cur->identifier.nameOfRecord, var);
                while(var!=NULL)
                {
                    printf("\t%s %s\n",idRepr(var->identifier.type),var->identifier.name);
                    var=var->next;            
                }
            }
        }
        else
            printf("%s %s\n",idRepr(cur->identifier.type),cur->identifier.name);
        cur=cur->next;
    }
    printf("\n");
}

IdentifierTable *addContents(IdentifierTable *from, IdentifierTable *to, unsigned long long lineNo)
{
    IdentifierTable *copy;
    copy = createNewIdentifierTable();
    
    copy = from;
    while(copy!=NULL)
    {
        if(findIdentifier(to, copy->identifier.name)!=0)
        {
            printf("Line No. %llu : Redeclaration of identifier with name %s.\n",lineNo,copy->identifier.name);
            semantic_flag=1;
        }
        else
        {
            to = addIdentifier(to, copy->identifier.name, copy->identifier.type, copy->identifier.nameOfRecord);
        }
        copy=copy->next;
    }
    return to;
}
        
int compareIdentifierTables(IdentifierTable *callOut, IdentifierTable *funcOut)
{
    IdentifierTable *iter1, *iter2;
    for(iter1 = callOut,iter2 = funcOut; iter1 != NULL && iter2 != NULL; iter1 = iter1->next, iter2 = iter2->next)
    {
        if(iter1->identifier.type != iter2->identifier.type)
        {
            return 0;
        }
        if(iter1->identifier.type==RECORD_DATATYPE && (strcmp(iter1->identifier.nameOfRecord, iter2->identifier.nameOfRecord) != 0))
        {
            return 0;
        }
    }

    if(iter1!=NULL)
    {
        return 0;
    }
    if(iter2!=NULL)
    {
        return 0;
    }
        
    return 1;
}

int semantic(parseTree *p, FunctionTable *funcs, RecordTable *recs, IdentifierTable *globals, char *currFunc)
{
    if(p->isTerminal==false)
    {
        int type1,type2;
        char *lhs;
        int i;
        int temporary;
        IdentifierTable *locals = createNewIdentifierTable();
        char *functionName;
        IdentifierTable *input,*output;
        
        switch(p->nonTerminal)
        {
            case function:
                //add the local variable to the global
                locals = addContents(globals, locals, p->children[0].terminal.lineNo);
                functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                strcpy(functionName, p->children[0].terminal.lexeme);
                
                input = createNewIdentifierTable();
                output = createNewIdentifierTable();
                
                input = findFunctionInput(funcs, functionName);
                output = findFunctionOutput(funcs, functionName);
                
                locals = addContents(input, locals, p->children[1].children[0].terminal.lineNo);
                locals = addContents(output, locals, p->children[2].children[0].terminal.lineNo);
                locals = populateLocalTable(p, locals);
                
                //remove the local variables from the global
                for(i = 0; i < p->noOfChildren; i++)
                {
                    temporary = semantic(&p->children[i], funcs, recs, locals, functionName);
                }
                return temporary;
                
            case mainFunction:
                //add the local variable to the global
                locals = createNewIdentifierTable();
                locals = addContents(globals, locals, p->children[0].terminal.lineNo);
                locals = populateLocalTable(p, locals);
                
                //remove the local variables from the global
                for(i = 0; i < p->noOfChildren; i++)
                {
                    temporary = semantic(&p->children[i], funcs, recs, locals, "main");
                }
                return temporary;
            
            case assignmentStmt:
                //-----------------------------
                lhs = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                
                int lhs_type;
                if(p->children[0].children[1].children[0].terminal.tokenClass==eps)
                {
                    //for the TK_INT and TK_REAL case
                    lhs = p->children[0].children[0].terminal.lexeme;
                    
                    lhs_type = findIdentifier(globals,lhs);
                    if(lhs_type==0)
                    {
                        printf("Line no. %llu : Identifier %s undeclared or out-of-scope.\n",p->children[0].children[0].terminal.lineNo,lhs);
                        semantic_flag=1;
                        return -2;
                    }
                    if(lhs_type==RECORD_DATATYPE)
                    {
                        char *recordName;
                        recordName = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        recordName = findIdentifierRecordName(globals, lhs);
                        lhs_type = findRecord(recs, recordName);
                    }
                }
                else
                {
                    //for the RECORD_DATATYPE case
                    lhs = p->children[0].children[0].terminal.lexeme;
                    
                    char *recordName;
                    recordName = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                    
                    if(findIdentifier(globals, lhs)==RECORD_DATATYPE)
                    {
                        recordName = findIdentifierRecordName(globals, lhs);
                        if(findRecord(recs,recordName)!=0)
                        {
                            IdentifierTable *var = createNewIdentifierTable();
                            var = findRecordDetails(recs, recordName, var);
                            lhs_type = findIdentifier(var, p->children[0].children[1].children[1].terminal.lexeme);
                            if(lhs_type==0)
                            {
                                //if record type is found, but the variable type of record. is not found
                                printf("Line no. %llu : Undefined/Required component of identifier %s of record type %s\n",p->children[0].children[0].terminal.lineNo,lhs,recordName);
                                semantic_flag=1;
                                return -2;
                            }
                        }
                        else
                        {
                            printf("Line no. %llu : Identifier %s undefined or out-of-scope record variable\n",p->children[0].children[1].terminal.lineNo,lhs);
                            semantic_flag=1;
                            return -2;
                        }
                             
                    }
                    else
                    {
                        printf("Line no. %llu : Identifier %s undeclared or out-of-scope.\n",p->children[0].children[0].terminal.lineNo,lhs);
                        semantic_flag=1;
                        return -2;
                    }
                    
                    
                }
                
                //rhs of assignment statement
                int rhs_type = semantic(&p->children[2], funcs, recs, globals, currFunc);
                if(lhs_type != rhs_type)
                {
                    if(lhs_type>RECORD_DATATYPE)
                        printf("Line no. %llu : Semantic error type mismatch in assignment to LHS.\n",p->children[1].terminal.lineNo);    
                    else    
                        printf("Line no. %llu : Semantic error type mismatch in assignment to %s.\n",p->children[1].terminal.lineNo,idRepr(lhs_type));
                    semantic_flag=1;
                    return -2;
                }
                return lhs_type;
                //---------------------------------------------
                
                
            case arithmeticExpression:
                type1 = semantic(&p->children[0], funcs, recs, globals, currFunc);
                type2 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                if(type1!=type2 && type2!=-1)
                {
                    semantic_flag=1;
                    return -2;
                }
                return type1;
            
            case expPrime:
                if(p->noOfChildren==1)
                    return -1;
                type1 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                type2 = semantic(&p->children[2], funcs, recs, globals, currFunc);
                if(type1!=type2 && type2!=-1)
                {
                    semantic_flag=1;
                    return -2;
                }
                return type1;
                
            case term:
                type1 = semantic(&p->children[0], funcs, recs, globals, currFunc);
                type2 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                if(type1!=type2 && type2!=-1)
                {
                    semantic_flag=1;
                    return -2;
                }
                return type1;
                
            case factor:
                if(p->noOfChildren==3)
                {
                    type1 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                    return type1;
                }
                
                return semantic(&p->children[0], funcs, recs, globals, currFunc);
                
            case all:
                if((p->children[0].terminal.tokenClass)==TK_NUM)
                    return TK_INT;
                if((p->children[0].terminal.tokenClass)==TK_RNUM)
                    return TK_REAL;
                if((p->children[0].terminal.tokenClass)==TK_ID && ((p->noOfChildren==1)||(p->children[1].children[0].terminal.tokenClass==eps) ))
                {
                    type1 = findIdentifier(globals,p->children[0].terminal.lexeme);
                    if(type1==0)
                    {
                        printf("Line no. %llu : Identifier %s undeclared or out-of-scope\n",p->children[0].terminal.lineNo, p->children[0].terminal.lexeme);
                        semantic_flag=1;
                        return -2;
                    }
                    else if(type1 == RECORD_DATATYPE)
                    {
                        char *recordName;
                        recordName = (char *)malloc(sizeof(char)*MAX_ID_SIZE);    
                        recordName = findIdentifierRecordName(globals, p->children[0].terminal.lexeme);
                        type1 = findRecord(recs,recordName);
                        if(type1==0)
                        {
                            printf("Line no. %llu : Identifier %s undeclared or out-of-scope\n",p->children[0].terminal.lineNo, p->children[0].terminal.lexeme);
                            semantic_flag=1;
                            return -2;
                        }
                    }
                    return type1;
                }
                
                else
                {
                    // the record part
                    char *recordName;
                    recordName = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                    if(findIdentifier(globals,p->children[0].terminal.lexeme)==RECORD_DATATYPE)
                    {
                        recordName = findIdentifierRecordName(globals, p->children[0].terminal.lexeme);
                        if(findRecord(recs,recordName)!=0)
                        {
                            IdentifierTable *var = createNewIdentifierTable();
                            var = findRecordDetails(recs, recordName, var);
                            type1 = findIdentifier(var, p->children[1].children[1].terminal.lexeme);
                            if(type1==0)
                            {
                                //if record type is found, but the variable type of record. is not found
                                printf("Line no. %llu : Undefined/Required component of identifier %s of record type %s\n",p->children[0].terminal.lineNo,p->children[0].terminal.lexeme,recordName);
                                semantic_flag=1;
                                return -2;
                            }
                            return type1;
                        }
                        else
                        {
                            printf("Line no. %llu : Identifier %s undefined or out-of-scope record variable\n",p->children[0].terminal.lineNo,recordName);
                            semantic_flag=1;
                            return -2;
                        }
                    }
                    else
                    {
                        printf("Line no. %llu : Identifier %s undeclared or out-of-scope.\n",p->children[0].terminal.lineNo,p->children[0].terminal.lexeme);
                        semantic_flag=1;
                        return -2;
                    }
                }
                break;
                
            case termPrime:
                if(p->noOfChildren==1)
                    return -1;
                type1 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                type2 = semantic(&p->children[2], funcs, recs, globals, currFunc);
                if(type1!=type2 && type2!=-1)
                {
                    semantic_flag=1;
                    return -2;
                }
                return type1; 
                
            // assignment statement ends here
            //ioStmt starts here
            
            case ioStmt:
                if(p->children[0].terminal.tokenClass==TK_READ)
                {
                    char *name;
                    char *recordName;
                    char *fieldName;
                    name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    recordName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    fieldName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    
                    strcpy(name, p->children[2].children[0].terminal.lexeme);
                    //either TK_INT, TK_REAL or RECORD_DATATYPE
                    int find = findIdentifier(globals, name);
                    if(find==0)
                    {
                        printf("Line No. %llu : Identifier %s undeclared or out-of-scope\n",p->children[2].children[0].terminal.lineNo,name);
                        semantic_flag=1;
                        return -2;
                    }
                    if(find==RECORD_DATATYPE)
                    {
                        if(p->children[2].noOfChildren>1 && p->children[2].children[1].children[0].terminal.tokenClass!=eps)
                        {
                            strcpy(fieldName, p->children[2].children[1].children[1].terminal.lexeme);
                            recordName = findIdentifierRecordName(globals, name);
                            
                            //found variable name of the record type recordName
                            if(findRecord(recs,recordName)!=0)
                            {
                                IdentifierTable *var = createNewIdentifierTable();
                                var = findRecordDetails(recs, recordName, var);
                                lhs_type = findIdentifier(var, fieldName);
                            
                                if(lhs_type==0)
                                {
                                    //if record type is found, but the variable type of record. is not found
                                    printf("Line no. %llu : Undefined/Required component of identifier %s of record type %s\n",p->children[2].children[1].children[1].terminal.lineNo,name,recordName);
                                    semantic_flag=1;
                                    return -2;
                                }
                            }
                            else
                            {
                                printf("Line no. %llu : Identifier %s undefined or out-of-scope record variable\n",p->children[2].children[0].terminal.lineNo,name);
                                semantic_flag=1;
                                return -2;
                            }
                        }
                    }
                    
                }
                
                //else for TK_WRITE to be done
                else if(p->children[0].terminal.tokenClass==TK_WRITE)
                {
                    char *name;
                    char *recordName;
                    char *fieldName;
                    name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    recordName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    fieldName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    
                    if(p->children[2].noOfChildren>1 && p->children[2].children[1].children[0].terminal.tokenClass==TK_DOT)
                    {
                        //it is a record
                        strcpy(name, p->children[2].children[0].terminal.lexeme);
                        strcpy(fieldName, p->children[2].children[1].children[1].terminal.lexeme);
                    
                        if(findIdentifier(globals, name)==RECORD_DATATYPE)
                        {                      
                            recordName = findIdentifierRecordName(globals, name);
                    
                            //found variable name of the record type recordName
                            if(findRecord(recs,recordName)!=0)
                            {
                                IdentifierTable *var = createNewIdentifierTable();
                                var = findRecordDetails(recs, recordName, var);
                                lhs_type = findIdentifier(var, fieldName);
                                if(lhs_type==0)
                                {
                                    //if record type is found, but the variable type of record. is not found
                                    printf("Line no. %llu : Undefined/Required component of identifier %s of record type %s\n",p->children[2].children[0].terminal.lineNo,name,recordName);
                                    semantic_flag=1;
                                    return -2;
                                }
                            }
                            else
                            {
                                printf("Line no. %llu : Identifier %s undefined or out-of-scope record variable\n",p->children[2].children[0].terminal.lineNo, name);
                                semantic_flag=1;
                                return -2;
                            }
                        }
                        else
                        {
                            printf("Line no. %llu : Not a declared record variable %s\n",p->children[2].children[0].terminal.lineNo,name);
                            semantic_flag=1;
                            return -2;
                        }
                    }
                    else
                    {
                        //it is either TK_INT or TK_REAL, or TK_NUM or TK_RNUM
                        if(p->children[2].children[0].terminal.tokenClass!=TK_NUM && p->children[2].children[0].terminal.tokenClass!=TK_RNUM)
                        {
                            strcpy(name, p->children[2].children[0].terminal.lexeme);
                    
                            lhs_type = findIdentifier(globals, name);
                            if(lhs_type==0)   
                            {
                                printf("Line No. %llu : Identifier %s undeclared or out-of-scope\n",p->children[2].children[0].terminal.lineNo,name);
                                semantic_flag=1;
                                return -2;
                            }
                        }
                    }
                }
                
                return 0;
                break;
            
            //ioStmt done
              
            //function call statement
            case funCallStmt:
                functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                strcpy(functionName, p->children[2].terminal.lexeme);
                int declaredBefore = functionOrder(funcs, functionName, currFunc);
                if(declaredBefore == 1)
                {
                    printf("Line no. %llu : Function %s being called before declaration\n",p->children[2].terminal.lineNo, functionName);
                    semantic_flag=1;
                    return -2;
                }
                else if(declaredBefore == 0)
                {
                    printf("Line no. %llu : Function %s being called does not exist\n",p->children[2].terminal.lineNo, functionName);
                    semantic_flag=1;
                    return -2;
                }
                else if(declaredBefore == 2)
                {
                    //recursion
                    printf("Line no. %llu : Inside function %s, recursive function call not allowed\n",p->children[2].terminal.lineNo, functionName);
                    semantic_flag=1;
                    return -2;
                }
                else
                {
                    //have to verify parameters on calling
                    //input and output need to be verified
                    
                    IdentifierTable *callIn = createNewIdentifierTable();
                    IdentifierTable *callOut = createNewIdentifierTable();
                    
                    IdentifierTable *funcIn = createNewIdentifierTable();
                    IdentifierTable *funcOut = createNewIdentifierTable();
                    
                    funcOut = findFunctionOutput(funcs, functionName);
                    if(p->children[0].children[0].terminal.tokenClass==eps)
                    {
                        callOut = NULL;
                    }
                    else
                    {
                        parseTree tempo = p->children[0].children[1];
                        char *idName;
                        idName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                        
                        while(1)
                        {
                            strcpy(idName, tempo.children[0].terminal.lexeme);
                            int findId = findIdentifier(globals, idName);
                            if(findId==0)
                            {
                                printf("Line No. %llu : Identifier %s inside output parameters of function call undeclared or out-of-scope\n",tempo.children[0].terminal.lineNo,idName);
                                semantic_flag=1;
                                return -2;
                            }
                            else if(findId==RECORD_DATATYPE)
                            {
                                char *idRecordName;
                                idRecordName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                                idRecordName = findIdentifierRecordName(globals,idName);
                                callOut = addIdentifier(callOut, idName, RECORD_DATATYPE, idRecordName);
                            }
                            else
                            {
                                callOut = addIdentifier(callOut, idName, findId, "");
                            }
                            if(tempo.children[1].children[0].terminal.tokenClass!=eps)
                                tempo = tempo.children[1].children[1];
                            else
                                break;
                        }
                    }
                    
                    //callOut computed
                    //compare with funcOut
                    //computing funcOut
                    int out_compare = compareIdentifierTables(callOut, funcOut);
                    if(out_compare==0)
                    {
                        printf("Line No. %llu : Mismatch of output parameters in function call\n",p->children[2].terminal.lineNo);
                        semantic_flag=1;
                        return -2;
                    }
                    //output parameters compared
                    
                    //input parameter work
                    funcIn = findFunctionInput(funcs, functionName);
                    parseTree tempo = p->children[5].children[1];
                    char *idName;
                    idName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                    
                    while(1)
                    {
                        strcpy(idName, tempo.children[0].terminal.lexeme);
                        int findId = findIdentifier(globals, idName);
                        if(findId==0)
                        {
                            printf("Line No. %llu : Identifier %s inside input parameters of function call undeclared or out-of-scope\n",tempo.children[0].terminal.lineNo,idName);
                            semantic_flag=1;
                            return -2;
                        }
                        else if(findId==RECORD_DATATYPE)
                        {
                            char *idRecordName;
                            idRecordName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                            idRecordName = findIdentifierRecordName(globals,idName);
                            callIn = addIdentifier(callIn, idName, RECORD_DATATYPE, idRecordName);
                        }
                        else
                        {
                            callIn = addIdentifier(callIn, idName, findId, "");
                        }
                        if(tempo.children[1].children[0].terminal.tokenClass!=eps)
                            tempo = tempo.children[1].children[1];
                        else
                            break;
                    }
                    
                    
                    int in_compare = compareIdentifierTables(callIn, funcIn);
                    if(in_compare==0)
                    {
                        printf("Line No. %llu : Mismatch of input parameters in function call\n",p->children[2].terminal.lineNo);
                        semantic_flag=1;
                        return -2;
                    }
                
                }
                return 0;
                break;
                
            case returnStmt:
                if(strcmp(currFunc,"main")!=0)
                {
                    IdentifierTable *callOut = createNewIdentifierTable();
                    IdentifierTable *funcOut = createNewIdentifierTable();
                    
                    if(p->children[1].children[0].terminal.tokenClass==eps)
                    {
                        callOut = NULL;
                    }
                    else
                    {
                        parseTree tempo = p->children[1].children[1];
                        char *idName;
                        idName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                        
                        while(1)
                        {
                            strcpy(idName, tempo.children[0].terminal.lexeme);
                            int findId = findIdentifier(globals, idName);
                            if(findId==0)
                            {
                                printf("Line No. %llu : Identifier %s inside return statement of function %s undeclared or out-of-scope\n",tempo.children[0].terminal.lineNo,idName, currFunc);
                                semantic_flag=1;
                                return -2;
                            }
                            else if(findId==RECORD_DATATYPE)
                            {
                                char *idRecordName;
                                idRecordName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                                idRecordName = findIdentifierRecordName(globals,idName);
                                callOut = addIdentifier(callOut, idName, RECORD_DATATYPE, idRecordName);
                            }
                            else
                            {
                                callOut = addIdentifier(callOut, idName, findId, "");
                            }
                            if(tempo.children[1].children[0].terminal.tokenClass!=eps)
                                tempo = tempo.children[1].children[1];
                            else
                                break;
                        }
                    }
                    
                    funcOut = findFunctionOutput(funcs, currFunc);
                    int out_compare = compareIdentifierTables(callOut, funcOut);
                    if(out_compare==0)
                    {
                        printf("Line No. %llu : Mismatch between return identifiers and  output parameters in function c%s\n",p->children[2].terminal.lineNo, currFunc);
                        semantic_flag=1;
                        return -2;
                    }
                    return 0;
                }
                return 0;
            
            case booleanExpression:
                
                if(p->noOfChildren==3)
                {
                    int left_t, right_t;
                    if(p->children[0].children[0].terminal.tokenClass == TK_NUM)
                        left_t = TK_INT;
                    else if(p->children[0].children[0].terminal.tokenClass == TK_RNUM)
                        left_t = TK_REAL;
                    else 
                    {
                        left_t = findIdentifier(globals, p->children[0].children[0].terminal.lexeme);
                        if(left_t==0)
                        {
                            printf("Line No. %llu : Identifier %s undeclared or out-of-scope\n",p->children[0].children[0].terminal.lineNo,p->children[0].children[0].terminal.lexeme);
                            semantic_flag=1;
                            return -2;
                        }
                        if(left_t==RECORD_DATATYPE)
                        {
                            printf("Line No. %llu : Identifier %s used in conditional statement must be of type INT or REAL.\n",p->children[0].children[0].terminal.lineNo,p->children[0].children[0].terminal.lexeme);
                            semantic_flag=1;
                            return -2;
                        }
                    }
                    
                    if(p->children[2].children[0].terminal.tokenClass == TK_NUM)
                        right_t = TK_INT;
                    else if(p->children[2].children[0].terminal.tokenClass == TK_RNUM)
                        right_t = TK_REAL;
                    else 
                    {
                        right_t = findIdentifier(globals, p->children[2].children[0].terminal.lexeme);
                        if(right_t==0)
                        {
                            printf("Line No. %llu : Identifier %s undeclared or out-of-scope\n",p->children[2].children[0].terminal.lineNo,p->children[2].children[0].terminal.lexeme);
                            semantic_flag=1;
                            return -2;
                        }
                        if(right_t==RECORD_DATATYPE)
                        {
                            printf("Line No. %llu : Identifier %s used in conditional statement must be of type INT or REAL.\n",p->children[2].children[0].terminal.lineNo,p->children[2].children[0].terminal.lexeme);
                            semantic_flag=1;
                            return -2;
                        }
                    }
                    
                    if(left_t != right_t)
                    {
                        printf("Line no. %llu : IF conditional statement should evaluate to boolean.\n",p->children[1].children[0].terminal.lineNo);
                        semantic_flag=1;
                        return -2;
                    }
                }
                else
                {
                    if(p->noOfChildren==7)
                    {
                        int d1 = semantic(&p->children[1], funcs, recs, globals, currFunc);
                        if(d1!=-2)
                        {
                            int d2 = semantic(&p->children[5], funcs, recs, globals, currFunc);
                        }
                    }
                    else if(p->noOfChildren==2)
                    {
                        int d1 = semantic(&p->children[1], funcs, recs, globals, currFunc);    
                    }
                }
                return 0;           
                        
            default:
                for(i = 0; i < p->noOfChildren; i++)
                {
                    temporary = semantic(&p->children[i], funcs, recs, globals, currFunc);
                }
                return temporary;
                
        }
    }
    else
    {
        if(p->terminal.tokenClass==TK_ID)
        {
            int check = findIdentifier(globals, p->terminal.lexeme);
            if(check==0)
            {
                printf("Line No. %llu : Undefined identifier %s. Undeclared or out-of-scope\n",p->terminal.lineNo,p->terminal.lexeme);
                semantic_flag=1;
                return -2;
            }
        }
    }
    return 0;
}

int functionOrder(FunctionTable *funcs, char *calledFunc, char *currFunc)
{
    int first_find;
    first_find = findFunction(funcs, calledFunc);
    
    if(first_find==0)
        return 0;
    
    int second_find;
    second_find = findFunction(funcs, currFunc);
    
    if(first_find<second_find)
        return 1;
    if(first_find==second_find)
        return 2;
        
    return 3;
}  
    
int displaySymbolTableForFunction(IdentifierTable *globals, RecordTable *recs, char *funcName, int curOffset)
{
    IdentifierTable *cur = createNewIdentifierTable();
    cur = globals;
    
    int j;
    int tot_done = 0;
    while(cur!=NULL)
    {
        tot_done=0;
        if(cur->identifier.type == RECORD_DATATYPE)
        {
            if(findRecord(recs,cur->identifier.nameOfRecord)!=0)
            {
                IdentifierTable *var = createNewIdentifierTable();
                var = findRecordDetails(recs,cur->identifier.nameOfRecord, var);
                
                printf("%s",cur->identifier.name);
                for(j=strlen(cur->identifier.name);j<=20;j++)
                    printf(" ");
                while(var->next!=NULL)
                {
                    printf("%sx",idRepr(var->identifier.type));
                    
                    if(var->identifier.type == TK_INT)
                        tot_done += 2;
                    else if(var->identifier.type == TK_REAL)
                        tot_done += 4;
                    var=var->next;
                }
                printf("%s\t",idRepr(var->identifier.type));
                if(var->identifier.type == TK_INT)
                    tot_done += 2;
                else if(var->identifier.type == TK_REAL)
                    tot_done += 4;
                    
                if(strcmp(funcName,"Global")==0)
                    printf("%s              -\n",funcName);
                else
                {
                    printf("%s",funcName);
                    for(j=strlen(funcName);j<=15;j++)
                        printf(" ");
                    printf("%d\n",curOffset);
                    curOffset += tot_done;
                }
            }
        }
        else
        {
            if(cur->identifier.type == TK_INT)
                tot_done += 2;
            else if(cur->identifier.type == TK_REAL)
                tot_done += 4;
            if(strcmp(funcName,"Global")==0)
            {
                printf("%s",cur->identifier.name);
                for(j=strlen(cur->identifier.name);j<=20;j++)
                    printf(" ");
                
                printf("%s\t\t\t",idRepr(cur->identifier.type));
                printf("%s",funcName);
                for(j=strlen(funcName);j<=15;j++)
                    printf(" ");
                printf("-\n");
            }
            else
            {
                printf("%s",cur->identifier.name);
                for(j=strlen(cur->identifier.name);j<=20;j++)
                    printf(" ");
                printf("%s\t\t\t",idRepr(cur->identifier.type));
                printf("%s",funcName);
                for(j=strlen(funcName);j<=15;j++)
                    printf(" ");
                printf("%d\n",curOffset);
                    
                curOffset += tot_done;
            }
        }
        cur=cur->next;
    }
    return curOffset;
}

IdentifierTable *addFunctionIdentifiers(IdentifierTable *locals, char *currFunc, parseTree *p)
{
    if(p->isTerminal==false)
    {
        char *functionName;
        int i;
        switch(p->nonTerminal)
        {
            case function:
                functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
                strcpy(functionName, p->children[0].terminal.lexeme);
                if(strcmp(functionName, currFunc)==0)
                {
                    locals = populateLocalTable(p, locals);
                    return locals;
                }
                else
                    return NULL;
                break;
            
            case mainFunction:
                if(strcmp(currFunc,"main")==0)
                {
                    locals = populateLocalTable(p, locals);  
                    return locals;
                }
                else
                    return NULL;
                break;
            
            default:
                for(i=0;i<p->noOfChildren;i++)
                {
                    IdentifierTable *temp = createNewIdentifierTable();
                    temp = addFunctionIdentifiers(locals, currFunc, &p->children[i]); 
                    if(temp!=NULL) 
                        return temp;//locals = addContents(temp, locals, 0);
                }
                return locals;
        }
    }    
    return NULL;               
}

IdentifierTable *reverse(IdentifierTable *locals)
{
    IdentifierTable *prev = createNewIdentifierTable();
    IdentifierTable *curr = createNewIdentifierTable();
    IdentifierTable *link = createNewIdentifierTable();
    
    curr = locals;
    while(curr!=NULL)
    {
        link = curr->next;
        curr->next = prev;
        prev = curr;
        curr = link;
    }
    return prev;
}

void displaySymbolTable(IdentifierTable *globals, RecordTable *recs, FunctionTable *funcs, parseTree *p)
{
    printf("\n");
    printf("Lexeme                  Type\t\t\t Scope         Offset\n");
    
    int garbage = displaySymbolTableForFunction(globals, recs, "Global", 0);
    
    FunctionTable *curFunc = createFuncTable();
    curFunc = funcs;
    while(curFunc!=NULL)
    {
        int offset_func = 0;
        IdentifierTable *locals = createNewIdentifierTable();
        char *functionName;
        functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
        strcpy(functionName, curFunc->functionName);
    
        IdentifierTable *input = createNewIdentifierTable();
        IdentifierTable *output = createNewIdentifierTable();
    
        input = curFunc->inputParameterList;
        output = curFunc->outputParameterList;
        
        locals = addContents(input, locals, 0);
        offset_func = displaySymbolTableForFunction(locals, recs, functionName,0);
        locals = createNewIdentifierTable();
        
        locals = addContents(output, locals, 0);
        offset_func = displaySymbolTableForFunction(locals, recs, functionName,offset_func);
        locals = createNewIdentifierTable();
 
        locals = addFunctionIdentifiers(locals, functionName, p);
        locals = reverse(locals);
        offset_func = displaySymbolTableForFunction(locals, recs, functionName,offset_func);
        curFunc=curFunc->next;
    }
    
    IdentifierTable *locals = createNewIdentifierTable();
    char *functionName;
    functionName = (char *)malloc(MAX_ID_SIZE*sizeof(char));
    strcpy(functionName, "_main");

    locals = addFunctionIdentifiers(locals, "main", p);    
    locals = reverse(locals);
    garbage = displaySymbolTableForFunction(locals, recs, functionName, 0);
    
    printf("\n");
}
