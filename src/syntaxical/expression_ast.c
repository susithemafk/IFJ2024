/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @author <247581> Martin Mendl
 * @file expression_ast.c
 * @date 20.10.2024
 * @brief Implementation of abstract syntax tree functions - resource releasing
 */

#include <stdlib.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/expression_ast.h"
#else
#include "expression_ast.h"
#endif

// Function to free the data type
void freeDataType(DataType *data_type) {
    DEBUG_PRINT("Freeing data type");
    (void)data_type; // not needed?
}

// Function to free the identifier
void freeIdentifier(Identifier *identifier) {
    DEBUG_PRINT("Freeing identifier");
    if (!identifier) return;
    // free(identifier->name); // not needed, stuff freed in the buffer
}

// Function to free a func call
void freeFunctionCall(FunctionCall *func_call) {
    DEBUG_PRINT("Freeing function call");
    if (!func_call) return;
    freeIdentifier(&func_call->func_id);
    int size = getSize(func_call->arguments);
    for (int i = 0; i < size; i++) {
        Expression *expr = (Expression *)getDataAtIndex(func_call->arguments, i);
        freeExpression(expr);
    }

    removeList(&func_call->arguments);
}

// Function to free Expression
void freeExpression(Expression *expr) {
    DEBUG_PRINT("Freeing expression");
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

// Function to free a literal
void freeLiteral(Literal *literal) {
    DEBUG_PRINT("Freeing literal");
    if (!literal) return;
    // free(literal->value); // not needed, stuff freed in the buffer
}
