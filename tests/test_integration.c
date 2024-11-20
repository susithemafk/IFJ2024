/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_integration.c
 * @date 25.10.2024
 * @brief main src file for syntat integration testing
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>  // For directory operations
#include <sys/stat.h> // For file type checks
#include "utility/enumerations.h"
#include "syntaxical/parser.h"
#include "syntaxical/parser_pass1.h"

// file name should be in this format: program_<number_of_test>_<expected_err_code>.txt
void return_err_code_and_test_number(const char *fileName, enum ERR_CODES *errCode, int *testNumber) {
    // Initialize to -1 to indicate potential parsing failure
    *errCode = (enum ERR_CODES)-1;
    *testNumber = -1;

    // Make a writable copy of the file name
    char *fileNameCopy = strdup(fileName);
    if (!fileNameCopy) {
        perror("Memory allocation failed");
        return;
    }

    // Tokenize and extract parts
    char *token = strtok(fileNameCopy, "_"); // Skip "program"
    if (!token) {
        free(fileNameCopy);
        return; // Parsing failed
    }

    token = strtok(NULL, "_"); // Extract test number
    if (!token) {
        free(fileNameCopy);
        return; // Parsing failed
    }
    *testNumber = atoi(token);

    token = strtok(NULL, "_"); // Extract error code (with .txt)
    if (!token) {
        free(fileNameCopy);
        return; // Parsing failed
    }

    // Remove the .txt suffix
    char *dot = strchr(token, '.');
    if (dot) {
        *dot = '\0'; // Null-terminate to remove .txt
    }

    // Convert the extracted error code to an enum
    int code = atoi(token);
    *errCode = (enum ERR_CODES)code;

    // Free the allocated copy
    free(fileNameCopy);
}


// function to run a test on the file
void processFile(const char *filePath, const char *fileName, TestInstancePtr test) {

    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    enum ERR_CODES exprectedErrCode;
    int testNumber;
    return_err_code_and_test_number(fileName, &exprectedErrCode, &testNumber);    

    if (testNumber == -1) {
        printf("%sfaild to execute the test, invalid file name:%s %s%s%s\n", COLOR_WARN, COLOR_RESET, COLOR_FILE, fileName, COLOR_RESET);
        return;
    }

    // Initialize the parser
    enum ERR_CODES err = parser_init(file);

    // Create the test name string
    char testName[512];
    snprintf(testName, sizeof(testName), "testing syntax on file -> %s%s%s", COLOR_INFO, fileName, COLOR_RESET);

    // Run the test case
    testCase(
        test,
        err == exprectedErrCode,
        testName,
        (exprectedErrCode == SUCCESS) ? "Syntaxical analysis passed (expected)" : "Syntax analysis failed (expected)",
        (exprectedErrCode == SUCCESS) ? "Syntaxical analysis failed (unexpected)" : "Syntax analysis passed (unexpected)"
    );

    if (err != exprectedErrCode) {
        printf("%sError code -> %s%u%s\n", COLOR_FAIL, COLOR_WARN, err, COLOR_RESET);
        printf("%sExpected code -> %s%u%s\n\n", COLOR_FAIL, COLOR_WARN, exprectedErrCode, COLOR_RESET);
    }

    fclose(file);
}

// main function, which will go through all the files in the directory, if the name is not input.txt, it will process the file
int main(void) {
    const char *directoryPath = "./tests/syntaxical/";  // Set the directory path
    const char *excludeFileName = "input.txt";

    DIR *dir = opendir(directoryPath);
    if (!dir) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    TestInstancePtr test = initTestInstance("Lexical, syntax and semantic analysis tests");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, excludeFileName) == 0) {
            // Skip excluded file
            continue;
        }

        // Construct full file path
        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, entry->d_name);

        // Check if it is a regular file
        struct stat pathStat;
        if (stat(filePath, &pathStat) == 0 && S_ISREG(pathStat.st_mode)) {
            processFile(filePath, entry->d_name, test);
        }
    }

    finishTestInstance(test);
    closedir(dir);
    return EXIT_SUCCESS;
}
