/** AUTHOR
 *
 * @author  <xsucha18> Marek Sucharda
 * @file parser.c
 * @date 16.11.2024
 * @brief main src file parsing
 */

#include "syntaxical/parser.h"
#include "syntaxical/precident.h"

static int tokenIndex = 0;
static LinkedList *buffer = NULL;
static SymTable *table = NULL; // TODO: vhodit do mainu table, pak parser, pak v mainu free
static LinkedList *astList = NULL;
static enum ERR_CODES err;

// vnoreny funkce resi first pass
// udelat parser dle good_asts.c
// vracet error kody

TOKEN_PTR currentToken(void)
{
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

TOKEN_PTR getNextToken(void)
{
	tokenIndex++;
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

enum ERR_CODES parser_init(FILE *input)
{
#ifdef DEBUG
	puts("parser_init");
#endif

	table = symTableInit();
	buffer = initLinkedList(false);
	astList = initLinkedList(false);

	if (!table || !buffer || !astList)
	{
#ifdef DEBUG
		puts("Error in parser init");
#endif
		return E_INTERNAL;
	}

	return parser_parse(input);
}

enum ERR_CODES parser_parse(FILE *input)
{
#ifdef DEBUG
	puts("parser_parse");
#endif

	enum ERR_CODES err = firstPass(table, input, buffer);
	if (err != SUCCESS)
	{
#ifdef DEBUG
		puts("Error in first pass");
#endif
		return err;
	}

#ifdef DEBUG
	puts("parser_second_pass");
#endif

#ifdef DEBUG
	printf("Current token: %s\n", currentToken()->value);
#endif

	if (!parse_program())
	{
#ifdef DEBUG
		puts("Error in second pass");
#endif
		return E_SYNTAX;
	}

	freeBuffer(&buffer);
	symTableFree(&table);

// #ifdef DEBUG
	printf("Successfully parsed program, error code: %d\n", err);
// #endif
	return SUCCESS;
}

bool match(enum TOKEN_TYPE tokenType)
{
	TOKEN_PTR token = currentToken();

	if (!token || token->type != tokenType)
	{
#ifdef DEBUG
		DEBUG_MSG("Expected token type: ");
		printTokenType(tokenType);
		DEBUG_MSG(" but got: ");
		if (token)
		{
			printTokenType(currentToken()->type);
		}
		else
		{
			DEBUG_MSG("No token");
		}
#endif
		return false;
	}

#ifdef DEBUG
	printf("Matched token: \t%s\n", token->value);
#endif
	getNextToken();
	return true;
}

bool is_truth_expr(void)
{
	TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

	if (nextToken->type == TOKEN_RPAR)
	{
		return false;
	}

	return true;
}

bool parse_program(void)
{
#ifdef DEBUG
	printf("Parsing <program>\n");
#endif
	if (!parse_prolog())
		return false;
	if (!parse_functions())
		return false;
	if (!match(TOKEN_EOF))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <program>\n");
#endif
	return true;
}

bool parse_prolog(void)
{
#ifdef DEBUG
	printf("Parsing <prolog>\n");
#endif
	if (!match(TOKEN_CONST))
		return false;
	if (!match(TOKEN_IFJ))
		return false;
	if (!match(TOKEN_ASSIGN))
		return false;
	if (!match(TOKEN_AT))
		return false;
	if (!match(TOKEN_IMPORT))
		return false;
	if (!match(TOKEN_LPAR))
		return false;
	if (!match(TOKEN_STRING))
		return false;
	if (!match(TOKEN_RPAR))
		return false;
	if (!match(TOKEN_SEMICOLON))
		return false;
#ifdef DEBUG
	printf("Successfully parsed <prolog>\n");
#endif
	return true;
}

bool parse_functions(void)
{
#ifdef DEBUG
	printf("Parsing <functions>\n");
#endif

	if (!parse_function())
		return false;

	while (parse_next_function())
	{
		if (!parse_function())
			break;
	}

#ifdef DEBUG
	printf("Successfully parsed <functions>\n");
#endif
	return true;
}

bool parse_function(void)
{
#ifdef DEBUG
	printf("Parsing <function>\n");
#endif

	if (!match(TOKEN_PUB))
		return false;

	if (!match(TOKEN_FN))
		return false;

	if (currentToken()->type != TOKEN_IDENTIFIER)
		return false;
#ifdef DEBUG
	printf("Function name: %s\n", currentToken()->value);
#endif
	getNextToken();

	if (!match(TOKEN_LPAR))
		return false;

	if (!parse_params())
		return false;

	if (!match(TOKEN_RPAR))
		return false;

	if (!parse_data_type())
		return false;

	if (!match(TOKEN_LBRACE))
		return false;

	if (!parse_func_body())
		return false;

	if (!match(TOKEN_RBRACE))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <function>\n");
#endif
	return true;
}

bool parse_next_function(void)
{
#ifdef DEBUG
	printf("Parsing <next_function>\n");
#endif

	if (currentToken()->type == TOKEN_EOF)
	{
#ifdef DEBUG
		printf("End of tokens reached, end the program.\n");
#endif
		return false;
	}

	if (currentToken()->type == TOKEN_PUB)
		return true;

#ifdef DEBUG
	printf("Successfully parsed <next_function> (empty)\n");
#endif
	return false;
}

bool parse_params()
{
#ifdef DEBUG
	printf("Parsing <params>\n");
#endif

	// empty params
	if (currentToken()->type == TOKEN_RPAR)
	{
#ifdef DEBUG
		printf("Successfully parsed <params> (empty)\n");
#endif
		return true;
	}

	if (!parse_parameter())
	{
		return false;
	}

	if (!parse_parameter_next())
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <params>\n");
#endif
	return true;
}

bool parse_parameter(void)
{
#ifdef DEBUG
	printf("Parsing <parameter>\n");
#endif

	// empty parameter
	if (currentToken()->type == TOKEN_RPAR)
	{
#ifdef DEBUG
		printf("Successfully parsed <parameter> (empty)\n");
#endif
		return true;
	}

	// question mark
	if (currentToken()->type == TOKEN_QUESTION_MARK)
	{
		getNextToken();
	}

	if (currentToken()->type == TOKEN_IDENTIFIER)
	{
#ifdef DEBUG
		printf("Parameter name: %s\n", currentToken()->value);
#endif
		getNextToken(); // consume identifier

		if (!match(TOKEN_COLON))
		{
			return false;
		}

		if (!parse_data_type())
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <parameter>\n");
#endif
	return true;
}

bool parse_parameter_next(void)
{
#ifdef DEBUG
	printf("Parsing <parameter_next>\n");
#endif

	if (currentToken()->type != TOKEN_COMMA)
	{
#ifdef DEBUG
		printf("Successfully parsed <parameter_next> (empty)\n");
#endif
		return true;
	}

	getNextToken(); // consume comma
	if (!parse_parameter())
	{
		return false;
	}

	if (!parse_parameter_next())
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <parameter_next>\n");
#endif
	return true;
}

// Tam může jít i32 f64 []u8 nebo identifier nebo taky nic nebo string
bool parse_arguments(void)
{
#ifdef DEBUG
	printf("Parsing <arguments>\n");
#endif

	// empty arguments
	if (currentToken()->type == TOKEN_RPAR)
	{
#ifdef DEBUG
		printf("Successfully parsed <arguments> (empty)\n");
#endif
		return true;
	}

	if (!parse_argument())
	{
		return false;
	}

	if (!parse_argument_next())
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <arguments>\n");
#endif
	return true;
}

bool parse_argument(void)
{
#ifdef DEBUG
	printf("Parsing <argument>\n");
#endif

	// empty argument
	if (currentToken()->type == TOKEN_RPAR)
	{
#ifdef DEBUG
		printf("Successfully parsed <argument> (empty)\n");
#endif
		return true;
	}

	if (currentToken()->type == TOKEN_I32 || currentToken()->type == TOKEN_F64 || currentToken()->type == TOKEN_U8 || TOKEN_IDENTIFIER || TOKEN_STRING)
	{
#ifdef DEBUG
		printf("Argument: %s\n", currentToken()->value);
#endif
		getNextToken();
		return true;
	}
	else
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <argument>\n");
#endif
	return true;
}

bool parse_argument_next(void)
{
#ifdef DEBUG
	printf("Parsing <argument_next>\n");
#endif

	if (currentToken()->type != TOKEN_COMMA)
	{
#ifdef DEBUG
		printf("Successfully parsed <argument_next> (empty)\n");
#endif
		return true;
	}

	getNextToken(); // consume comma
	if (!parse_argument())
	{
		return false;
	}

	if (!parse_argument_next())
	{
		return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <argument_next>\n");
#endif
	return true;
}

bool parse_data_type(void)
{
#ifdef DEBUG
	printf("Parsing <data_type>\n");
#endif

	// bool isNullable = false;
	// enum DATA_TYPES dataType = dTypeUndefined;

	if (currentToken()->type == TOKEN_QUESTION_MARK)
	{
		// isNullable = true;
		getNextToken();
	}

	// switch (currentToken()->type)
	// {
	// case TOKEN_I32:
	// 	dataType = dTypeI32;
	// 	break;
	// case TOKEN_F64:
	// 	dataType = dTypeF64;
	// 	break;
	// case TOKEN_U8:
	// 	dataType = dTypeU8;
	// 	break;
	// case TOKEN_VOID:
	// 	dataType = dTypeVoid;
	// 	break;
	// default:
	// 	printf("Expected data type but got: %s\n", currentToken()->value);
	// 	printf("Data type: %d\n", dataType);
	// 	return false;
	// }

	getNextToken();

// #ifdef DEBUG
	// printf("Successfully parsed <data_type>: %d\n", dataType);
// #endif
	return true;
}

bool parse_func_body(void)
{
#ifdef DEBUG
	printf("Parsing <func_body>\n");
#endif
	if (!parse_body())
		return false;

#ifdef DEBUG
	printf("Successfully parsed <func_body>\n");
#endif
	return true;
}

bool parse_body(void)
{
#ifdef DEBUG
	printf("Parsing <body>\n");
#endif

	// empty body
	if (currentToken()->type == TOKEN_RBRACE)
	{
#ifdef DEBUG
		printf("Successfully parsed <body> (empty)\n");
#endif
		return true;
	}

	if (!parse_body_content())
		return false;
	if (!parse_body_content_next())
		return false;

#ifdef DEBUG
	printf("Successfully parsed <body>\n");
#endif
	return true;
}

bool parse_body_content(void)
{
#ifdef DEBUG
	printf("Parsing <body_content>\n");
#endif

	TOKEN_PTR token = currentToken();
	TOKEN_PTR nextToken;

#ifdef DEBUG
	printf("Deciding on token: \t%s\n", token->value);
#endif

	switch (token->type)
	{
	case TOKEN_CONST:
	case TOKEN_VAR:
		return parse_var_def();
	case TOKEN_IF:
		return parse_if();
	case TOKEN_WHILE:
		return parse_while();
	case TOKEN_RETURN:
		return parse_ret_value();
	case TOKEN_IFJ:
		return parse_native_func_call();
	case TOKEN_IDENTIFIER:
		token = currentToken();
		nextToken = getDataAtIndex(buffer, tokenIndex + 1);

		if (nextToken->type == TOKEN_LPAR)
		{
			return parse_func_call();
		}
		else if (nextToken->type == TOKEN_ASSIGN)
		{
			return parse_var_assign();
		}
		else
			return false;

	case TOKEN_UNDERSCORE:
		return parse_var_assign();

	default:
		printf("Unexpected token %s\n, expected: const, var, if, while, return, identifier, ifj, _", token->value);
		return false;
	}

	return true;
}

bool parse_body_content_next(void)
{
#ifdef DEBUG
	printf("Parsing <body_content_next>\n");
#endif

	// Check if we have more body content
	TOKEN_PTR token = currentToken();
	if (token->type == TOKEN_RBRACE)
	{
#ifdef DEBUG
		printf("End of body content reached\n");
#endif
		return true;
	}

	if (!parse_body_content())
		return false;
	if (!parse_body_content_next())
		return false;

#ifdef DEBUG
	printf("Successfully parsed <body_content_next>\n");
#endif
	return true;
}

bool parse_var_def(void)
{
#ifdef DEBUG
	printf("Parsing <var_def>\n");
#endif

	if (currentToken()->type != TOKEN_CONST && currentToken()->type != TOKEN_VAR)
		return false;
	getNextToken();

	if (!match(TOKEN_IDENTIFIER))
		return false;

	if (currentToken()->type == TOKEN_COLON)
	{
		getNextToken();
		if (!parse_data_type())
			return false;
	}

	if (!match(TOKEN_ASSIGN))
		return false;

	// <var_def> -> const <var_id> : <data_type> = <no_truth_expr>;
	// <var_def> -> const <var_id> : <data_type> = <func_call>;
	TOKEN_PTR token = currentToken();
	TOKEN_PTR nextToken = getNextToken();
	if (token->type == TOKEN_IDENTIFIER && nextToken->type == TOKEN_SEMICOLON)
	{
		tokenIndex--;
		if (!parse_var_assign())
		{
			return false;
		}
	}
	else if (token->type == TOKEN_STRING && nextToken->type == TOKEN_SEMICOLON)
	{
		tokenIndex--;
		if (!parse_var_assign())
		{
			return false;
		}
	}
	else if (token->type == TOKEN_IDENTIFIER && nextToken->type == TOKEN_LPAR)
	{
		tokenIndex--;
		if (!parse_func_call())
		{
			return false;
		}
		return true;
	}
	else if (token->type == TOKEN_IFJ && nextToken->type == TOKEN_CONCATENATE)
	{
		tokenIndex--;
		if (!parse_native_func_call())
		{
			return false;
		}
		return true;
	}
	else
	{
		tokenIndex--;
		if (!parse_no_truth_expr())
		{
			return false;
		}
	}

	if (!match(TOKEN_SEMICOLON))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <var_def>\n");
#endif
	return true;
}

bool parse_if(void)
{
#ifdef DEBUG
	printf("Parsing <if>\n");
#endif

	if (!match(TOKEN_IF))
		return false;
	if (!match(TOKEN_LPAR))
		return false;
	if (is_truth_expr())
	{
		if (!parse_truth_expr())
			return false;
		if (!match(TOKEN_RPAR))
			return false;
	}
	else
	{
		if (!match(TOKEN_IDENTIFIER))
			return false;
		if (!match(TOKEN_RPAR))
			return false;
		if (!match(TOKEN_VBAR))
			return false;
		if (!match(TOKEN_IDENTIFIER))
			return false;
		if (!match(TOKEN_VBAR))
			return false;
	}

	if (!match(TOKEN_LBRACE))
		return false;
	if (!parse_body())
		return false;
	if (!match(TOKEN_RBRACE))
		return false;
	if (!parse_else())
		return false;

#ifdef DEBUG
	printf("Successfully parsed <if>\n");
#endif
	return true;
}

bool parse_else(void)
{
#ifdef DEBUG
	printf("Parsing <else>\n");
#endif

	if (currentToken()->type != TOKEN_ELSE)
	{
#ifdef DEBUG
		printf("Expected 'else' but got: %s\n", currentToken()->value);
#endif
		return false;
	}

	getNextToken();
	if (!match(TOKEN_LBRACE))
		return false;
	if (!parse_body())
		return false;
	if (!match(TOKEN_RBRACE))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <else>\n");
#endif
	return true;
}

bool parse_while(void)
{
#ifdef DEBUG
	printf("Parsing <while>\n");
#endif

	if (!match(TOKEN_WHILE))
		return false;
	if (!match(TOKEN_LPAR))
		return false;
	if (is_truth_expr())
	{
		if (!parse_truth_expr())
			return false;
		if (!match(TOKEN_RPAR))
			return false;
	}
	else
	{
		if (!match(TOKEN_IDENTIFIER))
			return false;
		if (!match(TOKEN_RPAR))
			return false;
		if (!match(TOKEN_VBAR))
			return false;
		if (!match(TOKEN_IDENTIFIER))
			return false;
		if (!match(TOKEN_VBAR))
			return false;
	}

	if (!match(TOKEN_LBRACE))
		return false;
	if (!parse_body())
		return false;
	if (!match(TOKEN_RBRACE))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <while>\n");
#endif
	return true;
}

bool parse_ret_value(void)
{
#ifdef DEBUG
	printf("Parsing <ret_value>\n");
#endif
	getNextToken();

	// empty return
	if (currentToken()->type == TOKEN_SEMICOLON)
	{
#ifdef DEBUG
		printf("Empty return value\n");
#endif
		getNextToken();
		return true;
	}

	if (!parse_no_truth_expr())
		return false;

	if (!match(TOKEN_SEMICOLON))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <ret_value>\n");
#endif
	return true;
}

bool parse_func_call(void)
{
#ifdef DEBUG
	printf("Parsing <func_call>\n");
#endif

	if (currentToken()->type == TOKEN_IFJ)
	{
		if (!parse_native_func_call())
			return false;
	}
	else
	{
		if (!parse_user_func_call())
			return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <func_call>\n");
#endif
	return true;
}

bool parse_native_func_call(void)
{
#ifdef DEBUG
	printf("Parsing <native_func_call>\n");
#endif

	if (!match(TOKEN_IFJ))
		return false;
	if (!match(TOKEN_CONCATENATE))
		return false;
	if (!match(TOKEN_IDENTIFIER))
		return false;
	if (!match(TOKEN_LPAR))
		return false;
	if (!parse_arguments())
		return false;
	if (!match(TOKEN_RPAR))
		return false;
	if (!match(TOKEN_SEMICOLON))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <native_func_call>\n");
#endif
	return true;
}

bool parse_user_func_call(void)
{
#ifdef DEBUG
	printf("Parsing <user_func_call>\n");
#endif

	if (!match(TOKEN_IDENTIFIER))
		return false;
	if (!match(TOKEN_LPAR))
		return false;
	if (!parse_arguments())
		return false;
	if (!match(TOKEN_RPAR))
		return false;
	if (!match(TOKEN_SEMICOLON))
		return false;

#ifdef DEBUG
	printf("Successfully parsed <user_func_call>\n");
#endif
	return true;
}

bool parse_var_assign(void)
{
#ifdef DEBUG
	printf("Parsing <var_assign>\n");
#endif

	getNextToken();

	if (!match(TOKEN_ASSIGN))
		return false;

	TOKEN_PTR token = currentToken();
	TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

	// todo asi
	// if (token->type == TOKEN_IFJ && nextToken->type == TOKEN_CONCATENATE)
	// {
	// 	if (!parse_native_func_call())
	// 		return false;
	// }

	if (token->type == TOKEN_IDENTIFIER && nextToken->type == TOKEN_LPAR)
	{
		if (!parse_func_call())
			return false;
	}
	else
	{
		if (!parse_no_truth_expr())
			return false;
		if (!match(TOKEN_SEMICOLON))
			return false;
	}

#ifdef DEBUG
	printf("Successfully parsed <var_assign>\n");
#endif
	return true;
}

bool parse_no_truth_expr(void)
{
#ifdef DEBUG
	printf("Parsing <no_truth_expr>\n");
#endif

	TOKEN_PTR token = currentToken();
	TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

	// pokud jde o literal a za nim je strednik, tak si ho zparsuju sam
	if ((token->type == TOKEN_STRING || token->type == TOKEN_I32 || token->type == TOKEN_F64 || token->type == TOKEN_U8) && nextToken->type == TOKEN_SEMICOLON)
	{
		getNextToken();
		return true;
	}

	err = startPrecedentAnalysis(buffer, (unsigned int *)&tokenIndex, true);
	if (err != SUCCESS)
	{
		return false;
	}

	return true;
}

bool parse_truth_expr(void)
{
	printf("Parsing <truth_expr>\n");

	err = startPrecedentAnalysis(buffer, (unsigned int *)&tokenIndex, false);
	if (err != SUCCESS)
	{
		return false;
	}

	return true;
}
