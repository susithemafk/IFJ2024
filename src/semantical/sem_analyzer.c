#include <stdlib.h>
#include <string.h>
#include "semantical/sem_analyzer.h"
#include "syntaxical/ast.h"
#include "utility/enumerations.h"
#include <stdio.h> 

// Function to analyze the whole program
enum ERR_CODES analyzeProgram(Program *program, SymTable *table) {
    #ifdef DEBUG
    printf("\n\n=== %sAnalyzing program%s ===\n\n", COLOR_WARN, COLOR_RESET);
    #endif
    if (!program) return E_INTERNAL;

    enum ERR_CODES err;

    #ifdef DEBUG
    puts("Analyzing functions");
    #endif
    // go function by function
    unsigned int size = getSize(program->functions);
    for (unsigned int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        SymFunctionPtr funDef = symTableFindFunction(table, function->id.name);
        if (!funDef) return E_INTERNAL;


        // enter the function scope
        if (!symTableMoveScopeDown(table, SYM_FUNCTION)) return E_INTERNAL;

        unsigned int size1 = getSize(function->params);
        #ifdef DEBUG
        printf("Analyzing function %s\n", function->id.name);
        #endif
        for (unsigned int j = 0; j < size1; j++) {
            // add the params to the function scope
            Param *param = (Param *)getDataAtIndex(function->params, j);
            SymFunctionParamPtr arg = (SymFunctionParamPtr)getDataAtIndex(funDef->paramaters, j);
            err = analyzeParam(param, table, arg);
            if (err != SUCCESS) return err;
        }

        int retCount = 0;   
        err = analyzeBody(&function->body, table, funDef, &retCount);
        if (err != SUCCESS) return err;


        // invalid amount of returns
        if (strcmp(funDef->funcName, "main") != 0) {
            if (funDef->returnType != dTypeVoid && retCount == 0) return E_SEMANTIC_BAD_FUNC_RETURN;
        }

        // exit the function scope
        err = symTableExitScope(table);
        if (err != SUCCESS) return err;
    }
    return SUCCESS;
}

// function to analyze a paramater
enum ERR_CODES analyzeParam(Param *param, SymTable *table, SymFunctionParamPtr arg) {
    if (!param || !table) return E_INTERNAL;

    #ifdef DEBUG
    printf("Analyzing param %s\n", param->id.name);
    #endif
    
    SymVariable *var = symTableDeclareVariable(
        table, 
        param->id.name, 
        arg->type,
        false, 
        arg->nullable
    );
    if (!var) return E_SEMANTIC_REDIFINITION;
    #ifdef DEBUG
    printf("Param %s declared\n", param->id.name);
    if (!var) printf("ERROC redifinition\n");
    printf("Param analyzed\n");
    #endif
    return SUCCESS;
}

// Function to analyze the body of a function
enum ERR_CODES analyzeBody(Body *body, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    #ifdef DEBUG
    puts("Analyzing body");
    #endif
    if (!body) return E_INTERNAL;
    enum ERR_CODES err;

    unsigned int size = getSize(body->statements);
    for (unsigned int i = 0; i < size; i++) {
        #ifdef DEBUG
        printf("Analyzing statement %d\n", i);
        #endif
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        err = analyzeStatement(statement, table, currentFunc, retCount);
        #ifdef DEBUG
        printf("Statement %d analyzed\n", i);
        printf("err: %d\n", err);   
        #endif
        if (err != SUCCESS) return err;
    }

    #ifdef DEBUG
    puts("Body analyzed");
    #endif
    return SUCCESS;
}

