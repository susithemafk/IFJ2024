/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the Abstract Syntax Tree
 */

#ifndef AST_H
#define AST_H

#ifdef USE_CUSTOM_STRUCTURE
#include "semantical/symtable.h"
#include "syntaxical/expression_ast.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#else
#include "symtable.h"
#include "expression_ast.h"
#include "enumerations.h"
#include "linked_list.h"
#endif

typedef struct Program {
    LinkedList *functions;
} Program;

typedef struct Body {
    LinkedList *statements;
} Body;

typedef enum StatementType {
    FunctionCallStatementType,
    ReturnStatementType,
    WhileStatementType,
    IfStatementType,
    AssigmentStatementType,
    VariableDefinitionStatementType,
} StatementType;

typedef struct ReturnStatement {
    bool empty;
    struct Expression value;
} ReturnStatement;

typedef struct WhileStatement {
    struct Expression condition;
    struct Body body;

    struct Identifier non_nullable;
    bool code_gen_defined; // if the identifier is defined in generated code
} WhileStatement;

typedef struct IfStatement {
    struct Expression condition;
    struct Body if_body;
    struct Body else_body;

    struct Identifier non_nullable;
    bool code_gen_defined; // if the identifier is defined in generated code
} IfStatement;

typedef struct AssigmentStatement {
    struct Identifier id;
    SymVariable *var;
    bool discard;
    struct Expression value;
} AssigmentStatement;

typedef struct VariableDefinitionStatement {
    struct Identifier id;
    struct DataType type;
    bool isConst;
    struct Expression value;
    bool code_gen_defined; // if the identifier is defined in generated code
} VariableDefinitionStatement;

typedef struct Param {
    struct Identifier id;
    struct DataType type;
} Param;

typedef struct Function {
    struct Identifier id;
    LinkedList *params;
    SymFunctionPtr symFunction;

    struct Body body;

    struct DataType returnType;
} Function;

typedef struct Statement {
    enum StatementType type;
    union {
        struct FunctionCall function_call_statement;
        struct ReturnStatement return_statement;
        struct WhileStatement while_statement;
        struct IfStatement if_statement;
        struct AssigmentStatement assigment_statement;
        struct VariableDefinitionStatement variable_definition_statement;
    } data;
} Statement;

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

/**
 * Function to free a program structure
 *
 * @param program - the program structure to free
 */
void freeProgram(Program *program);

/**
 * Function to free a function structure
 *
 * @param function - the function structure to free
 */
void freeFunction(Function *function);

/**
 * Function to free a body structure
 *
 * @param body - the body structure to free
 */
void freeBody(Body *body);

/**
 * Function to free a parameter structure
 *
 * @param param - the parameter structure to free
 */
void freeParam(Param *param);

/**
 * Function to free a variable definition statement structure
 *
 * @param variableDefinitionStatement - the variable definition statement structure to free
 */
void freeVariableDefinitionStatement(VariableDefinitionStatement *variableDefinitionStatement);

/**
 * Function to free an assignment statement structure
 *
 * @param assigmentStatement - the assignment statement structure to free
 */
void freeAssigmentStatement(AssigmentStatement *assigmentStatement);

/**
 * Function to free an if statement structure
 *
 * @param ifStatement - the if statement structure to free
 */
void freeIfStatement(IfStatement *ifStatement);

/**
 * Function to free a while statement structure
 *
 * @param whileStatement - the while statement structure to free
 */
void freeWhileStatement(WhileStatement *whileStatement);

/**
 * Function to free a return statement structure
 *
 * @param returnStatement - the return statement structure to free
 */
void freeReturnStatement(ReturnStatement *returnStatement);

/**
 * Function to free a statement structure
 *
 * @param statement - the statement structure to free
 */
void freeStatement(Statement *statement);


#endif // AST_H
