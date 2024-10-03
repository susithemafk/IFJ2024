/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file enumerations.h
 * @date 28.9. 2024
 * @brief All shared enumerations like err. codes, etc.
 */

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

/**
 * @brief Enums for error codes
 * 
 * The error codes are used to identify the type of error that occured
 * They are used as return values 
 */
enum ERR_CODES {
    E_NONE = 0, /*
        no error
    */
    E_LEXICAL = 1, /*
        lexical error
    */ 
    E_SYNTAX = 2, /*
        syntax error 
    */ 
    E_SEMANTIC_UND_FUNC_OR_VAR = 3, /*
        undefined function or variable
    */ 
    E_SEMANTIC_INVALID_FUN_PARAM = 4, /* 
        wrong count/type of function parameters, bad type or forbitten 
        throw away of function return value
    */
    E_SEMANTIC_REDIFINITION = 5, /*
        redefinition of function or variable, assignment to imutable variable
    */ 
    E_SEMANTIC_BAD_FUNC_RETURN = 6, /*
        missing or to many return values, when returning from a function
    */
    E_SEMANTIC_INCOMPATABLE_TYPES = 7, /*
        semantic mistake in compatability of types in arithmetic, string or relational
        expressions, incompatable type (eg. assigning string to int)
    */
    E_SEMANTIC_UNKNOWN_TYPE = 8, /*
        the type of a variable is not defined, and cannot be deduced form the context
        form the used expression
    */
    E_SEMANTIC_UNUSED_VAR = 9, /*
        unused variable in their function scope, modifiable variable without the 
        possibility of modifiing their value, after their initialization
    */
    E_SEMANTIC_OTHER = 10, /*
        just other semantic errors, not covered by the previous categories
    */
    E_INTERNAL = 99 /*
        internal error of the compiler, that is a mistake, not caused by the source code
        eg. memory allocation error, accesing out of bounds, etc.
    */
};

/**
 * @brief Struct for token, representing a meaningful part of the code
 */
typedef struct TOKEN {
    enum KEY_WORDS_TYPES keyWord; // the key word of the token
    char* value; // the value of the token
} TOKEN;

/**
 * @brief Enums for key words
 * @note used in condunction with some functions
 * @note WT stands for Word Type
 */
enum KEY_WORDS_TYPES {
    WT_CONST = 0,
    WT_ELSE = 1,
    WT_FN = 2,
    WT_IF = 3,
    WT_I32 = 4,
    WT_F64 = 5,
    WT_NULL = 6,
    WT_PUB = 7,
    WT_RETURN = 8,
    WT_U8 = 9,
    WT_VAR = 10,
    WT_VOID = 11,
    WT_WHILE = 12,
    WT_UNKNOWN = 13
};

enum DATA_TYPES {
    dTypeI32,
    dTypeF64,
    dTypeU8
};

/**
 * Return the coresponding string for the key word
 * 
 * @param keyWord The key, for which the string should be returned
 * @return The string representation of the key word
 */
char *_getKeyWordFromString(enum KEY_WORDS_TYPES keyWord);

/**
 * Return the coresponding enum keyWord for the string
 * 
 * @param keyWord The string, for which the key should be returned
 * @return The enum representation of the key word, or -1 if the key word is not found
 */
enum KEY_WORDS_TYPES _getEnumfromKeyWord(char *keyWord);

/**
 * Function to hash a string
 * 
 * @param str The string to hash
 * @return The hash of the string
*/
unsigned int hashString(const char* str);

#endif // ENUMERATIONS_H
