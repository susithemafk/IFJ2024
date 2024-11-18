/**
 * @file parser.c
 *
 * @author Marek Sucharda xsucha18
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "syntaxical/parser.h"
#include "lexical/scanner.h"

static enum ERR_CODES status;
static struct TOKEN currentToken;
static struct TOKEN lookaheadToken;

// Global variables for storing parsed data
char *modifier = NULL;
char *identifier = NULL;
char *data_type = NULL;
char expression[256] = {0}; // Buffer to store the expression
char *return_type = NULL;
char parameter_list[256] = {0}; // Buffer to store the parameter list

/**
 * Intro functions
 */
enum ERR_CODES parser_init()
{
	currentToken.type = TOKEN_NONE;
	currentToken.value = NULL;

	lookaheadToken.type = TOKEN_NONE;
	lookaheadToken.value = NULL;

	return SUCCESS;
}

void getNextToken()
{
	status = scanner_get_token(&currentToken);
}

void getLookaheadToken()
{
	status = scanner_peek_token(&lookaheadToken);
}

void resetGlobalVariables()
{
	modifier = NULL;
	identifier = NULL;
	data_type = NULL;
	memset(expression, 0, sizeof(expression));
	return_type = NULL;
	memset(parameter_list, 0, sizeof(parameter_list));
}

void printGlobalVariables()
{
	puts("\n");
	printf("Modifier: %s\n", modifier);
	printf("Identifier: %s\n", identifier);
	printf("Data type: %s\n", data_type);
	printf("Expression: %s\n", expression);
	printf("Return type: %s\n", return_type);
	printf("Parameter list: %s\n", parameter_list);
	puts("\n");
}

enum ERR_CODES parser_parse()
{
	// load first token
	getNextToken();

	while (currentToken.type != TOKEN_EOF)
	{
		if (!parser_decide())
		{
			return E_SYNTAX;
		}
		getNextToken();
	}

	return SUCCESS;
}

bool a() // TODO
{
	return true;
	// pravidla dle verči ale nějak mi to nesedí

	// <program> -> <prolog> <u_functions> <main> <u_functions> // TODO
	// bool parse_program() { return true; }

	// <prolog> -> const ifj = @ import ("ifj24.zig"); // TODO
	// bool parse_prolog() { return true; }

	// <main> -> pub fn main () void {< >} // TODO
	// bool parse_main() { return true; }

	// <u_functions> -> <function> <u_functions> | ε // TODO
	// bool parse_u_functions() { return true; }

	// <function> -> pub fn <func_id> (<params>) <data_type> {<func_body> <exit>} // TODO
	// bool parse_function() { return true; }

	// <func_call> -> <func_id> (<params>); // TODO
	// bool parse_func_call() { return true; }

	// <func_assign> -> <var_id> = <func_call>; // TODO
	// bool parse_func_assign() { return true; }

	// <params> -> <parameter> <parameter_next> | ε // TODO
	// bool parse_params() { return true; }

	// <parameter> -> <var_id> : <data_type> // TODO
	// bool parse_parameter() { return true; }

	// <parameter_next> -> , <parameter> | ε  // TODO
	// bool parse_parameter_next() { return true; }

	// <exit> -> <exit_value> | ε // TODO
	// bool parse_exit() { return true; }

	// <exit_value> -> return <no_truth_expr>; // TODO
	// bool parse_exit_value() { return true; }

	// <var_def> -> <const | var> <var_id> : <data_type> = <no_truth_expr>;  // TODO
	// bool parse_var_def() { return true; }

	// <var_assign> -> <var_id> = <no_truth_expr>; // TODO
	// bool parse_var_assign() { return true; }

	// <if> -> if (<truth_expr>) {<body>} <else>           // TODO: cekame na vercu
	// <if> -> if (<no_truth_expr>) |<arithm_expr>| {<body>} <else> // TODO: cekame na vercu
	// <else> -> else {<body>} // TODO: cekame na vercu
	// <else> -> ε // TODO: cekame na vercu

	// <while> -> while (<truth_expr>) {<body>}            // TODO: cekame na vercu
	// <while> -> while (<no_truth_expr>) |<arithm_expr>| {<body>}  // TODO: cekame na vercu

	// <expr> -> <truth_expr> | <no_truth_expr> // TODO
	// bool parse_expr() { return true; }

	// <truth_expr> -> <term> <comparison_operator> <term> // TODO
	// bool parse_truth_expr() { return true; }

	// <no_truth_expr> -> <arithm_expr> | <null_expr>  // TODO
	// bool parse_no_truth_expr() { return true; }
}

