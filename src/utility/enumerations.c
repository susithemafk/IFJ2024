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

char *_getKeyWordFromString(enum KEY_WORDS_TYPES keyWord) {
    switch (keyWord) {
        case WT_CONST:
            return "const";
        case WT_ELSE:
            return "else";
        case WT_FN:
            return "fn";
        case WT_IF:
            return "if";
        case WT_I32:
            return "i32";
        case WT_F64:
            return "f64";
        case WT_NULL:
            return "null";
        case WT_PUB:
            return "pub";
        case WT_RETURN:
            return "return";
        case WT_U8:
            return "u8";
        case WT_VAR:
            return "var";
        case WT_VOID:
            return "void";
        case WT_WHILE:
            return "while";
        default:
            return "unknown";
    }
}

enum KEY_WORDS_TYPES _getEnumfromKeyWord(char *keyWord) {

    if (strcmp(keyWord, "const") == 0) {
        return WT_CONST;
    } else if (strcmp(keyWord, "else") == 0) {
        return WT_ELSE;
    } else if (strcmp(keyWord, "fn") == 0) {
        return WT_FN;
    } else if (strcmp(keyWord, "if") == 0) {
        return WT_IF;
    } else if (strcmp(keyWord, "i32") == 0) {
        return WT_I32;
    } else if (strcmp(keyWord, "f64") == 0) {
        return WT_F64;
    } else if (strcmp(keyWord, "null") == 0) {
        return WT_NULL;
    } else if (strcmp(keyWord, "pub") == 0) {
        return WT_PUB;
    } else if (strcmp(keyWord, "return") == 0) {
        return WT_RETURN;
    } else if (strcmp(keyWord, "u8") == 0) {
        return WT_U8;
    } else if (strcmp(keyWord, "var") == 0) {
        return WT_VAR;
    } else if (strcmp(keyWord, "void") == 0) {
        return WT_VOID;
    } else if (strcmp(keyWord, "while") == 0) {
        return WT_WHILE;
    }

    return -1;
}

unsigned int hashString(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)hash;
}

TestInstancePtr initTestInstance(char *testName) {
    TestInstancePtr testInstance = (TestInstancePtr)malloc(sizeof(struct TestInstance));
    testInstance->testNumber = 0;
    testInstance->passes = 0;
    testInstance->fails = 0;
    printf("\n\n%s=========== %s ==========%s\n", COLOR_WARN, testName, COLOR_RESET);
    return testInstance;
}

void finishTestInstance(TestInstancePtr testInstance) {
    printf("\n%s=========== SUMMARY ==========%s\n", COLOR_WARN, COLOR_RESET);
    printf("%sPASSED%s: %u/%u\n", COLOR_PASS, COLOR_RESET, testInstance->passes, testInstance->testNumber);
    printf("%sFAILED%s: %u/%u\n", COLOR_FAIL, COLOR_RESET, testInstance->fails, testInstance->testNumber);
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


