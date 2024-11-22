#include <stdlib.h>
#include <string.h>
#include "semantical/sem_analyzer.h"
#include "syntaxical/ast.h"
#include "utility/enumerations.h"
#include <stdio.h> 

// Function to analyze the whole program
enum ERR_CODES analyzeProgram(Program *program, SymTable *table) {
    if (!program) return E_INTERNAL;

    enum ERR_CODES err;

    // go function by function
    unsigned int size = getSize(program->functions);
    for (unsigned int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        if (!symTableMoveScopeDown(table, SYM_FUNCTION)) return E_INTERNAL;
        unsigned int size1 = getSize(function->params);
        for (unsigned int j = 0; j < size1; j++) {
            // add the params to the function scope
            Param *param = (Param *)getDataAtIndex(function->params, j);
            err = analyzeParam(param, table);
            if (err != SUCCESS) return err;
        }

        SymFunctionPtr funDef = symTableFindFunction(table, function->id.name);
        if (!funDef) return E_INTERNAL;

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

enum ERR_CODES analyzeParam(Param *param, SymTable *table) {
    if (!param || !table) return E_INTERNAL;
    
    SymVariable *var = symTableDeclareVariable(
        table, 
        param->id.name, 
        param->id.data_type.data_type, 
        false, 
        param->id.data_type.is_nullable 
    );
    if (!var) return E_SEMANTIC_REDIFINITION;
    return SUCCESS;
}

enum ERR_CODES analyzeBody(Body *body, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    if (!body) return E_INTERNAL;
    enum ERR_CODES err;

    unsigned int size = getSize(body->statements);
    for (unsigned int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        err = analyzeStatement(statement, table, currentFunc, retCount);
        if (err != SUCCESS) return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeStatement(Statement *statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    if (!statement) return E_INTERNAL;
    enum ERR_CODES err;

    switch (statement->type) {
        case FunctionCallStatementType:
            // check if the function is void 
            err = analyzeFunctionCall(&statement->data.function_call_statement, table);
            if (err != SUCCESS) return err;

            // check if the function is void
            SymFunctionPtr SymFunction = symTableFindFunction(table, statement->data.function_call_statement.func_id.name);
            if (!SymFunction) return E_SEMANTIC_UND_FUNC_OR_VAR;
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

bool nullCompatabilityCheck(bool nullMain, bool nullSecond) {
    if (!nullMain && nullSecond) return false;
    return true;
}

enum ERR_CODES analyzeReturnStatement(ReturnStatement *return_statement, SymTable *table,  SymFunctionPtr currentFunc, int *retCount) {
    if (!return_statement) return E_INTERNAL;

    enum ERR_CODES err;

    enum DATA_TYPES returnType;
    bool nullable;
    err = analyzeExpression(&return_statement->value, table, &returnType, &nullable);
    if (err != SUCCESS) return err;

    // check if the return type is correct
    if (currentFunc->returnType != returnType) return E_SEMANTIC_BAD_FUNC_RETURN;
    if (!currentFunc->nullableReturn && nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    (*retCount)++;

    return SUCCESS;
}

enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement, SymTable *table, SymFunctionPtr currentFunc, int* retCount) {
    if (!while_statement) return E_INTERNAL;

    enum ERR_CODES err;
    enum DATA_TYPES type;
    bool nullable;

    err = analyzeExpression(&while_statement->condition, table, &type, &nullable);
    if (err != SUCCESS) return err;

    if (!symTableMoveScopeDown(table, SYM_WHILE)) return E_INTERNAL; // move to while

    // we should have this type if whiele -> while (exp) |a| { body }
    if (while_statement->non_nullable.name) {
        // need to somehow find the variable in the while scope?
        char * varName = while_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            while_statement->non_nullable.name,
            var->type,
            var->mutable,
            false
        );
        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;
        while_statement->non_nullable_var = nonNullVar;
    } else {
        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    err = analyzeBody(&while_statement->body, table, currentFunc, retCount);
    if (err != SUCCESS) return err;

    // exit the while scope
    return symTableExitScope(table);
}

enum ERR_CODES analyzeIfStatement(IfStatement *if_statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount) {
    if (!if_statement) return E_INTERNAL;

    enum ERR_CODES err = SUCCESS;

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&if_statement->condition, table, &type, &nullable);
    if (err != SUCCESS) return err;

    // move to if scope
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL;

    if (if_statement->non_nullable.name) {
        // need to somehow find the variable in the while scope?
        char * varName = if_statement->condition.data.identifier.name;
        SymVariable *var = symTableFindVariable(table, varName);
        if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
        if (!var->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
        if (var->type == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

        SymVariable *nonNullVar = symTableDeclareVariable(
            table,
            if_statement->non_nullable.name,
            var->type,
            var->mutable,
            false
        );
        if (!nonNullVar) return E_SEMANTIC_REDIFINITION;
        if_statement->non_nullable_var = nonNullVar;
    } else {
        if (type != dTypeBool) return E_SEMANTIC_INCOMPATABLE_TYPES; // this shoud never happen, we would have a syntax err
    }

    // analyze the if body
    err = analyzeBody(&if_statement->if_body, table, currentFunc, retCount);
    if (err != SUCCESS) return err;

    // exit the if scope
    err = symTableExitScope(table);
    if (err != SUCCESS) return err;

    // analyze the else body
    if (!symTableMoveScopeDown(table, SYM_IF)) return E_INTERNAL; // move to else
    err = analyzeBody(&if_statement->else_body, table, currentFunc, retCount);
    if (err != SUCCESS) return err;

    // exit the else scope
    return symTableExitScope(table);
}

enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement, SymTable *table) {
    if (!statement) return E_INTERNAL;

    enum ERR_CODES err;

    SymVariable *var = symTableFindVariable(table, statement->id.name);
    if (!var) return E_SEMANTIC_UND_FUNC_OR_VAR;
    if (!var->mutable && var->id != 0) return E_SEMANTIC_REDIFINITION;

    enum DATA_TYPES type;
    bool nullable;
    err = analyzeExpression(&statement->value, table, &type, &nullable);
    if (err != SUCCESS) return err;

    if (type == dTypeUndefined && var->nullable) return SUCCESS;

    if (var->nullable != nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
    if (var->type == dTypeNone) {
        var->type = type;
        return SUCCESS;
    }
    if (var->type != type) return E_SEMANTIC_INCOMPATABLE_TYPES;
    return SUCCESS;
}

enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement, SymTable *table) {
    if (!statement) return E_INTERNAL;
    enum ERR_CODES err;

    // declare the var
    SymVariable *var = symTableDeclareVariable(
        table, 
        statement->id.name, 
        statement->id.data_type.data_type, 
        !statement->isConst,
        statement->id.data_type.is_nullable
    );
    if (!var) return E_SEMANTIC_REDIFINITION;
    bool nullable;
    err = analyzeExpression(&statement->value, table, &var->type, &nullable);
    if (err != SUCCESS) return err;

    // var a = null;
    if (var->type == dTypeUndefined) return E_SEMANTIC_UNKNOWN_TYPE;

    var->nullable = nullable;
    return SUCCESS;
}

enum ERR_CODES analyzeExpression(Expression *expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable) {
    if (!expr) return E_INTERNAL;

    enum ERR_CODES err = SUCCESS;
    SymVariable *var;
    SymFunctionPtr fncDef;


    switch (expr->expr_type) {
    case FunctionCallExpressionType:
        err = analyzeFunctionCall(&expr->data.function_call, table);
        // try to find the defintion and the return type and set then
        fncDef = symTableFindFunction(table, expr->data.function_call.func_id.name);
        if (!fncDef) return E_SEMANTIC_UND_FUNC_OR_VAR;
        *returnType = fncDef->returnType;
        *resultNullable = fncDef->nullableReturn;
        break;

    case LiteralExpressionType:
        *returnType = expr->data.literal.data_type.data_type;
        *resultNullable = expr->data.literal.data_type.is_nullable; // since null literal is nullable i guess
        break;

    case BinaryExpressionType:
        err = analyzeBinaryExpression(&expr->data.binary_expr, table, returnType, resultNullable);
        break;

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

    *returnType = dTypeF64;
    *resultNullable = false;
    return SUCCESS;
}
