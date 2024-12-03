/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the Abstract Syntax Tree
 */

#ifndef EXPRESSION_AST_H
#define EXPRESSION_AST_H

#ifdef USE_CUSTOM_STRUCTURE
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#else
#include "enumerations.h"
#include "linked_list.h"
#include "symtable.h"
#endif

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

enum ExpressionConversion {
    NoConversion,
    IntToFloat,
    FloatToInt,
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
    enum ExpressionConversion conversion;
    union {
        struct FunctionCall function_call;
        struct Identifier identifier;
        struct Literal literal;
        struct BinaryExpression binary_expr;
    } data;
} Expression;

/**
 * Function to free a data type structure
 *
 * @param data_type - the data type structure to free
 */
void freeDataType(DataType *data_type);

/**
 * Function to free an identifier structure
 *
 * @param identifier - the identifier structure to free
 */
void freeIdentifier(Identifier *identifier);

/**
 * Function to free a function call structure
 *
 * @param func_call - the function call structure to free
 */
void freeFunctionCall(FunctionCall *func_call);

/**
 * Function to free an expression structure
 *
 * @param expr - the expression structure to free
 */
void freeExpression(Expression *expr);

/**
 * Function to free a literal structure
 *
 * @param literal - the literal structure to free
 */
void freeLiteral(Literal *literal);


#endif // EXPRESSION_AST_H
