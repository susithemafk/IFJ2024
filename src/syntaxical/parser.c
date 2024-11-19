#include "syntaxical/parser.h"

static int tokenIndex = 0;
static LinkedList *buffer = NULL;
static SymTable *table = NULL;

TOKEN_PTR currentToken(void)
{
	if ((unsigned int)tokenIndex >= getSize(buffer))
		return NULL;
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

TOKEN_PTR getNextToken(void)
{
	tokenIndex++;
	if ((unsigned int)tokenIndex >= getSize(buffer))
		return NULL;
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

enum ERR_CODES parser_init(void)
{
	puts("parser_init");

	table = symTableInit();
	buffer = initLinkedList(false);

	return parser_parse();
}

enum ERR_CODES parser_parse(void)
{
	puts("parser_parse");

	enum ERR_CODES err = firstPass(table, stdin, buffer);
	if (err != SUCCESS)
	{
		puts("Error in first pass");
		return err;
	}

	puts("parser_second_pass");

#ifdef DEBUG
	printf("Current token: %s\n", currentToken()->value);
#endif

	if (!parse_program())
	{
		puts("Error in second pass");
		return E_SYNTAX;
	}

	freeBuffer(&buffer);
	symTableFree(&table);

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
// if (!match(TOKEN_CONST)) return false;
// if (!match(TOKEN_IFJ)) return false;
// if (!match(TOKEN_ASSIGN)) return false;
// if (!match(TOKEN_AT)) return false;
// if (!match(TOKEN_IMPORT)) return false;
// if (!match(TOKEN_LPAR)) return false;
// if (!match(TOKEN_STRING)) return false;
// if (!match(TOKEN_RPAR)) return false;
// if (!match(TOKEN_SEMICOLON)) return false;
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

bool parse_params(void)
{
#ifdef DEBUG
	printf("Parsing <params>\n");
#endif

	// Handle empty params case
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

	// Handle empty parameter case
	if (currentToken()->type != TOKEN_IDENTIFIER)
	{
#ifdef DEBUG
		printf("Successfully parsed <parameter> (empty)\n");
#endif
		return true;
	}

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

	if (!parse_parameter_next())
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

#ifdef DEBUG
	printf("Successfully parsed <parameter_next>\n");
#endif
	return true;
}

bool parse_data_type(void)
{
#ifdef DEBUG
	printf("Parsing <data_type>\n");
#endif

	if (currentToken()->type == TOKEN_QUESTION_MARK) // TODO: lexical
		getNextToken();

	if (currentToken()->type != TOKEN_I32 && currentToken()->type != TOKEN_F64 && currentToken()->type != TOKEN_U8 && currentToken()->type != TOKEN_VOID)
	{
#ifdef DEBUG
		printf("Expected data type but got: %s\n", currentToken()->value);
#endif
		return false;
	}

#ifdef DEBUG
	printf("Data type: %s\n", currentToken()->value);
#endif
	getNextToken();

#ifdef DEBUG
	printf("Successfully parsed <data_type>\n");
#endif
	return true;
}

bool parse_func_body(void)
{
#ifdef DEBUG
	printf("Parsing <func_body>\n");
#endif

#ifdef DEBUG
	printf("Successfully parsed <func_body>\n");
#endif
	return true;
}
