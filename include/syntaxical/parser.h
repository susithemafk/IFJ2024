#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "utility/enumerations.h"

enum ERR_CODES parser_init();
enum ERR_CODES parser_parse();
void getNextToken();

bool parse_variable_definition();
bool parse_modifier();
bool parse_identifier();
bool parse_optional_data_type();
bool parse_data_type();
bool parse_assignment();
bool parse_expression();
bool parser_parse_statement();
bool parse_end_with_semicolon();

#endif
