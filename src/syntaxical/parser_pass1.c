/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  <xsvobov00> Veronika Svobodová
 * @file parser.c
 * @date 17.11. 2024
 * @brief main src file for the syntaxical parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "syntaxical/parser_pass1.h"

static enum ERR_CODES status;
static struct TOKEN token;

void freeBuffer(LinkedList **buffer) {

    if (buffer == NULL || *buffer == NULL) return;

    for (unsigned int i = 0; i < getSize(*buffer); i++) {
        TOKEN_PTR token = (TOKEN_PTR)getDataAtIndex(*buffer, i);
        free(token->value);
        free(token);
    }

    removeList(buffer);
}

// Function to save a new token to the buffer
bool saveNewToken(struct TOKEN token, LinkedList *buffer) {
    
    // create a new token
    TOKEN_PTR newToken = (TOKEN_PTR)malloc(sizeof(struct TOKEN));
    if (newToken == NULL) {
        freeBuffer(&buffer);
        return false;
    }

    newToken->type = token.type;    
    newToken->value = malloc(sizeof(char) * (strlen(token.value) + 1));
    if (newToken->value == NULL) {
        free(newToken);
        freeBuffer(&buffer);
        return false;
    }
    strcpy(newToken->value, token.value);

    // save the token to the buffer
    if (!insertNodeAtIndex(buffer, (void *)newToken, -1)) {
        freeBuffer(&buffer);
        return false;
    }

    return true;
}

enum ERR_CODES pubfn(LinkedList *buffer, SymTable *table) {

    #ifdef DEBUG
    DEBUG_MSG("Parsing function definition\n");
    #endif

    // try to find the fn    
    enum ERR_CODES status = scanner_get_token(&token);
    if (!saveNewToken(token, buffer)) return E_INTERNAL;
    if (token.type != TOKEN_FN) return E_SYNTAX;

    // try to find the function name
    status = scanner_get_token(&token);
    if (!saveNewToken(token, buffer)) return E_INTERNAL;
    if (token.type != TOKEN_IDENTIFIER) return E_SYNTAX;

    SymFunctionPtr symFunction = symInitFuncDefinition();
    if (symFunction == NULL) {
        freeBuffer(&buffer);
        return E_INTERNAL;
    }

    symEditFuncDef(symFunction, token.value, dTypeUndefined, -1); 
    status = symTableAddFunction(table, symFunction); // we will finish it later ..
    if (status != SUCCESS) {
        freeBuffer(&buffer);
        return status;
    }

    // try to find the left parenthesis
    status = scanner_get_token(&token);
    if (!saveNewToken(token, buffer)) return E_INTERNAL;
    if (token.type != TOKEN_LPAR) return E_SYNTAX;

    bool first = true;

    while (1) {
        
        // ) or Identifier
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (token.type == TOKEN_RPAR && first) break;
        first = false;
        if (token.type != TOKEN_IDENTIFIER) return E_SYNTAX;

        // :
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (token.type != TOKEN_COLON) return E_SYNTAX;

        // type
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (token.type != TOKEN_I32 && token.type != TOKEN_F64 && token.type != TOKEN_U8) {
            #ifdef DEBUG
            DEBUG_MSG("Expected i32 or f64 or u8 but got: ");
            printTokenType(token.type);
            printf("value of token: %s\n", token.value);
            #endif
            return E_SYNTAX;
        }

        symAddParamToFunc(symFunction, covertTokneDataType(token.type) , 0); // save the param

        // now can be , or )
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (token.type != TOKEN_COMMA && token.type != TOKEN_RPAR) {
            #ifdef DEBUG
            DEBUG_MSG("Expected , or ) but got: ");
            printTokenType(token.type); 
            #endif
            return E_SYNTAX;
        }

        if (token.type == TOKEN_RPAR) break;
    }

    // check for the return type we can have ? followed by i32 of f64 or u8 or void
    status = scanner_get_token(&token);
    if (!saveNewToken(token, buffer)) return E_INTERNAL;

    if (token.type == TOKEN_QUESTION_MARK) {
        status = scanner_get_token(&token);
        if (!saveNewToken(token, buffer)) return E_INTERNAL;
        if (token.type != TOKEN_I32 && token.type != TOKEN_F64 && token.type != TOKEN_U8) return E_SYNTAX;
        if (!symEditFuncDef(symFunction, NULL, covertTokneDataType(token.type), 1)) return E_INTERNAL;
    } else if (token.type == TOKEN_VOID) {
        // we are fine
        if (!symEditFuncDef(symFunction, NULL, dTypeVoid, -1)) return E_INTERNAL;
    } else if (token.type == TOKEN_I32 || token.type == TOKEN_F64 || token.type == TOKEN_U8) {
        if (!symEditFuncDef(symFunction, NULL, covertTokneDataType(token.type), -1)) return E_INTERNAL;
    } else {
        return E_SYNTAX;
    }

    return SUCCESS;
}

// Functio to do the first pass over the program
enum ERR_CODES firstPass(SymTable *table, FILE *input, LinkedList *buffer) {

    scanner_init(input);
    enum ERR_CODES status = SUCCESS;
    while (status == SUCCESS) {
        // get the token
        status = scanner_get_token(&token);
        if (status != SUCCESS) return E_INTERNAL;
        if (token.type == TOKEN_EOF) break;
        // save the token
        if (!saveNewToken(token, buffer)) return E_INTERNAL;

        // detect, if we have perhaps found a function definition
        if (token.type == TOKEN_PUB) {
            enum ERR_CODES err = pubfn(buffer, table);
            if (err != SUCCESS) return err;
        }
    }

    // check, if we have the main function
    if (getSize(buffer) == 0) return E_SYNTAX;
    if (!symTableFindFunction(table, "main")) return E_SEMANTIC_OTHER;
    return SUCCESS;
}





