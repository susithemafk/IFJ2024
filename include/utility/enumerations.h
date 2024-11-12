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
#define COLOR_RESET "\033[0m"   // Reset to default

/**
 * @brief Enums for error codes
 *
 * The error codes are used to identify the type of error that occured
 * They are used as return values
 */
enum ERR_CODES {
    SUCCESS = 0, /*
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
    E_SEMANTIC_UNKNOWN_TYPE = 8,       /*
              the type of a variable is not defined, and cannot be deduced form the context
              form the used expression
          */
    E_SEMANTIC_UNUSED_VAR = 9,         /*
                unused variable in their function scope, modifiable variable without the
                possibility of modifiing their value, after their initialization
            */
    E_SEMANTIC_OTHER = 10,             /*
                    just other semantic errors, not covered by the previous categories
                */
    E_NONE = 11,                        /*
                    No Error, used internally
                */

    E_INTERNAL = 99                    /*
                           internal error of the compiler, that is a mistake, not caused by the source code
                           eg. memory allocation error, accesing out of bounds, etc.
                       */
};

enum TOKEN_TYPE
{
	TOKEN_NONE,		  // 0. No token
	TOKEN_IDENTIFIER, // 1. Název proměnné, funkce, ...
	TOKEN_EOF,		  // 2. End of file

	TOKEN_PLUS,		// 3. +
	TOKEN_MINUS,	// 4. -
	TOKEN_DIVIDE,	// 5. /
	TOKEN_MULTIPLY, // 6. *
	TOKEN_EQUALS,	// 7. ==

	TOKEN_LESSTHAN,		  // 8. <
	TOKEN_LESSOREQUAL,	  // 9. <=
	TOKEN_GREATERTHAN,	  // 10. >
	TOKEN_GREATEROREQUAL, // 11. >=
	TOKEN_NOTEQUAL,		  // 12. !=

	TOKEN_STRING, // 13. "string"

	TOKEN_CONCATENATE, // 14. .
	TOKEN_ASSIGN,	   // 15. =
	TOKEN_COLON,	   // 16. :
	TOKEN_COMMA,	   // 17. ,
	TOKEN_LPAR,		   // 18. (
	TOKEN_RPAR,		   // 19. )
	TOKEN_LBRACE,	   // 20. {
	TOKEN_RBRACE,	   // 21. }
	TOKEN_SEMICOLON,   // 22. ;

	/**
	 * Keywords: const, else, fn, if, i32, f64, null, pub, return, u8, var, void, while
	 */
	TOKEN_CONST,  // 23. const
	TOKEN_ELSE,	  // 24. else
	TOKEN_FN,	  // 25. fn
	TOKEN_IF,	  // 26. if
	TOKEN_I32,	  // 27. i32
	TOKEN_F64,	  // 28. f64
	TOKEN_NULL,	  // 29. null
	TOKEN_PUB,	  // 30. pub
	TOKEN_RETURN, // 31. return
	TOKEN_U8,	  // 32. u8
	TOKEN_VAR,	  // 33. var
	TOKEN_VOID,	  // 34. void
	TOKEN_WHILE,  // 35. while
    TOKEN_DELETE_VALUE, // 36 delte value ('_')
};

typedef struct TOKEN
{
    char *value;          // Token value
    enum TOKEN_TYPE type; // Token type
} *TOKEN_PTR;


enum DATA_TYPES {
    dTypeUndefined, // 0. Undefined data type (used internaly to indacate ignoring of the tyep)
    dTypeNone, // 1. No data type
    dTypeI32, // 2. Integer 32 bit
    dTypeF64, // 3. Float 64 bit
    dTypeU8, // 4. Unsigned 8 bit
    dTypeVoid // 5. Void
};

/**
 * Function to negate a comparason operand
 * 
 * @param operand - token type to negate
 * @return The negated token type
*/
enum TOKEN_TYPE negateOperand(enum TOKEN_TYPE operand);

/**
 * Function to convert a token type to a data type
 * 
 * @param type The token type to convert
 * @return The converted data type
*/
enum DATA_TYPES covertTokneDataType(enum TOKEN_TYPE type);


/**
 * Function to hash a string
 *
 * @param str The string to hash
 * @return The hash of the string
 */
unsigned int hashString(const char *str);

typedef struct TestInstance
{
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
void testCase(TestInstancePtr testInstance, bool testResult, char *testName, char *testResultPassMsg, char *testResultFailMsg);

#endif // ENUMERATIONS_H
