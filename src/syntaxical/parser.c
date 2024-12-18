/** AUTHOR
 *
 * @author <xsucha18> Marek Sucharda 
 * @author <253171> Vanesa Zimmermannová
 * @author <247581> Martin Mendl   
 * @file precident.c
 * @date 17.11. 2024
 * @brief main src file for the precedent analysis
 */

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/parser.h"
#include "syntaxical/precedent.h"
#include "utility/my_utils.h"
#else
#include "parser.h"
#include "precedent.h"
#include "my_utils.h"
#endif

static unsigned int tokenIndex = 0;
static LinkedList *buffer = NULL;
static SymTable *table = NULL;
static struct TOKEN token;
enum ERR_CODES globalError = SUCCESS;

// Function to get the current token
TOKEN_PTR currentToken(void) {
    return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

// Function to get the next token
TOKEN_PTR getNextToken(void) {
    tokenIndex++;
    return (TOKEN_PTR)getDataAtIndex(buffer, tokenIndex);
}

// Function to match the token
bool match(enum TOKEN_TYPE tokenType) {
    TOKEN_PTR token = currentToken();

    if (!token || token->type != tokenType) {
        DEBUG_PRINT("Expected token type: ");
        printTokenType(tokenType);
        DEBUG_PRINT(" but got: ");

        if (token) {
            printTokenType(currentToken()->type);
        } else {
            DEBUG_PRINT("No token");
        }

        return false;
    }

    DEBUG_PRINT("Matched token: \t%s\n", token->value);
    getNextToken();
    return true;
}

// Function to save a new token to the buffer
bool saveNewToken(struct TOKEN token, LinkedList *buffer) {
    
    // create a new token
    TOKEN_PTR newToken = (TOKEN_PTR)malloc(sizeof(struct TOKEN));
    if (newToken == NULL) return false;

    newToken->type = token.type;    
    newToken->value = token.value;

    DEBUG_PRINT("SAVING TOKEN: %s\n", newToken->value);
    
    // save the token to the buffer
    if (!insertNodeAtIndex(buffer, (void *)newToken, -1)) return false;
    return true;
}

// Function to initialize the parser
void parser_init(SymTable *tbl) {
    buffer = tbl->tokenBuffer;
    table = tbl;
}

// Functio to do the first pass over the program
enum ERR_CODES firstPass(FILE *input, LinkedList *buffer) {

    scanner_init(input);
    enum ERR_CODES status = SUCCESS;
    while (status == SUCCESS) {
        // get the token
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (status != SUCCESS) return status;
        // save the token
        if (token.type == TOKEN_EOF) break;
    }

    // check, if we have the main function
    if (getSize(buffer) == 0) return E_SYNTAX;
    return SUCCESS;
}

// Function to parse the program
enum ERR_CODES parser_parse(FILE *input, struct Program *program) {
    DEBUG_PRINT("parser_parse");

    // svae all the tokens to the buffer
    enum ERR_CODES err = firstPass(input, buffer);
    if (err != SUCCESS) {
        DEBUG_PRINT("Error in first pass");
        DEBUG_PRINT("Error code: %d\n", err);
        return err;
    }

    DEBUG_PRINT("parser_second_pass");
    DEBUG_PRINT("Current token: %s\n", currentToken()->value);

    // parse the program
    if (!parse_program(program)) return (globalError == SUCCESS) ? E_SYNTAX : globalError;
    return SUCCESS;
}

// Function to parse the program
bool parse_program(struct Program *program) {
    DEBUG_PRINT("Parsing <program>\n");

    // init the ast
    program->functions = initLinkedList(true);
    if (!program->functions) {return false; globalError = E_INTERNAL;}

    if (!parse_prolog()) return false; // prolog
    if (!parse_functions(program->functions)) return false; // prolog functions[0 - n] 
    if (!match(TOKEN_EOF)) return false; // program EOF

    DEBUG_PRINT("Successfully parsed <program>\n");
    return true;
}

// Function to parse the prolog
bool parse_prolog(void) {
    DEBUG_PRINT("Parsing <prolog>\n");

    if (!match(TOKEN_CONST)) return false; // const
    if (!match(TOKEN_IFJ)) return false; // const ifj
    if (!match(TOKEN_ASSIGN)) return false; // const ifj =
    if (!match(TOKEN_AT)) return false; // const ifj = @
    if (!match(TOKEN_IMPORT)) return false; // const ifj = @import
    if (!match(TOKEN_LPAR)) return false; // const ifj = @import(
    if (!match(TOKEN_STRING_LITERAL)) return false; // const ifj = @import("string")
    tokenIndex--; // go back to the string literal
    if (strcmp(currentToken()->value, "ifj24.zig") != 0) return false; // const ifj = @import("ifj24.zig"
    tokenIndex++; // go to the next token
    if (!match(TOKEN_RPAR)) return false; // const ifj = @import("ifj24.zig")
    if (!match(TOKEN_SEMICOLON)) return false; // const ifj = @import("ifj24.zig");

    DEBUG_PRINT("Successfully parsed <prolog>\n");
    return true;
}

// Function to parse the functions
bool parse_functions(LinkedList *functions) {
    DEBUG_PRINT("Parsing <functions>\n");

    while (parse_next_function()) {

        // alocate memory for ast
        Function *function = malloc(sizeof(struct Function));
        if (!function) {return false; globalError = E_INTERNAL;}
        memset(function, 0, sizeof(struct Function));

        // init the ast
        function->params = initLinkedList(true);
        if (!function->params) {return false; globalError = E_INTERNAL;}
        if (!insertNodeAtIndex(functions, (void *)function, -1)) {return false; globalError = E_INTERNAL;}

        // parse the function
        if (!parse_function(function)) return false;
    }

    DEBUG_PRINT("Successfully parsed <functions>\n");
    return true;
}

// Function to parse the function
bool parse_function(struct Function *function) {
    DEBUG_PRINT("Parsing <function>\n");

    if (!match(TOKEN_PUB)) return false; // pub
    if (!match(TOKEN_FN)) return false; // pub fn
    if (currentToken()->type != TOKEN_IDENTIFIER) return false; // pub fn identifier

    DEBUG_PRINT("Function name: %s\n", currentToken()->value);

    function->id.name = currentToken()->value;
    if (!function->id.name) {return false; globalError = E_INTERNAL;}
    getNextToken();

    if (!match(TOKEN_LPAR)) return false; // pub fn identifier(
    if (!parse_params(function->params)) return false; // pub fn identifier(params
    if (!match(TOKEN_RPAR)) return false; // pub fn identifier(params)
    if (!parse_data_type(&function->returnType)) return false; // pub fn identifier(params) ret_type
    if (!match(TOKEN_LBRACE)) return false; // pub fn identifier(params) ret_type {
    if (!parse_func_body(&function->body)) return false; // pub fn identifier(params) ret_type { ...
    if (!match(TOKEN_RBRACE)) return false; // pub fn identifier(params) ret_type { ... }

    DEBUG_PRINT("Successfully parsed <function>\n");
    return true;
}

// Function to parse the next function
bool parse_next_function(void) {
    DEBUG_PRINT("Parsing <next_function>\n");

    if (currentToken()->type == TOKEN_EOF) {
        DEBUG_PRINT("End of tokens reached, end the program.\n");
        return false;
    }
    if (currentToken()->type != TOKEN_PUB) return false; 

    // pub
    DEBUG_PRINT("Successfully parsed <next_function> (empty)\n");
    return true;
}

// Function to parse the parameters
bool parse_params(LinkedList *params) {
    DEBUG_PRINT("Parsing <params>\n");

    // Handle empty params case
    if (currentToken()->type == TOKEN_RPAR) {
        DEBUG_PRINT("Successfully parsed <params> (empty)\n");
        return true;
    }

    while (1) {
        // ast prepare
        Param *param = malloc(sizeof(Param));
        if (!param) {return false; globalError = E_INTERNAL;}
        memset(param, 0, sizeof(Param));
        if (!insertNodeAtIndex(params, (void *)param, -1)) {return false; globalError = E_INTERNAL;}

        // parse the parameter
        if (!parse_parameter(param)) return false;  
        if (currentToken()->type != TOKEN_COMMA) break;
        getNextToken();
    }

    DEBUG_PRINT("Successfully parsed <params>\n");
    return true;
}

// Function to parse the parameter
bool parse_parameter(Param *param) {
    DEBUG_PRINT("Parsing <parameter>\n");

    // Handle empty parameter case
    if (currentToken()->type != TOKEN_IDENTIFIER) {
        DEBUG_PRINT("Successfully parsed <parameter> (empty)\n");
        return true;
    }

    // save the parameter to the ast
    DEBUG_PRINT("Parameter name: %s\n", currentToken()->value);
    param->id.name = currentToken()->value;
    if (!param->id.name) {return false; globalError = E_INTERNAL;}
    getNextToken(); // consume identifier

    if (!match(TOKEN_COLON)) return false; // identifier :
    if (!parse_data_type(&param->type)) return false; // identifier : ...

    DEBUG_PRINT("Successfully parsed <parameter>\n");
    return true;
}

// Function to parse the data type
bool parse_func_call_params(LinkedList *attributes) {
    DEBUG_PRINT("Parsing <function_call_params>\n");

    if (currentToken()->type == TOKEN_RPAR) { // )
        DEBUG_PRINT("Successfully parsed function parameters (empty)\n");
        return true;
    }

    while (1) {
        Expression *expr = malloc(sizeof(Expression));
        if (!expr) {return false; globalError = E_INTERNAL;}
        memset(expr, 0, sizeof(Expression));
        if (!insertNodeAtIndex(attributes, (void *)expr, -1)) {return false; globalError = E_INTERNAL;} 

        if (!parse_func_call_param(expr)) return false; // parse the parameter
        if (currentToken()->type != TOKEN_COMMA) break;
        getNextToken();
    }

    DEBUG_PRINT("Successfully parsed <function_call_params>\n");
    return true;
}

// Function to parse the function call parameter
bool parse_func_call_param(Expression *expr) {
    DEBUG_PRINT("Parsing <function_call_param>\n");

    // empty params
    if (currentToken()->type == TOKEN_RPAR) {
        DEBUG_PRINT("Successfully parsed <function_call_params> (empty)\n");
        return true;
    }

    // literal
    if (isLiteral(currentToken()->type)) {
        DEBUG_PRINT("Data type: %s\n", currentToken()->value);

        expr->expr_type = LiteralExpressionType;
        expr->data.literal.value = currentToken()->value;
        if (!expr->data.literal.value) {return false; globalError = E_INTERNAL;}

        expr->data_type.data_type = covertTokneDataType(currentToken()->type);
        expr->data.literal.data_type = expr->data_type;

        getNextToken();
        return true;
    }

    // identifier
    if (currentToken()->type == TOKEN_IDENTIFIER) {

        DEBUG_PRINT("Identifier: %s\n", currentToken()->value);

        expr->expr_type = IdentifierExpressionType;
        expr->data.identifier.name = currentToken()->value;
        if (!expr->data.identifier.name) {return false; globalError = E_INTERNAL;} 

        getNextToken();
        return true;
    }

    // string literal
    if (currentToken()->type == TOKEN_STRING_LITERAL) {
        DEBUG_PRINT("String: %s\n", currentToken()->value);

        expr->expr_type = LiteralExpressionType;
        expr->data.literal.value = currentToken()->value;
        if (!expr->data.literal.value) {return false; globalError = E_INTERNAL;}

        expr->data_type.data_type = dTypeU8;
        expr->data.literal.data_type = expr->data_type;

        getNextToken();
        return true;
    }

    DEBUG_PRINT("Successfully parsed <function_call_param>\n");
    return false;
}

// Function to parse the data type
bool parse_data_type(DataType *data_type) {
    DEBUG_PRINT("Parsing <data_type>\n");

    // Check if the data type is nullable
    if (currentToken()->type == TOKEN_QUESTION_MARK) { 
        data_type->is_nullable = true;
        getNextToken();
    }

    // Check if the data type is valid
    if (!isDataType(currentToken()->type)) {
        DEBUG_PRINT("Expected data type but got: %s\n", currentToken()->value);
        return false;
    }

    data_type->data_type = covertTokneDataType(currentToken()->type);
    getNextToken();

    DEBUG_PRINT("Successfully parsed <data_type>: %d\n", data_type->data_type);
    return true;
}

// Function to parse the function body
bool parse_func_body(Body *body) {
    DEBUG_PRINT("Parsing <func_body>\n");
    if (!parse_body(body)) return false;

    DEBUG_PRINT("Successfully parsed <func_body>\n");
    return true;
}

// Function to parse the body
bool parse_body(Body *body) {
    DEBUG_PRINT("Parsing <body>\n");

    // empty body
    if (currentToken()->type == TOKEN_RBRACE) {
        DEBUG_PRINT("Successfully parsed <body> (empty)\n");
        return true;
    }

    body->statements = initLinkedList(true);
    if (!body->statements) {return false; globalError = E_INTERNAL;}

    while (parse_body_content_next()) {
        Statement *statement = malloc(sizeof(Statement));
        if (!statement) {return false; globalError = E_INTERNAL;}
        memset(statement, 0, sizeof(Statement));

        if (!insertNodeAtIndex(body->statements, (void *)statement, -1)) {return false; globalError = E_INTERNAL;}
        if (!parse_body_content(statement)) return false;
    }

    DEBUG_PRINT("Successfully parsed <body>\n");
    return true;
}

// Function to parse the body content
bool parse_body_content(struct Statement *statement) {
    DEBUG_PRINT("Parsing <body_content>\n");

    TOKEN_PTR token = currentToken();
    TOKEN_PTR nextToken;

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
        if (!parse_ret_value(&statement->data.return_statement)) return false;
        return match(TOKEN_SEMICOLON);

    case TOKEN_IDENTIFIER:
        nextToken = getDataAtIndex(buffer, tokenIndex + 1);
        if (nextToken && nextToken->type == TOKEN_ASSIGN) {
            statement->type = AssigmentStatementType;
            return parse_var_assign(&statement->data.assigment_statement);
        }

        statement->type = FunctionCallStatementType;
        return parse_func_call_statement(&statement->data.function_call_statement);

	case TOKEN_IFJ: 
        statement->type = FunctionCallStatementType;
        if (!parse_native_func_call(&statement->data.function_call_statement)) return false;
		if (!match(TOKEN_SEMICOLON)) return false;
		return true; 

    case TOKEN_DELETE_VALUE:
        statement->type = AssigmentStatementType;
        return parse_var_assign(&statement->data.assigment_statement);

    default:
        DEBUG_PRINT("Syntax error: unexpected token %s\n, expected: const, var, if, while, return, "" identifier, ifj, _", token->value);
        return false;
    }

    return true;
}

