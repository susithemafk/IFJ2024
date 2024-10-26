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


