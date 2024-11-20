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
#include <ctype.h>
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

#define MAX_FILES 1024  // Maximum number of files to process

// Helper function to extract the test number from the file name
int extractTestNumber(const char *fileName) {
    const char *prefix = "test_";
    const char *ptr = strstr(fileName, prefix);
    if (!ptr) {
        return -1; // Return an invalid number if the prefix is not found
    }
    ptr += strlen(prefix); // Move past "test_"
    
    // Extract the numeric part
    int testNumber = 0;
    while (isdigit(*ptr)) {
        testNumber = testNumber * 10 + (*ptr - '0');
        ptr++;
    }
    return testNumber;
}

// Comparison function for sorting by test number
int compareFileNames(const void *a, const void *b) {
    const char *fileA = *(const char **)a;
    const char *fileB = *(const char **)b;

    int numA = extractTestNumber(fileA);
    int numB = extractTestNumber(fileB);

    // Compare based on extracted test numbers
    return numA - numB;
}

// Main function
int main(void) {
    const char *directoryPath = "./tests/syntaxical/";  // Set the directory path
    const char *excludeFileName = "input.txt";

    DIR *dir = opendir(directoryPath);
    if (!dir) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    TestInstancePtr test = initTestInstance("Lexical, syntax and semantic analysis tests");

    // Collect file names
    char *fileNames[MAX_FILES];
    size_t fileCount = 0;

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
            // Allocate memory for the file name and store it
            fileNames[fileCount] = strdup(entry->d_name);
            if (!fileNames[fileCount]) {
                perror("Error allocating memory");
                closedir(dir);
                return EXIT_FAILURE;
            }
            fileCount++;
        }

        if (fileCount >= MAX_FILES) {
            fprintf(stderr, "Error: Too many files in directory.\n");
            closedir(dir);
            return EXIT_FAILURE;
        }
    }
    closedir(dir);

    // Sort file names alphabetically
    qsort(fileNames, fileCount, sizeof(char *), compareFileNames);

    // Process each file in sorted order
    for (size_t i = 0; i < fileCount; i++) {
        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, fileNames[i]);
        processFile(filePath, fileNames[i], test);

        // Free the allocated memory for the file name
        free(fileNames[i]);
    }

    finishTestInstance(test);
    return EXIT_SUCCESS;
}
