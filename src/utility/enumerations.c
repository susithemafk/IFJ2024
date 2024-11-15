/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file enumerations.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include "utility/enumerations.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

// Function to print out a human-readable error code with colors
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

// Function to print out a debug message
void printDebug(const char *file_name, const char *function_name, const char *message) {
    printf("%sDEBUG:%s %sFILE: %s%s \n%sFUNC: %s%s \n%smsg: %s %s\n", 
            COLOR_INFO,       // Color for DEBUG label
            COLOR_RESET,      // Reset color after DEBUG label
            COLOR_FILE,       // Color for FILE label
            file_name,        // File name passed as argument
            COLOR_RESET,      // Reset color after file name
            COLOR_FUNC,       // Color for FUNC label
            function_name,    // Function name passed as argument
            COLOR_RESET,      // Reset color after function name
            COLOR_PASS,       // Color for the "msg->" prefix
            COLOR_RESET,      // Reset color after "msg->"
            message);         // The actual message
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
        case TOKEN_I32:
            return dTypeI32;
        case TOKEN_F64:
            return dTypeF64;
        case TOKEN_U8:
            return dTypeU8;
        default:
            return dTypeNone;
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


