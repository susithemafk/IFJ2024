#include "syntaxical/ast.h"
#include <stdio.h>

void freeProgram(Program *program) {
    int size = getSize(program->functions);
    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        freeFunction(function);
    }

    removeList(&program->functions);
}

void freeFunction(Function *function) {
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
    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        freeStatement(statement);
    }

    removeList(&body->statements);
}

void freeParam(Param *param) {
    freeIdentifier(&param->id);
    freeDataType(&param->type);
}

void freeVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    freeIdentifier(&statement->id);
    freeDataType(&statement->type);
    freeExpression(&statement->value);
}

void freeAssigmentStatement(AssigmentStatement *statement) {
    freeIdentifier(&statement->id);
    freeExpression(&statement->value);
}

void freeIfStatement(IfStatement *statement) {
    freeBody(&statement->if_body);
    freeBody(&statement->else_body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeWhileStatement(WhileStatement *statement) {
    freeBody(&statement->body);
    freeExpression(&statement->condition);
    freeIdentifier(&statement->non_nullable);
}

void freeReturnStatement(ReturnStatement *statement) {
    freeExpression(&statement->value);
}

void freeStatement(Statement *statement) {
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
