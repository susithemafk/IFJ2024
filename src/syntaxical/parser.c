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

/**
 * Intro functions
 */
enum ERR_CODES parser_init()
{
	currentToken.type = TOKEN_NONE;
	currentToken.value = NULL;

	return SUCCESS;
}

void getNextToken()
{
	status = scanner_get_token(&currentToken);
}

enum ERR_CODES parser_parse()
{
	// load first token
	getNextToken();

	while (currentToken.type != TOKEN_EOF)
	{
		if (!parser_parse_statement())
		{
			return E_SYNTAX;
		}
		getNextToken();
	}

	return SUCCESS;
}

/**
 * Helper functions
 */
bool parse_end_with_semicolon()
{
	if (currentToken.type != TOKEN_SEMICOLON)
	{
		puts("Expected semicolon");
		return false;
	}
	return true;
}

/**
 * Variable definition
 */

// <variable_definition> -> <modifier> <identifier> <optional_data_type> <assignment> <expression> ;
bool parse_variable_definition()
{
	if (!parse_modifier())
		return false;
	if (!parse_identifier())
		return false;
	if (!parse_optional_data_type())
		return false;
	if (!parse_assignment())
		return false;
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

// <assignment> -> =
bool parse_assignment()
{
	if (currentToken.type == TOKEN_ASSIGN)
	{
		printf("Parsed assignment: \t%s\n", currentToken.value);
		getNextToken();
		return true;
	}
	puts("Expected '=' for assignment");
	return false;
}

// <expression> -> EXPR
bool parse_expression()
{
	printf("Parsed expression: \t%s\n", currentToken.value);
	getNextToken();
	return true;
}

bool parser_parse_statement()
{
	switch (currentToken.type)
	{
	case TOKEN_VAR:
		return parse_variable_definition();
		break;
	case TOKEN_CONST:
		return parse_variable_definition();
		break;
	case TOKEN_NONE:
		return true;
		break;
	case TOKEN_IDENTIFIER:
		return true;
		break;
	case TOKEN_EOF:
		return true;
		break;
	case TOKEN_PLUS:
		return true;
		break;
	case TOKEN_MINUS:
		return true;
		break;
	case TOKEN_DIVIDE:
		return true;
		break;
	case TOKEN_MULTIPLY:
		return true;
		break;
	case TOKEN_EQUALS:
		return true;
		break;
	case TOKEN_LESSTHAN:
		return true;
		break;
	case TOKEN_LESSOREQUAL:
		return true;
		break;
	case TOKEN_GREATERTHAN:
		return true;
		break;
	case TOKEN_GREATEROREQUAL:
		return true;
		break;
	case TOKEN_NOTEQUAL:
		return true;
		break;
	case TOKEN_STRING:
		return true;
		break;
	case TOKEN_CONCATENATE:
		return true;
		break;
	case TOKEN_ASSIGN:
		return true;
		break;
	case TOKEN_COLON:
		return true;
		break;
	case TOKEN_COMMA:
		return true;
		break;
	case TOKEN_LPAR:
		return true;
		break;
	case TOKEN_RPAR:
		return true;
		break;
	case TOKEN_LBRACE:
		return true;
		break;
	case TOKEN_RBRACE:
		return true;
		break;
	case TOKEN_SEMICOLON:
		return true;
		break;
	case TOKEN_ELSE:
		return true;
		break;
	case TOKEN_FN:
		return true;
		break;
	case TOKEN_IF:
		return true;
		break;
	case TOKEN_I32:
		return true;
		break;
	case TOKEN_F64:
		return true;
		break;
	case TOKEN_NULL:
		return true;
		break;
	case TOKEN_PUB:
		return true;
		break;
	case TOKEN_RETURN:
		return true;
		break;
	case TOKEN_U8:
		return true;
		break;
	case TOKEN_VOID:
		return true;
		break;
	case TOKEN_WHILE:
		return true;
		break;
	}

	return true;
}