// Function to parse the next body content
bool parse_body_content_next(void) {
    switch (currentToken()->type) {
        case TOKEN_IDENTIFIER:
        case TOKEN_CONST:
        case TOKEN_IF:
        case TOKEN_RETURN:
        case TOKEN_VAR:
        case TOKEN_IFJ:
        case TOKEN_WHILE:
        case TOKEN_DELETE_VALUE:
            return true;
        default:
            return false;
    }
}

// Function to parse the variable definition
bool parse_var_def(VariableDefinitionStatement *variable_definition_statement) {
    DEBUG_PRINT("Parsing <var_def>\n");

    // Check if it starts with 'const' or 'var'
    if (currentToken()->type != TOKEN_CONST && currentToken()->type != TOKEN_VAR) return false; // const | var

    // ast save
    if (currentToken()->type == TOKEN_CONST) {
        variable_definition_statement->isConst = true;
    } else {
        variable_definition_statement->isConst = false;
    }
    getNextToken();

    // Variable identifier
    if (currentToken()->type != TOKEN_IDENTIFIER) return false; // const | var identifier

    // ast save
    variable_definition_statement->id.name = currentToken()->value;
    if (!variable_definition_statement->id.name) return false;
    getNextToken();

    // Variable type (not required)
    if (currentToken()->type == TOKEN_COLON) {
        if (!match(TOKEN_COLON)) return false; // const | var identifier :
        if (!parse_data_type(&variable_definition_statement->type)) return false; // const | var identifier : ...
    } else {
        variable_definition_statement->type.data_type = dTypeNone;
        variable_definition_statement->type.is_nullable = false;
    }

    if (!match(TOKEN_ASSIGN)) return false; // const | var identifier ... =

    // parser the right side of the expression
    if (!parse_no_truth_expr(&variable_definition_statement->value)) return false; // const | var identifier ... = ...
    if (!match(TOKEN_SEMICOLON)) return false; // const | var identifier ... = ... ;

    DEBUG_PRINT("Successfully parsed <var_def>\n");
    return true;
}