// Function to analyze a statement
enum ERR_CODES analyzeStatement(Statement *statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    #ifdef DEBUG
    puts("Analyzing statement");
    #endif
    if (!statement) return E_INTERNAL;
    enum ERR_CODES err;

    switch (statement->type) {
        case FunctionCallStatementType:
            #ifdef DEBUG
            puts("Analyzing function call");
            #endif
            // check if the function is void 
            err = analyzeFunctionCall(&statement->data.function_call_statement, table);
            #ifdef DEBUG
            printf("Function call analyzed\n");
            printf("err: %d\n", err);
            #endif
            if (err != SUCCESS) return err;

            // check if the function is void
            SymFunctionPtr SymFunction = symTableFindFunction(table, statement->data.function_call_statement.func_id.name);
            #ifdef DEBUG
            if (!SymFunction) printf("Function definition not found\n");
            #endif
            if (!SymFunction) return E_SEMANTIC_UND_FUNC_OR_VAR;
            #ifdef DEBUG
            printf("Function return type: %d\n", SymFunction->returnType);
            if (SymFunction->returnType != dTypeVoid) printf("Function is not void\n");
            printf("Function is void, we good\n");
            #endif
            if (SymFunction->returnType != dTypeVoid) return E_SEMANTIC_BAD_FUNC_RETURN;
            return SUCCESS;

        case ReturnStatementType:
            return analyzeReturnStatement(&statement->data.return_statement, table, currentFunc, retCount);
        case WhileStatementType:
            return analyzeWhileStatement(&statement->data.while_statement, table, currentFunc, retCount);
        case IfStatementType:
            return analyzeIfStatement(&statement->data.if_statement, table, currentFunc, retCount);
        case AssigmentStatementType:
            return analyzeAssigmentStatement(&statement->data.assigment_statement, table);
        case VariableDefinitionStatementType:
            return analyzeVariableDefinitionStatement(&statement->data.variable_definition_statement, table);
        default:
            return E_INTERNAL;
    }
}

// # note we nont check th return value here, beccuase this is used in mulaple places
enum ERR_CODES analyzeFunctionCall(FunctionCall *function_call, SymTable *table) {
    if (!function_call) return E_INTERNAL;
    enum ERR_CODES err = SUCCESS;

    // check if the function is defined
    SymFunctionPtr SymFunction = symTableFindFunction(table, function_call->func_id.name);
    if (!SymFunction) return E_SEMANTIC_UND_FUNC_OR_VAR;

    unsigned int defSize = getSize(SymFunction->paramaters);
    unsigned int size = getSize(function_call->arguments);

    if (defSize != size) return E_SEMANTIC_INVALID_FUN_PARAM;

    // check if the types of the arguments is correct
    for (unsigned int i = 0; i < size; i++) {
        Expression *param = (Expression *)getDataAtIndex(function_call->arguments, i);
        SymFunctionParamPtr defParam = (SymFunctionParamPtr)getDataAtIndex(SymFunction->paramaters, i);
        if (!param || !defParam) return E_INTERNAL;

        // if it is a literal, it can be converted
        if (param->expr_type == LiteralExpressionType) {

            if (defParam->type == dTypeNone) return SUCCESS;

            enum COVERSION_FLAGS flag = canCovertLiteral(&param->data.literal, defParam->type);
            if (flag == CONV_NOT_POSSIBLE) {
                return E_SEMANTIC_INVALID_FUN_PARAM;
                }
            if (!nullCompatabilityCheck(defParam->nullable, param->data.literal.data_type.is_nullable)) return E_SEMANTIC_INCOMPATABLE_TYPES;
            if (err != SUCCESS) return err;
            switch(flag) {
                case CONV_NONE:
                    param->conversion = NoConversion;
                    break;
                case CONV_TO_INT:
                    param->conversion = IntToFloat;
                    break;
                case CONV_TO_FLOAT:
                    param->conversion = FloatToInt;
                    break;
                default:
                    return E_INTERNAL;
            }
            continue;
        }

        // if it is an identifier, we need to check if it is defined, and its types
        if (param->expr_type == IdentifierExpressionType) {
            // null compatability
            SymVariable *var = symTableFindVariable(table, param->data.identifier.name);
            if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
            if (defParam->type == dTypeNone) return SUCCESS;
            if (!nullCompatabilityCheck(defParam->nullable, var->nullable)) {
                return E_SEMANTIC_INVALID_FUN_PARAM;
            }
            if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE; // if we call a func, with a var that has not been assingend a type yet
            if (defParam->type != var->type) return E_SEMANTIC_INVALID_FUN_PARAM;
            continue;
        }
        // no other types are allowed
        return E_INTERNAL;
    }
    return SUCCESS;
}

