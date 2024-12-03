/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.h
 * @date 20.10.2024
 * @brief Declarations for the code generator
 */

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/ast.h"
#else
#include "ast.h"
#endif

/**
 * Function to generate code for the entire program.
 * 
 * @param program Pointer to the Program structure.
 */
void generateCodeProgram(Program *program);

/**
 * Function to generate code for a function.
 * 
 * @param function Pointer to the Function structure.
 */
void generateCodeFunction(Function *function);

/**
 * Function to generate code for the body of a function or program.
 * 
 * @param body Pointer to the Body structure.
 */
void generateCodeBody(Body *body);

/**
 * Function to generate code for a statement.
 * 
 * @param statement Pointer to the Statement structure.
 */
void generateCodeStatement(Statement *statement);

/**
 * Function to generate code for a parameter.
 * 
 * @param param Pointer to the Param structure.
 */
void generateCodeParam(Param *param);

/**
 * Function to generate code for a variable definition statement.
 * 
 * @param statement Pointer to the VariableDefinitionStatement structure.
 */
void generateCodeVariableDefinitionStatement(VariableDefinitionStatement *statement);

/**
 * Function to generate code for an assignment statement.
 * 
 * @param statement Pointer to the AssigmentStatement structure.
 */
void generateCodeAssigmentStatement(AssigmentStatement *statement);

/**
 * Function to generate code for an if statement.
 * 
 * @param statement Pointer to the IfStatement structure.
 */
void generateCodeIfStatement(IfStatement *statement);

/**
 * Function to generate code for a while statement.
 * 
 * @param statement Pointer to the WhileStatement structure.
 */
void generateCodeWhileStatement(WhileStatement *statement);

/**
 * Function to generate code for a return statement.
 * 
 * @param statement Pointer to the ReturnStatement structure.
 */
void generateCodeReturnStatement(ReturnStatement *statement);

/**
 * Function to generate code for an expression.
 * 
 * @param expression Pointer to the Expression structure.
 */
void generateCodeExpression(Expression *expression);

/**
 * Function to generate code for a function call.
 * 
 * @param function_call Pointer to the FunctionCall structure.
 */
void generateCodeFunctionCall(FunctionCall *function_call);

/**
 * Function to generate code for an identifier.
 * 
 * @param identifier Pointer to the Identifier structure.
 */
void generateCodeIdentifier(Identifier *identifier);

/**
 * Function to generate code for a literal value.
 * 
 * @param literal Pointer to the Literal structure.
 */
void generateCodeLiteral(Literal *literal);

/**
 * Function to generate code for a binary expression.
 * 
 * @param binary_expression Pointer to the BinaryExpression structure.
 */
void generateCodeBinaryExpression(BinaryExpression *binary_expression);

/**
 * Function to pre-generate code for the body of a function or program.
 * 
 * @param body Pointer to the Body structure.
 */
void preGenerateBody(Body *body);

/**
 * Function to pre-generate code for a statement.
 * 
 * @param statement Pointer to the Statement structure.
 */
void preGenerateStatement(Statement *statement);

/**
 * Function to pre-generate code for a variable definition statement.
 * 
 * @param statement Pointer to the VariableDefinitionStatement structure.
 */
void preGenerateVariableDefinitionStatement(VariableDefinitionStatement *statement);

/**
 * Function to pre-generate code for an if statement.
 * 
 * @param statement Pointer to the IfStatement structure.
 */
void preGenerateIfStatement(IfStatement *statement);

/**
 * Function to pre-generate code for a while statement.
 * 
 * @param statement Pointer to the WhileStatement structure.
 */
void preGenerateWhileStatement(WhileStatement *statement);

#endif //CODE_GENERATOR_H