// Function to parse the if statement
bool parse_if(IfStatement *if_statement) {
    DEBUG_PRINT("Parsing <if>\n");

    if (!match(TOKEN_IF)) return false;
    if (!match(TOKEN_LPAR)) return false;

    TOKEN_PTR curToken = currentToken();
    TOKEN_PTR nextToken = getNextToken();

    DEBUG_PRINT("Current token: %s\n", curToken->value);
    DEBUG_PRINT("Next token: %s\n", nextToken->value);

    // handeling if (a) |na| {...}
    if (curToken->type == TOKEN_IDENTIFIER && nextToken->type == TOKEN_RPAR) {
        DEBUG_PRINT("Handeling if (a) |na| {...}\n");
        // save the current token into the expression ..
        if_statement->condition.expr_type = IdentifierExpressionType;
        if_statement->condition.data.identifier.name = curToken->value;
        if (!if_statement->condition.data.identifier.name) {return false; globalError = E_INTERNAL;}

        tokenIndex++;
        if (!match(TOKEN_PIPE)) return false;
        if (!match(TOKEN_IDENTIFIER)) return false;
        tokenIndex--;

        // saving the not nullable var name
        if_statement->non_nullable.name = currentToken()->value;
        if (!if_statement->non_nullable.name) return false;
        tokenIndex++;
        if (!match(TOKEN_PIPE)) return false;
        DEBUG_PRINT("Successfully parsed if (a) |na|\n");

        // handeling if (exp) {...}
    } else {
        DEBUG_PRINT("Handeling if (exp) {...}\n");
        tokenIndex--; // got back to the start of the expression
        if (!parse_truth_expr(&if_statement->condition)) return false;
        if (!match(TOKEN_RPAR)) return false;
        DEBUG_PRINT("Successfully parsed if (exp)\n");
    }
    // checking the if else bodyes
    if (!match(TOKEN_LBRACE)) return false; // if () {
    if (!parse_body(&if_statement->if_body)) return false; // if () { ...
    if (!match(TOKEN_RBRACE)) return false; // if () { ... }
    if (!parse_else(if_statement)) return false; // if () { ... } else { ... }

    DEBUG_PRINT("Successfully parsed <if>\n");
    return true;
}

