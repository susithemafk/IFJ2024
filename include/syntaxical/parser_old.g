#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "utility/enumerations.h"

/**
 * intro functions
 */
enum ERR_CODES parser_init();
enum ERR_CODES parser_parse();
void getNextToken();
void getLookaheadToken();
void resetGlobalVariables();

/**
 * parser functions
 */

bool a(); // remove

//* <var_def> -> <modifier: const | var> <var_id> : <optional_data_type> = <no_truth_expr>;
bool parse_var_def();
// <modifier> -> const | var
bool parse_modifier();
// <identifier> -> IDENTIFIER
bool parse_var_id();
// <optional_data_type> -> : <data_type> | ε
bool parse_optional_data_type();
// <data_type> -> i32 | f64 | string
bool parse_data_type();
// <assignment> -> <identifier> = <expression>
bool parse_assignment();
// <no_truth_expr> -> <arithm_expr> | <null_expr>  // TODO
bool parse_no_truth_expr();
// <term> -> IDENTIFIER | NUMBER | STRING | <function_call>
bool parse_term();
// <operator> -> + | - | * | /
bool parse_operator();
// <comparison_operator> -> == | != | < | > | <= | >=
bool parse_comparison_operator();
// <function_definition> -> pub fn <identifier> ( <parameter_list> ) <return_type> <block>
bool parse_function_definition();
// <parameter_list> -> <parameter> (, <parameter>)* | ε
bool parse_parameter_list();
// <parameter> -> <identifier> : <data_type>
bool parse_parameter();
// <return_type> -> : <data_type> | ε
bool parse_return_type();
// <if_statement> -> if ( <expression> ) <block> else <block>
bool parse_if_statement();
// <while_statement> -> while ( <expression> ) <block>
bool parse_while_statement();
// <function_call> -> <identifier> ( <argument_list> )
bool parse_function_call();
// <argument_list> -> <expression> (, <expression>)* | ε
bool parse_argument_list();
// <block> -> { <statement>* }
bool parse_block();
// <return_statement> -> return <expression>? ;
bool parse_return_statement();
// <end_with_semicolon> -> ;
bool parse_end_with_semicolon();
// parser decide where to go
bool parser_decide();

#endif
