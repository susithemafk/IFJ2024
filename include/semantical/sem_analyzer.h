/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 * @file sem_analyzer.h
 * @date 20.10. 2024
 * @brief main header file for the semantical analyzer
 */


#ifndef SEM_ANALYZER_H
#define SEM_ANALYZER_H

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/ast.h"
#include "utility/enumerations.h"
#else 
#include "ast.h"
#include "enumerations.h"
#endif

enum COVERSION_FLAGS {
    CONV_NONE ,
    CONV_NOT_POSSIBLE,
    CONV_TO_INT,
    CONV_TO_FLOAT,
};

/**
 * Function to analyze the whole program
 * 
 * @param program - the program to analyze
 * @param table - the symbol table
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeProgram(Program *program, SymTable *table);


/**
 * Function to analyze a paramater
 * 
 * @param param - the param to analyze
 * @param table - the symbol table
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeParam(Param *param, SymTable *table);

/**
 * Function to analyze a body
 * 
 * @param body - the body to analyze
 * @param table - the symbol table
 * @param currentFunc - the current function
 * @param retCount - the return count
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeBody(Body *body, SymTable *table, SymFunctionPtr currentFunc, int *retCount);

/**
 * Function to analyze a statement
 * 
 * @param statement - the statement to analyze
 * @param table - the symbol table
 * @param currentFunc - the current function
 * @param retCount - the return count
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeStatement(Statement *statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount);

/**
 * Function to analyze a return statement
 * 
 * @param return_statement - the return statement to analyze
 * @param table - the symbol table
 * @param currentFunc - the current function
 * @param retCount - the return count
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeReturnStatement(ReturnStatement *return_statement, SymTable *table,  SymFunctionPtr currentFunc, int *retCount);

/**
 * Function to analyze a binary expression
 * 
 * @param while_statement - the statement to analyze
 * @param table - the symbol table
 * @param currentFunc - the current function
 * @param retCount - the return count ptr
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeWhileStatement(WhileStatement *while_statement, SymTable *table, SymFunctionPtr currentFunc, int* retCount);

/**
 * Function to analyze an if statement
 * 
 * @param if_statement - the if statement to analyze
 * @param table - the symbol table
 * @param currentFunc - the current function
 * @param retCount - the return count
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeIfStatement(IfStatement *if_statement, SymTable *table, SymFunctionPtr currentFunc, int *retCount);

/**
 * Function to analyze a binary expression
 * 
 * @param statement - the statement to analyze
 * @param table - the symbol table
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeAssigmentStatement(AssigmentStatement *statement, SymTable *table);

/**
 * Function to analyze a binary expression
 * 
 * @param statement - the statement to analyze
 * @param table - the symbol table
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeVariableDefinitionStatement(VariableDefinitionStatement *statement, SymTable *table);

/**
 * Function to analyze an expression
 * 
 * @param expr - the expression to analyze
 * @param table - the symbol table
 * @param returnType - the return type
 * @param resultNullable - the result nullable
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeExpression(Expression *expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable);

/**
 * Function to analyze a function call
 * 
 * @param function_call - the function call to analyze
 * @param table - the symbol table
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeFunctionCall(FunctionCall *function_call, SymTable *table);

/**
 * Function to analyze a binary expression
 * 
 * @param binary_expr - the binary expression to analyze
 * @param table - the symbol table
 * @param returnType - the return type
 * @param resultNullable - the result nullable
 * @return enum ERR_CODES - the error code
 */
enum ERR_CODES analyzeBinaryExpression(BinaryExpression *binary_expr, SymTable *table, enum DATA_TYPES *returnType, bool *resultNullable);

/**
 * Function to analyze a unary expression
 * 
 * @param nullMain - the main nullable value
 * @param nullSecond - the second nullable value
 * @return bool
 */
bool nullCompatabilityCheck(bool nullMain, bool nullSecond);

/**
 * Function to check, if a f64 value can be converted to i32
 * 
 * @param value - the value to check
 * @return bool - if the value can be converted
 */
bool f64valueCanBeCovertedToi32(char *value);

#endif // SEM_ANALYZER_H
