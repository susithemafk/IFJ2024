/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file function_call_validation.h
 * @date 28.9. 2024
 * @brief Header file for the function call validation
 */

#ifndef FUNCTION_CALL_VALIDATION_H
#define FUNCTION_CALL_VALIDATION_H

#include <stdbool.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"

/**
 * @brief Struct for the function call validator
 * @param calledFunctions - list of called functions
 * @param definedFunctions - list of defined functions
 */
typedef struct FunctionCallValidator {
    struct LinkedList *calledFunctions;
    struct LinkedList *definedFunctions;
} *FunctionCallValidatorPtr;

/**
 * Function to initialize the function call validator
 * 
 * @return pointer to the initialized function call validator
 */
FunctionCallValidatorPtr initFunctionCallValidator(void);

/**
 * Function to add a function to the called functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return true, if the function was successfully added, false otherwise
 */
bool addCalledFunction(FunctionCallValidatorPtr validator, char *functionName);

/**
 * Function to add a function to the defined functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return true, if the function was successfully added, false otherwise
 */
bool addDefinedFunction(FunctionCallValidatorPtr validator, char *functionName);

/**
 * Function to find a function in the called functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return true, if the function was found, false otherwise
 */
bool _findCalledFunction(FunctionCallValidatorPtr validator, char *functionName);

/**
 * Function to find a function in the defined functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return true, if the function was found, false otherwise
 */
bool _findDefinedFunction(FunctionCallValidatorPtr validator, char *functionName);

/**
 * Function to validate if all called functions are defined
 * 
 * @param validator - pointer to the function call validator
 * @return true, if all called functions are defined, false otherwise
 */
bool validateALLFunctionCalls(FunctionCallValidatorPtr validator);

/**
 * Function to free the function call validator
 * 
 * @param validator - pointer to the function call validator
 */
void freeFunctionCallValidator(FunctionCallValidatorPtr validator);

#endif // FUNCTION_CALL_VALIDATION_H

