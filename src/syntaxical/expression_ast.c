#include "syntaxical/expression_ast.h"
#include <stdlib.h>

void freeDataType(DataType *data_type) {
    (void)data_type;
}

void freeIdentifier(Identifier *identifier) {
    free(identifier->name);
}

void freeFunctionCall(FunctionCall *func_call) {
    freeIdentifier(&func_call->func_id);
    int size = getSize(func_call->arguments);
    for (int i = 0; i < size; i++) {
        Expression *expr = (Expression *)getDataAtIndex(func_call->arguments, i);
        freeExpression(expr);
    }

    removeList(&func_call->arguments);
}

void freeExpression(Expression *expr) {
    if(!expr) return;

    switch (expr->expr_type) {
        case FunctionCallExpressionType:
            freeFunctionCall(&expr->data.function_call);
            break;
        case IdentifierExpressionType:
            freeIdentifier(&expr->data.identifier);
            break;
        case LiteralExpressionType:
            freeLiteral(&expr->data.literal);
            break;
        case BinaryExpressionType:
            if (expr->data.binary_expr.left)
                freeExpression(expr->data.binary_expr.left);
            if (expr->data.binary_expr.right)
                freeExpression(expr->data.binary_expr.right);
                
            free(expr->data.binary_expr.left);
            free(expr->data.binary_expr.right);
            break;
        default:
            break;
    }
}

void freeLiteral(Literal *literal) {
    free(literal->value);
}