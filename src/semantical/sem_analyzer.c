/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 * @file sem_analyzer.c
 * @date 20.10. 2024
 * @brief main src file for the semantical analyzer
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

#ifdef USE_CUSTOM_STRUCTURE
#include "semantical/sem_analyzer.h"
#include "syntaxical/ast.h"
#include "utility/enumerations.h"
#else
#include "sem_analyzer.h"
#include "ast.h"
#include "enumerations.h"
#endif

// Function to analyze the whole program
enum ERR_CODES analyzeProgram(Program *program, SymTable *table) {
    DEBUG_PRINT("\n\n=== %sAnalyzing program%s ===\n\n", COLOR_WARN, COLOR_RESET);
    if (!program) return E_INTERNAL;

    enum ERR_CODES err;
    DEBUG_PRINT("Gettinf function definitions");
    // go function by function, add the definitions
    unsigned int size = getSize(program->functions);
    for (unsigned int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        SymFunctionPtr funDef = symInitFuncDefinition();
        if (!funDef) return E_INTERNAL;
        if (strcmp(function->id.name, "main") == 0 && (function->returnType.data_type != dTypeVoid || getSize(function->params) != 0)) return E_SEMANTIC_INVALID_FUN_PARAM;
        DEBUG_PRINT("Function name: %s\nFunction return type: %d\nFunftion return nullable: %d", function->id.name, function->returnType.data_type, function->returnType.is_nullable);
        bool result = symEditFuncDef(funDef, function->id.name, function->returnType.data_type, (function->returnType.is_nullable) ? 1 : 0);
        if (!result) return E_INTERNAL;
        DEBUG_PRINT_IF(!result, "Function edit failed");
        DEBUG_PRINT("Function edit error: %d", err);

        // go thorougt the params, and add them to the function
        unsigned int size1 = getSize(function->params);
        for (unsigned int j = 0; j < size1; j++) {
            Param *param = (Param *)getDataAtIndex(function->params, j);
            bool result = symAddParamToFunc(funDef, param->type.data_type, param->type.is_nullable);
            if (!result) return E_INTERNAL;
        }
       
        // add the function to the symbol table
        err = symTableAddFunction(table, funDef);
        if (err != SUCCESS) return err;
    }

    // need to find the main function, it is is missing, return error
    SymFunctionPtr mainFunc = symTableFindFunction(table, "main");
    if (!mainFunc) return E_SEMANTIC_UND_FUNC_OR_VAR;

    // if the main has paramaters, or return type is not void, return error 4
    if (getSize(mainFunc->paramaters) != 0 || mainFunc->returnType != dTypeVoid) return E_SEMANTIC_BAD_FUNC_RETURN;

    DEBUG_PRINT("Analyzing function bodies");

    // go througt the function bodies
    for (unsigned int i = 0; i < size; i++) {

        Function *function = (Function *)getDataAtIndex(program->functions, i);
        DEBUG_PRINT("analyzing body of function %s", function->id.name);
        SymFunctionPtr funDef = symTableFindFunction(table, function->id.name);

        // enter the function scope
        if (!symTableMoveScopeDown(table, SYM_FUNCTION)) return E_INTERNAL;

        unsigned int size1 = getSize(function->params);
        DEBUG_PRINT("Adding args to symtable: %s", function->id.name);
        for (unsigned int j = 0; j < size1; j++) {
            // add the params to the function scope
            Param *param = (Param *)getDataAtIndex(function->params, j);
            err = analyzeParam(param, table);
            if (err != SUCCESS) return err;
        }

        int retCount = 0;   
        err = analyzeBody(&function->body, table, funDef, &retCount);
        if (err != SUCCESS) return err;

        // invalid amount of returns
        if (strcmp(funDef->funcName, "main") != 0) {
            if (funDef->returnType != dTypeVoid && retCount == 0) return E_SEMANTIC_BAD_FUNC_RETURN;
            if (funDef->returnType == dTypeVoid && retCount > 0) return E_SEMANTIC_BAD_FUNC_RETURN;
        }

        // exit the function scope
        err = symTableExitScope(table);
        if (err != SUCCESS) return err;
    }
    return SUCCESS;
}

// function to analyze a paramater
enum ERR_CODES analyzeParam(Param *param, SymTable *table) {
    if (!param || !table) return E_INTERNAL;

    DEBUG_PRINT("Analyzing param %s\n type of param: %d\nparam nullable: %d", param->id.name, param->type.data_type, param->type.is_nullable);
    
    SymVariable *var = symTableDeclareVariable(
        table, 
        param->id.name, 
        param->type.data_type,
        false, 
        param->type.is_nullable,
        false
    );
    if (!var) return E_SEMANTIC_REDIFINITION;
    param->id.var = var;

    DEBUG_PRINT("Param %s declared\n", param->id.name); 
    return SUCCESS;
}

