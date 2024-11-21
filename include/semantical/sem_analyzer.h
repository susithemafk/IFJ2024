#ifndef SEM_ANALYZER_H
#define SEM_ANALYZER_H

#include "syntaxical/ast.h"

enum ERR_CODES analyzeProgram(Program *program);
enum ERR_CODES analyzeFunction(Function *function);
enum ERR_CODES analyzeBody(Body *body);

enum ERR_CODES analyzeParam(Param *param);

/* analyze statements */
enum ERR_CODES analyzeStatement(Statement *statement);
enum ERR_CODES analyzeReturnStatement(ReturnStatement *statement);
enum ERR_CODES analyzeWhileStatement(WhileStatement *statement);
enum ERR_CODES analyzeIfStatement(IfStatement *statement);
enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement);
enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement);

enum ERR_CODES analyzeExpression(Expression *expr);
enum ERR_CODES analyzeIdentifier(Identifier *identifier);
enum ERR_CODES analyzeFunctionCall(FunctionCall *function_call);
enum ERR_CODES analyzeLiteral(Literal *literal);
enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr);

#endif // SEM_ANALYZER_H