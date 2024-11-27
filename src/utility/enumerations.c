/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 * @file enumerations.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef USE_CUSTOM_STRUCTURE
#include "utility/enumerations.h"
#else
#include "enumerations.h"
#endif

// Function to negate a comparason operand
enum TOKEN_TYPE negateOperand(enum TOKEN_TYPE operand) {
    switch(operand) {
        case TOKEN_EQUALS: // == => !=
            return TOKEN_NOTEQUAL;
        case TOKEN_NOTEQUAL: // != => ==
            return TOKEN_EQUALS;
        case TOKEN_GREATERTHAN: // > => <=
            return TOKEN_LESSOREQUAL;
        case TOKEN_LESSOREQUAL: // <= => >
            return TOKEN_GREATERTHAN;
        case TOKEN_LESSTHAN: // < => >=
            return TOKEN_GREATEROREQUAL;
        case TOKEN_GREATEROREQUAL: // >= => <
            return TOKEN_LESSTHAN;
        default:
            return TOKEN_NONE;
    }
}

bool isLiteral(enum TOKEN_TYPE type) {
    switch(type) {
        case TOKEN_INTEGER_LITERAL:
        case TOKEN_STRING_LITERAL:
        case TOKEN_FLOAT_LITERAL:
        case TOKEN_NULL:
            return true;
        default:
            return false;
    }
} // Function to print out a human-readable error code with colors

bool isDataType(enum TOKEN_TYPE type) {
    switch(type) {
        case TOKEN_I32:
        case TOKEN_F64:
        case TOKEN_U8:
        case TOKEN_VOID:
            return true;
        default:
            return false;
    }
}

void printErrCode(enum ERR_CODES errCode) {
    char errStr[100];
    const char *color = COLOR_RESET; // Default color

    switch(errCode) {
        case SUCCESS:
            strcpy(errStr, "SUCCESS");
            color = COLOR_PASS; // Green for success
            break;
        case E_LEXICAL:
            strcpy(errStr, "LEXICAL");
            color = COLOR_FAIL; // Red for errors
            break;
        case E_SYNTAX:
            strcpy(errStr, "SYNTAX");
            color = COLOR_FAIL; // Red for syntax errors
            break;
        case E_SEMANTIC_UND_FUNC_OR_VAR:
            strcpy(errStr, "SEMANTIC_UND_FUNC_OR_VAR");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_INVALID_FUN_PARAM:
            strcpy(errStr, "SEMANTIC_INVALID_FUN_PARAM");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_REDIFINITION:
            strcpy(errStr, "SEMANTIC_REDIFINITION");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_BAD_FUNC_RETURN:
            strcpy(errStr, "SEMANTIC_BAD_FUNC_RETURN");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_INCOMPATABLE_TYPES:
            strcpy(errStr, "SEMANTIC_INCOMPATABLE_TYPES");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_UNKNOWN_TYPE:
            strcpy(errStr, "SEMANTIC_UNKNOWN_TYPE");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_SEMANTIC_UNUSED_VAR:
            strcpy(errStr, "SEMANTIC_UNUSED_VAR");
            color = COLOR_WARN; // Yellow for warnings
            break;
        case E_SEMANTIC_OTHER:
            strcpy(errStr, "SEMANTIC_OTHER");
            color = COLOR_FAIL; // Red for semantic errors
            break;
        case E_NONE:
            strcpy(errStr, "NONE");
            color = COLOR_WARN; // Yellow for no error (special case)
            break;
        case E_INTERNAL:
            strcpy(errStr, "INTERNAL");
            color = COLOR_FAIL; // Red for internal errors
            break;
        default:
            strcpy(errStr, "UNKNOWN");
            color = COLOR_WARN; // Yellow for unknown errors
            break;
    }

    printf("Error -> %s%s%s\n", color, errStr, COLOR_RESET);
}
// Debug print function
void printDebug(const char *function_name, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("%sDEBUG:%s %sFUNC:%s %s %smsg%s -> ", 
           COLOR_INFO, COLOR_RESET, 
           COLOR_FUNC, function_name, COLOR_RESET, COLOR_PASS, COLOR_RESET);
    vprintf(format, args); // Print the formatted message
    printf(" %s\n", COLOR_RESET);
    va_end(args);
}


// Function to hash a string
unsigned int hashString(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)hash;
}

// Function to convert token type to data type
enum DATA_TYPES covertTokneDataType(enum TOKEN_TYPE type) {

    switch (type) {
        case TOKEN_VOID:
        case TOKEN_NULL:
            return dTypeVoid;
        case TOKEN_I32:
        case TOKEN_INTEGER_LITERAL:
            return dTypeI32;
        case TOKEN_F64:
        case TOKEN_FLOAT_LITERAL:
            return dTypeF64;
        case TOKEN_U8:
        case TOKEN_STRING_LITERAL:
            return dTypeU8;
        default:
            return dTypeNone;
    }
}