// Function to analyze the body of a function
enum ERR_CODES analyzeBody(Body *body, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    DEBUG_PRINT("Analyzing body");
    if (!body) return E_INTERNAL;
    enum ERR_CODES err;

    unsigned int size = getSize(body->statements);
    for (unsigned int i = 0; i < size; i++) {
        DEBUG_PRINT("Analyzing statement %d", i);
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        err = analyzeStatement(statement, table, currentFunc, retCount);
        DEBUG_PRINT("Statement %d analyzed", i);
        DEBUG_PRINT("err: %d", err);
        if (err != SUCCESS) return err;
    }

    DEBUG_PRINT("Body analyzed");
    return SUCCESS;
}

// Function to analyze a statement
enum ERR_CODES analyzeStatement(Statement *statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    DEBUG_PRINT("Analyzing statement");
    if (!statement) return E_INTERNAL;
    enum ERR_CODES err;

    switch (statement->type) {
        case FunctionCallStatementType:
            DEBUG_PRINT("Analyzing function call");
            // check if the function is void 
            err = analyzeFunctionCall(&statement->data.function_call_statement, table);
            DEBUG_PRINT("Function call analyzed");
            DEBUG_PRINT("err: %d\n", err);
            if (err != SUCCESS) return err;

            // check if the function is void
            SymFunctionPtr SymFunction = symTableFindFunction(table, statement->data.function_call_statement.func_id.name);
            DEBUG_PRINT((SymFunction) ? "Function definition found\n" : "Function definition not found");
            if (!SymFunction) return E_SEMANTIC_UND_FUNC_OR_VAR;
            DEBUG_PRINT("Function return type: %d\n", SymFunction->returnType);
            DEBUG_PRINT((SymFunction->returnType != dTypeVoid) ? "Function is not void\n" : "Function is void, we good");
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

    // check if the function is defined
    SymFunctionPtr SymFunction = symTableFindFunction(table, function_call->func_id.name);
    if (!SymFunction) return E_SEMANTIC_UND_FUNC_OR_VAR;

    function_call->func = SymFunction;
    function_call->return_type.data_type = SymFunction->returnType;
    function_call->return_type.is_nullable = SymFunction->nullableReturn;

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

            if (param->data.literal.data_type.data_type == dTypeU8) return E_SEMANTIC_INVALID_FUN_PARAM;

            if (!nullCompatabilityCheck(defParam->nullable, param->data.literal.data_type.is_nullable)) return E_SEMANTIC_INCOMPATABLE_TYPES;

            // in functions we dont convert the types
            if (param->data.literal.data_type.data_type != defParam->type) return E_SEMANTIC_INCOMPATABLE_TYPES;
            
            param->conversion = NoConversion;

            continue;
        }

        // if it is an identifier, we need to check if it is defined, and its types
        if (param->expr_type == IdentifierExpressionType) {
            // null compatability
            SymVariable *var = symTableFindVariable(table, param->data.identifier.name);
            param->data.identifier.var = var;

            if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
            if (defParam->type == dTypeNone) return SUCCESS;
            if (!nullCompatabilityCheck(defParam->nullable, var->nullable)) {
                return E_SEMANTIC_INVALID_FUN_PARAM;
            }
            if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE; // if we call a func, with a var that has not been assingend a type yet
            if (defParam->type != var->type) return E_SEMANTIC_INVALID_FUN_PARAM;

            param->conversion = NoConversion;

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

    DEBUG_PRINT("Analyzing return statement");

    enum ERR_CODES err;
    enum DATA_TYPES returnType;
    bool nullable;

    DEBUG_PRINT("adress of return statement: %p", return_statement->value);

    // empty return
    if (return_statement->empty) {
        if (currentFunc->returnType != dTypeVoid) return E_SEMANTIC_BAD_FUNC_RETURN;
        (*retCount)++;
        return SUCCESS;
    }

    // in case we have a not empty return, but the fucntion is void
    if (currentFunc->returnType == dTypeVoid) return E_SEMANTIC_BAD_FUNC_RETURN;

    // return null;
    if (
        return_statement->value.expr_type == LiteralExpressionType && 
        !return_statement->value.data.literal.value && 
        return_statement->value.data.literal.data_type.data_type == dTypeNone &&
        return_statement->value.data.literal.data_type.is_nullable
    ) {
        if (!currentFunc->nullableReturn) return E_SEMANTIC_INVALID_FUN_PARAM;
        (*retCount)++;
        return_statement->value.conversion = NoConversion;
        return SUCCESS;
    }

    // return <exp>;
    err = analyzeExpression(&return_statement->value, table, &returnType, &nullable);
    DEBUG_PRINT("Return type: %d\nNullable: %d\nError: %d\n", returnType, nullable, err);
    if (err != SUCCESS) return err;

    if (returnType == dTypeVoid && nullable) return SUCCESS; // epmty return

    DEBUG_PRINT_IF(returnType == dTypeUndefined, "Return type is undefined");
    DEBUG_PRINT_IF(currentFunc->returnType == dTypeNone, "Function return type is undefined");
    DEBUG_PRINT("return is valid");

    // check if the return type is correct
    if (currentFunc->returnType != returnType) return E_SEMANTIC_INVALID_FUN_PARAM;
    if (!currentFunc->nullableReturn && nullable) return E_SEMANTIC_INVALID_FUN_PARAM;

    (*retCount)++;

    return SUCCESS;
}

// Function to analyze a while statement
enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement, SymTable *table, SymFunctionPtr currentFunc, int* retCount) {
    if (!while_statement) return E_INTERNAL;
    DEBUG_PRINT("Analyzing while statement");

    enum ERR_CODES err;
    enum DATA_TYPES type;
    bool nullable;

    err = analyzeExpression(&while_statement->condition, table, &type, &nullable);
    DEBUG_PRINT("While type: %d\nWhile nullable: %d\nError: %d", type, nullable, err);
    if (err != SUCCESS) return err;

    if (!symTableMoveScopeDown(table, SYM_WHILE)) return E_INTERNAL; // move to while

    // we should have this type if whiele -> while (exp) |a| { body }
    if (while_statement->non_nullable.name) {
        DEBUG_PRINT("Analyzing while (a) |na| {...}");
        // need to somehow find the variable in the while scope?
        char * varName = while_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        while_statement->condition.data.identifier.var = var;

        DEBUG_PRINT_IF(!var->nullable, "Variable %s is not nullable", varName);
        DEBUG_PRINT_IF(var->type == dTypeNone, "Variable %s has no type", varName);
        DEBUG_PRINT("While var valid");

        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            while_statement->non_nullable.name,
            var->type,
            false, // this war will not exist after the while loop, so it should be const
            false,
            var->valueKnonwAtCompileTime
        );
     
        DEBUG_PRINT_IF(!nonNullVar, "Variable %s redifined", while_statement->non_nullable.name);
        DEBUG_PRINT("While non nullable var valid");

        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;

        while_statement->non_nullable.var = nonNullVar;

    } else {
        // we should have this type if whiele -> while (exp) { body }
        DEBUG_PRINT("Analyzing while (truthExp) {...}");
        DEBUG_PRINT_IF(type != dTypeBool, "While type is not bool");
        DEBUG_PRINT("While type valid");

        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    err = analyzeBody(&while_statement->body, table, currentFunc, retCount);

    DEBUG_PRINT("While body analyzed");
    DEBUG_PRINT("While error: %d", err);

    if (err != SUCCESS) return err;

    // exit the while scope
    return symTableExitScope(table);
}

