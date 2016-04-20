#include "populateSymbolTable.h"

extern bool semantic_flag;

int childIdToIndex(parseTree *parent, int id){
    int i;
    for(i=0;i<parent->numChildren;++i){
        if(parent->children[i].id==id)
            return i;
    }
    return -1;
}

void populateFunctionTable(parseTree *t, funcHashTable *functionTable, recHashTable* recordTable){
    
    if(!isTerm(t->id))
    {
        if(t->id == function)
        {
            char* name = (char *)malloc(SIZE_MAX_FUNID*sizeof(char));
            int nameChild;
            childName=childIdToIndex(t,TK_FUNID);
            strcpy(name,t->children[nameChild].token.lexeme);
            varHashTable* inputList = (varHashTable*)malloc(sizeof(varHashTable));
            varHashTable* outputList = (varHashTable*)malloc(sizeof(varHashTable));

            if(findFunc(functionTable,name)==true)
            {
                printf("Line %lu : Redeclaration of function : %s\n",t->children[childName].token.lineNumber,name);
                semantic_flag = false;
            }

            parseTree parameterListNode = t->children[childToIndex(input_par)].children[childIdToIndex(parameter_list)];
                        
            while(1)
            {
                int formalParameterType;
                
                if(parameterListNode.children[childToIndex(dataType)].children[0].id==primitiveDatatype)
                {
                    formalParameterType = parameterListNode.children[childToIndex(dataType)].children[childToIndex(primitiveDatatype)].children[0].id;
                    addVariable(inputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);

                }    
                else
                {
                    formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(dataType)].children[childToIndex(constructedDatatype)].children[childToIndex(TK_RECORDID)].token.lexeme);
                    addVariable(inputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);
                }
                if(parameterListNode.children[childToIndex(remaining_list)].children[0].id == eps)
                    break;
                parameterListNode = parameterListNode.children[childToIndex(remaining_list)].children[childToIndex(parameter_list)];
            }
            
            parseTree outputParameterNode = t->children[childToIndex(output_par)];
            if(outputParameterNode.children[0].id == eps)
                outputList=NULL;
            else
            {
                parameterListNode = t->children[childToIndex(output_par)].children[childIdToIndex(parameter_list)];
                while(1)
                {
                    if(parameterListNode.children[childToIndex(dataType)].children[0].id==primitiveDatatype)
                    {
                        formalParameterType = parameterListNode.children[childToIndex(dataType)].children[childToIndex(primitiveDatatype)].children[0].id;
                        addVariable(outputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);

                    }    
                    else
                    {
                        formalParameterType = findRecType(recordTable, parameterListNode.children[childToIndex(dataType)].children[childToIndex(constructedDatatype)].children[childToIndex(TK_RECORDID)].token.lexeme);
                        addVariable(outputList, parameterListNode.children[childToIndex(TK_ID)].token.lexeme, formalParameterType);
                    }
                
                    if(parameterListNode.children[childToIndex(remaining_list)].children[0].id == eps)
                        break;
                    parameterListNode = parameterListNode.children[childToIndex(remaining_list)].children[childToIndex(parameter_list)];
                }
            }

           addFunc(functionTable, name, inputList, outputList);
        }
        else
        {
            int i;
            for(i = 0; i < t->numChildren; i++)
                populateFunctionTable(&t->children[i], functionTable);
        }
    }    
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

