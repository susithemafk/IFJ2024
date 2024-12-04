/** AUTHOR
 *
 * @author <xsucha18> Marek Sucharda 
 * @author <253171> Vanesa Zimmermannová
 * @author <247581> Martin Mendl   
 * @file precedent.c
 * @date 17.11. 2024
 * @brief main src file for the precedent analysis
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "lexical/scanner.h"
#include "semantical/symtable.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#include "syntaxical/ast.h"
#else
#include "scanner.h"
#include "symtable.h"
#include "enumerations.h"
#include "linked_list.h"
#include "ast.h"
#endif

/** 
 * Function to do the first pass over the program
 * @param symtable - the symbol table
 * @return the buffer of tokens
 */
enum ERR_CODES firstPass(FILE *input, LinkedList *buffer);

/**
 * Function to save a new token to the buffer
 * 
 * @param token - the token to save
 * @param buffer - the buffer to save the token to
 * @return bool - if the token was saved correctly
 */
bool saveNewToken(struct TOKEN token, LinkedList *buffer);

/**
 * Function to init the parser
 * 
 * @param table - the symbol table
 */
void parser_init(SymTable *tbl);

/**
 * Function to parse the program
 * 
 * @param input - the input file
 * @param program - the AST root
 * @return enum ERR_CODES - the error code
*/
enum ERR_CODES parser_parse(FILE *input, Program *program);

/**
 * Function to match a token
 * 
 * @param tokenType - the type of the token to match
 * @return bool - if the token was matched
 */
bool match(enum TOKEN_TYPE tokenType);

/**
 * Function to get the current token
 * 
 * @param program - the AST root
 * @return bool - if the token was matched
 */
bool parse_program(Program *program);

/**
 * Function to parse the prolog
 * 
 * @return bool - if the prolog was parsed
 */
bool parse_prolog(void);

/**
 * Function to parse the functions in the program
 *
 * @param functions - a linked list of functions to parse
 * @return bool - if the functions were parsed successfully
 */
bool parse_functions(LinkedList *functions);

/**
 * Function to parse a single function
 *
 * @param function - the function to parse
 * @return bool - if the function was parsed successfully
 */
bool parse_function(Function *function);

/**
 * Function to parse the next function in the sequence
 *
 * @return bool - if the next function was parsed successfully
 */
bool parse_next_function(void);

/**
 * Function to parse the parameters of a function
 *
 * @param params - a linked list of parameters to parse
 * @return bool - if the parameters were parsed successfully
 */
bool parse_params(LinkedList *params);

/**
 * Function to parse a single parameter
 *
 * @param param - the parameter to parse
 * @return bool - if the parameter was parsed successfully
 */
bool parse_parameter(Param *param);

/**
 * Function to parse a data type
 *
 * @param data_type - the data type to parse
 * @return bool - if the data type was parsed successfully
 */
bool parse_data_type(DataType *data_type);

/**
 * Function to parse the body of a function
 *
 * @param body - the body of the function to parse
 * @return bool - if the body was parsed successfully
 */
bool parse_func_body(Body *body);

/**
 * Function to parse a generic body block
 *
 * @param body - the body block to parse
 * @return bool - if the body was parsed successfully
 */
bool parse_body(Body *body);

/**
 * Function to parse the content of a body block
 *
 * @param statement - the statement contained in the body block to parse
 * @return bool - if the body content was parsed successfully
 */
bool parse_body_content(Statement *statement);

/**
 * Function to parse the next body content in sequence
 *
 * @return bool - if the next body content was parsed successfully
 */
bool parse_body_content_next(void);

/**
 * Function to parse a variable definition
 *
 * @param variable_definition_statement - the variable definition to parse
 * @return bool - if the variable definition was parsed successfully
 */
bool parse_var_def(VariableDefinitionStatement *variable_definition_statement);

/**
 * Function to parse a non-truth expression
 *
 * @param expr - the expression to parse
 * @return bool - if the non-truth expression was parsed successfully
 */
bool parse_no_truth_expr(Expression *expr);

/**
 * Function to parse a truth expression
 *
 * @param expr - the expression to parse
 * @return bool - if the truth expression was parsed successfully
 */
bool parse_truth_expr(Expression *expr);

/**
 * Function to parse a function call statement
 *
 * @param function_call - the function call to parse
 * @return bool - if the function call was parsed successfully
 */
bool parse_func_call_statement(FunctionCall *function_call);

/**
 * Function to parse a variable assignment
 *
 * @param assign_statement - the assignment statement to parse
 * @return bool - if the variable assignment was parsed successfully
 */
bool parse_var_assign(AssigmentStatement *assign_statement);

/**
 * Function to parse an if statement
 *
 * @param if_statment - the if statement to parse
 * @return bool - if the if statement was parsed successfully
 */
bool parse_if(IfStatement *if_statment);

/**
 * Function to parse an else statement
 *
 * @param if_statment - the else statement to parse
 * @return bool - if the else statement was parsed successfully
 */
bool parse_else(IfStatement *if_statment);

/**
 * Function to parse a while statement
 *
 * @param while_statement - the while statement to parse
 * @return bool - if the while statement was parsed successfully
 */
bool parse_while(WhileStatement *while_statement);

/**
 * Function to parse a return value
 *
 * @param return_statement - the return statement to parse
 * @return bool - if the return value was parsed successfully
 */
bool parse_ret_value(ReturnStatement *return_statement);

/**
 * Function to parse a native function call
 *
 * @param function_call - the native function call to parse
 * @return bool - if the native function call was parsed successfully
 */
bool parse_native_func_call(FunctionCall *function_call);

/**
 * Function to parse a user-defined function call
 *
 * @param function_call - the user-defined function call to parse
 * @return bool - if the user-defined function call was parsed successfully
 */
bool parse_user_func_call(FunctionCall *function_call);

/**
 * Function to parse the parameters of a function call
 *
 * @param attributes - a linked list of attributes to parse
 * @return bool - if the function call parameters were parsed successfully
 */
bool parse_func_call_params(LinkedList *attributes);

/**
 * Function to parse a single parameter in a function call
 *
 * @param expr - the expression representing the parameter to parse
 * @return bool - if the function call parameter was parsed successfully
 */
bool parse_func_call_param(Expression *expr);

