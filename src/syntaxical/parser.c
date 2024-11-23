#include "syntaxical/parser.h"
#include "syntaxical/precident.h"
#include "utility/my_utils.h"


static unsigned int tokenIndex = 0;
static LinkedList *buffer = NULL;
static SymTable *table = NULL; // TODO: vhodit do mainu table, pak parser, pak v mainu free
static enum ERR_CODES err;

// vnoreny funkce resi first pass
// udelat parser dle good_asts.c
// todo return bez leve zavorky ve funkci

TOKEN_PTR currentToken(void) {
    return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

TOKEN_PTR getNextToken(void) {
    tokenIndex++;
    return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

enum ERR_CODES parser_init(SymTable *tbl) {
    buffer = initLinkedList(false);
	table = tbl;

    if (!buffer) {
		freeBuffer(&buffer);
		#ifdef DEBUG
        puts("Error in parser init");
		#endif
        return E_INTERNAL;
    }

    return SUCCESS;
}

void parser_cleanup(void) {
    if (buffer) {
        freeBuffer(&buffer);
	}

	if (table) {
		symTableFree(&table);
	}
}

enum ERR_CODES parser_parse(FILE *input, struct Program *program) {
#ifdef DEBUG
    puts("parser_parse");
#endif

    enum ERR_CODES err = firstPass(table, input, buffer);
    if (err != SUCCESS) {
		#ifdef DEBUG
        puts("Error in first pass");
        printf("Error code: %d\n", err);
		#endif
        return err;
    }

#ifdef DEBUG
    puts("parser_second_pass");
    printf("Current token: %s\n", currentToken()->value);
#endif

    if (!parse_program(program)) {
        puts("Error in second pass");
        return E_SYNTAX;
    }

    return SUCCESS;
}

bool match(enum TOKEN_TYPE tokenType) {
    TOKEN_PTR token = currentToken();

    if (!token || token->type != tokenType) {
#ifdef DEBUG
        DEBUG_MSG("Expected token type: ");
        printTokenType(tokenType);
        DEBUG_MSG(" but got: ");
        if (token) {
            printTokenType(currentToken()->type);
        } else {
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

bool parse_program(struct Program *program) {
#ifdef DEBUG
    printf("Parsing <program>\n");
#endif

    program->functions = initLinkedList(true);
    if (!program->functions)
        return false;

    if (!parse_prolog())
        return false;
    if (!parse_functions(program->functions))
        return false;
    if (!match(TOKEN_EOF))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <program>\n");
#endif
    return true;
}

bool parse_prolog(void) {
#ifdef DEBUG
    printf("Parsing <prolog>\n");
#endif
    if (!match(TOKEN_CONST))
        return false;
    if (!match(TOKEN_IDENTIFIER))
        return false;
    if (!match(TOKEN_ASSIGN))
        return false;
    if (!match(TOKEN_AT))
        return false;
    if (!match(TOKEN_IDENTIFIER))
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

bool parse_functions(LinkedList *functions) {
#ifdef DEBUG
    printf("Parsing <functions>\n");
#endif

    while (parse_next_function()) {

        Function *function = malloc(sizeof(struct Function));
        if (!function) {
            return false;
        }

        memset(function, 0, sizeof(struct Function));
        function->params = initLinkedList(true);
        if (!function->params)
            return false;

        if (!insertNodeAtIndex(functions, (void *)function, -1))
            return false;

        if (!parse_function(function))
            return false;
    }

#ifdef DEBUG
    printf("Successfully parsed <functions>\n");
#endif
    return true;
}

bool parse_function(struct Function *function) {
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

    function->id.name = copyString(currentToken()->value);
    if (!function->id.name)
        return false;

    getNextToken();

    if (!match(TOKEN_LPAR))
        return false;

    if (!parse_params(function->params))
        return false;

    if (!match(TOKEN_RPAR))
        return false;

    if (!parse_data_type(&function->returnType))
        return false;

    if (!match(TOKEN_LBRACE))
        return false;

    if (!parse_func_body(&function->body))
        return false;

    if (!match(TOKEN_RBRACE))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <function>\n");
#endif
    return true;
}

bool parse_next_function(void) {
#ifdef DEBUG
    printf("Parsing <next_function>\n");
#endif

    if (currentToken()->type == TOKEN_EOF) {
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

bool parse_params(LinkedList *params) {
#ifdef DEBUG
    printf("Parsing <params>\n");
#endif

    // Handle empty params case
    if (currentToken()->type == TOKEN_RPAR) {
#ifdef DEBUG
        printf("Successfully parsed <params> (empty)\n");
#endif
        return true;
    }

    bool param_enabled = true;

    while (param_enabled) {
        Param *param = malloc(sizeof(Param));
        if (!param) {
            return false;
        }

        memset(param, 0, sizeof(Param));

        if (!insertNodeAtIndex(params, (void *)param, -1)) {
            return false;
        }

        if (!parse_parameter(param)) {
            return false;
        }

        if (currentToken()->type == TOKEN_COMMA) {
            getNextToken();
        } else {
            param_enabled = false;
        }
    }

#ifdef DEBUG
    printf("Successfully parsed <params>\n");
#endif
    return true;
}

bool parse_parameter(Param *param) {
#ifdef DEBUG
    printf("Parsing <parameter>\n");
#endif

    // Handle empty parameter case
    if (currentToken()->type != TOKEN_IDENTIFIER) {
#ifdef DEBUG
        printf("Successfully parsed <parameter> (empty)\n");
#endif
        return true;
    }

#ifdef DEBUG
    printf("Parameter name: %s\n", currentToken()->value);
#endif
    param->id.name = copyString(currentToken()->value);
    if (!param->id.name)
        return false;

    getNextToken(); // consume identifier

    if (!match(TOKEN_COLON)) // todo nepovinny, nejspis staci jen vnorit parse_data_type nez hned returnovat false
    {
        return false;
    }

    if (!parse_data_type(&param->type)) {
        return false;
    }

#ifdef DEBUG
    printf("Successfully parsed <parameter>\n");
#endif
    return true;
}

// Tam může jít i32 f64 []u8 nebo identifier nebo taky nic nebo string
// TODO: pridat do LL gramatiky a podle ni pak upravit
bool parse_func_call_params(LinkedList *attributes) {
#ifdef DEBUG
    printf("Parsing <function_call_params>\n");
#endif

    if (currentToken()->type == TOKEN_RPAR) {
#ifdef DEBUG
        printf("Successfully parsed function parameters (empty)\n");
#endif
        return true;
    }

    bool param_enabled = true;

    while (param_enabled) {
        Expression *expr = malloc(sizeof(Expression));
        if (!expr) {
            return false;
        }

        memset(expr, 0, sizeof(Expression));

        if (!insertNodeAtIndex(attributes, (void *)expr, -1)) {
            return false;
        }

        if (!parse_func_call_param(expr)) {
            return false;
        }

        if (currentToken()->type == TOKEN_COMMA) {
            getNextToken();
        } else {
            param_enabled = false;
        }
    }

#ifdef DEBUG
    printf("Successfully parsed <function_call_params>\n");
#endif
    return true;
}

bool parse_func_call_param(Expression *expr) {
#ifdef DEBUG
    printf("Parsing <function_call_param>\n");
#endif

    // empty params
    if (currentToken()->type == TOKEN_RPAR) {
#ifdef DEBUG
        printf("Successfully parsed <function_call_params> (empty)\n");
#endif
        return true;
    }

    if (currentToken()->type == TOKEN_I32 || currentToken()->type == TOKEN_F64 || currentToken()->type == TOKEN_U8) {
#ifdef DEBUG
        printf("Data type: %s\n", currentToken()->value);
#endif

        expr->expr_type = LiteralExpressionType;
        expr->data.literal.value = copyString(currentToken()->value);
        if (!expr->data.literal.value) {
            return false;
        }

        switch (currentToken()->type) {
        case TOKEN_I32:
            expr->data_type.data_type = dTypeI32;
            break;
        case TOKEN_F64:
            expr->data_type.data_type = dTypeF64;
            break;
        case TOKEN_U8:
            expr->data_type.data_type = dTypeU8;
            break;
        default:
            break;
        }

        expr->data.literal.data_type = expr->data_type;

        getNextToken();
        return true;
    }

    if (currentToken()->type == TOKEN_IDENTIFIER) {

#ifdef DEBUG
        printf("Identifier: %s\n", currentToken()->value);
#endif

        expr->expr_type = IdentifierExpressionType;
        expr->data.identifier.name = copyString(currentToken()->value);
        if (!expr->data.identifier.name)
            return false;

        // TODO fill data_type

        getNextToken();
        return true;
    }

    if (currentToken()->type == TOKEN_STRING) {
#ifdef DEBUG
        printf("String: %s\n", currentToken()->value);
#endif

        expr->expr_type = LiteralExpressionType;
        expr->data.literal.value = copyString(currentToken()->value);
        if (!expr->data.literal.value)
            return false;

        expr->data_type.data_type = dTypeU8;
        expr->data.literal.data_type = expr->data_type;
        // TODO fill data_type

        getNextToken();
        return true;
    }

#ifdef DEBUG
    printf("Successfully parsed <function_call_param>\n");
#endif
    return false;
}

bool parse_data_type(DataType *data_type) {
#ifdef DEBUG
    printf("Parsing <data_type>\n");
#endif

    bool isNullable = false;
    enum DATA_TYPES dataType = dTypeNone;

    if (currentToken()->type == TOKEN_QUESTION_MARK) {
        isNullable = true;
        getNextToken();
    }

    switch (currentToken()->type) {
    case TOKEN_I32:
        dataType = dTypeI32;
        break;
    case TOKEN_F64:
        dataType = dTypeF64;
        break;
    case TOKEN_U8:
        dataType = dTypeU8;
        break;
    case TOKEN_VOID:
        dataType = dTypeVoid;
        break;
    default:
#ifdef DEBUG
        printf("Expected data type but got: %s\n", currentToken()->value);
#endif
        return false;
    }

    getNextToken();

    data_type->data_type = dataType;
    data_type->is_nullable = isNullable;

#ifdef DEBUG
    printf("Successfully parsed <data_type>: %d\n", dataType);
#endif
    return true;
}

bool parse_func_body(Body *body) {
#ifdef DEBUG
    printf("Parsing <func_body>\n");
#endif
    if (!parse_body(body))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <func_body>\n");
#endif
    return true;
}

bool parse_body(Body *body) {
#ifdef DEBUG
    printf("Parsing <body>\n");
#endif

    // empty body
    if (currentToken()->type == TOKEN_RBRACE) {
#ifdef DEBUG
        printf("Successfully parsed <body> (empty)\n");
#endif
        return true;
    }

    body->statements = initLinkedList(true);
    if (!body->statements)
        return false;

    while (parse_body_content_next()) {
        Statement *statement = malloc(sizeof(Statement));
        if (!statement) {
            return false;
        }

        memset(statement, 0, sizeof(Statement));

        if (!insertNodeAtIndex(body->statements, (void *)statement, -1)) {
            return false;
        }

        if (!parse_body_content(statement)) {
            return false;
        }
    }

#ifdef DEBUG
    printf("Successfully parsed <body>\n");
#endif
    return true;
}

bool parse_body_content(struct Statement *statement) {
#ifdef DEBUG
    printf("Parsing <body_content>\n");
#endif

    TOKEN_PTR token = currentToken();

    // printf("Deciding on token: \t%s\n", token->value);

    switch (token->type) {
    case TOKEN_CONST:
    case TOKEN_VAR:
        statement->type = VariableDefinitionStatementType;
        return parse_var_def(&statement->data.variable_definition_statement);
    case TOKEN_IF:
        statement->type = IfStatementType;
        return parse_if(&statement->data.if_statement);
    case TOKEN_WHILE:
        statement->type = WhileStatementType;
        return parse_while(&statement->data.while_statement);
    case TOKEN_RETURN:
        statement->type = ReturnStatementType;
        getNextToken(); // Consume 'return'
        if (!parse_ret_value(&statement->data.return_statement))
            return false;
        return match(TOKEN_SEMICOLON);

    case TOKEN_IDENTIFIER:
        // case TOKEN_IFJ: // TODO: lexical
        {
            TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);
            if (nextToken && nextToken->type == TOKEN_ASSIGN) {
                statement->type = AssigmentStatementType;
                return parse_var_assign(&statement->data.assigment_statement);
            } else {
                statement->type = FunctionCallStatementType;
                return parse_func_call_statement(&statement->data.function_call_statement);
            }
        }
    // case TOKEN_UNDERSCORE: // TODO: lexical
    // 	return parse_var_assign();
    default:
        printf("Syntax error: unexpected token %s\n, expected: const, var, if, while, return, identifier, ifj, _", token->value);
        return false;
    }

    return true;
}

bool parse_body_content_next(void) {
    switch (currentToken()->type) {
    case TOKEN_IDENTIFIER:
    case TOKEN_CONST:
    case TOKEN_IF:
    case TOKEN_RETURN:
    case TOKEN_VAR:
    case TOKEN_WHILE:
    case TOKEN_DELETE_VALUE:
        return true;
    default:
        return false;
    }
}

bool parse_var_def(VariableDefinitionStatement *variable_definition_statement) {
#ifdef DEBUG
    printf("Parsing <var_def>\n");
#endif

    // Check if it starts with 'const' or 'var'
    if (currentToken()->type != TOKEN_CONST && currentToken()->type != TOKEN_VAR)
        return false;

    if (currentToken()->type == TOKEN_CONST) {
        variable_definition_statement->isConst = true;
    } else {
        variable_definition_statement->isConst = false;
    }

    getNextToken();

    // Variable identifier
    if (currentToken()->type != TOKEN_IDENTIFIER)
        return false;

    variable_definition_statement->id.name = copyString(currentToken()->value);
    if (!variable_definition_statement->id.name)
        return false;

    getNextToken();

    if (currentToken()->type == TOKEN_COLON) {
        if (!match(TOKEN_COLON))
            return false;

        if (!parse_data_type(&variable_definition_statement->type))
            return false;
    } else {
        variable_definition_statement->type.data_type = dTypeNone;
        variable_definition_statement->type.is_nullable = false;
    }

    if (!match(TOKEN_ASSIGN))
        return false;

    // <var_def> -> const <var_id> : <data_type> = <no_truth_expr>;
    // <var_def> -> const <var_id> : <data_type> = <func_call>;
    if (!parse_no_truth_expr(&variable_definition_statement->value)) {
        return false;
    }

    if (!match(TOKEN_SEMICOLON))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <var_def>\n");
#endif
    return true;
}

bool parse_if(IfStatement *if_statement) {
#ifdef DEBUG
    printf("Parsing <if>\n");
#endif

    if (!match(TOKEN_IF))
        return false;
    if (!match(TOKEN_LPAR))
        return false;
    if (!parse_truth_expr(&if_statement->condition))
        return false;
    if (!match(TOKEN_RPAR))
        return false;

    // TODO
    /**
     * 1. pouze a tak musí být ještě |na|
     * 2. pokud tam je jen 5 nebo jinej literal tak error
     * 3.
     */

    if (currentToken()->type == TOKEN_PIPE) {
        getNextToken();
        if (currentToken()->type != TOKEN_IDENTIFIER) {
            return false;
        }

        if_statement->non_nullable.name = copyString(currentToken()->value);
        if (!if_statement->non_nullable.name)
            return false;
        getNextToken();

        if (!match(TOKEN_PIPE))
            return false;
    }

    if (!match(TOKEN_LBRACE))
        return false;
    if (!parse_body(&if_statement->if_body))
        return false;
    if (!match(TOKEN_RBRACE))
        return false;
    if (!parse_else(if_statement))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <if>\n");
#endif
    return true;
}

bool parse_else(IfStatement *if_statement) {
#ifdef DEBUG
    printf("Parsing <else>\n");
#endif

    if (currentToken()->type != TOKEN_ELSE) {
#ifdef DEBUG
        printf("No else clause found\n");
#endif
        return true;
    }

    getNextToken();
    if (!match(TOKEN_LBRACE))
        return false;
    if (!parse_body(&if_statement->else_body))
        return false;
    if (!match(TOKEN_RBRACE))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <else>\n");
#endif
    return true;
}

bool parse_while(WhileStatement *while_statement) {
#ifdef DEBUG
    printf("Parsing <while>\n");
#endif

    if (!match(TOKEN_WHILE))
        return false;
    if (!match(TOKEN_LPAR))
        return false;
    if (!parse_truth_expr(&while_statement->condition))
        return false;
    if (!match(TOKEN_RPAR))
        return false;

    if (currentToken()->type == TOKEN_PIPE) {
        getNextToken();
        if (currentToken()->type != TOKEN_IDENTIFIER) {
            return false;
        }

        while_statement->non_nullable.name = copyString(currentToken()->value);
        if (!while_statement->non_nullable.name)
            return false;
        getNextToken();

        if (!match(TOKEN_PIPE))
            return false;
    }

    if (!match(TOKEN_LBRACE))
        return false;
    if (!parse_body(&while_statement->body))
        return false;
    if (!match(TOKEN_RBRACE))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <while>\n");
#endif
    return true;
}

bool parse_ret_value(ReturnStatement *return_statement) {
#ifdef DEBUG
    printf("Parsing <ret_value>\n");
#endif

    // Check for empty return
    if (currentToken()->type == TOKEN_SEMICOLON) {
#ifdef DEBUG
        printf("Empty return value\n");
#endif
        return true;
    }

    if (!parse_no_truth_expr(&return_statement->value))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <ret_value>\n");
#endif
    return true;
}

bool parse_native_func_call(FunctionCall *function_call) {
#ifdef DEBUG
    printf("Parsing <native_func_call>\n");
#endif

    if (currentToken()->type != TOKEN_IDENTIFIER) {
        return false;
    }

    if (strcmp(currentToken()->value, "ifj")) {
        return false;
    }

    getNextToken();

    if (!match(TOKEN_CONCATENATE)) {
        return false;
    }

    if (currentToken()->type != TOKEN_IDENTIFIER) {
        return false;
    }

    char *func_name = malloc(strlen(currentToken()->value) + 5);
    if (!func_name) {
        return false;
    }

    strcpy(func_name, "ifj.");
    strcat(func_name, currentToken()->value);

    getNextToken();

    function_call->func_id.name = func_name;
    function_call->arguments = initLinkedList(true);

    if (!function_call->arguments) {
        return false;
    }

    if (!match(TOKEN_LPAR)) {
        return false;
    }

    if (!parse_func_call_params(function_call->arguments)) {
        return false;
    }

    if (!match(TOKEN_RPAR)) {
        return false;
    }

#ifdef DEBUG
    printf("Successfully parsed <native_func_call>\n");
#endif

    return true;
}

bool parse_user_func_call(FunctionCall *function_call) {
#ifdef DEBUG
    printf("Parsing <user_func_call>\n");
#endif

    if (currentToken()->type != TOKEN_IDENTIFIER) {
        return false;
    }

    function_call->func_id.name = copyString(currentToken()->value);
    if (!function_call->func_id.name) {
        return false;
    }

    function_call->arguments = initLinkedList(true);
    if (!function_call->arguments) {
        return false;
    }

    getNextToken();

    if (!match(TOKEN_LPAR)) {
        return false;
    }

    if (!parse_func_call_params(function_call->arguments)) {
        return false;
    }

    if (!match(TOKEN_RPAR)) {
        return false;
    }

#ifdef DEBUG
    printf("Successfully parsed <user_func_call>\n");
#endif
    return true;
}

bool parse_func_call_statement(FunctionCall *function_call) {
#ifdef DEBUG
    printf("Parsing <func_call_statement>\n");
#endif

    if (currentToken()->type != TOKEN_IDENTIFIER) {
        return false;
    }

    TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

    if (!strcmp(currentToken()->value, "ifj") && nextToken && nextToken->type == TOKEN_CONCATENATE) {
        if (!parse_native_func_call(function_call))
            return false;

        if (!match(TOKEN_SEMICOLON)) {
            return false;
        }

        return true;
    }

    if (!parse_user_func_call(function_call)) {
        return false;
    }

    if (!match(TOKEN_SEMICOLON))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <func_call_statement>\n");
#endif
    return true;
}

bool parse_var_assign(AssigmentStatement *assign_statement) {
#ifdef DEBUG
    printf("Parsing <var_assign>\n");
#endif

    if (currentToken()->type != TOKEN_IDENTIFIER)
        return false;

    assign_statement->id.name = copyString(currentToken()->value);
    if (!assign_statement->id.name)
        return false;

    getNextToken();

    if (!match(TOKEN_ASSIGN))
        return false;

    if (!parse_no_truth_expr(&assign_statement->value))
        return false;

    if (!match(TOKEN_SEMICOLON))
        return false;

#ifdef DEBUG
    printf("Successfully parsed <var_assign>\n");
#endif

    return true;
}

bool parse_no_truth_expr(Expression *expr) {
#ifdef DEBUG
    printf("Parsing <no_truth_expr>\n");
#endif

    if (currentToken()->type == TOKEN_IDENTIFIER) {
        TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

        if (nextToken && nextToken->type == TOKEN_LPAR) {
            expr->expr_type = FunctionCallExpressionType;
            return parse_user_func_call(&expr->data.function_call);
        }

        if (!strcmp(currentToken()->value, "ifj") && nextToken && nextToken->type == TOKEN_CONCATENATE) {
            expr->expr_type = FunctionCallExpressionType;
            return parse_native_func_call(&expr->data.function_call);
        }
    }

    err = startPrecedentAnalysis(buffer, &tokenIndex, true, expr);
    if (err != SUCCESS) {
        return false;
    }

    return true;
}

bool parse_truth_expr(Expression *expr) {
#ifdef DEBUG
    printf("Parsing <truth_expr>\n");
#endif

    err = startPrecedentAnalysis(buffer, &tokenIndex, false, expr);
    if (err != SUCCESS) {
        printf("Error in startPrecedentAnalysis: %d\n", err);
        return false;
    }

    return true;
}