// Function to analyze a binary expression
bool nullCompatabilityCheck(bool nullMain, bool nullSecond) {
    if (!nullMain && nullSecond) return false;
    return true;
}

// Function to analyze a binary expression
enum ERR_CODES analyzeReturnStatement(ReturnStatement *return_statement, SymTable *table,  SymFunctionPtr currentFunc, int *retCount) {
    if (!return_statement) return E_INTERNAL;

    #ifdef DEBUG
    puts("Analyzing return statement");
    #endif

    enum ERR_CODES err;

    enum DATA_TYPES returnType;
    bool nullable;
    err = analyzeExpression(&return_statement->value, table, &returnType, &nullable);
    #ifdef DEBUG
    printf("Return type: %d\n", returnType);
    printf("Nullable: %d\n", nullable);
    printf("err: %d\n", err);
    #endif
    if (err != SUCCESS) return err;

    #ifdef DEBUG
    if (currentFunc->returnType != returnType) printf("Return type is not the same\n");
    if (!currentFunc->nullableReturn && nullable) printf("Return type is nullable\n");
    printf("return valid\n");
    #endif

    // check if the return type is correct
    if (currentFunc->returnType != returnType) return E_SEMANTIC_BAD_FUNC_RETURN;
    if (!currentFunc->nullableReturn && nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    (*retCount)++;

    return SUCCESS;
}

// Function to analyze a while statement
enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement, SymTable *table, SymFunctionPtr currentFunc, int* retCount) {
    #ifdef DEBUG
    puts("Analyzing while statement");
    #endif
    if (!while_statement) return E_INTERNAL;

    enum ERR_CODES err;
    enum DATA_TYPES type;
    bool nullable;

    err = analyzeExpression(&while_statement->condition, table, &type, &nullable);
    #ifdef DEBUG
    printf("While type: %d\n", type);
    printf("While nullable: %d\n", nullable);
    printf("err: %d\n", err);
    #endif
    if (err != SUCCESS) return err;

    if (!symTableMoveScopeDown(table, SYM_WHILE)) return E_INTERNAL; // move to while

    // we should have this type if whiele -> while (exp) |a| { body }
    if (while_statement->non_nullable.name) {
        #ifdef DEBUG
        puts("analyzing while (a) |na| {...}");
        #endif
        // need to somehow find the variable in the while scope?
        char * varName = while_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        #ifdef DEBUG
        if (!var->nullable) printf("err Variable %s is not nullable\n", varName);
        if (var->type == dTypeNone) printf("err Variable %s has no type\n", varName);
        printf("while var valid\n");
        #endif
        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            while_statement->non_nullable.name,
            var->type,
            false, // this war will not exist after the while loop, so it should be const
            false
        );

        #ifdef DEBUG
        if (!nonNullVar) printf("Variable %s redifined\n", while_statement->non_nullable.name);
        printf("while non nullable var valid\n");
        #endif

        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;
        while_statement->non_nullable_var = nonNullVar;
    } else {
        #ifdef DEBUG
        puts("analyzing while (truthExp) {...}");
        if (type != dTypeBool) printf("While type is not bool\n");
        printf("while type valid\n");
        #endif
        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    err = analyzeBody(&while_statement->body, table, currentFunc, retCount);

    #ifdef DEBUG
    puts("while body analyzed");
    printf("while error: %d\n", err);
    #endif

    if (err != SUCCESS) return err;

    // exit the while scope
    return symTableExitScope(table);
}

