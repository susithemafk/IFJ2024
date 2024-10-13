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
#include "semantical/symtable.h"

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
 * @brief Struct for representing a function call/declaration
 * @param functionName - name of the function
 * @param returnType - return type of the function
*/
typedef struct FunctionHolder {
    char *functionName;
    enum DATA_TYPES returnType;
    LinkedList *arguments;
} *FunctionHolderPtr;

/**
 * Function to initialize the function holder
 * 
 * @return pointer to the initialized function holder
 */
FunctionHolderPtr initFunctionHolder(void);

/**
 * Function to free the function holder
 * 
 * @param holder - pointer to the function holder
 */
void freeFunctionHolder(FunctionHolderPtr holder);

/**
 * Function to add an argument to the function holder
 * 
 * @param holder - pointer to the function holder
 * @param argumentName - name of the argument
 * @param argumentType - type of the argument
 * @return true if successful, false if an error occurred
*/
bool addArgumentToFunctionHolder(
    FunctionHolderPtr holder, 
    SymVariable *argument
);

/**
 * Function to add the return type to the function holder
 * 
 * @param holder - pointer to the function holder
 * @param returnType - return type of the function
 * @return true if successful, false if an error occurred
*/
bool addFunctionNameToFunctionHolder(
    FunctionHolderPtr holder, 
    char *functionName
);

/**
 * Function to add the return type to the function holder
 * 
 * @param holder - pointer to the function holder
 * @param returnType - return type of the function
*/
void addReturnTypeToFunctionHolder(
    FunctionHolderPtr holder, 
    enum DATA_TYPES returnType
);

/**
 * Function to initialize the function call validator
 * 
 * @return pointer to the initialized function call validator
 */
FunctionCallValidatorPtr initFunctionCallValidator(void);

/**
 * Function to validate if two functions have same arguent types
 * 
 * @param declaredFunction - pointer to the declared function
 * @param holder - pointer to the function holder
 * @return error code
 */
enum ERR_CODES validateFunctionArguments(
    FunctionHolderPtr declaredFunction, 
    FunctionHolderPtr holder
);

/**
 * Function to add a function to the called functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return error code
 */
enum ERR_CODES addCalledFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder
);

/**
 * Function to add a function to the defined functions
 * 
 * @param validator - pointer to the function call validator
 * @param functionName - name of the function
 * @return error code
 */
enum ERR_CODES addDefinedFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder
);

/**
 * Function to find a function in the called functions
 * 
 * @param validator - pointer to the function call validator
 * @param holder - pointer to the function holder
 * @param lookDeclared - flag to look in the declared functions
 * @return true, if the function was found, false otherwise
 */
FunctionHolderPtr _findFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder,
    bool lookDeclared,
    unsigned int *index
);

/**
 * Function to validate if all called functions are defined
 * 
 * @param validator - pointer to the function call validator
 * @return error code
 */
enum ERR_CODES validateALLFunctionCalls(FunctionCallValidatorPtr validator);

/**
 * Function to free the function call validator
 * 
 * @param validator - pointer to the function call validator
 */
void freeFunctionCallValidator(FunctionCallValidatorPtr validator);





#endif // FUNCTION_CALL_VALIDATION_H