// Function to print out a token type
void printTokenType(enum TOKEN_TYPE token) {
    switch (token) {
        case TOKEN_NONE:
            printf("Token -> %sNONE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_IDENTIFIER:
            printf("Token -> %sIDENTIFIER%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_EOF:
            printf("Token -> %sEOF%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_PLUS:
            printf("Token -> %sPLUS%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_MINUS:
            printf("Token -> %sMINUS%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_DIVIDE:
            printf("Token -> %sDIVIDE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_MULTIPLY:
            printf("Token -> %sMULTIPLY%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_EQUALS:
            printf("Token -> %sEQUALS%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_LESSTHAN:
            printf("Token -> %sLESSTHAN%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_LESSOREQUAL:
            printf("Token -> %sLESSOREQUAL%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_GREATERTHAN:
            printf("Token -> %sGREATERTHAN%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_GREATEROREQUAL:
            printf("Token -> %sGREATEROREQUAL%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_NOTEQUAL:
            printf("Token -> %sNOTEQUAL%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_STRING_LITERAL:
            printf("Token -> %sSTRING%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_CONCATENATE:
            printf("Token -> %sCONCATENATE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_ASSIGN:
            printf("Token -> %sASSIGN%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_COLON:
            printf("Token -> %sCOLON%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_COMMA:
            printf("Token -> %sCOMMA%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_LPAR:
            printf("Token -> %sLPAR%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_RPAR:
            printf("Token -> %sRPAR%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_LBRACE:
            printf("Token -> %sLBRACE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_RBRACE:
            printf("Token -> %sRBRACE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_SEMICOLON:
            printf("Token -> %sSEMICOLON%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_CONST:
            printf("Token -> %sCONST%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_ELSE:
            printf("Token -> %sELSE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_FN:
            printf("Token -> %sFN%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_IF:
            printf("Token -> %sIF%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_I32:
            printf("Token -> %sI32%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_F64:
            printf("Token -> %sF64%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_NULL:
            printf("Token -> %sNULL%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_PUB:
            printf("Token -> %sPUB%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_RETURN:
            printf("Token -> %sRETURN%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_U8:
            printf("Token -> %sU8%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_VAR:
            printf("Token -> %sVAR%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_VOID:
            printf("Token -> %sVOID%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_WHILE:
            printf("Token -> %sWHILE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        case TOKEN_DELETE_VALUE:
            printf("Token -> %sDELETE_VALUE%s\n", COLOR_WARN, COLOR_RESET);
            break;
        default:
            printf("Token -> %sUNKNOWN%s\n", COLOR_WARN, COLOR_RESET);
            break;
    }
}

// Function to initialize a test instance
TestInstancePtr initTestInstance(char *testName) {
    TestInstancePtr testInstance = (TestInstancePtr)malloc(sizeof(struct TestInstance));
    testInstance->testNumber = 1;
    testInstance->passes = 0;
    testInstance->fails = 0;
    printf("\n\n%s=========== %s ==========%s\n", COLOR_WARN, testName, COLOR_RESET);
    return testInstance;
}

// Function to finish a test instance
void finishTestInstance(TestInstancePtr testInstance) {
    printf("\n%s=========== SUMMARY ==========%s\n", COLOR_WARN, COLOR_RESET);
    printf("%sPASSED%s: %u/%u\n", COLOR_PASS, COLOR_RESET, testInstance->passes, testInstance->testNumber-1);
    printf("%sFAILED%s: %u/%u\n", COLOR_FAIL, COLOR_RESET, testInstance->fails, testInstance->testNumber-1);
    if (testInstance->fails == 0) {
        printf("%s** ALL TESTS PASSED **%s\n", COLOR_PASS, COLOR_RESET);
        printf("Note: This is the part where we do a happy dance!\n");
    } else {
        printf("%sSOME TESTS FAILED%s\n", COLOR_FAIL, COLOR_RESET);
        printf("What are you doing here ?? !! Get back to work ...\n");
    }
    free(testInstance);
}

// Function to make it easier to test any function
void testCase(TestInstancePtr testInstance, bool testResult, char *testName, char *testResultPassMsg, char *testResultFailMsg) {
    // Print the test name
    printf("TEST %u: %s\n", testInstance->testNumber, testName);
    testInstance->testNumber++;

    // Check the test result and print either PASS or FAIL with appropriate color
    if (testResult) {
        printf("  %sPASS%s: %s\n", COLOR_PASS, COLOR_RESET, testResultPassMsg);
        testInstance->passes++;
    } else {
        printf("  %sFAIL%s: %s\n", COLOR_FAIL, COLOR_RESET, testResultFailMsg);
        testInstance->fails++;
    }
    printf("\n");
}


