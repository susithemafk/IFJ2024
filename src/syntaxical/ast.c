/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @addindex <247581> Martin Mendl
 * @file ast.c
 * @date 20.10.2024
 * @brief Implementation of abstract syntax tree functions - resource releasing
 */

#include <stdio.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/ast.h"
#else 
#include "ast.h"
#endif

void freeProgram(Program *program) {
    DEBUG_PRINT("Freeing program");
    if (!program) return;
    int size = getSize(program->functions);
    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        freeFunction(function);
    }

    removeList(&program->functions);
}

void freeFunction(Function *function) {
    DEBUG_PRINT("Freeing function");
    if (!function) return;
    freeIdentifier(&function->id);
    freeBody(&function->body);
    freeDataType(&function->returnType);

    int size = getSize(function->params);
    for (int i = 0; i < size; i++) {
        Param *param = (Param *)getDataAtIndex(function->params, i);
        freeParam(param);
    }

    removeList(&function->params);
}

void freeBody(Body *body) {
    DEBUG_PRINT("Freeing body");
    if (!body) return;
    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        freeStatement(statement);
    }

    removeList(&body->statements);
}

void freeParam(Param *param) {
    DEBUG_PRINT("Freeing param");
    if (!param) return;
    freeIdentifier(&param->id);
    freeDataType(&param->type);
}

void freeVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    DEBUG_PRINT("Freeing variable definition statement");
    if (!statement) return;
    freeIdentifier(&statement->id);
    freeDataType(&statement->type);
    freeExpression(&statement->value);
}

void freeAssigmentStatement(AssigmentStatement *statement) {
    DEBUG_PRINT("Freeing assigment statement");
    if (!statement) return;
    freeIdentifier(&statement->id);
    freeExpression(&statement->value);
}

void freeIfStatement(IfStatement *statement) {
    DEBUG_PRINT("Freeing if statement");
    if (!statement) return;
    freeBody(&statement->if_body);
    freeBody(&statement->else_body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeWhileStatement(WhileStatement *statement) {
    DEBUG_PRINT("Freeing while statement");
    if (!statement) return;
    freeBody(&statement->body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeReturnStatement(ReturnStatement *statement) {
    DEBUG_PRINT("Freeing return statement");
    if (!statement) return;
    freeExpression(&statement->value);
}

void freeStatement(Statement *statement) {
    DEBUG_PRINT("Freeing statement");
    if (!statement) return;
    switch (statement->type) {
    case FunctionCallStatementType:
        freeFunctionCall(&statement->data.function_call_statement);
        break;
    case VariableDefinitionStatementType:
        freeVariableDefinitionStatement(&statement->data.variable_definition_statement);
        break;
    case AssigmentStatementType:
        freeAssigmentStatement(&statement->data.assigment_statement);
        break;
    case IfStatementType:
        freeIfStatement(&statement->data.if_statement);
        break;
    case WhileStatementType:
        freeWhileStatement(&statement->data.while_statement);
        break;
    case ReturnStatementType:
        freeReturnStatement(&statement->data.return_statement);
        break;
    default:
        break;
    }
}
