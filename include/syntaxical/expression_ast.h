/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the Abstract Syntax Tree
 */

#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#include "utility/enumerations.h"
#include "utility/linked_list.h"
#include "semantical/symtable.h"

typedef struct DataType {
    enum DATA_TYPES data_type;
    bool is_nullable;
} DataType;

enum ExpressionType {
    IdentifierExpressionType,
    FunctionCallExpressionType,
    LiteralExpressionType,
    BinaryExpressionType,
};
typedef struct Identifier {
    char *name;
    DataType data_type;

    SymVariable *var;
} Identifier;

typedef struct FunctionCall {
    struct Identifier func_id;
    LinkedList *arguments; // list of expressions (Expression *)

    SymFunctionPtr func;
    DataType return_type;
} FunctionCall;

typedef struct Literal {
    char *value;
    DataType data_type;
} Literal;

typedef struct BinaryExpression {
    struct Expression *left;
    struct Expression *right;
    enum TOKEN_TYPE operation;
} BinaryExpression;

typedef struct Expression {
    struct DataType data_type;
    enum ExpressionType expr_type;
    union {
        struct FunctionCall function_call;
        struct Identifier identifier;
        struct Literal literal;
        struct BinaryExpression binary_expr;
    } data;
} Expression;

void freeDataType(DataType *data_type);
void freeIdentifier(Identifier *identifier);
void freeFunctionCall(FunctionCall *func_call);
void freeExpression(Expression *expr);
void freeLiteral(Literal *literal);

#endif // EXPRESSION_AST_H