// Function to parse the else statement
bool parse_else(IfStatement *if_statement) {
    DEBUG_PRINT("Parsing <else>\n");

    if (currentToken()->type != TOKEN_ELSE) { // else
        DEBUG_PRINT("No else clause found\n");
        return false;
    }

    getNextToken();
    if (!match(TOKEN_LBRACE)) return false; // else {
    if (!parse_body(&if_statement->else_body)) return false; // else { ...
    if (!match(TOKEN_RBRACE)) return false; // else { ... }

    DEBUG_PRINT("Successfully parsed <else>\n");
    return true;
}

// Function to parse the while statement
bool parse_while(WhileStatement *while_statement) {
    DEBUG_PRINT("Parsing <while>\n");

    if (!match(TOKEN_WHILE)) return false; // while
    if (!match(TOKEN_LPAR)) return false; // while (

    TOKEN_PTR curToken = currentToken();
    TOKEN_PTR nextToken = getNextToken();

    // handeling while (a) |na| {...}
    if (curToken->type == TOKEN_IDENTIFIER && nextToken->type == TOKEN_RPAR) {

        // save the current token into the expression ..
        while_statement->condition.expr_type = IdentifierExpressionType;
        while_statement->condition.data.identifier.name = curToken->value;
        if (!while_statement->condition.data.identifier.name)  {return false; globalError = E_INTERNAL;}

        tokenIndex++;
        if (!match(TOKEN_PIPE)) return false;
        if (!match(TOKEN_IDENTIFIER)) return false;
        tokenIndex--;

        // saving the not nullable var name
        while_statement->non_nullable.name = currentToken()->value;
        if (!while_statement->non_nullable.name) return false;
        tokenIndex++;
        if (!match(TOKEN_PIPE)) return false;

    // handeling while (exp) {...}
    } else {
        tokenIndex--; // got back to the start of the expression
        if (!parse_truth_expr(&while_statement->condition)) return false;
        if (!match(TOKEN_RPAR)) return false;
    }

    if (!match(TOKEN_LBRACE)) return false; // while () {
    if (!parse_body(&while_statement->body)) return false; // while () { ...
    if (!match(TOKEN_RBRACE)) return false; // while () { ... }

    DEBUG_PRINT("Successfully parsed <while>\n");
    return true;
}

