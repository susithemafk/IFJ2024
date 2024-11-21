#include "semantical/sem_analyzer.h"
#include "syntaxical/ast.h"
#include "utility/enumerations.h"

enum ERR_CODES analyzeProgram(Program *program) {
    if (!program) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    int size = getSize(program->functions);
    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);

        // TODO
    }

    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        err = analyzeFunction(function);
        if (err != SUCCESS) {
            return err;
        }
    }

    return SUCCESS;
}

enum ERR_CODES analyzeFunction(Function *function) {
    if (!function) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    // TODO

    int size = getSize(function->params);
    for (int i = 0; i < size; i++) {
        Param *param = (Param *)getDataAtIndex(function->params, i);
        err = analyzeParam(param);
        if (err != SUCCESS) {
            return err;
        }
    }

    // TODO

    err = analyzeBody(&function->body);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeBody(Body *body) {
    if (!body) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        err = analyzeStatement(statement);
        if (err != SUCCESS) {
            return err;
        }
    }

    return SUCCESS;
}

enum ERR_CODES analyzeParam(Param *param) {
    if (!param) {
        return E_INTERNAL;
    }

    //  TODO

    return SUCCESS;
}

enum ERR_CODES analyzeStatement(Statement *statement) {
    if (!statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    switch (statement->type) {
    case FunctionCallStatementType:
        err = analyzeFunctionCall(&statement->data.function_call_statement);
        break;
    case ReturnStatementType:
        err = analyzeReturnStatement(&statement->data.return_statement);
        break;
    case WhileStatementType:
        err = analyzeWhileStatement(&statement->data.while_statement);
        break;
    case IfStatementType:
        err = analyzeIfStatement(&statement->data.if_statement);
        break;
    case AssigmentStatementType:
        err = analyzeAssigmentStatement(&statement->data.assigment_statement);
        break;
    case VariableDefinitionStatementType:
        err = analyzeVariableDefinitionStatement(&statement->data.variable_definition_statement);
        break;
    default:
        err = E_INTERNAL;
        break;
    }

    return err;
}

enum ERR_CODES analyzeFunctionCall(FunctionCall *function_call) {
    if (!function_call) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    // TODO

    int size = getSize(function_call->arguments);
    for (int i = 0; i < size; i++) {
        Expression *param = (Expression *)getDataAtIndex(function_call->arguments, i);
        err = analyzeExpression(param);
        if (err != SUCCESS) {
            return err;
        }
    }

    // TODO

    return SUCCESS;
}

enum ERR_CODES analyzeReturnStatement(ReturnStatement *return_statement) {
    if (!return_statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    err = analyzeExpression(&return_statement->value);
    if (err != SUCCESS) {
        return err;
    }

    // TODO

    return SUCCESS;
}

enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement) {
    if (!while_statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    err = analyzeExpression(&while_statement->condition);
    if (err != SUCCESS) {
        return err;
    }

    // TODO

    err = analyzeBody(&while_statement->body);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeIfStatement(IfStatement *if_statement) {
    if (!if_statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    err = analyzeExpression(&if_statement->condition);
    if (err != SUCCESS) {
        return err;
    }

    // TODO

    err = analyzeBody(&if_statement->if_body);
    if (err != SUCCESS) {
        return err;
    }

    err = analyzeBody(&if_statement->else_body);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement) {
    if (!statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    // TODO

    err = analyzeExpression(&statement->value);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    if (!statement) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    // TODO

    err = analyzeExpression(&statement->value);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

enum ERR_CODES analyzeExpression(Expression *expr) {
    if (!expr) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    switch (expr->expr_type) {
    case FunctionCallExpressionType:
        err = analyzeFunctionCall(&expr->data.function_call);
        break;
    case LiteralExpressionType:
        err = analyzeLiteral(&expr->data.literal);
        break;
    case BinaryExpressionType:
        err = analyzeBinaryExpression(&expr->data.binary_expr);
        break;
    case IdentifierExpressionType:
        err = analyzeIdentifier(&expr->data.identifier);
        break;
    default:
        err = E_INTERNAL;
        break;
    }

    return err;
}

enum ERR_CODES analyzeIdentifier(Identifier *identifier) {
    if (!identifier) {
        return E_INTERNAL;
    }

    // TODO

    return SUCCESS;
}

enum ERR_CODES analyzeLiteral(Literal *literal) {
    if (!literal) {
        return E_INTERNAL;
    }

    // TODO

    return SUCCESS;
}

enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr) {
    if (!binary_expr) {
        return E_INTERNAL;
    }

    enum ERR_CODES err = SUCCESS;

    // TODO

    err = analyzeExpression(binary_expr->left);
    if (err != SUCCESS) {
        return err;
    }

    err = analyzeExpression(binary_expr->right);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}
