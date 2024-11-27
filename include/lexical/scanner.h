/**
 * @file scanner.h
 * @date 3. 10. 2024
 * @brief Header file for scanner.c
 * @author Marek Sucharda xsucha18
 *
 * Jak to funguje:
 * 		token type je typ tokenu se kterým pak dále pracujeme
 * 		token value je hodnota tokenu, např. type je TOKEN_IDENTIFIER, value je "mojePromenna"
 * 		token status je stav scanneru, podle kterého se scanner rozhoduje, co dělat dál,
 * 			je to pouze interní věc scanneru, pro funkčnost ostatních částí stačí jen používat již
 * 			vygenerované tokeny.
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdbool.h>
#ifdef USE_CUSTOM_STRUCTURE
#include "utility/enumerations.h"
#else
#include "enumerations.h"
#endif

typedef enum
{
	SCANNER_START,

	SCANNER_STRING_START,
	SCANNER_STRING_VALUE,
	SCANNER_STRING_END,
	SCANNER_ESCAPE_SEQ,

	SCANNER_LESS,
	SCANNER_GREATER,
	SCANNER_LESSOREQUAL,
	SCANNER_GREATEROREQUAL,
	SCANNER_EQUAL,
	SCANNER_NOTEQUAL,
	SCANNER_EXCLAMATION,
	SCANNER_1EQUAL,

	SCANNER_PLUS,
	SCANNER_MINUS,
	SCANNER_DIVIDE,
	SCANNER_MULTIPLY,
	SCANNER_COLON,
	SCANNER_COMMA,
	SCANNER_LPAR,
	SCANNER_RPAR,
	SCANNER_LBRACE,
	SCANNER_RBRACE,
	SCANNER_SEMICOLON,
	SCANNER_LSQUARE, // [
	SCANNER_RSQUARE, // ]
	SCANNER_U,		 // u
	SCANNER_8,		 // 8

	SCANNER_I32,	   // 1
	SCANNER_I64,	   // 1.0
	SCANNER_I64_POINT, // 1.
	SCANNER_EXP_I64,   // 1.0e
	SCANNER_EXP_BASE,  // 1.0e+
	SCANNER_EXP_SIGN,  // 1.0e+1
	SCANNER_EXP,	   // 1.0e+10

	SCANNER_CONCATENATE, // .

	SCANNER_IDENTIFIER, // a, b, c, ...

	SCANNER_COMMENT,
	SCANNER_COMMENT_START, //
	SCANNER_COMMENT_END,   // \n or EOF

	SCANNER_EOF,

	SCANNER_PIPE,
	SCANNER_QUESTION_MARK,
	SCANNER_AT,
} SCANNER_STATUS;

enum ERR_CODES scanner_init(FILE *input);
enum ERR_CODES scanner_get_token(struct TOKEN *tokenPointer);
enum ERR_CODES scanner_unget_token(struct TOKEN token);
enum ERR_CODES scanner_peek_token(struct TOKEN *tokenPointer);
enum ERR_CODES scanner_token_free(TOKEN_PTR tokenPointer);
enum ERR_CODES scanner_destroy(void);

#endif
