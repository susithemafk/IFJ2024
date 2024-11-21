/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the Abstract Syntax Tree
 */

#ifndef AST_H
#define AST_H

#include "semantical/symtable.h"
#include "syntaxical/expression_ast.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"
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
    struct Expression value;
} ReturnStatement;

typedef struct WhileStatement {
    struct Expression condition;
    struct Body body;

    struct Identifier non_nullable;
    SymVariable *non_nullable_var;
    bool code_gen_defined; // if the identifier is defined in generated code
} WhileStatement;

typedef struct IfStatement {
    struct Expression condition;
    struct Body if_body;
    struct Body else_body;

    struct Identifier non_nullable;
    SymVariable *non_nullable_var;
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
    SymVariable *var;
    bool isConst;
    struct Expression value;
    bool code_gen_defined; // if the identifier is defined in generated code
} VariableDefinitionStatement;

typedef struct Param {
    struct Identifier id;
    struct DataType type;
    SymVariable *var;
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
