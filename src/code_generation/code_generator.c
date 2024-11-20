
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "semantical/sem_enums.h"
#include "utility/linked_list.h"
#include "code_generation/code_generator.h"

// Function to init the return adress tracker
ReturnAdressTrackerPtr initReturnAdressTracker(void) {

    // alocate the memory
    ReturnAdressTrackerPtr tracker = (ReturnAdressTrackerPtr)malloc(sizeof(struct ReturnAdressTracker));
    if (tracker == NULL) return NULL;

    // init the list
    tracker->returnAdresses = initLinkedList(true);
    if (tracker->returnAdresses == NULL) {
        free(tracker);
        return NULL;
    }

    tracker->returnAdressCount = 0;

    return tracker;
}

// Function to free the return adress tracker
bool freeReturnAdressTracker(ReturnAdressTrackerPtr *returnAdressTracker) {

    // check if the input is valid
    if (returnAdressTracker == NULL || *returnAdressTracker == NULL) return false;

    // free the list
    removeList(&(*returnAdressTracker)->returnAdresses);

    // free the tracker
    free(*returnAdressTracker);
    *returnAdressTracker = NULL;

    return true;
}

// Function to generate the result code handler
ResultCodeHandlerPtr initResultCodehandler(bool print) {

    // alocate the memory
    ResultCodeHandlerPtr handler = (ResultCodeHandlerPtr)malloc(sizeof(struct ResultCodeHandler));
    if (handler == NULL) return NULL;

    // init the code
    handler->code = NULL;
    handler->print = print;
    handler->returnAdressTracker = initReturnAdressTracker();

    if (handler->returnAdressTracker == NULL) {
        free(handler);
        return NULL;
    }

    return handler;
}

// Function to free the result code handler
bool freeResultCodehandler(ResultCodeHandlerPtr *resultCodehandler) {

    // check if the input is valid
    if (resultCodehandler == NULL || *resultCodehandler == NULL) return false;

    // free the code
    if ((*resultCodehandler)->code != NULL) free((*resultCodehandler)->code);

    // free the return adress tracker
    freeReturnAdressTracker(&(*resultCodehandler)->returnAdressTracker);

    // free the handler
    free(*resultCodehandler);
    *resultCodehandler = NULL;

    return true;
}

// Function to fetch the current code
bool fetchCode(ResultCodeHandlerPtr resultCodehandler, char **code) {

    // check if the input is valid
    if (resultCodehandler == NULL || code == NULL) return false;

    strcpy(*code, resultCodehandler->code);
    if (*code == NULL) return false;

    return true;
}

// Function to handle new code
bool handleNewCode(ResultCodeHandlerPtr resultCodehandler, char *code) {

    // check if the input is valid
    if (resultCodehandler == NULL || code == NULL) return false;

    // free the currentaly stored code
    if (resultCodehandler->code != NULL) free(resultCodehandler->code);

    // print the code if needed
    if (resultCodehandler->print) {
        printf("%s", code);
        return true;
    }

    // store the new code (it was malloced somewhere else, no need to malloc)
    resultCodehandler->code = code;

    return true;
}

// Function to generate the prolog
char *generateProlog(void) {

    /*
    global variables will go here?
    jshould jump to main function
    than exit(0);
    */

   return NULL;

}

// Function to generate soem code based on the AST
bool generateCode(ResultCodeHandlerPtr codeGenhHandler, ASTNodePtr *currentFucn, ASTNodePtr ast) {

    // check if the call is valid
    if (codeGenhHandler == NULL || ast == NULL) return false;

    char *generatedCode = NULL;

    // switch based on the type of the AST
    switch (ast->type) {
        case AST_NODE_FUNCTION:
            *currentFucn = ast;
            generatedCode = _generateFunction(ast);
            break;
        case AST_NODE_RETURN:
            generatedCode = _generateReturn(ast);
            break;
        case AST_NODE_DECLARE:
            generatedCode = _generateDeclare(ast);
            break;
        case AST_NODE_ASSIGN:
            generatedCode = _generateAssign(ast);
            break;
        case AST_NODE_FUNC_CALL:
            generatedCode = _generateFunctionCall(ast);
            break;
        case AST_NODE_IF_ELSE:
            generatedCode = _generateIF(codeGenhHandler->returnAdressTracker, ast);
            break;
        case AST_NODE_WHILE:
            generatedCode = _generateWhile(codeGenhHandler->returnAdressTracker, ast);
            break;
        default: // for safety
            return false;
    }

    // check if the code was generated
    if (generatedCode == NULL) return false;

    // handle the new code
    if (!handleNewCode(codeGenhHandler, generatedCode)) {
        free(generatedCode);
        return false;
    }

    return false;
}

