/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file functin_call_validation.c
 * @date 9.10. 2024
 * @brief main src file for the function call validation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "semantical/function_call_validation.h"
#include "utility/linked_list.h"
#include "utility/enumerations.h"


// Function to initialize the function call validator
FunctionCallValidatorPtr initFunctionCallValidator(void) {

    // alocaate memory for the function call validator
    FunctionCallValidatorPtr validator = (FunctionCallValidatorPtr)malloc(sizeof(struct FunctionCallValidator));

    LinkedList *calledFunction = initLinkedList(false); // this may need to change in the futere, but we will seee (true/false)
    LinkedList *definedFunction = initLinkedList(false);

    if (validator == NULL || calledFunction == NULL || definedFunction == NULL) {
        return NULL;
    }

    validator->calledFunctions = calledFunction;
    validator->definedFunctions = definedFunction;

    return validator;
}

// Function to add a function to the called functions
bool addCalledFunction(FunctionCallValidatorPtr validator, char *functionName) {

    if (validator == NULL || functionName == NULL) {
        return false;
    }

    // check for the function call, to not save duplicates
    if (_findCalledFunction(validator, functionName)) {
        return true;
    }

    // add the function to the called functions
    return insertNodeAtIndex(validator->calledFunctions, (void*)functionName, -1);
}

// Function to add a function to the defined functions
bool addDefinedFunction(FunctionCallValidatorPtr validator, char *functionName) {

    if (validator == NULL || functionName == NULL) {
        return false;
    }

    // not saving duplicates, and cannot redefine a function
    if (_findDefinedFunction(validator, functionName)) {
        return false;
    }

    // add the function to the defined functions
    return insertNodeAtIndex(validator->definedFunctions, (void*)functionName, -1);
}

// Function to find a function in the called functions
bool _findDefinedFunction(FunctionCallValidatorPtr validator, char *functionName) {

    if (validator == NULL || functionName == NULL) {
        return false;
    }

    void *iterator = NULL;

    for (unsigned int i = 0; i < validator->definedFunctions->size; i++) {
        iterator = getDataAtIndex(validator->definedFunctions, i);
        if (iterator == NULL) continue;

        if (strcmp((char*)iterator, functionName) == 0) {
            return true;
        }
    }

    return false;
}

// Function to find a function in the defined functions
bool _findCalledFunction(FunctionCallValidatorPtr validator, char *functionName) {

    if (validator == NULL || functionName == NULL) {
        return false;
    }

    void *iterator = NULL;

    for (unsigned int i = 0; i < validator->calledFunctions->size; i++) {
        iterator = getDataAtIndex(validator->calledFunctions, i);
        if (iterator == NULL) continue;

        if (strcmp((char*)iterator, functionName) == 0) {
            return true;
        }
    }

    return false;
}

// Function to validate if all called functions are defined
bool validateALLFunctionCalls(FunctionCallValidatorPtr validator) {

    if (validator == NULL) {
        return false;
    }

    void *iterator = NULL;

    for (unsigned int i = 0; i < validator->calledFunctions->size; i++) {
        iterator = getDataAtIndex(validator->calledFunctions, i);
        if (iterator == NULL) continue;

        if (!_findDefinedFunction(validator, (char*)iterator)) { // this can be done better
            return false;
        }
    }

    return true;
}

// Function to free the function call validator
void freeFunctionCallValidator(FunctionCallValidatorPtr validator) {

    if (validator == NULL) {
        return;
    }

    removeList(validator->calledFunctions);
    removeList(validator->definedFunctions);

    free(validator);
}

