#include "semanticAnalysis.h"
extern bool semantic_flag;

int semantic(astNode *ast, funcHashTable *functionTable, recHashTable *recordTable, varHashTable *globalTable, char *funcPresent)
{
    if(!isTerm(ast->id))
    {
        char* nameLHS;
        int i;
        char *nameOfRecord;
        char *nameOfRHSRecord, *nameOfLHSRecord;
        int typeLHS, typeRHS, typeExpPrime,typeTerm, typeFactor,typeTermPrime, typeArithmeticExp, typeVar, boolExpFirst, boolExpSecond, booLExp, k;

        varHashTable *varTable;
        varHashTable local,*input,*output;
        createVarTable(&local);
        tokenInfo tempToken;
        char* name;
        
        switch(ast->id)
        {
        case function:
            createVarTable(input);
            createVarTable(output);

            tempToken = ast->children[0].token;
            input = getFuncInputList(functionTable,tempToken.lexeme);
            output = getFuncOutputList(functionTable,tempToken.lexeme);
            populateLocalTable(ast,&local,recordTable,globalTable);


            addVarHashTable(&local,globalTable);
            addVarHashTable(&local,input);
            addVarHashTable(&local,output);

            for(i = 0; i < ast->numChildren; i++){
                k=semantic(&ast->children[i], functionTable, recordTable,&local,tempToken.lexeme);
            }

            return k;

        case mainFunction:
            createVarTable(&local);
            populateLocalTable(ast,&local,recordTable,globalTable);
            addVarHashTable(&local,globalTable);

            for(i = 0; i < ast->numChildren; i++){
                k=semantic(&ast->children[i], functionTable, recordTable,&local,"main");
            }

            return k;
            
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
            typeRHS = semantic(&ast->children[childIdToIndex(ast,arithmeticExpression)], functionTable, recordTable, globalTable, funcPresent);

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
                    printf("Line %lu : Variable %s is not declared\n",ast->children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(ast,TK_ID)].token.lexeme);
                    semantic_flag=false;
                    return -2;
                }

                return typeVar;
            }

            else
            {
                // the record part
                nameOfRecord = (char *)malloc(sizeof(char)*MAX_ID_SIZE);
                typeVar = findVariableType(globalTable,ast->children[childIdToIndex(ast,TK_ID)].token.lexeme);
                if(typeVar>=2)
                {
                    nameOfRecord = getRecordName(recordTable,typeVar);

                    if(findRecType(recordTable,nameOfRecord)!=-1)


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
            name = (char *)malloc(MAX_ID_SIZE*sizeof(char));
            int type;
            varHashTable *temp;

            if(ast->children[0].id == TK_READ){
                tempToken = ast->children[1].children[0].token;
                type = findVariableType(globalTable,tempToken.lexeme);

                if(type == -1){
                    //ERROR
                }else if(type >= 2){
                    strcpy(name,getRecordName(recordTable,type));
                    temp = getRecFields(recordTable,name);

                    if(ast->children[1].children[1].children[0].id != eps){
                        tempToken = ast->children[1].children[1].children[0].token;
                        if(findVariableType(temp,tempToken.lexeme)==-1){
                            //ERROR
                        }
                    }
                }
            }else if(ast->children[0].id == TK_WRITE){
                if(ast->children[1].children[0].id != TK_NUM && ast->children[1].children[0].id != TK_RNUM){
                    tempToken = ast->children[1].children[0].token;
                    type = findVariableType(globalTable,tempToken.lexeme);

                    if(type == -1){
                        //ERROR
                    }else if(type >= 2){
                        if(ast->children[1].children[1].children[0].id != eps){
                            tempToken = ast->children[1].children[1].children[0].token;
                            strcpy(name,getRecordName(recordTable,type));
                            temp = getRecFields(recordTable,name);

                            if(findVariableType(temp,tempToken.lexeme) == -1){
                                //ERROR
                            }
                        }
                    }
                }
            }
            //ioStmt done

            //function call statement
        case funCallStmt:
            tempToken = ast->children[1].token;
            k = functionOrder(functionTable,tempToken.lexeme,funcPresent);

            if(k == -1){
                //recursion
            }else if(k == 0){
                //DNE
            }else if(k == 1){
                //outOfOrder
            }

            varHashTable *output,*input,outCall,inCall;
            output = getFuncOutputList(functionTable,tempToken.lexeme);
            input = getFuncOutputList(functionTable,tempToken.lexeme);
            createVarTable(&outCall);
            createVarTable(&inCall);
            astNode tempTree;

            if(ast->children[0].children[0].id == idList){
                tempTree = ast->children[0].children[0];

                do{
                    tempToken = tempTree.children[0].token;
                    if(findVariableType(globalTable,tempToken.lexeme) == -1){
                        //ERROR
                    }else{
                        int g;
                        g = findVariableType(globalTable,tempToken.lexeme);
                        addVariable(&outCall,tempToken.lexeme,g);
                    }

                    tempTree = tempTree.children[1].children[0];
                }while(tempTree.children[1].children[0].id != eps);
            }

            if(compareVarHashTables(output,&outCall) == 0){
                //ERROR
            }

            if(ast->children[2].children[0].id == idList){
                tempTree = ast->children[2].children[0];

                do{
                    tempToken = tempTree.children[0].token;
                    if(findVariableType(globalTable,tempToken.lexeme) == -1){
                        //ERROR
                    }else{
                        int g;
                        g = findVariableType(globalTable,tempToken.lexeme);
                        addVariable(&inCall,tempToken.lexeme,g);
                    }

                    tempTree = tempTree.children[1].children[0];
                }while(tempTree.children[1].children[0].id != eps);

            }

            if(compareVarHashTables(input,&inCall) == 0){
                //ERROR
            }


        case returnStmt:
            if(strcmp(funcPresent,"main")!=0){
                varHashTable *output,outCall;
                output = getFuncOutputList(functionTable,funcPresent);
                createVarTable(&outCall);
                astNode tempTree;

                if(ast->children[0].children[0].id == idList){
                    tempTree = ast->children[0].children[0];

                    do{
                        tempToken = tempTree.children[0].token;
                        if(findVariableType(globalTable,tempToken.lexeme) == -1){
                            //ERROR
                        }else{
                            int g;
                            g = findVariableType(globalTable,tempToken.lexeme);
                            addVariable(&outCall,tempToken.lexeme,g);
                        }

                        tempTree = tempTree.children[1].children[0];
                    }while(tempTree.children[1].children[0].id != eps);
                }

                if(compareVarHashTables(output,&outCall) == 0){
                    //ERROR
                }

            }
            
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
                        printf("Line %lu : Variable %s is not declared\n",ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lexeme);
                        semantic_flag=false;
                        return -2;
                    }
                    if(typeLHS>=2)
                    {
                        printf("Line %lu : Variable %s should be of type INT or REAL in a boolean expression\n",ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lineNumber, ast->children[childIdToIndex(ast,var)].children[childIdToIndex(ast,TK_ID)].token.lexeme);
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
                    boolExpFirst = semantic(&ast->children[0], functionTable, recordTable, globalTable, funcPresent);
                    boolExpSecond = semantic(&ast->children[2], functionTable, recordTable, globalTable, funcPresent);
                    if(boolExpFirst==-2||boolExpSecond==-2)
                        return -2;

                }
                else if(childIdToIndex(ast,TK_NOT)!=-1)
                {
                    booLExp = semantic(&ast->children[1], functionTable, recordTable, globalTable, funcPresent);
                    if(booLExp==-2)
                        return -2;
                }
            }
            return 0;

        default:
            for(i = 0; i < ast->numChildren; i++)
            {
                k = semantic(&ast->children[i], functionTable, recordTable, globalTable, funcPresent);
            }
            return k;

        }
    }
    else
    {
        if(ast->id==TK_ID)
        {
            int find;
            find = findVariableType(globalTable, ast->token.lexeme);
            if(find==-1)
            {
                printf("Line %lu : Variable %s is not declared\n",ast->token.lineNumber,ast->token.lexeme);
                semantic_flag=false;
                return -2;
            }
        }
    }
    return 0;
}