// TODO:
/**
 * init symtable v mainu a poslat si na ni odkaz sem
 * potom bych si ji zavolal, dostal z ni value
 * a az tu pak posilat do ast
 *
 * checkovat expressions a az pak je vkladat do astcka
 */

//* <var_def> -> <modifier: const | var> <var_id> : <optional_data_type> = <no_truth_expr>;
bool parse_var_def()
{
	if (!parse_modifier())
		return false;
	modifier = strdup(currentToken.value);

	getNextToken();

	if (!parse_var_id())
		return false;
	identifier = strdup(currentToken.value);

	getNextToken();

	if (!parse_optional_data_type())
		return false;
	data_type = strdup(currentToken.value);

	// printf("Current token: %s\n", currentToken.value);
	// getNextToken();

	if (currentToken.type != TOKEN_ASSIGN)
		return false;
	getNextToken();

	if (!parse_no_truth_expr())
		return false;

	getNextToken();

	// printGlobalVariables();

	printf("Added to AST\n\n");
	return true;
}

// <modifier> -> const | var
bool parse_modifier()
{
	if (currentToken.type == TOKEN_CONST || currentToken.type == TOKEN_VAR)
	{
		printf("Parsed modifier: \t%s\n", currentToken.value);
		return true;
	}
	puts("Expected 'const' or 'var' modifier");
	return false;
}

// <identifier> -> IDENTIFIER
bool parse_var_id()
{
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		printf("Parsed identifier: \t%s\n", currentToken.value);
		return true;
	}
	puts("Expected identifier");
	return false;
}

// <optional_data_type> -> : <data_type> | ε
bool parse_optional_data_type()
{
	if (currentToken.type == TOKEN_COLON)
	{
		getNextToken();
		if (!parse_data_type())
		{
			puts("Error parsing data type after ':'");
			return false;
		}
		return true;
	}
	printf("No data type specified (optional)\n");
	return true; // ε
}

// <data_type> -> i32 | f64 | string
bool parse_data_type()
{
	if (currentToken.type == TOKEN_I32 || currentToken.type == TOKEN_F64 || currentToken.type == TOKEN_STRING)
	{
		printf("Parsed data type: \t%s\n", currentToken.value);
		getNextToken();
		return true;
	}
	puts("Expected data type (i32, f64, or string)");
	return false;
}

// <assignment> -> <identifier> = <expression>
bool parse_assignment()
{
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		printf("Parsed identifier: \t%s\n", currentToken.value);
		getNextToken();

		if (currentToken.type == TOKEN_ASSIGN)
		{
			printf("Parsed assignment: \t%s\n", currentToken.value);
			getNextToken();

			if (!parse_no_truth_expr())
			{
				puts("Error parsing assignment expression");
				return false;
			}

			parse_end_with_semicolon();
			printf("Added assignment to AST\n");
			printf("CurrentToken: %s\n", currentToken.value);
			return true;
		}
	}

	printf("Expected assignment operator: '=', got %s\n", currentToken.value);

	return false;
}

// <no_truth_expr> -> <term> | <term> <operator> <term>
bool parse_no_truth_expr()
{
	memset(expression, 0, sizeof(expression));

	if (!parse_term())
	{
		return false;
	}

	while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS ||
		   currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE)
	{
		if (!parse_operator())
			return false;

		if (!parse_term())
			return false;
	}

	printf("Parsed no truth expression: %s\n", expression);
	return true;
}

// <truth_expr> -> <term> <comparison_operator> <term>
bool parse_truth_expr()
{
	memset(expression, 0, sizeof(expression));

	// handle true / false // TODO odkomentovat až přidáme do lexikálky true false
	// if (currentToken.type == TOKEN_TRUE || currentToken.type == TOKEN_FALSE)
	// {
	// 	strncat(expression, currentToken.value, sizeof(expression) - strlen(expression) - 1);
	// 	getNextToken();
	// 	printf("Parsed truth expression: %s\n", expression);
	// 	return true;
	// }

	if (!parse_term())
		return false;

	if (!parse_comparison_operator())
		return false;

	if (!parse_term())
		return false;

	printf("Parsed truth expression: %s\n", expression);
	return true;
}

