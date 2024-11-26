#include "lexical/scanner.h"
#include "semantical/symtable.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syntaxical/ast.h"

/** 
 * Function to do the first pass over the program
 * @param symtable - the symbol table
 * @return the buffer of tokens
 */
enum ERR_CODES firstPass(FILE *input, LinkedList *buffer);

/**
 * Function to parse a function call
 * 
 * @param buffer - the buffer of tokens
 * @param table - the symbol table
 * @return enum ERR_CODES
*/
void freeBuffer(LinkedList **buffer);

/**
 * Function to save a new token to the buffer
 * 
 * @param token - the token to save
 * @param buffer - the buffer to save the token to
 * @return bool - if the token was saved correctly
 */
bool saveNewToken(struct TOKEN token, LinkedList *buffer);


enum ERR_CODES parser_init(SymTable *tbl, LinkedList *buf);
enum ERR_CODES parser_parse(FILE *input, Program *program);

bool match(enum TOKEN_TYPE tokenType);
bool parse_program(Program *program);
bool parse_prolog(void);
bool parse_functions(LinkedList *functions);
bool parse_function(Function *function);
bool parse_next_function(void);
bool parse_params(LinkedList *params);
bool parse_parameter(Param *param);
bool parse_data_type(DataType *data_type);
bool parse_func_body(Body *body);
bool parse_body(Body *body);
bool parse_body_content(Statement *statement);
bool parse_body_content_next(void);
bool parse_var_def(VariableDefinitionStatement *variable_definition_statement);
bool parse_no_truth_expr(Expression *expr);
bool parse_truth_expr(Expression *expr);
bool parse_func_call_statement(FunctionCall *function_call);
bool parse_var_assign(AssigmentStatement *assign_statement);
bool parse_if(IfStatement *if_statment);
bool parse_else(IfStatement *if_statment);
bool parse_while(WhileStatement *while_statement);
bool parse_ret_value(ReturnStatement *return_statement);
bool parse_native_func_call(FunctionCall *function_call);
bool parse_user_func_call(FunctionCall *function_call);
bool parse_func_call_params(LinkedList *attributes);
bool parse_func_call_param(Expression *expr);