// Function to enter a function
char *_generateFunction(ASTNodePtr ast) {
    /*
    add the function name as the lable
    */
    ast = NULL;
    return NULL;
}

// Function to generate the return from a function
char *_generateReturn(ASTNodePtr ast) {
    /*
    exit from the current function frame
    */
    ast = NULL;
    return NULL;
}

// Function to generate the end of a void function
char *generateEndOfFunction(void) {
    return NULL;
}

// Function to generate Declare node
char *_generateDeclare(ASTNodePtr ast) {
    ast = NULL;
    return NULL;
}

// Function to generate Assign node
char *_generateAssign(ASTNodePtr ast) {
    ast = NULL;
    return NULL;
}

// Function to escape special characters in a string
char *escapeString(const char *input) {
    if (input == NULL) return NULL;

    // Allocate a buffer large enough for the escaped string
    // Worst case: every character is escaped (3x original size + 1 for null terminator)
    size_t len = strlen(input);
    size_t maxLen = len * 4 + 1; // Each char might expand to \xyz (4 chars including '\')
    char *escaped = malloc(maxLen);
    if (escaped == NULL) return NULL;

    size_t j = 0; // Index for the escaped string
    for (size_t i = 0; i < len; i++) {
        unsigned char c = input[i];
        if ((c <= 32) || (c == 35) || (c == 92)) {
            // Special character: replace with \xyz
            j += snprintf(&escaped[j], maxLen - j, "\\%03d", c);
        } else {
            // Regular character: copy as-is
            escaped[j++] = c;
        }
    }

    escaped[j] = '\0'; // Null-terminate the string
    return escaped;
}

// Function to generate a value
char *_generateValue(ASTNodePtr ast) {
    if (ast == NULL) return NULL;
    if (ast->type != AST_NODE_VALUE) return NULL;

    // Get the string representation of the value, e.g., "3.14"
    char *value = ast->data->value->value;

    // Determine the type and generate the formatted string
    switch (ast->data->value->type) {
        case dTypeI32: {
            // Integer type: int@<value>
            int size = snprintf(NULL, 0, "int@%s", value) + 1;
            char *result = malloc(size);
            if (result == NULL) return NULL; // Handle allocation failure
            snprintf(result, size, "int@%s", value);
            return result;
        }

       case dTypeF64: {
            // Floating-point type: float@<value>
            double floatValue = atof(value); // Convert string to double
            int size = snprintf(NULL, 0, "float@%a", floatValue) + 1;
            char *result = malloc(size);
            if (result == NULL) return NULL; // Handle allocation failure
            snprintf(result, size, "float@%a", floatValue);
            return result;
        }

        case dTypeU8: {
            // Unsigned integer type: u8@<value>
            int size = snprintf(NULL, 0, "string@%s", value) + 1;
            char *result = malloc(size);
            if (result == NULL) return NULL; // Handle allocation failure
            snprintf(result, size, "string@%s", value);
            return escapeString(result);
        }

        default:
            return NULL; // Unsupported type
    }
}


// Function to generate a variable
char *_generateVariable(enum DATA_TYPES type, char *varName) {
    type = dTypeNone;
    return NULL;
}

// Function to generate a function call
char *_generateFunctionCall(ASTNodePtr ast) {
    ast = NULL;
    return NULL;
}

// Function to generate an expression root
char *_generateExpresionRoot(ASTNodePtr ast) {
    ast = NULL;
    return NULL;
}

// Function to generate a truth expression
char *_generateTruthExpresion(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast) {
    returnAdressTracker = NULL;
    ast = NULL;
    return NULL;
}

// Function to generate an if statement
char *_generateIF(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast) {
    returnAdressTracker = NULL;
    ast = NULL;
    return NULL;
}

// Function to generate a while loop
char *_generateWhile(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast) {
    returnAdressTracker = NULL;
    ast = NULL;
    return NULL;
}

// Function to generate the start of an else block
char *generateElseStart(void) {
    return NULL;
}

// Function to generate the end of an if else block
char *_generateEndIfElse(void) {
    return NULL;
}

