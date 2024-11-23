/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 20.10.2024
 * @brief Implementation of abstract syntax tree functions - resource releasing
 */

#include "syntaxical/ast.h"
#include <stdio.h>

void freeProgram(Program *program) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing program");
    #endif
    if (!program) return;
    int size = getSize(program->functions);
    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        freeFunction(function);
    }

    removeList(&program->functions);
}

void freeFunction(Function *function) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing function");
    #endif
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
    #ifdef DEBUG
    DEBUG_MSG("Freeing body");
    #endif
    if (!body) return;
    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        freeStatement(statement);
    }

    removeList(&body->statements);
}

void freeParam(Param *param) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing param");
    #endif
    if (!param) return;
    freeIdentifier(&param->id);
    freeDataType(&param->type);
}

void freeVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing variable definition statement");
    #endif
    if (!statement) return;
    freeIdentifier(&statement->id);
    freeDataType(&statement->type);
    freeExpression(&statement->value);
}

void freeAssigmentStatement(AssigmentStatement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing assigment statement");
    #endif
    if (!statement) return;
    freeIdentifier(&statement->id);
    freeExpression(&statement->value);
}

void freeIfStatement(IfStatement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing if statement");
    #endif
    if (!statement) return;
    freeBody(&statement->if_body);
    freeBody(&statement->else_body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeWhileStatement(WhileStatement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing while statement");
    #endif
    if (!statement) return;
    freeBody(&statement->body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeReturnStatement(ReturnStatement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing return statement");
    #endif
    if (!statement) return;
    freeExpression(&statement->value);
}

void freeStatement(Statement *statement) {
    #ifdef DEBUG
    DEBUG_MSG("Freeing statement");
    #endif
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
