#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "syntaxical/parser_pass1.h"
#include "semantical/sem_enums.h"

enum ERR_CODES parser_init(FILE *input);
enum ERR_CODES parser_parse(FILE *input);

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
bool parse_body(void);
bool parse_body_content(void);
bool parse_body_content_next(void);
bool parse_var_def(void);
bool parse_no_truth_expr(void);
bool parse_truth_expr(void);
bool parse_func_call(void);
bool parse_var_assign(void);
bool parse_if(void);
bool parse_else(void);
bool parse_while(void);
bool parse_ret_value(void);
bool parse_native_func_call(void);
bool parse_user_func_call(void);
bool parse_arguments(void);
bool parse_argument(void);
bool parse_argument_next(void);

// void parser_cleanup(void);