// <term> -> IDENTIFIER | NUMBER | STRING | <function_call>
bool parse_term()
{
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		strncat(expression, currentToken.value, sizeof(expression) - strlen(expression) - 1);

		getLookaheadToken();
		if (lookaheadToken.type == TOKEN_LPAR)
		{
			// Handle function call
			if (!parse_function_call())
				return false;

			return true;
		}
		else
		{
			// It’s just an identifier, so consume the token
			getNextToken();
			return true;
		}
	}
	else if (currentToken.type == TOKEN_I32 || currentToken.type == TOKEN_F64 || currentToken.type == TOKEN_STRING)
	{
		strncat(expression, currentToken.value, sizeof(expression) - strlen(expression) - 1);
		getNextToken();
		return true;
	}

	puts("Expected term (identifier, number, string, or function call)");
	return false;
}

// <operator> -> + | - | * | /
bool parse_operator()
{
	if (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS || currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE)
	{
		strncat(expression, currentToken.value, sizeof(expression) - strlen(expression) - 1);
		getNextToken();
		return true;
	}

	puts("Expected operator (+, -, *, /)");
	return false;
}

// <comparison_operator> -> == | != | < | > | <= | >=
bool parse_comparison_operator()
{
	printf("CUrrentToken: %s\n", currentToken.value);
	if (currentToken.type == TOKEN_EQUALS || currentToken.type == TOKEN_NOTEQUAL || currentToken.type == TOKEN_LESSTHAN || currentToken.type == TOKEN_GREATERTHAN || currentToken.type == TOKEN_LESSOREQUAL || currentToken.type == TOKEN_GREATEROREQUAL)
	{
		strncat(expression, currentToken.value, sizeof(expression) - strlen(expression) - 1);
		getNextToken();
		return true;
	}

	puts("Expected comparison operator (==, !=, <, >, <=, >=)");
	return false;
}

// <function_definition> -> pub fn <identifier> ( <parameter_list> ) <return_type> <block>
bool parse_function_definition()
{
	if (currentToken.type == TOKEN_PUB)
	{
		getNextToken();
		if (currentToken.type == TOKEN_FN)
		{
			getNextToken();
			if (currentToken.type == TOKEN_IDENTIFIER)
			{
				printf("Parsed function name: \t%s\n", currentToken.value);
				getNextToken();
				if (currentToken.type == TOKEN_LPAR)
				{
					printf("Parsed left parenthesis: \t%s\n", currentToken.value);
					getNextToken();
					if (!parse_parameter_list())
						return false;
					if (currentToken.type == TOKEN_RPAR)
					{
						printf("Parsed right parenthesis: \t%s\n", currentToken.value);
						getNextToken();
						if (parse_return_type() && parse_block())
						{
							printf("CUrrentToken: %s\n", currentToken.value);
							puts("Added function definition to AST\n");
							return true;
						}
					}
				}
			}
		}
	}

	puts("Expected function definition");
	return false;
}

// <parameter_list> -> <parameter> (, <parameter>)* | ε
bool parse_parameter_list()
{
	if (!parse_parameter())
		return false;

	while (currentToken.type == TOKEN_COMMA)
	{
		getNextToken();
		if (!parse_parameter())
			return false;
	}

	return true;
}

// <parameter> -> <identifier> : <data_type>
bool parse_parameter()
{
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		printf("Parsed parameter: \t%s\n", currentToken.value);
		getNextToken();
		if (currentToken.type == TOKEN_COLON)
		{
			getNextToken();
			return parse_data_type();
		}
	}
	puts("Expected parameter (identifier followed by a data type)");
	return false;
}

// <return_type> -> : <data_type> | ε
bool parse_return_type()
{
	if (currentToken.type == TOKEN_COLON)
	{
		getNextToken();
		return parse_data_type();
	}
	return true;
}

// <if_statement> -> if ( <expression> ) <block> else <block>
bool parse_if_statement()
{
	printf("Parsed if statement: \t%s\n", currentToken.value);
	getNextToken();

	if (!currentToken.type == TOKEN_LPAR)
	{
		puts("Expected '(' after if statement");
		return false;
	}

	getNextToken();

	if (!parse_truth_expr())
	{
		puts("Error in if statement's expression");
		return false;
	}

	if (!currentToken.type == TOKEN_RPAR)
	{
		puts("Expected ')' after if statement's expression");
		return false;
	}

	getNextToken();

	if (!parse_block())
	{
		puts("Error in if statement's block");
		return false;
	}

	getNextToken();

	if (currentToken.type == TOKEN_ELSE)
	{
		printf("Parsed 'else': \t\t%s\n", currentToken.value);
		getNextToken();

		if (!parse_block())
		{
			puts("Error in 'else' statement's block");
			return false;
		}
	}

	printf("Parsed 'if' statement with optional 'else' block\n");
	return true;
}

