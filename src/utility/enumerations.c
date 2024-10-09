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