RecordTable *populateRecordTable(parseTree *p, RecordTable *recs)
{
    if(p->isTerminal == false)
    {
        if(p->nonTerminal == typeDefinition)
        {
            char *recordName;
            recordName = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
            IdentifierTable *record_fields = createNewIdentifierTable();
            
            if(p->children[1].isTerminal == true)
            {
                strcpy(recordName,p->children[1].terminal.lexeme);
                if(findRecord(recs,recordName)!=0)
                {
                    printf("Line no. %llu : Redeclaration of record with name %s.\n",p->children[1].terminal.lineNo,recordName);
                    //exit(EXIT_FAILURE);
                    semantic_flag=1;
                    return recs;
                }
            }
            
            parseTree temp = p->children[2];
            int type;
            
            type = temp.children[0].children[1].children[0].terminal.tokenClass;
            record_fields = addIdentifier(record_fields, temp.children[0].children[3].terminal.lexeme, type, "");
            
            type = temp.children[1].children[1].children[0].terminal.tokenClass;
            record_fields = addIdentifier(record_fields, temp.children[1].children[3].terminal.lexeme, type, "");
            
            temp = temp.children[2];
            while(temp.children[0].isTerminal!=true && temp.children[0].terminal.tokenClass!=eps)
            {
                type = temp.children[0].children[1].children[0].terminal.tokenClass;
                record_fields = addIdentifier(record_fields, temp.children[0].children[3].terminal.lexeme, type, "");
                temp = temp.children[1];
            }
            
            recs = addRecord(recs, recordName, record_fields);
        }
        else
        {
            int i;
            for(i = 0; i < p->noOfChildren; i++)
                recs = populateRecordTable(&p->children[i], recs);
        }
    }    
    return recs;
    
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

int semantic(parseTree *ast, funcHashTable *functionTable, recHashTable *recordTable, varHashTable *globalTable, char *funcPresent)
{
    if(p->isTerminal==false)
    {
        char* nameLHS;
        int i;
        char *nameOfRecord;
        char *nameOfRHSRecord, *nameOfLHSRecord;
        int typeLHS, typeRHS, typeExpPrime,typeTerm, typeFactor,typeTermPrime, typeArithmeticExp, typeVar;

        varHashTable *varTable;
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



                nameLHS=(char *)malloc(sizeof(char)*MAX_ID_SIZE);
                strcpy(nameLHS, ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lexeme);
                if(ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,singleOrRecIdPrime)].children[0].id==eps)// if a variable like 'a'
                {
                    //for the TK_INT and TK_REAL case

                    typeLHS = findVariableType(globalTable,nameLHS);
                    if(typeLHS ==-1)
                    {
                        printf("Line %lu : Variable %s is not declared\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber,nameLHS);
                        semantic_flag=false;
                        return -2;
                    }
                    if(typeLHS >= 2)
                    {

                        nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        nameOfRecord = getRecordName(recordTable, typeLHS);
                    }
                }
                else //if a field variable like book_instance.page
                {
                    //for the RECORD_DATATYPE case


                    nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                    
                    if(findVariableType(globalTable,nameLHS)>=2) //if book_instance's type is a record
                    {
                        nameOfRecord = getRecordName(recordTable, typeLHS);


                        createVarTable(varTable);
                        varTable = getRecFields(recordTable, nameOfRecord); //book's fields
                        typeLHS = findVariableType(varTable, ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,singleOrRecIdPrime)].children[childIdToIndex(ast,TK_FIELDID)].token.lexeme/*field id*/); //field "page"'s type
                        if(typeLHS==-1)//didn't find field page in field list
                        {
                            //if record type is found, but the variable type of record. is not found
                            printf("Line %lu : Field %s (of record type %s) not declared\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,singleOrRecIdPrime)].children[childIdToIndex(ast,TK_FIELDID)].token.lineNumber,ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,singleOrRecIdPrime)].children[childIdToIndex(ast,TK_FIELDID)].token.lexeme,nameOfRecord);
                            semantic_flag=false;
                            return -2;
                        }


                    }
                    else //book_instance is either not stored in id table or is not an instance of record type book
                    {
                        printf("Line %lu : Identifier %s not declared as record identifier\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber,nameLHS);
                        semantic_flag=false;
                        return -2;
                    }
                    
                    
                }
                
                //rhs of assignment statement
                typeRHS = semantic(&p->children[childIdToIndex(ast,arithmeticExpression)], functionTable, recordTable, globalTable, funcPresent);

                if(typeLHS != typeRHS)
                {

                    if(typeLHS>=2 && typeRHS>=2){
                        nameOfRHSRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        nameOfRHSRecord = getRecordName(recordTable, typeRHS);
                        nameOfLHSRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        nameOfLHSRecord = getRecordName(recordTable, typeLHS);
                        printf("Line %lu : Type Mismatch <LHS-%s of type record:%s and RHS of type record:%s>\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, nameLHS,nameOfLHSRecord,nameOfRHSRecord);
                    }
                    else if(typeLHS>=2 && typeRHS<2){
                        nameOfLHSRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        nameOfLHSRecord = getRecordName(recordTable, typeLHS);
                        printf("Line %lu : Type Mismatch <LHS-%s of type record:%s and RHS of type %s>\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, nameLHS,nameOfLHSRecord,idToName(typeIntOrReal(typeRHS)));
                    }
                    else if(typeLHS<2 && typeRHS>=2){
                        nameOfRHSRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                        nameOfRHSRecord = getRecordName(recordTable, typeRHS);
                        printf("Line %lu : Type Mismatch <LHS-%s of type %s and RHS of type record:%s >\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, nameLHS,idToName(typeIntOrReal(typeLHS)),nameOfRHSRecord);
                    }
                    else
                        printf("Line %lu : Type Mismatch <LHS-%s of type %s and RHS of type %s >\n",ast->children[childIdToIndex(ast,singleOrRecId)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, nameLHS,idToName(typeIntOrReal(typeLHS)),idToName(typeIntOrReal(typeRHS)));
                    semantic_flag=false;
                    return -2;
                }
                return typeLHS;
                //---------------------------------------------
                
                
            case arithmeticExpression:
                typeTerm = semantic(&ast->children[childIdToIndex(ast,term)], functionTable, recordTable, globalTable, funcPresent);
                typeExpPrime = semantic(&ast->children[childIdToIndex(ast,expPrime)], functionTable, recordTable, globalTable, funcPresent);
                if(typeTerm!=typeExpPrime && typeExpPrime!=-1)
                {
                    semantic_flag=false;
                    return -2;
                }
                return typeTerm;
            
            case expPrime:
                if(childIdToIndex(ast,eps)!=-1)
                    return -1;
                typeTerm = semantic(&ast->children[childIdToIndex(ast,term)], functionTable, recordTable, globalTable, funcPresent);
                typeExpPrime = semantic(&ast->children[childIdToIndex(ast,expPrime)], functionTable, recordTable, globalTable, funcPresent);
                if(typeTerm!=typeExpPrime && typeExpPrime!=-1)
                {
                    semantic_flag=false;
                    return -2;
                }
                return typeTerm;

                
            case term:
                typeFactor = semantic(&ast->children[childIdToIndex(ast,factor)], functionTable, recordTable, globalTable, funcPresent);
                typeTermPrime = semantic(&ast->children[childIdToIndex(ast,termPrime)], functionTable, recordTable, globalTable, funcPresent);
                if(typeFactor!=typeTermPrime && typeTermPrime!=-1)
                {
                    semantic_flag=false;
                    return -2;
                }
                return typeFactor;
                
            case factor:
                if(childIdToIndex(ast,arithmeticExpression)!=-1)
                {
                    typeArithmeticExp = semantic(&ast->children[childIdToIndex(ast,arithmeticExpression)], functionTable, recordTable, globalTable, funcPresent);
                    return typeArithmeticExp;
                }
                
                return semantic(&ast->children[childIdToIndex(ast,all)], functionTable, recordTable, globalTable, funcPresent);
                
            case all:
                if((ast->children[0].id)==TK_NUM)
                    return TK_INT;
                if((ast->children[0].id)==TK_RNUM)
                    return TK_REAL;
                if((ast->children[0].id)==TK_ID && (ast->children[childIdToIndex(ast,temp)].children[0].id==eps))
                {
                    typeVar = findVariableType(globalTable,ast->children[childIdToIndex(ast,TK_ID)].token.lexeme);
                    if(typeVar==-1)
                    {
                        printf("Line %lu : Variable %s is not declared\n",ast->children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(ast,TK_ID)].token.lineNumber);
                        semantic_flag=false;
                        return -2;
                    }

                    return typeVar;
                }
                
                else
                {
                    // the record part
                    char *nameOfRecord;
                    nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                    int typeVar = findVariableType(globalTable,ast->children[childIdToIndex(ast,TK_ID)].token.lexeme);
                    if(typeVar>=2)
                    {
                        nameOfRecord = getRecordName(recordTable,typeVar);

                        if(findRecType(recordTable,nameOfRecord)!=-1)

                        varHashTable varTable;
                        varTable = getRecFields(recordTable, nameOfRecord);
                        typeVar = findVariableType(varTable, ast->children[childIdToIndex(ast,temp)].children[childIdToIndex(ast,TK_FIELDID)].token.lexeme);
                        if(typeVar==-1)
                        {
                            //if record type is found, but the variable type of record. is not found
                            printf("Line %lu : Field %s (of record type %s) not declared\n",ast->children[childIdToIndex(ast,temp)].children[childIdToIndex(ast,TK_FIELDID)].token.lineNumber,ast->children[childIdToIndex(ast,temp)].children[childIdToIndex(ast,TK_FIELDID)].token.lexeme,nameOfRecord);
                            semantic_flag=false;
                            return -2;
                        }
                        return typeVar;

                       
                    }
                    else
                    {
                        printf("Line %lu : Identifier %s not declared as record identifier\n",ast->children[childIdToIndex(ast,TK_ID)].token.lineNumber,ast->children[childIdToIndex(ast,TK_ID)].token.lexeme);
                        semantic_flag=false;
                        return -2;
                    }
                }
                break;
                
            case termPrime:
                if(childIdToIndex(ast,eps) !=-1)
                    return -1;
                typeFactor = semantic(&ast->children[childIdToIndex(ast,factor)], functionTable, recordTable, globalTable, funcPresent);
                typeTermPrime = semantic(&ast->children[childIdToIndex(ast,termPrime)], functionTable, recordTable, globalTable, funcPresent);
                if(typeFactor!=typeTermPrime && typeTermPrime!=-1)
                {
                    semantic_flag=false;
                    return -2;
                }
                return typeFactor;
                
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
                                callOut = ad-dIdentifier(callOut, idName, findId, "");
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
                
                if(childIdToIndex(ast,relationalOp)!=-1)
                {
                    if(ast->children[childIdToIndex(ast,var)].children[0].id == TK_NUM)
                        typeLHS = TK_INT;
                    else if(ast->children[childIdToIndex(ast,var)].children[0].id == TK_RNUM)
                        typeLHS = TK_REAL;
                    else 
                    {
                        typeLHS = findVariableType(globalTable, ast->children[childIdToIndex(ast,var)].children[0].token.lexeme);
                        if(typeLHS==-1)
                        {
                            printf("Line %lu : Variable %s is not declared\n",ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(var)].children[childIdToIndex(ast,TK_ID)].token.lexeme);
                            semantic_flag=false;
                            return -2;
                        }
                        if(typeLHS>=2)
                        {
                            printf("Line %lu : Variable %s should be of type INT or REAL in a boolean expression\n",ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(var)].children[childIdToIndex(ast,TK_ID)].token.lexeme);
                            semantic_flag=false;
                            return -2;
                        }
                    }
                    
                    if(ast->children[2].children[0].id == TK_NUM)
                        typeRHS = TK_INT;
                    else if(ast->children[2].children[0].id == TK_RNUM)
                        typeRHS = TK_REAL;
                    else 
                    {
                        typeRHS = findVariableType(globalTable, ast->children[2].children[0].token.lexeme);
                        if(typeRHS==-1)
                        {
                            printf("Line %lu : Variable %s is not declared\n",ast->children[2].children[0].token.lineNumber, ast->children[2].children[0].token.lexeme);
                            semantic_flag=false;
                            return -2;
                        }
                        if(typeRHS>=2)
                        {
                            printf("Line %lu : Variable %s should be of type INT or REAL in a boolean expression\n",ast->children[2].children[0].token.lineNumber, ast->children[2].children[0].token.lexeme);
                            semantic_flag=false;
                            return -2;
                        }
                    }
                    
                    if(typeLHS != typeRHS)
                    {
                        printf("Line %lu : Type Mismatch between left entities being compared in relational operation: %s and %s \n",ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lineNumber,idToName(typeIntOrReal(typeLHS)),idToName(typeIntOrReal(typeRHS)));
                        semantic_flag=false;
                        return -2;
                    }
                }
                else
                {
                    if(childIdToIndex(ast,logicalOp)!=-1)
                    {
                        int boolExpFirst = semantic(&ast->children[0], functionTable, recordTable, globalTable, funcPresent);
                        int boolExpSecond = semantic(&ast->children[2], functionTable, recordTable, globalTable, funcPresent);
                        if(boolExpFirst==-2||boolExpSecond==-2)
                            return -2;

                    }
                    else if(childIdToIndex(ast,TK_NOT)!=-1)
                    {
                        int booLExp = semantic(&ast->children[1], functionTable, recordTable, globalTable, funcPresent);
                        if(booLExp==-2)
                            return -2;
                    }
                }
                return 0;           
                        
            default:
                for(i = 0; i < ast->numChildren; i++)
                {
                    temporary = semantic(&ast->children[i], functionTable, recordTable, globalTable, funcPresent);
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