// Function to analyze an if statement
enum ERR_CODES analyzeIfStatement(IfStatement *if_statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    if (!if_statement) return E_INTERNAL;

    DEBUG_PRINT("Analyzing if statement");

    enum ERR_CODES err = SUCCESS;

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&if_statement->condition, table, &type, &nullable);

    DEBUG_PRINT("If type: %d\nIf nullable: %d\nError: %d", type, nullable, err);

    if (err != SUCCESS) return err;

    // move to if scope
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL;

    // we should have this type if if -> if (exp) |a| { body }
    if (if_statement->non_nullable.name) {
        DEBUG_PRINT("Analyzing if (a) |na| {...}");
        // need to somehow find the variable in the while scope?
        char * varName = if_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        if_statement->condition.data.identifier.var = var;

        DEBUG_PRINT_IF(!var->nullable, "Variable %s is not nullable", varName);
        DEBUG_PRINT_IF(var->type == dTypeNone, "Variable %s has no type", varName);
        DEBUG_PRINT("If var valid");

        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            if_statement->non_nullable.name,
            var->type,
            false, // this war will not exist after the while loop, so it should be const
            false,
            var->valueKnonwAtCompileTime
        );

        DEBUG_PRINT_IF(!nonNullVar, "Variable %s redifined", if_statement->non_nullable.name);
        DEBUG_PRINT("If non nullable var valid");

        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;

        if_statement->non_nullable.var = nonNullVar;

    } else {
        // we should have this type if if -> if (exp) { body }
        DEBUG_PRINT("Analyzing if (truthExp) {...}");
        DEBUG_PRINT_IF(type != dTypeBool, "If type is not bool");
        DEBUG_PRINT("If type valid\n");

        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    // analyze the if body
    err = analyzeBody(&if_statement->if_body, table, currentFunc, retCount);
    DEBUG_PRINT("If body analyzed");
    DEBUG_PRINT("If error: %d", err);

    if (err != SUCCESS) return err;

    // exit the if scope
    err = symTableExitScope(table);
    if (err != SUCCESS) return err;

    // analyze the else body
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL; // move to else
    err = analyzeBody(&if_statement->else_body, table, currentFunc, retCount);
    DEBUG_PRINT("Else body analyzed");
    DEBUG_PRINT("Else error: %d", err);
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
    statement->var = var;

    // variable cannot have an unkown type
    if (var->type == dTypeNone && var->id != 0) return E_SEMANTIC_UNKNOWN_TYPE;

    DEBUG_PRINT_IF(!var->mutable && var->id != 0, "Variable %s is not mutable", statement->id.name);
    DEBUG_PRINT_IF(var->id == 0, "Variable %s is global", statement->id.name);
    DEBUG_PRINT("Var valid");

    // we cannot assign to global var, apart from _ var
    if (!var->mutable && var->id != 0) return E_SEMANTIC_REDIFINITION; // var-id 0 is the global var _

    var->modified = true; // we modifed the variable

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&statement->value, table, &type, &nullable);

    DEBUG_PRINT("Assigment type: %d\nAssigment nullable: %d\nError: %d", type, nullable, err);
    if (err != SUCCESS) return err;

    if (statement->value.expr_type == LiteralExpressionType && type == dTypeU8) return E_SEMANTIC_INCOMPATABLE_TYPES;

    DEBUG_PRINT_IF(var->id == 0, "Variable %s is global", statement->id.name);
    DEBUG_PRINT_IF(var->nullable, "Variable %s is nullable", statement->id.name);
    DEBUG_PRINT_IF(nullable, "Value is nullable");
    DEBUG_PRINT_IF(!nullable, "Value is not nullable");
    DEBUG_PRINT("Variable type: %d\nValue type: %d", var->type, type);
    DEBUG_PRINT_IF(var->type == dTypeNone, "Variable %s has no type", statement->id.name);

    // after we know, the expresion is valid, we can check if the types are the same
    if (var->id == 0) return SUCCESS; // we can assign anything to the global var _ the value should be discarded

    // var a = null;
    if (
        var->nullable &&
        statement->value.expr_type == LiteralExpressionType && 
        nullable && type == dTypeNone
    ) return SUCCESS;

    // null compatability
    if (!var->nullable && nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    DEBUG_PRINT_IF(var->type != type, "Variable %s is not the same type", statement->id.name);
    DEBUG_PRINT("Var type valid");

    // check if the types are the same
    if (var->type != type) {
        // here, in case the value is 3.0, can be converted to 3, also for 3, we can convert it to 3.0
        // comment this out???? start
        if (var->type == dTypeI32 && type == dTypeF64) {
            if (var->valueKnonwAtCompileTime) {
                statement->value.conversion = FloatToInt;
                return SUCCESS;
            }
            if (statement->value.expr_type == LiteralExpressionType && type == dTypeF64 && var->type == dTypeI32 && f64valueCanBeCovertedToi32(statement->value.data.literal.value)) {
                statement->value.conversion = FloatToInt;
                return SUCCESS;
            }
        }
        // we can also assign NULL, in csae it is nullable
        if (var->nullable && type == dTypeNone && nullable && statement->value.expr_type == LiteralExpressionType && !statement->value.data.literal.value) {
            DEBUG_PRINT("assigning NULL to nullable var");
            return SUCCESS;
        }

        // comment this out???? end
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }
    return SUCCESS;
}