// Function to parse the return statement
bool parse_ret_value(ReturnStatement *return_statement) {
    DEBUG_PRINT("Parsing <ret_value>\n");

    // Check for empty return
    if (currentToken()->type == TOKEN_SEMICOLON) {
        return_statement->empty = true;
        DEBUG_PRINT("Empty return value\n");
        return true;
    }

    return_statement->empty = false;
    if (!parse_no_truth_expr(&return_statement->value)) return false;

    DEBUG_PRINT("Successfully parsed <ret_value>\n");
    return true;
}

// Function to parse the native function call
bool parse_native_func_call(FunctionCall *function_call) {
    DEBUG_PRINT("Parsing <native_func_call>\n");

	if (currentToken()->type != TOKEN_IFJ) return false;
    getNextToken();

    if (!match(TOKEN_CONCATENATE)) return false; // ifj.
    if (!match(TOKEN_IDENTIFIER)) return false; // ifj.identifier
	tokenIndex--; 

    // save the func call to the ast
    char *func_name = malloc(strlen(currentToken()->value) + 6);
    if (!func_name) return false;

    strcpy(func_name, "$ifj_");
    strcat(func_name, currentToken()->value);

    free(currentToken()->value);
    currentToken()->value = func_name;
	tokenIndex++; 

    function_call->func_id.name = func_name;
    function_call->arguments = initLinkedList(true);

    if (!function_call->arguments) {return false; globalError = E_INTERNAL;}
    if (!match(TOKEN_LPAR)) return false; // ifj.identifier(
    if (!parse_func_call_params(function_call->arguments)) return false; // ifj.identifier(...)
    if (!match(TOKEN_RPAR)) return false; // ifj.identifier(...)

    DEBUG_PRINT("Successfully parsed <native_func_call>\n");
    return true;
}

