#include "syntaxical/parser.h"

static int tokenIndex = 0;
static LinkedList *buffer = NULL;
static SymTable *table = NULL;

TOKEN_PTR currentToken()
{
	if (tokenIndex >= getSize(buffer))
		return NULL;
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

TOKEN_PTR getNextToken()
{
	tokenIndex++;
	if (tokenIndex >= getSize(buffer))
		return NULL;
	return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

enum ERR_CODES parser_init()
{
	puts("parser_init");

	table = symTableInit();
	buffer = initLinkedList(false);

	return parser_parse();
}

enum ERR_CODES parser_parse()
{
	puts("parser_parse");

	enum ERR_CODES err = firstPass(table, stdin, buffer);
	if (err != SUCCESS)
	{
		puts("Error in first pass");
		return err;
	}

	puts("parser_second_pass");

	printf("Current token: %s\n", currentToken()->value);

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

	printf("Matched token: \t%s\n", token->value);
	getNextToken();
	return true;
}

bool parse_program()
{
	printf("Parsing <program>\n");
	if (!parse_prolog())
		return false;
	if (!parse_functions())
		return false;

	if (!currentToken())
	{
		puts("Dosly tokeny, chybi EOF token");
		return false;
	}
	if (!match(TOKEN_EOF))
		return false;

	printf("Successfully parsed <program>\n");
	return true;
}

bool parse_prolog()
{
	printf("Parsing <prolog>\n");
	// if (!match(TOKEN_CONST)) return false;
	// if (!match(TOKEN_IFJ)) return false;
	// if (!match(TOKEN_ASSIGN)) return false;
	// if (!match(TOKEN_AT)) return false;
	// if (!match(TOKEN_IMPORT)) return false;
	// if (!match(TOKEN_LPAR)) return false;
	// if (!match(TOKEN_STRING)) return false;
	// if (!match(TOKEN_RPAR)) return false;
	// if (!match(TOKEN_SEMICOLON)) return false;
	printf("Successfully parsed <prolog>\n");
	return true;
}

bool parse_functions()
{
	printf("Parsing <functions>\n");

	if (!parse_function())
		return false;

	while (parse_next_function())
	{
		if (!parse_function())
			break;
	}

	printf("Successfully parsed <functions>\n");
	return true;
}
bool parse_function()
{
	printf("Parsing <function>\n");

	if (!match(TOKEN_PUB))
		return false;

	if (!match(TOKEN_FN))
		return false;

	if (currentToken()->type != TOKEN_IDENTIFIER)
		return false;
	printf("Function name: %s\n", currentToken()->value);
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

	printf("Successfully parsed <function>\n");
	return true;
}

bool parse_next_function()
{
	printf("Parsing <next_function>\n");

	if (!currentToken() || currentToken()->type == TOKEN_EOF)
	{
		printf("End of tokens reached.\n");
		return false;
	}

	if (currentToken()->type == TOKEN_PUB)
		return true;

	printf("Successfully parsed <next_function> (empty)\n");
	return false;
}

bool parse_params()
{
	printf("Parsing <params>\n");

	// Handle empty params case
	if (currentToken()->type == TOKEN_RPAR)
	{
		printf("Successfully parsed <params> (empty)\n");
		return true;
	}

	if (!parse_parameter())
	{
		return false;
	}

	printf("Successfully parsed <params>\n");
	return true;
}

bool parse_parameter()
{
	printf("Parsing <parameter>\n");

	// Handle empty parameter case
	if (currentToken()->type != TOKEN_IDENTIFIER)
	{
		printf("Successfully parsed <parameter> (empty)\n");
		return true;
	}

	printf("Parameter name: %s\n", currentToken()->value);
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

	printf("Successfully parsed <parameter>\n");
	return true;
}

bool parse_parameter_next()
{
	printf("Parsing <parameter_next>\n");

	if (currentToken()->type != TOKEN_COMMA)
	{
		printf("Successfully parsed <parameter_next> (empty)\n");
		return true;
	}

	getNextToken(); // consume comma
	if (!parse_parameter())
	{
		return false;
	}

	printf("Successfully parsed <parameter_next>\n");
	return true;
}

bool parse_data_type()
{
	printf("Parsing <data_type>\n");

	if (currentToken()->type == TOKEN_QUESTION_MARK) // TODO: lexical
		getNextToken();

	if (currentToken()->type != TOKEN_I32 && currentToken()->type != TOKEN_F64 && currentToken()->type != TOKEN_U8 && currentToken()->type != TOKEN_VOID)
	{
		printf("Expected data type but got: %s\n", currentToken()->value);
		return false;
	}

	printf("Data type: %s\n", currentToken()->value);
	getNextToken();

	printf("Successfully parsed <data_type>\n");
	return true;
}

bool parse_func_body()
{
	printf("Parsing <func_body>\n");

	printf("Successfully parsed <func_body>\n");
	return true;
}