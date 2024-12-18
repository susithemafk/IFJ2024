/**
 * @file scanner.c
 *
 * @author Marek Sucharda xsucha18
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "utility/my_utils.h"
#else
#include "scanner.h"
#include "enumerations.h"
#include "my_utils.h"
#endif

#define ALLOC_SIZE 64

static FILE *file;
static int nextCharacter;
static struct TOKEN nextToken;

// Function to init the scanner
enum ERR_CODES scanner_init(FILE *input) {
    file = input;
    nextCharacter = EOF;
    nextToken.value = NULL;
    nextToken.type = TOKEN_NONE;
    return SUCCESS;
}

// Function to end the scanner
enum ERR_CODES scanner_end(char input, int *nextCharacter, struct TOKEN *tokenPointer, int string_index) {
    tokenPointer->value[string_index] = '\0';
    *nextCharacter = input;
    return SUCCESS;
}

// Function to get the next token
enum ERR_CODES scanner_get_token(struct TOKEN *tokenPointer) {
    SCANNER_STATUS state = SCANNER_START;
    unsigned string_index = 0;
    unsigned allocated_length = ALLOC_SIZE;
    int input = 0;
    bool assign_value = true;

    tokenPointer->value = (char *)malloc(allocated_length);
    if (tokenPointer->value == NULL) {
        return E_INTERNAL; // memory allocation failed
    }

    if (nextToken.type != TOKEN_NONE) {
        *tokenPointer = nextToken;
        nextToken.type = TOKEN_NONE;
        return SUCCESS;
    }

    while (true) {
        if (nextCharacter == EOF)
            input = getc(file);
        else {
            input = nextCharacter;
            nextCharacter = EOF;
        }
        assign_value = true;
        char lookahead; 
        bool is_newline = false;

        switch (state) {
        case SCANNER_START:
            string_index = 0;

            switch (input) {
            case EOF:
                tokenPointer->type = TOKEN_EOF;
                state = SCANNER_EOF;
                break;
            case '!':
                state = SCANNER_EXCLAMATION;
                break;
            case '=':
                state = SCANNER_1EQUAL;
                break; 
			case '\\':
				state = SCANNER_MULTILINE_STRING; 
				break; 
            case '\"':
                assign_value = false;
                state = SCANNER_STRING_START;
                tokenPointer->type = TOKEN_STRING_LITERAL;
                break;
            case ':':
                state = SCANNER_COLON;
                tokenPointer->type = TOKEN_COLON;
                break;
            case '+':
                state = SCANNER_PLUS;
                tokenPointer->type = TOKEN_PLUS;
                break;
            case '-':
                state = SCANNER_MINUS;
                tokenPointer->type = TOKEN_MINUS;
                break;
            case '*':
                state = SCANNER_MULTIPLY;
                tokenPointer->type = TOKEN_MULTIPLY;
                break;
            case '/':
                state = SCANNER_DIVIDE;
                tokenPointer->type = TOKEN_DIVIDE;
                break;
            case '.':
                state = SCANNER_CONCATENATE;
                tokenPointer->type = TOKEN_CONCATENATE;
                break;
            case ';':
                state = SCANNER_SEMICOLON;
                tokenPointer->type = TOKEN_SEMICOLON;
                break;
            case '<':
                state = SCANNER_LESS;
                break;
            case '>':
                state = SCANNER_GREATER;
                break;
            case ',':
                state = SCANNER_COMMA;
                tokenPointer->type = TOKEN_COMMA;
                break;
            case '(':
                state = SCANNER_LPAR;
                tokenPointer->type = TOKEN_LPAR;
                break;
            case ')':
                state = SCANNER_RPAR;
                tokenPointer->type = TOKEN_RPAR;
                break;
            case '{':
                state = SCANNER_LBRACE;
                tokenPointer->type = TOKEN_LBRACE;
                break;
            case '}':
                state = SCANNER_RBRACE;
                tokenPointer->type = TOKEN_RBRACE;
                break;
            case '[':
                state = SCANNER_LSQUARE;
                break;
            case '|':
                state = SCANNER_PIPE;
                tokenPointer->type = TOKEN_PIPE;
                break;
            case '@':
                state = SCANNER_AT;
                tokenPointer->type = TOKEN_AT;
                break;
            case '?':
                state = SCANNER_QUESTION_MARK;
                tokenPointer->type = TOKEN_QUESTION_MARK;
                break;

            default:
                if (isspace(input))
                    // white space znaky
                    continue;
                else if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') ||
                         input == '_') {
                    // identifikátor
                    state = SCANNER_IDENTIFIER;
                } else if (isDigit(input)) {
                    // číslo
                    state = SCANNER_I32;
                } else
                    return E_LEXICAL;
            }
            break;

        case SCANNER_STRING_START:
            if (input == '"') {
                assign_value = false; // poslední uvozovka se neuloží
                state = SCANNER_STRING_END;
            } else if (input == '\\') {
                state = SCANNER_ESCAPE_SEQ;
                assign_value = false;
            } else if (input >= ' ')
                state = SCANNER_STRING_VALUE;
            else
                return E_LEXICAL;
            break;

		case SCANNER_MULTILINE_STRING: 
            input = getc(file);
            DEBUG_PRINT("Multiline string\n");
            string_index = 0;

            while (input != EOF) {
                // Reallocate memory if needed
                if (string_index + 4 >= allocated_length) {
                    allocated_length += ALLOC_SIZE;
                    char *temp = realloc(tokenPointer->value, allocated_length);
                    if (temp == NULL) {
                        return E_INTERNAL;
                    }
                    tokenPointer->value = temp;
                }

                // Handle newlines
                if (input == '\n') {
                    is_newline = true;
                    tokenPointer->value[string_index++] = input;
                    input = getc(file); // Fetch next character after newline
                    while (input == ' ') { // Skip leading spaces
                        input = getc(file);
                    }
                    DEBUG_PRINT("New line detected: '%c'\n", input);
                    continue;
                }

                // Validate newline escape sequence
                if (is_newline) {
                    // End of string conditions
                    if (input == ';' || input == ')') {
                        DEBUG_PRINT("input: %c\n", input);
                        DEBUG_PRINT("END OF STRING");
                        string_index--; // overwrite the newline character
                        break;
                    }

                    lookahead = getc(file);

                    if (input != '\\' || lookahead != '\\') {
                        DEBUG_PRINT("Invalid multiline string format\n");
                        DEBUG_PRINT("char1: %c, char2: %c\n", input, lookahead);
                        return E_LEXICAL; // Invalid multiline string format
                    }
                    // Skip valid double backslashes after a newline
                    is_newline = false;
                    input = getc(file); // Fetch next character after backslashes
                    continue;
                }

                // Append character to string
                tokenPointer->value[string_index++] = input;

                // Get the next character
                input = getc(file);
            }

            if (input == EOF) {
                return E_LEXICAL; 
            }
            
            tokenPointer->type = TOKEN_STRING_LITERAL;
            return scanner_end(input, &nextCharacter, tokenPointer, string_index); 


        case SCANNER_STRING_VALUE:
            if (input == '"') {
                assign_value = false; // poslední uvozovka se neuloží
                state = SCANNER_STRING_END;
            } else if (input == '\\') {
                assign_value = false;
                state = SCANNER_ESCAPE_SEQ;
            } else if (input < ' ')
                return E_LEXICAL;
            break;

        case SCANNER_STRING_END:
            tokenPointer->type = TOKEN_STRING_LITERAL;
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_ESCAPE_SEQ: {
            int hex1, hex2;

            switch (input) {
            case 't':
                assign_value = false;
                tokenPointer->value[string_index++] = '\t';
                state = SCANNER_STRING_VALUE;
                break;
            case 'n':
                assign_value = false;
                tokenPointer->value[string_index++] = '\n';
                state = SCANNER_STRING_VALUE;
                break;
            case 'r':
                assign_value = false;
                tokenPointer->value[string_index++] = '\r';
                state = SCANNER_STRING_VALUE;
                break;
            case '"':
                assign_value = false;
                tokenPointer->value[string_index++] = '"';
                state = SCANNER_STRING_VALUE;
                break;
            case '\\':
                assign_value = false;
                tokenPointer->value[string_index++] = '\\';
                state = SCANNER_STRING_VALUE;
                break;
            case 'x':
                hex1 = getc(file);
                if (isxdigit(hex1)) {
                    hex2 = getc(file);
                    if (isxdigit(hex2)) {
                        assign_value = false;
                        tokenPointer->value[string_index++] =
                            (char)((hex_to_int(hex1) << 4) | hex_to_int(hex2));
                        state = SCANNER_STRING_VALUE;
                    } else
                        return E_LEXICAL;
                } else
                    return E_LEXICAL;
                break;

            default:
                return E_LEXICAL;
            }
            break;
        }

        case SCANNER_MINUS:
            if (input == '-') {
                return E_LEXICAL;
            } else {
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_PLUS:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_MULTIPLY:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_DIVIDE:
            if (input == '/') {
                state = SCANNER_COMMENT;
            } else {
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_1EQUAL:
            if (input == '=') {
                state = SCANNER_EQUAL;
                tokenPointer->type = TOKEN_EQUALS;
            } else {
                tokenPointer->type = TOKEN_ASSIGN;
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_EQUAL:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_GREATER:
            if (input == '=') {
                state = SCANNER_GREATEROREQUAL;
                tokenPointer->type = TOKEN_GREATEROREQUAL;
            } else {
                tokenPointer->type = TOKEN_GREATERTHAN;
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_GREATEROREQUAL:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_LESS:
            if (input == '=') {
                state = SCANNER_LESSOREQUAL;
                tokenPointer->type = TOKEN_LESSOREQUAL;
            } else {
                tokenPointer->type = TOKEN_LESSTHAN;
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_LESSOREQUAL:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_EXCLAMATION:
            if (input == '=') {
                tokenPointer->type = TOKEN_NOTEQUAL;
                state = SCANNER_NOTEQUAL;
            } else {
                return E_LEXICAL;
            }
            break;

        case SCANNER_COMMENT:
            if (input == '\n' || input == EOF) {
                state = SCANNER_START;
                break;
            }
            break;

        case SCANNER_I32:
            if (!isDigit(input)) {
                if (input == '.') {
                    // f64 je rozdělen znakem tečka
                    state = SCANNER_I64_POINT;
                } else if (input == 'e' || input == 'E') {
                    state = SCANNER_EXP_BASE;
                } else {
                    tokenPointer->type = TOKEN_INTEGER_LITERAL;
                    return scanner_end(input, &nextCharacter, tokenPointer, string_index);
                }
            }
            break;

        case SCANNER_I64_POINT:
            if (isDigit(input))
                state = SCANNER_I64;
            else
                return E_LEXICAL;
            break;

        case SCANNER_I64:
            if (!isDigit(input)) {
                if (input == 'e' || input == 'E') {
                    state = SCANNER_EXP_BASE;
                } else {
                    tokenPointer->type = TOKEN_FLOAT_LITERAL;
                    return scanner_end(input, &nextCharacter, tokenPointer, string_index);
                }
            }
            break;

        case SCANNER_EXP_BASE:
            if (isDigit(input)) {
                state = SCANNER_EXP;
            } else if (input == '+' || input == '-') {
                state = SCANNER_EXP_SIGN;
            } else {
                return E_LEXICAL;
            }
            break;

        case SCANNER_EXP_SIGN:
            if (isDigit(input)) {
                state = SCANNER_EXP;
            } else {
                return E_LEXICAL;
            }
            break;

        case SCANNER_EXP:
            if (!(isDigit(input))) {
                tokenPointer->type = TOKEN_FLOAT_LITERAL;
                return scanner_end(input, &nextCharacter, tokenPointer, string_index);
            }
            break;

        case SCANNER_LSQUARE:
            if (input == ']')
                state = SCANNER_RSQUARE;
            else
                return E_LEXICAL;
            break;

        case SCANNER_RSQUARE:
            if (input == 'u')
                state = SCANNER_U;
            else
                return E_LEXICAL;
            break;

        case SCANNER_U:
            if (input == '8') {
                state = SCANNER_8;
                tokenPointer->type = TOKEN_U8;
            } else
                return E_LEXICAL;
            break;

        case SCANNER_8:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        case SCANNER_IDENTIFIER:
            if (!((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') ||
                  input == '_' || (isDigit(input)))) {
                /**
                 * Keywords: const, else, fn, if, i32, f64, null, pub, return, []u8, var, void,
                 * while
                 */
                tokenPointer->value[string_index] = '\0';
                nextCharacter = input;
                if (!strcmp(tokenPointer->value, "const")) {
                    tokenPointer->type = TOKEN_CONST;
                } else if (!strcmp(tokenPointer->value, "else")) {
                    tokenPointer->type = TOKEN_ELSE;
                } else if (!strcmp(tokenPointer->value, "fn")) {
                    tokenPointer->type = TOKEN_FN;
                } else if (!strcmp(tokenPointer->value, "if")) {
                    tokenPointer->type = TOKEN_IF;
                } else if (!strcmp(tokenPointer->value, "i32")) {
                    tokenPointer->type = TOKEN_I32;
                } else if (!strcmp(tokenPointer->value, "f64")) {
                    tokenPointer->type = TOKEN_F64;
                } else if (!strcmp(tokenPointer->value, "null")) {
                    tokenPointer->type = TOKEN_NULL;
                } else if (!strcmp(tokenPointer->value, "pub")) {
                    tokenPointer->type = TOKEN_PUB;
                } else if (!strcmp(tokenPointer->value, "return")) {
                    tokenPointer->type = TOKEN_RETURN;
                } else if (!strcmp(tokenPointer->value, "var")) {
                    tokenPointer->type = TOKEN_VAR;
                } else if (!strcmp(tokenPointer->value, "void")) {
                    tokenPointer->type = TOKEN_VOID;
                } else if (!strcmp(tokenPointer->value, "while")) {
                    tokenPointer->type = TOKEN_WHILE;
                } else if (!strcmp(tokenPointer->value, "ifj")) {
					tokenPointer->type = TOKEN_IFJ; 
				} else if (!strcmp(tokenPointer->value, "import")) {
					tokenPointer->type = TOKEN_IMPORT;  
				} else if (!strcmp(tokenPointer->value, "_")) {
                    tokenPointer->type = TOKEN_DELETE_VALUE;

                    // Identifikátor je poslední, jelikož se nejedná o rezervované slovo
                } else {
                    tokenPointer->type = TOKEN_IDENTIFIER;
                }
                return SUCCESS;
            }
            break;

        case SCANNER_CONCATENATE:
        case SCANNER_COLON:
        case SCANNER_NOTEQUAL:
        case SCANNER_EOF:
        case SCANNER_LPAR:
        case SCANNER_RPAR:
        case SCANNER_LBRACE:
        case SCANNER_RBRACE:
        case SCANNER_COMMA:
        case SCANNER_SEMICOLON:
        case SCANNER_PIPE:
        case SCANNER_AT:
        case SCANNER_QUESTION_MARK:
            return scanner_end(input, &nextCharacter, tokenPointer, string_index);

        default:
            return E_LEXICAL;
        }

        if (string_index + 4 >= allocated_length) {
            allocated_length += ALLOC_SIZE;
            char *temp = realloc(tokenPointer->value, allocated_length);
            if (temp == NULL)
                return E_INTERNAL;

            tokenPointer->value = temp;
        }
        // print char input
        // printf("Input: %c\n", input);

        if (assign_value == true) {
            tokenPointer->value[string_index++] = (char)input;
        }
    }
}