// Function to parse the user function call
bool parse_user_func_call(FunctionCall *function_call) {
    DEBUG_PRINT("Parsing <user_func_call>\n");

    if (currentToken()->type != TOKEN_IDENTIFIER) return false; // identifier

    // add to the ast
    function_call->func_id.name = currentToken()->value;
    if (!function_call->func_id.name) {return false; globalError = E_INTERNAL;}

    function_call->arguments = initLinkedList(true);
    if (!function_call->arguments) {return false; globalError = E_INTERNAL;}
    getNextToken();

    if (!match(TOKEN_LPAR)) return false; // identifier(
    if (!parse_func_call_params(function_call->arguments)) return false; // identifier(...
    if (!match(TOKEN_RPAR)) return false; // identifier(...)

    DEBUG_PRINT("Successfully parsed <user_func_call>\n");
    return true;
}

// Function to parse the function call statement
bool parse_func_call_statement(FunctionCall *function_call) {
    DEBUG_PRINT("Parsing <func_call_statement>\n");

    if (currentToken()->type != TOKEN_IDENTIFIER) return false; // identifier
    if (!parse_user_func_call(function_call)) return false; // identifier(...)
    if (!match(TOKEN_SEMICOLON)) return false; // identifier(...);

    DEBUG_PRINT("Successfully parsed <func_call_statement>\n");
    return true;
}

