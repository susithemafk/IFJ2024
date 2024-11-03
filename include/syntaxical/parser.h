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
void getPreviousToken();

/**
 * parser functions
 */
// <program> -> <statement> <program> | ε
bool parse_program();

// <statement> -> <variable_definition> ; | <assignment> ; | <function_definition> | <if_statement> | <while_statement> | <function_call> ;
bool parse_statement();

// <variable_definition> -> <modifier> <identifier> <optional_data_type> <assignment> <expression> ;
bool parse_variable_definition();

// <modifier> -> const | var
bool parse_modifier();

// <identifier> -> IDENTIFIER
bool parse_identifier();

// <optional_data_type> -> : <data_type> | ε
bool parse_optional_data_type();

// <data_type> -> i32 | f64 | string
bool parse_data_type();

// <assignment> -> <identifier> = <expression>
bool parse_assignment();

// <expression> -> <term> | <term> <operator> <term>
bool parse_expression();

// <term> -> IDENTIFIER | NUMBER | STRING | <function_call>
bool parse_term();

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
