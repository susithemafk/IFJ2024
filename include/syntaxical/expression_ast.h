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

struct DataType {
    enum DATA_TYPES data_type;
    bool is_nullable;
};

enum ExpressionType {
    IdentifierExpressionType,
    FunctionCallExpressionType,
    LiteralExpressionType,
    BinaryExpressionType,
    UnaryExpressionType,
};

struct Identifier {
    char *name;
};

struct FunctionCall {
    struct Identifier func_id;
    LinkedList *arguments; // list of expressions (Expression *)
};

struct Literal {
    char *value;
};

struct BinaryExpression {
    struct Expression *left;
    struct Expression *right;
    enum TOKEN_TYPE operation;
};

struct Expression {
    struct DataType data_type;
    enum ExpressionType expr_type;
    union {
        struct FunctionCall function_call;
        struct Identifier identifier;
        struct Literal literal;
        struct BinaryExpression binary_expr;
    } data;
};

typedef struct Expression Expression;
typedef struct FunctionCall FunctionCall;
typedef struct Identifier Identifier;
typedef struct DataType DataType;
typedef struct Literal Literal;
typedef struct BinaryExpression BinaryExpression;

void freeDataType(struct DataType *data_type);
void freeIdentifier(struct Identifier *identifier);
void freeFunctionCall(struct FunctionCall *func_call);
void freeExpression(struct Expression *expr);
void freeLiteral(struct Literal *literal);

#endif // EXPRESSION_AST_H