// Function to analyze a variable definition statement
enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement, SymTable *table) {

    if (!statement) return E_INTERNAL;

    DEBUG_PRINT("Analyzing variable definition statement");
    enum ERR_CODES err;

    DEBUG_PRINT("declaring variable %s\nVariable type: %d\nVariable nullable: %d\nVariable const: %d", statement->id.name, statement->type.data_type, statement->type.is_nullable, statement->isConst);

    bool nullable;
    enum DATA_TYPES type;
    err = analyzeExpression(&statement->value, table, &type, &nullable);

    if (statement->value.expr_type == LiteralExpressionType && type == dTypeU8) return E_SEMANTIC_UNKNOWN_TYPE;

    DEBUG_PRINT("analyzing right side of the definition\nexp type: %d\nexp nullable: %d\nerr: %d", type, nullable, err);
    if (err != SUCCESS) return err;

    // in case we get void, we need to return an error
    if (type == dTypeVoid) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // if the value is a constant f64 literal, we can convert it to i32, so save the value
    bool canBeConvertedToI32 = false;
    if (statement->isConst && statement->value.expr_type == LiteralExpressionType && type != dTypeNone) {
        canBeConvertedToI32 = f64valueCanBeCovertedToi32(statement->value.data.literal.value);
    }

    // declare the var
    SymVariable *var = symTableDeclareVariable(
        table, 
        statement->id.name, 
        statement->type.data_type,
        !statement->isConst,
        statement->type.is_nullable, 
        canBeConvertedToI32
    );

    DEBUG_PRINT_IF(!var, "Variable %s redifined", statement->id.name);
    DEBUG_PRINT("Var declare valid");

    if (!var) return E_SEMANTIC_REDIFINITION;
    statement->id.var = var;

    // if the type is defined, and they dont match, error out
    if (var->type == dTypeNone) {
        if (type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE; // we cannot have a variable, with no type
        var->type = type;
    }

    // nullability can be overwritten on declaration
    if (!var->nullable && nullable) var->nullable = true;

    DEBUG_PRINT("Variable type: %d\nVariable nullable: %d", var->type, var->nullable);
    DEBUG_PRINT_IF(var->type != type, "Variable %s is not the same type", statement->id.name);
    DEBUG_PRINT("Var type valid");

    // check if the types are the same
    if (var->type != type) {
        // here, in case the value is 3.0, can be converted to 3, also for 3, we can convert it to 3.0
        // comment this out???? start
        if (var->type == dTypeI32 && type == dTypeF64) {
            if (canBeConvertedToI32) {
                statement->value.conversion = FloatToInt;
                return SUCCESS;
            }
            if (statement->value.expr_type == LiteralExpressionType && type == dTypeF64 && var->type == dTypeI32 && f64valueCanBeCovertedToi32(statement->value.data.literal.value)) {
                statement->value.conversion = IntToFloat;
                return SUCCESS;
            }
        }
        // we can also assign NULL, in csae it is nullable
        if (var->nullable && type == dTypeNone && nullable && statement->value.expr_type == LiteralExpressionType && !statement->value.data.literal.value) {
            DEBUG_PRINT("assigning NULL to nullable var");
            return SUCCESS;
        }
        // comment this out???? end
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

    DEBUG_PRINT("Analyzing eppression of type: %d", expr->expr_type);

    switch (expr->expr_type) {
        // function call
        case FunctionCallExpressionType:
            DEBUG_PRINT("Analyzing function call");
            err = analyzeFunctionCall(&expr->data.function_call, table);
            // try to find the defintion and the return type and set then
            fncDef = symTableFindFunction(table, expr->data.function_call.func_id.name);
            DEBUG_PRINT_IF(!fncDef, "Function %s not found", expr->data.function_call.func_id.name);
            if (!fncDef) return E_SEMANTIC_UND_FUNC_OR_VAR;
            *returnType = fncDef->returnType;
            *resultNullable = fncDef->nullableReturn;
            expr->conversion = NoConversion;
            break;

        // literal
        case LiteralExpressionType:
            DEBUG_PRINT("Analyzing literal");
             // should be handelted in the syntaxical analysis
            if (!expr->data.literal.value) {
                DEBUG_PRINT("Literal is NULL");
            }
            *returnType = expr->data.literal.data_type.data_type;
            *resultNullable = expr->data.literal.data_type.is_nullable; // since null literal is nullable i guess
            expr->conversion = NoConversion;
            break;

        // binary expression
        case BinaryExpressionType:
            DEBUG_PRINT("Analyzing binary expression");
            err = analyzeBinaryExpression(&expr->data.binary_expr, table, returnType, resultNullable);
            break;

        // identifier
        case IdentifierExpressionType:
            DEBUG_PRINT("Analyzing identifier");
            var = symTableFindVariable(table, expr->data.identifier.name);
            // check for NULL i guess, since null is and identifier
            if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
            expr->data.identifier.var = var;

            *returnType = var->type;
            *resultNullable = var->nullable;
            expr->conversion = NoConversion;
            break;

        // bad ast construction
        default:
            err = E_INTERNAL;
            break;
    }

    expr->data_type.data_type = *returnType;
    expr->data_type.is_nullable = *resultNullable;

    return err;
}

// Function to determin, if the f64 value ends in .000000 ...
bool f64valueCanBeCovertedToi32(char *value) {
    char *dotPosition = strchr(value, '.');
    if (dotPosition == NULL) return false; // a float, but no dot

    char *p = dotPosition + 1;
    while (*p == '0') p++;

    if (*p != '\0') return false;

    return true;

}

// Function to analyze a binary expression
enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable) {
    DEBUG_PRINT("Analyzing binary expression");

    // base case
    if (!binary_expr) {
        *resultNullable = false;
        *returnType = dTypeNone;
        DEBUG_PRINT("Binary expression is NULL");
        return SUCCESS;
    };

    enum ERR_CODES err;

    enum DATA_TYPES leftType, rightType;
    bool leftNullable, rightNullable;

    Expression *left = binary_expr->left;
    Expression *right = binary_expr->right;

    // get the ret type of the left epx
    DEBUG_PRINT("Analyzing left expression");
    err = analyzeExpression(left, table, &leftType, &leftNullable);
    DEBUG_PRINT_IF(err != SUCCESS, "Error in left expression");
    if (err != SUCCESS) return err;

    // get the ret type of the right exp
    DEBUG_PRINT("Analyzing right expression");
    err = analyzeExpression(right, table, &rightType, &rightNullable);
    DEBUG_PRINT_IF(err != SUCCESS, "Error in right expression");
    if (err != SUCCESS) return err;

    DEBUG_PRINT("\nleft type: %d\nright type: %d", leftType, rightType);
    DEBUG_PRINT("\nleft nullable: %d\nright nullable: %d", leftNullable, rightNullable);
    DEBUG_PRINT("\noperation: %d", binary_expr->operation);

    // first we filter out bad examples, and some special cases, so after this switch, we only have i32 and f64, that are not nullable
    switch(binary_expr->operation) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
            /*
            here we check the following:
            A is nulable, or B is nullable
            A is NULL or B is NULL 
            A is not i32 and A is not f64 and B is not i32 and B is not f64
            */
            // check nullability of both sides
            if (leftNullable || rightNullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
            // check if A is NULL or B is NULL
            if (
                (left->expr_type == LiteralExpressionType && !left->data.literal.value && left->data_type.data_type == dTypeNone && left->data_type.is_nullable) ||
                (right->expr_type == LiteralExpressionType && !right->data.literal.value && right->data_type.data_type == dTypeNone && right->data_type.is_nullable)
            ) return E_SEMANTIC_INCOMPATABLE_TYPES; // this should not happen, syntax should catch this, but just to be surre
            break; 
           
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:
            // here, just check, if A is not nullable, and B is NULL
            // or A is NULL and B is not nullable
            if (!leftNullable && right->expr_type == LiteralExpressionType && !right->data.literal.value) return E_SEMANTIC_INCOMPATABLE_TYPES;
            if (!rightNullable && left->expr_type == LiteralExpressionType && !left->data.literal.value) return E_SEMANTIC_INCOMPATABLE_TYPES;
            // allowed A is NULL and B is NULL
            if (
                left->expr_type == LiteralExpressionType && !left->data.literal.value && left->data_type.data_type == dTypeNone && left->data_type.is_nullable &&
                right->expr_type == LiteralExpressionType && !right->data.literal.value && right->data_type.data_type == dTypeNone && right->data_type.is_nullable
            ) {
                *resultNullable = false;
                *returnType = dTypeBool;
                left->conversion = NoConversion;
                right->conversion = NoConversion;
                return SUCCESS;
            }
            // A is U8 and B is NULL
            if (
                (leftType == dTypeU8 && right->expr_type == LiteralExpressionType && !right->data.literal.value && right->data.literal.data_type.data_type == dTypeNone && right->data.literal.data_type.is_nullable) ||
                (rightType == dTypeU8 && left->expr_type == LiteralExpressionType && !left->data.literal.value && left->data.literal.data_type.data_type == dTypeNone && left->data.literal.data_type.is_nullable)
            ) {
                *resultNullable = false;
                *returnType = dTypeBool;
                left->conversion = NoConversion;
                right->conversion = NoConversion;
                return SUCCESS;
            }
            break;

        default:
            return E_INTERNAL;
    }

    DEBUG_PRINT("checking, if types are i32 or f64");

    // here we are just workign with strings
    if (leftType != dTypeI32 && leftType != dTypeF64) return E_SEMANTIC_INCOMPATABLE_TYPES;
    if (rightType != dTypeI32 && rightType != dTypeF64) return E_SEMANTIC_INCOMPATABLE_TYPES;

    DEBUG_PRINT("types are i32 or f64");

    // if both types are the same, we dont have a problem
    if (leftType == rightType) {
        switch(binary_expr->operation) {
            case TOKEN_PLUS:
            case TOKEN_MINUS:
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
                *returnType = leftType;
                break;
            case TOKEN_LESSTHAN:
            case TOKEN_LESSOREQUAL:
            case TOKEN_GREATERTHAN:
            case TOKEN_GREATEROREQUAL:
            case TOKEN_EQUALS:
            case TOKEN_NOTEQUAL:
                *returnType = dTypeBool;
                break;
            default:
                return E_INTERNAL;
        }
        *resultNullable = false;
        left->conversion = NoConversion;
        right->conversion = NoConversion;
        return SUCCESS;
    }

    DEBUG_PRINT("trying to convert types");

    // differemtn rules for + - * < > <= >= == != and / for conversions
    bool convPossible = false;
    enum DATA_TYPES operationRetType = dTypeUndefined;
    switch(binary_expr->operation) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:

            //A is i32 literal && B is binary Expression f64
            //    -> convert A to f64
            if (leftType == dTypeI32 && left->expr_type == LiteralExpressionType && rightType == dTypeF64 && right->expr_type == BinaryExpressionType) {
                DEBUG_PRINT("A is i32 literal && B is binary Expression f64, convert A to f64");
                left->conversion = IntToFloat;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is binary Expression f64 && B is i32 literal
            //    -> convert B to f64
            if (leftType == dTypeF64 && left->expr_type == BinaryExpressionType && rightType == dTypeI32 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is binary Expression f64 && B is i32 literal, convert B to f64");
                left->conversion = NoConversion;
                right->conversion = IntToFloat;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is f64 literal && B is binary Expression i32
            //   -> convert A to i32, if decimal place of A is 0s
            if (leftType == dTypeF64 && left->expr_type == LiteralExpressionType && rightType == dTypeI32 && right->expr_type == BinaryExpressionType) {
                DEBUG_PRINT("A is f64 literal && B is binary Expression i32, convert A to i32");
                if (f64valueCanBeCovertedToi32(left->data.literal.value)) {
                    left->conversion = FloatToInt;
                    right->conversion = NoConversion;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            //A is binary Expression i32 && B is f64 literal
            //    -> convert B to i32, if decimal place of B is 0s
            if (leftType == dTypeI32 && left->expr_type == BinaryExpressionType && rightType == dTypeF64 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is binary Expression i32 && B is f64 literal, convert B to i32");
                if (f64valueCanBeCovertedToi32(right->data.literal.value)) {
                    left->conversion = NoConversion;
                    right->conversion = FloatToInt;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            //A is constant f64 variable && B is binary Expression i32
            //    -> convert A to i32, if decimal place of A is 0s
            if (leftType == dTypeF64 && left->expr_type == IdentifierExpressionType && left->data.identifier.var->valueKnonwAtCompileTime && rightType == dTypeI32 && right->expr_type == BinaryExpressionType) {
                DEBUG_PRINT("A is constant f64 variable && B is binary Expression i32, convert A to i32");
                left->conversion = FloatToInt;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            //A is binary Expression i32 && B is constant f64 variable
            //    -> convert B to i32, if decimal place of B is 0s
            if (leftType == dTypeI32 && left->expr_type == BinaryExpressionType && rightType == dTypeF64 && right->expr_type == IdentifierExpressionType && right->data.identifier.var->valueKnonwAtCompileTime) {
                DEBUG_PRINT("A is binary Expression i32 && B is constant f64 variable, convert B to i32");
                left->conversion = NoConversion;
                right->conversion = FloatToInt;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            //A is i32 ligeral && B is f64 literal
            //    -> convert A to f64
            if (leftType == dTypeI32 && left->expr_type == LiteralExpressionType && rightType == dTypeF64 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is i32 ligeral && B is f64 literal, convert A to f64");
                left->conversion = IntToFloat;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is f64 literal && B is i32 literal
            //    -> convert B to f64
            if (leftType == dTypeF64 && left->expr_type == LiteralExpressionType && rightType == dTypeI32 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is f64 literal && B is i32 literal, convert B to f64");
                left->conversion = NoConversion;
                right->conversion = IntToFloat;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is variable i32 && B is f64 literal
            //    -> (if deciaml place of B is 0s, convert B f64 -> SUCESS, else ERROR)
            if (leftType == dTypeI32 && left->expr_type == IdentifierExpressionType && rightType == dTypeF64 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is variable i32 && B is f64 literal, convert B f64 -> SUCESS, else ERROR");
                if (f64valueCanBeCovertedToi32(right->data.literal.value)) {
                    right->conversion = FloatToInt;
                    left->conversion = NoConversion;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            //A is f64 literal && B is i32 variable
            //    -> (if decimal place of A is 0s, convert A f64 -> SUCCESS, else ERROR)
            if (leftType == dTypeF64 && left->expr_type == LiteralExpressionType && rightType == dTypeI32 && right->expr_type == IdentifierExpressionType) {
                DEBUG_PRINT("A is f64 literal && B is i32 variable, convert A f64 -> SUCCESS, else ERROR");
                if (f64valueCanBeCovertedToi32(left->data.literal.value)) {
                    left->conversion = FloatToInt;
                    right->conversion = NoConversion;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            //A is variable f64 && B is i32 literal
            //    -> (Convert B to f64 -> SUCCESS)
            if (leftType == dTypeF64 && left->expr_type == IdentifierExpressionType && rightType == dTypeI32 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is a constant variable f64 && B is i32 literal, convert B to f64");
                right->conversion = IntToFloat;
                left->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is i32 literal && B is variable f64
            //    -> (Convert A to f64 -> SUCCESS)
            if (leftType == dTypeI32 && left->expr_type == LiteralExpressionType && rightType == dTypeF64 && right->expr_type == IdentifierExpressionType) {
                DEBUG_PRINT("A is i32 literal && B is a constant variable f64, convert A to f64");
                left->conversion = IntToFloat;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is f64 constant expresion (value known at compile time) && B is i32 variable
            //    -> (if decimal place of A is 0s, convert A i32, SUCCESS, else -> ERROR)
            if (leftType == dTypeF64 && left->expr_type == IdentifierExpressionType && left->data.identifier.var->valueKnonwAtCompileTime && rightType == dTypeI32 && right->expr_type == IdentifierExpressionType) {
                DEBUG_PRINT("A is f64 constant expresion (value known at compile time) && B is i32 variable, convert A i32, SUCCESS, else -> ERROR");
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                left->conversion = FloatToInt;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            //A is i32 variable && B is f64 constant expresion (value known at compile time)
            //    -> (if decimal place of B is 0s, convert B i32, SUCCESS, else -> ERROR)
            if (leftType == dTypeI32 && left->expr_type == IdentifierExpressionType && rightType == dTypeF64 && right->expr_type == IdentifierExpressionType && right->data.identifier.var->valueKnonwAtCompileTime) {
                DEBUG_PRINT("A is i32 variable && B is f64 constant expresion (value known at compile time), convert B i32, SUCCESS, else -> ERROR");
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                right->conversion = FloatToInt;
                left->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            //A is f64 constant expresion (value known at compile time) && B is i32 literal
            //    -> (Convert B to f64 -> SUCCESS)
            if (leftType == dTypeF64 && left->expr_type == IdentifierExpressionType && left->data.identifier.var->valueKnonwAtCompileTime && rightType == dTypeI32 && right->expr_type == LiteralExpressionType) {
                DEBUG_PRINT("A is f64 constant expresion (value known at compile time) && B is i32 literal, convert B to f64");
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                right->conversion = IntToFloat;
                left->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            //A is i32 literal && B is f64 constant expresion (value known at compile time)
            //    -> (Convert A to f64 -> SUCCESS)
            if (leftType == dTypeI32 && left->expr_type == LiteralExpressionType && rightType == dTypeF64 && right->expr_type == IdentifierExpressionType && right->data.identifier.var->valueKnonwAtCompileTime) {
                DEBUG_PRINT("A is i32 literal && B is f64 constant expresion (value known at compile time), convert A to f64");
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                left->conversion = IntToFloat;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeF64;
                break;
            }

            break;

        case TOKEN_DIVIDE:

            // division by 0
            if (right->expr_type == LiteralExpressionType && (strcmp(right->data.literal.value, "0") == 0 || strcmp(right->data.literal.value, "0.0") == 0)) return E_SEMANTIC_OTHER;

            //A is constant f64 expression (value known at compile time) && B is i32 (any type: literal, variable, bianry expression ...)
            //    -> (if decimal place of A is only 0s, we convert A to i32 -> SUCESS else ERROR)
            if (leftType == dTypeF64 && left->expr_type == IdentifierExpressionType && left->data.identifier.var->valueKnonwAtCompileTime && rightType == dTypeI32) {
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                left->conversion = FloatToInt;
                right->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            //A is i32 (any type: literal, variable, bianry expression ...) && B is a constant f64 expression (value known at compile time)
            //    -> (if decimal place of B is only 0s, we convert B to i32 -> SUCCESS else ERROR)
            if (leftType == dTypeI32 && rightType == dTypeF64 && right->expr_type == IdentifierExpressionType && right->data.identifier.var->valueKnonwAtCompileTime) {
                // if the var->valueKnonwAtCompileTime is true, it means, that the value can be covnerted to i32
                right->conversion = FloatToInt;
                left->conversion = NoConversion;
                convPossible = true;
                operationRetType = dTypeI32;
                break;
            }

            // A is f64 literal && B is i32 literal
            //    -> (Convert A to i32 -> SUCCESS)
            if (leftType == dTypeF64 && left->expr_type == LiteralExpressionType && rightType == dTypeI32 && right->expr_type == LiteralExpressionType) {
                if (f64valueCanBeCovertedToi32(left->data.literal.value)) {
                    left->conversion = FloatToInt;
                    right->conversion = NoConversion;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            // A is i32 literal && B is f64 literal
            //    -> (Convert B to i32 -> SUCCESS)
            if (leftType == dTypeI32 && left->expr_type == LiteralExpressionType && rightType == dTypeF64 && right->expr_type == LiteralExpressionType) {
                if (f64valueCanBeCovertedToi32(right->data.literal.value)) {
                    right->conversion = FloatToInt;
                    left->conversion = NoConversion;
                    convPossible = true;
                    operationRetType = dTypeI32;
                    break;
                }
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            break;  

        default:
            return E_INTERNAL;
    }

    DEBUG_PRINT("conversion possible: %d", convPossible);

    if (!convPossible) {
        *returnType = dTypeUndefined;
        *resultNullable = false;
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // now we need to set the return type, in case it is + - * /, 
    switch(binary_expr->operation) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
            *returnType = operationRetType;
            *resultNullable = false;
            return SUCCESS;
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:
            *returnType = dTypeBool;
            *resultNullable = false;
            return SUCCESS;
        default:
            return E_INTERNAL;
    }
    
    return E_INTERNAL;
}
