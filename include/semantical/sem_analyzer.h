#ifndef SEM_ANALYZER_H
#define SEM_ANALYZER_H

#include "syntaxical/ast.h"
#include "utility/enumerations.h"

enum COVERSION_FLAGS {
    CONV_NONE ,
    CONV_NOT_POSSIBLE,
    CONV_TO_INT,
    CONV_TO_FLOAT,
};

enum ERR_CODES analyzeProgram(Program *program, SymTable *table);
enum ERR_CODES analyzeParam(Param *param, SymTable *table);
enum ERR_CODES analyzeBody(Body *body, SymTable *table, SymFunctionPtr currentFunc, int *retCount);

/* analyze statements */
enum ERR_CODES analyzeStatement(Statement *statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount);
enum ERR_CODES analyzeReturnStatement(ReturnStatement *return_statement, SymTable *table,  SymFunctionPtr currentFunc, int *retCount);
enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement, SymTable *table, SymFunctionPtr currentFunc, int* retCount);

enum ERR_CODES analyzeIfStatement(IfStatement *if_statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount);
enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement, SymTable *table);
enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement, SymTable *table);

enum ERR_CODES analyzeExpression(Expression *expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable);
enum ERR_CODES analyzeFunctionCall(FunctionCall *function_call, SymTable *table);
enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable);

bool nullCompatabilityCheck(bool nullMain, bool nullSecond);

enum COVERSION_FLAGS canCovertLiteral(Literal *literal, enum DATA_TYPES expectedType);

#endif // SEM_ANALYZER_H
