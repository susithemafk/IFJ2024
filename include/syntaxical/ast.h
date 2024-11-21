/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the Abstract Syntax Tree
 */

#ifndef AST_H
#define AST_H

#include "syntaxical/expression_ast.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"

struct Program {
    LinkedList *functions;
};

struct Body {
    LinkedList *statements;
};

enum StatementType {
    FunctionCallStatementType,
    ReturnStatementType,
    WhileStatementType,
    IfStatementType,
    AssigmentStatementType,
    VariableDefinitionStatementType,
};

struct ReturnStatement {
    struct Expression value;
};

struct WhileStatement {
    struct Expression condition;
    struct Body body;

    struct Identifier non_nullable;
    bool code_gen_defined; // if the identifier is defined in generated code
};

struct IfStatement {
    struct Expression condition;
    struct Body if_body;
    struct Body else_body;

    struct Identifier non_nullable;
    bool code_gen_defined; // if the identifier is defined in generated code
};

struct AssigmentStatement {
    struct Identifier id;
    bool discard;
    struct Expression value;
};

struct VariableDefinitionStatement {
    struct Identifier id;
    struct DataType type;
    bool isConst;
    struct Expression value;
    bool code_gen_defined; // if the identifier is defined in generated code
};

struct Param {
    struct Identifier id;
    struct DataType type;
};

struct Function {
    struct Identifier id;
    LinkedList *params;
    struct Body body;

    struct DataType returnType;
};

struct Statement {
    enum StatementType type;
    union {
        struct FunctionCall function_call_statement;
        struct ReturnStatement return_statement;
        struct WhileStatement while_statement;
        struct IfStatement if_statement;
        struct AssigmentStatement assigment_statement;
        struct VariableDefinitionStatement variable_definition_statement;
    } data;
};

typedef struct Statement Statement;
typedef struct ReturnStatement ReturnStatement;
typedef struct WhileStatement WhileStatement;
typedef struct IfStatement IfStatement;
typedef struct AssigmentStatement AssigmentStatement;
typedef struct VariableDefinitionStatement VariableDefinitionStatement;
typedef struct Param Param;
typedef struct Body Body;
typedef struct Function Function;
typedef struct Program Program;

void freeProgram(Program *program);
void freeFunction(Function *function);
void freeBody(Body *body);
void freeParam(Param *param);
void freeVariableDefinitionStatement(VariableDefinitionStatement *variableDefinitionStatement);
void freeAssigmentStatement(AssigmentStatement *assigmentStatement);
void freeIfStatement(IfStatement *ifStatement);
void freeWhileStatement(WhileStatement *whileStatement);
void freeReturnStatement(ReturnStatement *returnStatement);
void freeStatement(Statement *statement);

#endif // AST_H