// Function to analyze an if statement
enum ERR_CODES analyzeIfStatement(IfStatement *if_statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    if (!if_statement) return E_INTERNAL;

    #ifdef DEBUG
    puts("Analyzing if statement");
    #endif

    enum ERR_CODES err = SUCCESS;

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&if_statement->condition, table, &type, &nullable);

    #ifdef DEBUG
    printf("If type: %d\n", type);
    printf("If nullable: %d\n", nullable);
    printf("err: %d\n", err);
    #endif

    if (err != SUCCESS) return err;

    // move to if scope
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL;

    if (if_statement->non_nullable.name) {
        #ifdef DEBUG
        puts("analyzing if (a) |na| {...}");
        #endif
        // need to somehow find the variable in the while scope?
        char * varName = if_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        #ifdef DEBUG
        if (!var->nullable) printf("Variable %s is not nullable\n", varName);
        if (var->type == dTypeNone) printf("Variable %s has no type\n", varName);
        printf("if var valid\n");
        #endif
        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            if_statement->non_nullable.name,
            var->type,
            false, // this war will not exist after the while loop, so it should be const
            false
        );

        #ifdef DEBUG
        if (!nonNullVar) printf("Variable %s redifined\n", if_statement->non_nullable.name);
        printf("if non nullable var valid, with name: %s\n", if_statement->non_nullable.name);
        #endif
        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;
        if_statement->non_nullable_var = nonNullVar;
    } else {
        #ifdef DEBUG
        puts("analyzing if (truthExp) {...}");
        if (type != dTypeBool) printf("If type is not bool\n");
        printf("if type valid\n");
        #endif
        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    // analyze the if body
    err = analyzeBody(&if_statement->if_body, table, currentFunc, retCount);
    #ifdef DEBUG
    puts("if body analyzed");
    printf("if error: %d\n", err);
    #endif
    if (err != SUCCESS) return err;

    // exit the if scope
    err = symTableExitScope(table);
    if (err != SUCCESS) return err;

    // analyze the else body
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL; // move to else
    err = analyzeBody(&if_statement->else_body, table, currentFunc, retCount);
    #ifdef DEBUG
    puts("else body analyzed");
    printf("else error: %d\n", err);
    #endif
    if (err != SUCCESS) return err;

    // exit the else scope
    return symTableExitScope(table);
}

// Function to analyze an assigment statement
enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement, SymTable *table) {
    if (!statement) return E_INTERNAL;

    enum ERR_CODES err;

    SymVariable *var = symTableFindVariable(table, statement->id.name);
    if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
    #ifdef DEBUG
    if (!var->mutable && var->id != 0) printf("Variable %s is not mutable\n", statement->id.name);
    if (var->id == 0) printf("Variable %s is global\n", statement->id.name);
    printf("var valid\n");
    #endif
    if (!var->mutable && var->id != 0) return E_SEMANTIC_REDIFINITION; // var-id 0 is the global var _

    var->modified = true; // we modifed the variable

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&statement->value, table, &type, &nullable);
    #ifdef DEBUG
    printf("Assigment type: %d\n", type);
    printf("Assigment nullable: %d\n", nullable);
    printf("err: %d\n", err);
    #endif
    if (err != SUCCESS) return err;

    #ifdef DEBUG
    if (var->id == 0) printf("Variable %s is global\n", statement->id.name);
    if (type == dTypeUndefined && var->nullable) printf("Variable %s is nullable\n", statement->id.name);
    if (var->nullable != nullable) printf("Variable %s is not nullable\n", statement->id.name);
    if (var->type == dTypeNone) {
        printf("Variable %s has no type\n", statement->id.name);
        printf("assigning type %d\n", type);    
    }
    #endif
    // after we know, the expresion is valid, we can check if the types are the same
    if (var->id == 0) return SUCCESS; // we can assign anything to the global var _ the value should be discarded

    // var a = null;
    if (type == dTypeUndefined && var->nullable) return SUCCESS;

    // null compatability
    if (var->nullable != nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    #ifdef DEBUG
    if (var->type == dTypeNone) {
        printf("Variable %s has no type\n", statement->id.name);
        printf("assigning type %d\n", type);    
    }
    #endif
    // in case we did not define the type, we can do it now
    if (var->type == dTypeNone) {
        var->type = type;
        return SUCCESS;
    }

    #ifdef DEBUG
    if (var->type != type) printf("Variable %s is not the same type\n", statement->id.name);
    printf("var type valid\n");
    #endif

    // check if the types are the same
    if (var->type != type) {
        if (var->nullable && type == dTypeVoid)
            return SUCCESS; // var a = null;
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }
    return SUCCESS;
}

