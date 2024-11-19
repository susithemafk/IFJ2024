#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "syntaxical/parser_pass1.h"

enum ERR_CODES parser_parse(void);

bool match(enum TOKEN_TYPE tokenType);
bool parse_program(void);
bool parse_prolog(void);
bool parse_functions(void);
bool parse_function(void);
bool parse_next_function(void);
bool parse_params(void);
bool parse_parameter(void);
bool parse_parameter_next(void);
bool parse_data_type(void);
bool parse_func_body(void);
