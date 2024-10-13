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
#include "semantical/symtable.h"

// Function to initialize the function holder
FunctionHolderPtr initFunctionHolder(void) {

    // allocate memory for the function holder
    FunctionHolderPtr holder = (FunctionHolderPtr)malloc(sizeof(struct FunctionHolder));

    if (holder == NULL) {
        return NULL;
    }

    holder->functionName = NULL;
    holder->returnType = dTypeNone;
    holder->arguments = initLinkedList(true);

    return holder;
}

// Function to free the function holder
void freeFunctionHolder(FunctionHolderPtr holder) {

    if (holder == NULL) {
        return;
    }

    if (holder->functionName != NULL) {
        free(holder->functionName);
    }

    removeList(holder->arguments);

    free(holder);
}

// Function to add an argument to the function holder
bool addArgumentToFunctionHolder(
    FunctionHolderPtr holder, 
    SymVariable *argument
) {

    if (holder == NULL || argument == NULL) return false;

    // check for duplicates
    SymVariable *iterator = NULL;
    for (unsigned int i = 0; i < getSize(holder->arguments); i++) {
        iterator = (SymVariable *)getDataAtIndex(holder->arguments, i);
        if (iterator == NULL) continue;

        if (strcmp((char*)iterator->name, argument->name) == 0) {
            return false;
        }
    }

    // since the symtable where the argument will come from, will be freed
    // we need to duplicate this variable

    SymVariable *newArgument = (SymVariable *)malloc(sizeof(SymVariable));
    if (newArgument == NULL) return false;

    newArgument->name = (char *)malloc(sizeof(char) * (strlen(argument->name) + 1));
    if (newArgument->name == NULL) {
        free(newArgument);
        return false;
    }

    strcpy(newArgument->name, argument->name);
    newArgument->type = argument->type;
    newArgument->mutable = argument->mutable;
    newArgument->accesed = argument->accesed;

    return insertNodeAtIndex(holder->arguments, (void*)newArgument, -1);
}

// Function to add the return type to the function holder
void addReturnTypeToFunctionHolder(
    FunctionHolderPtr holder, 
    enum DATA_TYPES returnType
) {

    if (holder == NULL) return;

    holder->returnType = returnType;
}

// Function to add the function name to the function holder
bool addFunctionNameToFunctionHolder(
    FunctionHolderPtr holder, 
    char *functionName
) {

    if (holder == NULL || functionName == NULL) return false;
    
    if (holder->functionName != NULL) return false;

    // alocate memory for the function name    
    holder->functionName = (char *)malloc(sizeof(char) * (strlen(functionName) + 1));
    if (holder->functionName == NULL) return false;

    strcpy(holder->functionName, functionName);

    return true;
}

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

FunctionHolderPtr _findFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder,
    bool lookDeclared, 
    unsigned int *index
) {

    if (validator == NULL || holder == NULL) return NULL;

    LinkedList *list = lookDeclared ? validator->definedFunctions : validator->calledFunctions;

    FunctionHolderPtr iterator = NULL;
    for (unsigned int i = 0; i < getSize(list); i++) {
        iterator = (FunctionHolderPtr)getDataAtIndex(list, i);
        if (iterator == NULL) continue;

        if (strcmp(iterator->functionName, holder->functionName) == 0) {
            if (index != NULL) *index = i;
            return iterator;
        }
    }

    return NULL;
}

// Function to validate if two functions have same arguent types
enum ERR_CODES validateFunctionArguments(
    FunctionHolderPtr declaredFunction, 
    FunctionHolderPtr holder
) {

    if (declaredFunction == NULL || holder == NULL) return E_INTERNAL;

    // need to check, if all the arguments and return type match
    LinkedList *declaredArguments = declaredFunction->arguments;
    SymVariable *declaredArgument = NULL;

    unsigned int declaredArgsSize = getSize(declaredArguments);
    unsigned int holderArgsSize = getSize(holder->arguments);

    if (declaredArgsSize != holderArgsSize) {
        return E_SEMANTIC_INVALID_FUN_PARAM;
    }

    if (declaredArgsSize == 0 && holderArgsSize == 0) return SUCCESS; // no need to check, void functions

    for (unsigned int i = 0; i < getSize(declaredArguments); i++) {
        declaredArgument = (SymVariable *)getDataAtIndex(declaredArguments, i);

        SymVariable *holderArgument = (SymVariable *)getDataAtIndex(holder->arguments, i);
        if (holderArgument == NULL) return E_INTERNAL;

        if (declaredArgument->type != holderArgument->type) {
            return E_SEMANTIC_INVALID_FUN_PARAM;
        }
    }

    return SUCCESS;
    
}

// Function to add a function to the called functions
enum ERR_CODES addCalledFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder
) {

    if (validator == NULL || holder == NULL) return E_INTERNAL;

    // in case the function is already declared, we need to check if the return type and arguments match

    // first we need to check if the function is already declared
    FunctionHolderPtr declaredFunction = _findFunction(validator, holder, true, NULL);

    if (declaredFunction != NULL) {
        return validateFunctionArguments(declaredFunction, holder);        
    }
    
    // in here, we have not seen this function yet, so we will just add it to the called functions, checking, if it is not already there
    // in case it is, allready in the called functions, we can check, if return types and arguments match if not, return coresponding error

    declaredFunction = _findFunction(validator, holder, false, NULL);

    // we have never seen this function before, so we can just add it to the called functions
    if (declaredFunction == NULL) {
        if (!insertNodeAtIndex(validator->calledFunctions, (void*)holder, -1)) return E_INTERNAL;
        return SUCCESS;
    }

    // we have seen this function before, so we need to check if the return type and arguments match
    return validateFunctionArguments(declaredFunction, holder);
}

// Function to add a function to the defined functions
enum ERR_CODES addDefinedFunction(
    FunctionCallValidatorPtr validator, 
    FunctionHolderPtr holder
) {

    if (validator == NULL || holder == NULL) return E_INTERNAL;

    // check if the function is allready defined
    FunctionHolderPtr iterator = _findFunction(validator, holder, true, NULL);
    if (iterator != NULL) return E_SEMANTIC_REDIFINITION;

    // add the function to the defined functions
    if (!insertNodeAtIndex(validator->definedFunctions, (void*)holder, -1)) return E_INTERNAL;

    unsigned int index;
    // check if the function is allready called, if so, check if the return type and arguments match
    iterator = _findFunction(validator, holder, false, &index);

    if (iterator == NULL) return SUCCESS;

    enum ERR_CODES result = validateFunctionArguments(holder, iterator);
    if (result != SUCCESS) return result;

    // remove the function from the called functions
    if (!removeNodeAtIndex(validator->calledFunctions, (int)index)) return E_INTERNAL;

    return SUCCESS;
}

// Function to validate if all called functions are defined
enum ERR_CODES validateALLFunctionCalls(FunctionCallValidatorPtr validator) {

    // for each function call, we need to check, if the function is defined, and if the arguments match and if the return type match
    FunctionHolderPtr holder = NULL;
    for (unsigned int i = 0; i < getSize(validator->calledFunctions); i++) {
        holder = (FunctionHolderPtr)getDataAtIndex(validator->calledFunctions, i);
        if (holder == NULL) return E_INTERNAL;

        FunctionHolderPtr definedFunction = _findFunction(validator, holder, true, NULL);
        if (definedFunction == NULL) return E_SEMANTIC_UND_FUNC_OR_VAR;

        enum ERR_CODES result = validateFunctionArguments(definedFunction, holder);
        if (result != SUCCESS) return result;

    }

    return SUCCESS;
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

