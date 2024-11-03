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

// TODO:
/**
 * init symtable v mainu a poslat si na ni odkaz sem
 * potom bych si ji zavolal, dostal z ni value
 * a az tu pak posilat do ast
 *
 * checkovat expressions a az pak je vkladat do astcka
 */

// <program> -> <statement> <program> | ε
bool parse_program() { return true; }

// <statement> -> <variable_definition> ; | <assignment> ; | <function_definition> | <if_statement> | <while_statement> | <function_call> ;
bool parse_statement() { return true; }

// <variable_definition> -> <modifier> <identifier> <optional_data_type> <assignment> <expression> ;
bool parse_variable_definition()
{
	// do symtablu, ta vrati data, pak do ast
	if (!parse_modifier())
		return false;
	if (!parse_identifier())
		return false;
	if (!parse_optional_data_type())
		return false;

	if (currentToken.type != TOKEN_ASSIGN)
		return false;
	getNextToken();

	if (!parse_expression())
		return false;
	if (!parse_end_with_semicolon())
		return false;

	// TODO: add to AST
	printf("Added to AST\n\n");
	return true;
}

// <modifier> -> const | var
bool parse_modifier()
{
	if (currentToken.type == TOKEN_CONST || currentToken.type == TOKEN_VAR)
	{
		printf("Parsed modifier: \t%s\n", currentToken.value);
		getNextToken();
		return true;
	}
	puts("Expected 'const' or 'var' modifier");
	return false;
}

// <identifier> -> IDENTIFIER
bool parse_identifier()
{
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		printf("Parsed identifier: \t%s\n", currentToken.value);
		getNextToken();
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
		return parse_data_type();
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

			if (parse_expression())
			{
				printf("CUrrentToken: %s\n", currentToken.value);
				parse_end_with_semicolon();
				getNextToken();
				return true;
			}
		}
	}

	printf("Expected assignment operator: '=', got %s\n", currentToken.value);

	return false;
}

// <expression> -> <term> | <term> <operator> <term> | <term> <comparison_operator> <term>
bool parse_expression()
{
	if (!parse_term())
		return false;

	while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS || currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE || currentToken.type == TOKEN_EQUALS)
	{
		if (currentToken.type == TOKEN_EQUALS)
		{
			if (!parse_comparison_operator())
				return false;
		}
		else
		{
			if (!parse_operator())
				return false;
		}

		if (!parse_term())
			return false;
	}

	printf("Parsed expression\n");
	return true;
}

// <term> -> IDENTIFIER | NUMBER | STRING | <function_call>
bool parse_term()
{
	if (currentToken.type == TOKEN_IDENTIFIER || currentToken.type == TOKEN_I32 || currentToken.type == TOKEN_F64 || currentToken.type == TOKEN_STRING)
	{
		printf("Parsed term: \t%s\n", currentToken.value);
		getNextToken();
		return true;
	}
	else if (currentToken.type == TOKEN_IDENTIFIER)
	{
		return parse_function_call();
	}

	puts("Expected term (identifier, number, string, or function call)");
	return false;
}

// <operator> -> + | - | * | /
bool parse_operator()
{
	if (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS || currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE)
	{
		printf("Parsed operator: \t%s\n", currentToken.value);
		getNextToken();
		return true;
	}

	puts("Expected operator (+, -, *, /)");
	return false;
}

// <comparison_operator> -> == | != | < | > | <= | >=
bool parse_comparison_operator()
{
	if (currentToken.type == TOKEN_EQUALS || currentToken.type == TOKEN_NOTEQUAL || currentToken.type == TOKEN_LESSTHAN || currentToken.type == TOKEN_GREATERTHAN || currentToken.type == TOKEN_LESSOREQUAL || currentToken.type == TOKEN_GREATEROREQUAL)
	{
		printf("Parsed comparison operator: \t%s\n", currentToken.value);
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
	if (currentToken.type == TOKEN_IF)
	{
		printf("Parsed if statement: \t%s\n", currentToken.value);
		getNextToken();
		if (currentToken.type == TOKEN_LPAR)
		{
			getNextToken();
			if (parse_expression())
			{
				if (currentToken.type == TOKEN_RPAR)
				{
					getNextToken();
					if (parse_block())
					{
						getNextToken();

						if (currentToken.type == TOKEN_ELSE)
						{
							getNextToken();
							return parse_block();
						}
						return true;
					}
				}
			}
		}
	}
	puts("Error in if statement");
	return false;
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
			if (parse_expression())
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
	if (currentToken.type == TOKEN_IDENTIFIER)
	{
		printf("Parsed function call: \t%s\n", currentToken.value);
		getNextToken();
		if (currentToken.type == TOKEN_LPAR)
		{
			getNextToken();
			if (parse_argument_list())
			{
				if (currentToken.type == TOKEN_RPAR)
				{
					getNextToken();
					return true;
				}
			}
		}
	}
	puts("Expected function call");
	return false;
}

// <argument_list> -> <expression> (, <expression>)* | ε
bool parse_argument_list()
{
	if (!parse_expression())
		return false;

	while (currentToken.type == TOKEN_COMMA)
	{
		getNextToken();
		if (!parse_expression())
			return false;
	}

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
	printf("Parsed block: \t\t%s\n", currentToken.value);
	getNextToken();

	while (currentToken.type != TOKEN_RBRACE)
	{
		if (!parser_decide())
		{
			puts("Error in statement inside block");
			return false;
		}
	}

	printf("Finished block: \t\t%s\n", currentToken.value);
	// getNextToken();
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
			if (!parse_expression())
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
	return true;
}

// parser decide where to go
bool parser_decide()
{
	printf("Deciding on token: \t%s\n", currentToken.value);
	switch (currentToken.type)
	{
	case TOKEN_VAR:
	case TOKEN_CONST:
		return parse_variable_definition();
	case TOKEN_PUB:
		return parse_function_definition();
	case TOKEN_IDENTIFIER:
		getLookaheadToken();
		if (lookaheadToken.type == TOKEN_ASSIGN)
			return parse_assignment();
		else
			return parse_function_call();
	case TOKEN_IF:
		return parse_if_statement();
	case TOKEN_WHILE:
		return parse_while_statement();
	case TOKEN_RETURN:
		return parse_return_statement();
	default:
		printf("Syntax error: unexpected token %s\n", currentToken.value);
		return false;
	}

	return true;
}
