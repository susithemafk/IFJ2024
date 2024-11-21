/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.h
 * @date 20.10.2024
 * @brief Declarations for the code generator
 */

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "syntaxical/ast.h"

void generateCodeProgram(Program *program);
void generateCodeFunction(Function *function);
void generateCodeBody(Body *body);
void generateCodeStatement(Statement *statement);
void generateCodeParam(Param *param);
void generateCodeVariableDefinitionStatement(VariableDefinitionStatement *statement);
void generateCodeAssigmentStatement(AssigmentStatement *statement);
void generateCodeIfStatement(IfStatement *statement);
void generateCodeWhileStatement(WhileStatement *statement);
void generateCodeReturnStatement(ReturnStatement *statement);

void generateCodeExpression(Expression *expression);
void generateCodeFunctionCall(FunctionCall *function_call);
void generateCodeIdentifier(Identifier *identifier);
void generateCodeLiteral(Literal *literal, DataType *data_type);
void generateCodeBinaryExpression(BinaryExpression *binary_expression);

void preGenerateBody(Body *body);
void preGenerateStatement(Statement *statement);
void preGenerateVariableDefinitionStatement(VariableDefinitionStatement *statement);
void preGenerateIfStatement(IfStatement *statement);
void preGenerateWhileStatement(WhileStatement *statement);

#endif // CODE_GENERATOR_H
