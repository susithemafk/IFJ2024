/** AUTHOR
 *
 * @author <247581> Martin Mendl
 * @file enumerations.h
 * @date 28.9. 2024
 * @brief All shared enumerations like err. codes, etc.
 */

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

#include <stdbool.h>

// ANSI color codes for console output
#define COLOR_PASS "\033[0;32m" // Green
#define COLOR_FAIL "\033[0;31m" // Red
#define COLOR_WARN "\033[0;33m" // Yellow
#define COLOR_INFO "\033[0;36m" // Cyan
#define COLOR_RESET "\033[0m"   // Reset to default
#define COLOR_FILE "\033[2;37m" // Faint White
#define COLOR_FUNC "\033[0;35m" // Magenta

/**
 * @brief Enums for error codes
 *
 * The error codes are used to identify the type of error that occured
 * They are used as return values
 */
enum ERR_CODES {
    /* no error */
    SUCCESS = 0,

    /* lexical error */
    E_LEXICAL = 1,

    /* syntax error */
    E_SYNTAX = 2,

    /* undefined function or variable */
    E_SEMANTIC_UND_FUNC_OR_VAR = 3,

    /* wrong count/type of function parameters, bad type or forbidden
       throw away of function return value */
    E_SEMANTIC_INVALID_FUN_PARAM = 4,

    /* redefinition of function or variable, assignment to immutable variable */
    E_SEMANTIC_REDIFINITION = 5,

    /* missing or too many return values, when returning from a function */
    E_SEMANTIC_BAD_FUNC_RETURN = 6,

    /* semantic mistake in compatibility of types in arithmetic, string or relational
       expressions, incompatible type (e.g., assigning string to int) */
    E_SEMANTIC_INCOMPATABLE_TYPES = 7,

    /* the type of a variable is not defined, and cannot be deduced from the context
       from the used expression */
    E_SEMANTIC_UNKNOWN_TYPE = 8,

    /* unused variable in their function scope, modifiable variable without the
       possibility of modifying their value, after their initialization */
    E_SEMANTIC_UNUSED_VAR = 9,

    /* other semantic errors, not covered by the previous categories */
    E_SEMANTIC_OTHER = 10,

    /* No Error, used internally */
    E_NONE = 11,

    /* internal error of the compiler, that is a mistake, not caused by the source code
       e.g., memory allocation error, accessing out of bounds, etc. */
    E_INTERNAL = 99
};

enum TOKEN_TYPE {
    TOKEN_NONE,       // 0. No token
    TOKEN_IDENTIFIER, // 1. Název proměnné, funkce, ...
    TOKEN_EOF,        // 2. End of file

    TOKEN_PLUS,     // 3. +
    TOKEN_MINUS,    // 4. -
    TOKEN_DIVIDE,   // 5. /
    TOKEN_MULTIPLY, // 6. *
    TOKEN_EQUALS,   // 7. ==

    TOKEN_LESSTHAN,       // 8. <
    TOKEN_LESSOREQUAL,    // 9. <=
    TOKEN_GREATERTHAN,    // 10. >
    TOKEN_GREATEROREQUAL, // 11. >=
    TOKEN_NOTEQUAL,       // 12. !=

    TOKEN_STRING_LITERAL,  // 13. "string"
    TOKEN_INTEGER_LITERAL, // 14. 123
    TOKEN_FLOAT_LITERAL,   // 15. 123.456

    TOKEN_CONCATENATE,   // 16. .
    TOKEN_ASSIGN,        // 17. =
    TOKEN_COLON,         // 18. :
    TOKEN_COMMA,         // 19. ,
    TOKEN_LPAR,          // 20. (
    TOKEN_RPAR,          // 21. )
    TOKEN_LBRACE,        // 22. {
    TOKEN_RBRACE,        // 23. }
    TOKEN_SEMICOLON,     // 24. ;
    TOKEN_PIPE,          // 25. |
    TOKEN_AT,            // 26. @
    TOKEN_DELETE_VALUE,  // 27. delete value ('_')
    TOKEN_QUESTION_MARK, // 28. question mark ('?')