// <while_statement> -> while ( <expression> ) <block>
bool parse_while_statement()
{
	if (currentToken.type == TOKEN_WHILE)
	{
		printf("Parsed while statement: \t%s\n", currentToken.value);
		getNextToken();
		if (currentToken.type == TOKEN_LPAR)
		{
			printf("Parsed left parenthesis: \t%s\n", currentToken.value);
			getNextToken();
			if (parse_truth_expr())
			{
				if (currentToken.type == TOKEN_RPAR)
				{
					printf("Parsed right parenthesis: \t%s\n", currentToken.value);
					getNextToken();
					return parse_block();
				}
			}
		}
	}
	puts("Error in while statement");
	return false;
}

// <function_call> -> <identifier> ( <argument_list> )
bool parse_function_call()
{
	printf("Parsing function call for: %s\n", currentToken.value);

	getNextToken();
	if (currentToken.type != TOKEN_LPAR)
	{
		puts("Expected '(' for function call");
		return false;
	}

	getNextToken();
	if (currentToken.type != TOKEN_RPAR)
	{
		if (!parse_argument_list())
			return false;
	}

	if (currentToken.type != TOKEN_RPAR)
	{
		puts("Expected ')' at the end of function call");
		return false;
	}

	getNextToken();
	return true;
}

// <argument_list> -> <expression> (, <expression>)* | ε
bool parse_argument_list()
{
	if (currentToken.type == TOKEN_RPAR)
		return true;

	if (!parse_no_truth_expr())
		return false;

	while (currentToken.type == TOKEN_COMMA)
	{
		getNextToken();
		if (!parse_no_truth_expr())
			return false;
	}

	printf("Parsed argument list: %s\n", expression);
	return true;
}

// <block> -> { <statement>* }
bool parse_block()
{
	if (currentToken.type != TOKEN_LBRACE)
	{
		puts("Expected '{' at the start of block");
		return false;
	}
	getNextToken();

	while (currentToken.type != TOKEN_RBRACE)
	{
		if (!parser_decide())
		{
			puts("Error in statement inside block");
			return false;
		}
		getNextToken();
	}

	return true;
}

// <return_statement> -> return <expression>? ;
bool parse_return_statement()
{
	if (currentToken.type == TOKEN_RETURN)
	{
		printf("Parsed 'return': \t%s\n", currentToken.value);
		getNextToken();

		if (currentToken.type != TOKEN_SEMICOLON)
		{
			if (!parse_no_truth_expr())
			{
				puts("Error: Invalid return expression");
				return false;
			}
		}

		if (currentToken.type == TOKEN_SEMICOLON)
		{
			printf("Parsed ';': \t\t%s\n", currentToken.value);
			getNextToken();
			return true;
		}
		else
		{
			puts("Expected ';' after return statement");
			return false;
		}
	}
	return false;
}

// <end_with_semicolon> -> ;
bool parse_end_with_semicolon()
{
	if (currentToken.type != TOKEN_SEMICOLON)
	{
		puts("Expected semicolon");
		return false;
	}
	printf("Parsed semicolon: \t%s\n", currentToken.value);
	return true;
}

// parser decide where to go
bool parser_decide()
{
	printf("\nDeciding on token: \t%s\n", currentToken.value);
	switch (currentToken.type)
	{
	case TOKEN_VAR:
	case TOKEN_CONST:
		printf("Going for var def\n");
		return parse_var_def();
	case TOKEN_PUB:
		printf("Going for function definition\n");
		return parse_function_definition();
	case TOKEN_IDENTIFIER:
		getLookaheadToken();
		if (lookaheadToken.type == TOKEN_ASSIGN)
		{
			printf("Going for assignment\n");
			return parse_assignment();
		}
		else if (lookaheadToken.type == TOKEN_LPAR)
		{

			printf("Going for function call\n");
			return parse_function_call();
		}
		else
			return parse_no_truth_expr();
	case TOKEN_IF:
		printf("Going for if statement\n");
		return parse_if_statement();
	case TOKEN_WHILE:
		printf("Going for while statement\n");
		return parse_while_statement();
	case TOKEN_RETURN:
		printf("Going for return statement\n");
		return parse_return_statement();
	default:
		printf("Syntax error: unexpected token %s\n", currentToken.value);
		return false;
	}

	return true;
}