// Function to analyze a variable definition statement
enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement, SymTable *table) {
    #ifdef DEBUG
    puts("Analyzing variable definition statement");
    #endif

    if (!statement) return E_INTERNAL;
    enum ERR_CODES err;

    #ifdef DEBUG
    printf("declaring variable %s\n", statement->id.name);
    printf("Variable type: %d\n", statement->type.data_type);
    printf("Variable nullable: %d\n", statement->type.is_nullable);
    printf("Variable const: %d\n", statement->isConst);
    #endif

    // declare the var
    SymVariable *var = symTableDeclareVariable(
        table, 
        statement->id.name, 
        statement->type.data_type,
        !statement->isConst,
        statement->type.is_nullable
    );
    #ifdef DEBUG
    if (!var) printf("Variable %s redifined\n", statement->id.name);
    printf("var declare valid\n");
    #endif
    if (!var) return E_SEMANTIC_REDIFINITION;

    bool nullable;
    enum DATA_TYPES type;
    err = analyzeExpression(&statement->value, table, &type, &nullable);
    #ifdef DEBUG
    printf("analyzing right side of the definition\n");
    printf("exp type: %d\n", type);
    printf("exp nullable: %d\n", nullable);
    printf("err: %d\n", err);
    #endif
    if (err != SUCCESS) return err;
    
    #ifdef DEBUG
    if (type == dTypeUndefined) printf("variable has no type, return UNKNOWN_TYPE\n");
    if (var->type == dTypeNone) printf("variable has no type, return assigning form exp\n");
    #endif
    // var a = null;
    if (type == dTypeUndefined) return E_SEMANTIC_UNKNOWN_TYPE;
    if (type == dTypeVoid) return E_SEMANTIC_INCOMPATABLE_TYPES;
    
    // nullability can be assinged on declaration
    var->nullable = nullable;

    // in case we did not define the type, we can do it now
    if (var->type == dTypeNone) {
        var->type = type;
        return SUCCESS;
    }

    #ifdef DEBUG
    printf("Variable type: %d\n", var->type);
    printf("Variable nullable: %d\n", var->nullable);
    if (var->type != type) printf("Variable %s is not the same type\n", statement->id.name);
    printf("var type valid\n");
    #endif

    // check if the types are the same
    if (var->type != type) {
        // if (var->nullable && type == dTypeVoid)  // TODO check if this is correct
        //     return SUCCESS; // var a = null;
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }
    return SUCCESS;
}

// Function to analyze an expression
enum ERR_CODES analyzeExpression(Expression *expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable) {
    if (!expr) return E_INTERNAL;

    enum ERR_CODES err = SUCCESS;
    SymVariable *var;
    SymFunctionPtr fncDef;


    switch (expr->expr_type) {
        // function call
        case FunctionCallExpressionType:
            err = analyzeFunctionCall(&expr->data.function_call, table);
            // try to find the defintion and the return type and set then
            fncDef = symTableFindFunction(table, expr->data.function_call.func_id.name);
            if (!fncDef) return E_SEMANTIC_UND_FUNC_OR_VAR;
            *returnType = fncDef->returnType;
            *resultNullable = fncDef->nullableReturn;
            break;

        // literal
        case LiteralExpressionType:
            *returnType = expr->data.literal.data_type.data_type;
            *resultNullable = expr->data.literal.data_type.is_nullable; // since null literal is nullable i guess
            break;

        // binary expression
        case BinaryExpressionType:
            err = analyzeBinaryExpression(&expr->data.binary_expr, table, returnType, resultNullable);
            break;

        // identifier
        case IdentifierExpressionType:
            if (!expr->data.identifier.name) {
                *returnType = dTypeUndefined;
                *resultNullable = true;
                break;
            } // null
            var = symTableFindVariable(table, expr->data.identifier.name);
            // check for NULL i guess, since null is and identifier
            if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
            *returnType = var->type;
            *resultNullable = var->nullable;
            break;

        // bad ast construction
        default:
            err = E_INTERNAL;
            break;
    }
    return err;
}

