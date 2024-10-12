/**
 * @file lexical_test.c
 * @date 9.10. 2024
 * @author <247581> Martin Mendl
 * @brief Test file for the lexical analysis
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utility/enumerations.h"
#include "lexical/scanner.h"

#define MAX_TOKEN_LEN 100
#define SUCCESS 0


char* read_line(FILE *file) {
    static char buffer[MAX_TOKEN_LEN];
    
    if (fgets(buffer, MAX_TOKEN_LEN, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        return buffer;
    }
    
    return NULL; // Return NULL if no more lines to read
}


// Custom test macros (adjust as needed)
TestInstancePtr testInstance;

// Function to compare tokens from the lexical analyzer and expected output
void compare_tokens(FILE *expected_file) {
    static struct TOKEN token;
    enum ERR_CODES status;
    char expected_token[MAX_TOKEN_LEN];

    // Initialize scanner
    FILE *input = stdin;
    scanner_init(input);

    // Initialize test instance
    testInstance = initTestInstance("Lexical Analyzer Token Comparison Test");

    // Read the first line from the expected file
    char *expectedLine = read_line(expected_file);

    // First token from lexical analyzer
    status = scanner_get_token(&token);

    // Loop through tokens from both lexical analyzer and expected file
    while (expectedLine != NULL) {
        // Copy expected token into expected_token
        strncpy(expected_token, expectedLine, MAX_TOKEN_LEN);

        // Prepare the error message for token comparison
        char tokenErrorMessage[256]; // Adjust size as necessary
        snprintf(tokenErrorMessage, sizeof(tokenErrorMessage), 
                 "Token does not match expected token: Expected '%s', but got '%s'",
                 expected_token, token.value);

        // Test case: Compare each token
        testCase(
            testInstance,
            strcmp(token.value, expected_token) == 0,
            expected_token,
            "Token matches expected token",
            tokenErrorMessage // Pass the formatted error message
        );

        // Prepare the error message for status check
        char statusErrorMessage[256]; // Adjust size as necessary
        snprintf(statusErrorMessage, sizeof(statusErrorMessage), 
                 "Token read unsuccessful: Expected status SUCCESS but got '%d'",
                 status);

        // Test if the token was read successfully
        testCase(
            testInstance,
            status == SUCCESS,
            "Test if token was read successfully",
            "Token read successfully",
            statusErrorMessage // Pass the formatted error message
        );

        // Get the next token from lexical analyzer and file
        status = scanner_get_token(&token);
        expectedLine = read_line(expected_file);
    }

    // Finish test instance
    finishTestInstance(testInstance);
}


int main(void) {
    // Open the expected tokens file
    FILE *expected_file = fopen("./src/expected_lex_out.txt", "r");
    if (expected_file == NULL) {
        perror("Error opening expected token file");
        return 1;
    }

    // Compare tokens from the analyzer with the expected output
    compare_tokens(expected_file);

    // Close the expected file
    fclose(expected_file);

    return 0;
}