// Function to parse the variable assignment
bool parse_var_assign(AssigmentStatement *assign_statement) {
    DEBUG_PRINT("Parsing <var_assign>\n");

    if (currentToken()->type != TOKEN_IDENTIFIER && currentToken()->type != TOKEN_DELETE_VALUE) return false; // identifier || _

    // save to the ast
    assign_statement->id.name = currentToken()->value;
    if (!assign_statement->id.name) return false;
    if (currentToken()->type == TOKEN_DELETE_VALUE) assign_statement->discard = true;
    getNextToken();

    if (!match(TOKEN_ASSIGN)) return false; // identifier =
    if (!parse_no_truth_expr(&assign_statement->value)) return false; // identifier = ...
    if (!match(TOKEN_SEMICOLON)) return false; // identifier = ...;

    DEBUG_PRINT("Successfully parsed <var_assign>\n");
    return true;
}

// Function to parse the no truth expression
bool parse_no_truth_expr(Expression *expr) {
    DEBUG_PRINT("Parsing <no_truth_expr>\n");
    DEBUG_PRINT("Current token: %s\n", currentToken()->value);

    if (currentToken()->type == TOKEN_IDENTIFIER || isLiteral(currentToken()->type)) {
        TOKEN_PTR nextToken = getDataAtIndex(buffer, tokenIndex + 1);

        // = <identifier>; or = <literal>;
        if (nextToken && nextToken->type == TOKEN_SEMICOLON) {
            // = <identifier>;
            if (currentToken()->type == TOKEN_IDENTIFIER) {
                DEBUG_PRINT("Identifier ending with ;");
                expr->expr_type = IdentifierExpressionType;
                expr->data.identifier.name = currentToken()->value;
                getNextToken(); // move to semicolon
                return true;
            }

            // = <literal>;
            if (isLiteral(currentToken()->type)) {
                DEBUG_PRINT("Literar ending with ;");
                expr->expr_type = LiteralExpressionType;
                expr->data.literal.value = currentToken()->value;
                if (currentToken()->type == TOKEN_NULL) { // handeling of null;
                    expr->data.literal.data_type.is_nullable = true;
                    expr->data.literal.data_type.data_type = dTypeNone;
                    expr->data.literal.value = NULL;
                } else {
                    expr->data.literal.data_type.is_nullable = false;
                    expr->data.literal.data_type.data_type = covertTokneDataType(currentToken()->type);
                }

                getNextToken(); // move to semicolon
                return true;
            }
        }

        // = <identifier>(...)
        if (currentToken()->type == TOKEN_IDENTIFIER && nextToken && nextToken->type == TOKEN_LPAR) {
            expr->expr_type = FunctionCallExpressionType;
            return parse_user_func_call(&expr->data.function_call);
        }
    } 

	if (currentToken()->type == TOKEN_IFJ) {
		expr->expr_type = FunctionCallExpressionType;
		return parse_native_func_call(&expr->data.function_call);
	}

    enum ERR_CODES err = startPrecedentAnalysis(buffer, &tokenIndex, true, expr);
    if (err != SUCCESS) return false;
    return true;
}

// Function to parse the truth expression
bool parse_truth_expr(Expression *expr) {
    DEBUG_PRINT("Parsing <truth_expr>\n");

    enum ERR_CODES err = startPrecedentAnalysis(buffer, &tokenIndex, false, expr);
    if (err != SUCCESS) {
        printf("Error in startPrecedentAnalysis: %d\n", err);
        globalError = err;
        return false;
    }
    return true;
}