    /**
     * Keywords: const, else, fn, if, i32, f64, null, pub, return, u8, var, void, while
     */
    TOKEN_VAR,    // 29. var
    TOKEN_CONST,  // 30. const
    TOKEN_IF,     // 31. if
    TOKEN_ELSE,   // 32. else
    TOKEN_RETURN, // 33. return
    TOKEN_WHILE,  // 34. while
    TOKEN_FN,     // 35. fn
    TOKEN_NULL,   // 36. null
    TOKEN_PUB,    // 37. pub

    TOKEN_VOID, // 38. void
    TOKEN_I32,  // 39. i32
    TOKEN_F64,  // 40. f64
    TOKEN_U8,   // 41. []u8
};

typedef struct TOKEN {
    char *value;          // Token value
    enum TOKEN_TYPE type; // Token type
} *TOKEN_PTR;

enum DATA_TYPES {
    dTypeUndefined, // 0. Undefined data type (used internaly to indacate ignoring of the tyep)
    dTypeNone,      // 1. No data type
    dTypeI32,       // 2. Integer 32 bit
    dTypeF64,       // 3. Float 64 bit
    dTypeU8,        // 4. Unsigned 8 bit
    dTypeVoid,      // 5. Void
    dTypeBool,      // 6. Boolean (used internaly)
};

/**
 * Function to negate a comparason operand
 *
 * @param operand - token type to negate
 * @return The negated token type
 */
enum TOKEN_TYPE negateOperand(enum TOKEN_TYPE operand);

/**
 * @brief Checks if the given token type is a literal.
 * 
 * @param type The token type to check.
 * @return true if the token type is a literal, false otherwise.
 */
bool isLiteral(enum TOKEN_TYPE type);


/**
 * @brief Checks if the given token type is a data type.
 * 
 * @param type The token type to check.
 * @return true if the token type is a data type, false otherwise.
 */
bool isDataType(enum TOKEN_TYPE type);

/**
 * Function to convert a token type to a data type
 *
 * @param type The token type to convert
 * @return The converted data type
 */
enum DATA_TYPES covertTokneDataType(enum TOKEN_TYPE type);

// Debug macro
#define DEBUG_MSG(message) printDebug(__FILE__, __func__, message)

/**
 * Function to print out a debug message
 *
 * @param file_name The name of the file
 * @param message The message to print
 * @return void
 */
void printDebug(const char *file_name, const char *function_name, const char *message);

/**
 * Function to hash a string
 *
 * @param str The string to hash
 * @return The hash of the string
 */
unsigned int hashString(const char *str);

typedef struct TestInstance {
    char *testName;
    unsigned int testNumber;
    unsigned int fails;
    unsigned int passes;
} *TestInstancePtr;

/**
 *  Function to create a new test instance
 *
 * @param testName The name of the test
 * @return The new test instance
 */
TestInstancePtr initTestInstance(char *testName);

/**
 * Function to finish the test instance
 *
 * @param testInstance The test instance to finish
 */
void finishTestInstance(TestInstancePtr testInstance);

/**
 * Function to make it ez to create a testn case
 *
 * @param testInstance A pointer to an unsigned int that will be incremented
 * @param testResult The result of the test
 * @param testName The name of the test
 * @param testResultPassMsg The message to print if the test passes
 * @param testResultFailMsg The message to print if the test fails
 */
void testCase(TestInstancePtr testInstance, bool testResult, char *testName,
              char *testResultPassMsg, char *testResultFailMsg);

/**
 * Function to print the error code
 *
 * @param errCode The error code to print
 * @return void
 */
void printErrCode(enum ERR_CODES errCode);

/**
 * Function to print the token type
 *
 * @param token The token to print
 * @return void
 */
void printTokenType(enum TOKEN_TYPE token);

#endif // ENUMERATIONS_H