// Function to set soeme flag,s if can be convert to i32 or f64
enum COVERSION_FLAGS canCovertLiteral(Literal *literal, enum DATA_TYPES expectedType) {

    if (expectedType == dTypeNone) return CONV_NONE;

    // tryitn to convert to i32
    if (expectedType == dTypeF64) {
        if (literal->data_type.data_type == dTypeF64) return CONV_NONE;
        if (literal->data_type.data_type == dTypeI32) return CONV_TO_FLOAT;
        return CONV_NOT_POSSIBLE;
    }

    // tryitn to convert to f64
    if (expectedType == dTypeI32) {
        if (literal->data_type.data_type == dTypeI32) return CONV_NONE;
        if (literal->data_type.data_type != dTypeF64) return CONV_NOT_POSSIBLE;

        char *value = literal->value;
        char *dotPosition = strchr(value, '.');
        if (dotPosition == NULL) return CONV_NOT_POSSIBLE; // a float, but no dot

        char *p = dotPosition + 1;
        while (*p == '0') p++;

        if (*p != '\0') return CONV_NOT_POSSIBLE;

        return CONV_TO_INT;
    }

    return CONV_NOT_POSSIBLE;
}

// Function to analyze a binary expression
enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable) {
    // base case
    if (!binary_expr) {
        *resultNullable = false;
        *returnType = dTypeNone;
        return SUCCESS;
    };

    enum ERR_CODES err;

    enum DATA_TYPES leftType, rightType;
    bool leftNullable, rightNullable;

    // get the ret type of the left epx
    err = analyzeExpression(binary_expr->left, table, &leftType, &leftNullable);
    if (err != SUCCESS) return err;

    // get the ret type of the right exp
    err = analyzeExpression(binary_expr->right, table, &rightType, &rightNullable);
    if (err != SUCCESS) return err;

    // in here, we know, theat these two types, are valid by themselfs, need to check, if they are compatible
    if (
        leftType == dTypeU8 || 
        rightType == dTypeU8
    ) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // check for unkown types at compile time
    if (leftType == dTypeNone || rightType == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

    // handle not allowd operators between nullable types
    switch(binary_expr->operation) {
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
            if (leftNullable || rightNullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        default:
            break;
    }

    // we can compare i32 and f64
    switch(binary_expr->operation) {
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
            *returnType = dTypeBool;
            *resultNullable = false;
            return SUCCESS;
        default:
            break;
    }

    // aretmetic operations
    if (leftType == rightType) {
        *returnType = leftType;
        *resultNullable = false;
        return SUCCESS;
    }

    // left is an i32 literal, try to conver it to f64 since the types are not the same
    if (binary_expr->left->expr_type == LiteralExpressionType && binary_expr->left->data_type.data_type == dTypeI32) {
        enum COVERSION_FLAGS flag = canCovertLiteral(&binary_expr->left->data.literal, rightType);
        if (flag == CONV_NOT_POSSIBLE) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (flag == CONV_TO_FLOAT) binary_expr->left->conversion = FloatToInt;
        return E_INTERNAL; 
    }

    // right is a i32 literal, try to conver it to f64 since the types are not the same
    if (binary_expr->right->expr_type == LiteralExpressionType && binary_expr->right->data_type.data_type == dTypeI32) {
        enum COVERSION_FLAGS flag = canCovertLiteral(&binary_expr->right->data.literal, leftType);
        if (flag == CONV_NOT_POSSIBLE) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (flag == CONV_TO_FLOAT) binary_expr->right->conversion = FloatToInt;
        return E_INTERNAL; 
    }

    // two variables, not the same type
    if (leftType != rightType && binary_expr->left->expr_type == IdentifierExpressionType && binary_expr->right->expr_type == IdentifierExpressionType) {
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // we have incompatible types, but we can convert them
    *returnType = dTypeF64;
    *resultNullable = false;
    return SUCCESS;
}
