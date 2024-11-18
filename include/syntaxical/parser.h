#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "syntaxical/parser_pass1.h"

enum ERR_CODES parser_parse();

bool match(enum TOKEN_TYPE tokenType);
bool parse_program();
bool parse_prolog();
bool parse_functions();
bool parse_function();
bool parse_next_function();
bool parse_params();
bool parse_parameter();
bool parse_data_type();