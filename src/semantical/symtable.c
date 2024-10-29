/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symtable.c
 * @date 28.9. 2024
 * @brief main scr file for the symtable
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "semantical/symtable.h"
#include "semantical/sem_enums.h"
#include "utility/binary_search_tree.h"
#include "semantical/abstract_syntax_tree.h"

// ####################### Function Call Validation #######################

// Function to init the funtion call validator
fnCallValidatorPtr initFunctionCallValidator(void) {

    // create the validator struct
    fnCallValidatorPtr validator  = (fnCallValidatorPtr)malloc(sizeof(struct fnCallValidator));
    if (validator == NULL) return NULL;

    // create the BSTs for the validator
    validator->funcDefinitions = bstInit((void (*)(void **))removeList);
    validator->functionCalls = bstInit((void (*)(void **))removeList);

    if (validator->funcDefinitions == NULL || validator->functionCalls == NULL) {
        bstFree(&validator->funcDefinitions);
        bstFree(&validator->functionCalls);
        free(validator);
        return NULL;
    }

    return validator;
}


// Function to find the same hash funcitons in the validator
LinkedList *_findSameHashFunction(BST *validator, unsigned int hash) {

    // check if the call is valid
    if (validator == NULL) return NULL;

    // find the functions with the same hash

    LinkedList *sameHashFuncs = (LinkedList *)bstSearchForNode(validator, hash);

    return sameHashFuncs;
}   

// Function to find a function by name
enum ERR_CODES _findFunction(BST *validator, char *name, ASTNodePtr *result) {

    // check if call is valid
    if (validator == NULL || name == NULL) return E_INTERNAL;

    // get the hash of the name
    unsigned int hash = hashString(name);

    LinkedList *sameHashFuncs = (LinkedList *)bstSearchForNode(validator, hash);

    if (sameHashFuncs == NULL) return E_INTERNAL;

    // we have the hash, need to find the func
    for (unsigned int i = 0; i < getSize(sameHashFuncs); i++) {
        ASTNodePtr function  = (ASTNodePtr)getDataAtIndex(sameHashFuncs, i);
        if (function == NULL || function->type != AST_NODE_FUNCTION) return E_INTERNAL;
        if (strcmp(function->data->function->functionName, name) == 0) {
            *result = function;
            return SUCCESS;
        } 
    }

    *result = NULL;
    return SUCCESS;
}

// Function to validate any function calls, that can be validated based on the new function definition
enum ERR_CODES _updateFuncCallsOnNewfuncDefinition(BST *functionCalls, ASTNodePtr newfuncDef) {

    char *name = newfuncDef->data->function->functionName;
    unsigned int hash = hashString(name);

    LinkedList *sameHashFuncCalls = _findSameHashFunction(functionCalls, hash);

    // function not called yet
    if (sameHashFuncCalls == NULL) return SUCCESS;

    for (unsigned int i = 0; i < getSize(sameHashFuncCalls); i++) {
        fnCallwTypePtr funCall = (fnCallwTypePtr)getDataAtIndex(sameHashFuncCalls, i);
        if (funCall == NULL) return E_INTERNAL;
        if (funCall->call == NULL) return E_INTERNAL;

        if (strcmp(funCall->call->data->functionCall->functionName, name) == 0) {
            enum ERR_CODES result = _semanticCheckFuncCall(newfuncDef, funCall);
            if (result != SUCCESS) return result;

            // remove the function call from the list, and return SUCCESS
            if (!removeNodeAtIndex(sameHashFuncCalls, i)) return E_INTERNAL;
            return SUCCESS;
        }
    }
    return SUCCESS;
}

// Function to add a new function definition to the validator
enum ERR_CODES addFunctionDefinition(fnCallValidatorPtr fnValidator, ASTNodePtr func) {
    
    BST *validator = fnValidator->funcDefinitions;

    // check if the call is valid
    if (validator == NULL || func == NULL) return E_INTERNAL;

    // check for right type of function AST
    if (func->type != AST_NODE_FUNCTION) return E_INTERNAL;

    // get the function name
    char *name = func->data->function->functionName;
    unsigned int hash = hashString(name);

    // find the funcitons with the same hash
    LinkedList *sameHashFuncs = _findSameHashFunction(validator, hash);

    // we do not have this hash yet, need to init
    if (sameHashFuncs == NULL) {
        sameHashFuncs = initLinkedList(false);
        if (sameHashFuncs == NULL) return E_INTERNAL;
        if (!insertNodeAtIndex(sameHashFuncs, (void *)func, 0)) return E_INTERNAL;
        if (!bstInsertNode(validator, hash, (void *)sameHashFuncs)) return E_INTERNAL;
        // we are checking, if we have some unvalidated function calls
        return _updateFuncCallsOnNewfuncDefinition(fnValidator->functionCalls, func); 
    }

    // we have the hash, need to find, if we are not redifining the function
    for (unsigned int i = 0; i < getSize(sameHashFuncs); i++) {
        ASTNodePtr function = (ASTNodePtr)getDataAtIndex(sameHashFuncs, i);
        if (function == NULL || function->type != AST_NODE_FUNCTION) return E_INTERNAL;
        if (strcmp(function->data->function->functionName, name) == 0) return E_SEMANTIC_REDIFINITION;
    }

    // insert the function into the list
    if (!insertNodeAtIndex(sameHashFuncs, (void *)func, -1)) return E_INTERNAL;

    // we are checking, if we have some unvalidated function calls
    return _updateFuncCallsOnNewfuncDefinition(fnValidator->functionCalls, func); 
}

// Functio to validate one func call based on a definition
enum ERR_CODES _semanticCheckFuncCall(ASTNodePtr definition, fnCallwTypePtr call) {

    enum DATA_TYPES retType = call->returnType;
    ASTNodePtr funCall = call->call;

    if (definition == NULL || funCall == NULL) return E_INTERNAL;

    // check the return type of the functions
    if (definition->data->function->returnType != retType) return E_SEMANTIC_BAD_FUNC_RETURN;

    // check for nullability of return type here as well, need to modify the enum 

    unsigned int defSize = getSize(definition->data->function->arguments);

    // check the argument cound
    if (defSize != getSize(funCall->data->functionCall->arguments)) return E_SEMANTIC_INVALID_FUN_PARAM;

    // check the argument types
    for (unsigned int i = 0; i < defSize; i++) {
        ASTNodePtr defArg = (ASTNodePtr)getDataAtIndex(definition->data->function->arguments, i);
        ASTNodePtr callArg = (ASTNodePtr)getDataAtIndex(funCall->data->functionCall->arguments, i);

        enum DATA_TYPES defArgType = defArg->data->variable->type;

        // function call can have value or variable as an argument
        switch(callArg->type) {
            case AST_NODE_VALUE:
                if (callArg->data->value->type != defArgType) return E_SEMANTIC_INVALID_FUN_PARAM;
                break;
            case AST_NODE_VARIABLE:
                if (callArg->data->variable->type != defArgType) return E_SEMANTIC_INVALID_FUN_PARAM;
                break;
            default:
                return E_INTERNAL;
        }

    }

    return SUCCESS;
}

// Function to validate a function call
enum ERR_CODES _handleNewFuncCall(fnCallValidatorPtr functionDefinitions, fnCallwTypePtr funcCall, bool expectDefined) {

    ASTNodePtr fnCall = funcCall->call;
    unsigned int hash = hashString(fnCall->data->functionCall->functionName);

    // get the same hash definned functions
    LinkedList *samehashDefined = _findSameHashFunction(functionDefinitions->funcDefinitions, hash);

    // in case we are expecting the function to be defined, and the saem hash is empty, we have an error
    if (samehashDefined == NULL && expectDefined) return E_SEMANTIC_UND_FUNC_OR_VAR;

    // if the function definitions is missing, save it for later validation
    if (samehashDefined == NULL) {
        samehashDefined = _findSameHashFunction(functionDefinitions->functionCalls, hash);
        // we have never seen the function hash before, init and save for later validation
        if (samehashDefined == NULL) {
            samehashDefined = initLinkedList(false);
            if (samehashDefined == NULL) return E_INTERNAL;
            if (!insertNodeAtIndex(samehashDefined, (void *)funcCall, 0)) return E_INTERNAL;
            if (!bstInsertNode(functionDefinitions->functionCalls, hash, (void *)samehashDefined)) return E_INTERNAL;
            return SUCCESS;
        }
        // we have seen the function hash before, save the function call for later validation
        if (!insertNodeAtIndex(samehashDefined, (void *)funcCall, -1)) return E_INTERNAL;
        return SUCCESS;
    }

    // we have the function definitions with the same hash
    // try to find the function definition and validate the function call
    for (unsigned int i = 0; i < getSize(samehashDefined); i++) {
        ASTNodePtr definedFunc = (ASTNodePtr)getDataAtIndex(samehashDefined, i);
        if (definedFunc == NULL || definedFunc->type != AST_NODE_FUNCTION) return E_INTERNAL;

        // found the definition, validate the function call
        if (strcmp(definedFunc->data->function->functionName, fnCall->data->functionCall->functionName) == 0) {
            return _semanticCheckFuncCall(definedFunc, funcCall);
        }
    }

    // here we have not seen the fun definition yet, and we should have -> error
    if (expectDefined) return E_SEMANTIC_UND_FUNC_OR_VAR;

    // save the function for later validation
    samehashDefined = _findSameHashFunction(functionDefinitions->functionCalls, hash);
    if (samehashDefined == NULL) {
        samehashDefined = initLinkedList(false);
        if (samehashDefined == NULL) return E_INTERNAL;
        if (!insertNodeAtIndex(samehashDefined, (void *)funcCall, 0)) return E_INTERNAL;
        if (!bstInsertNode(functionDefinitions->functionCalls, hash, (void *)samehashDefined)) return E_INTERNAL;
        return SUCCESS;
    }

    // save the function for later validation
    if (!insertNodeAtIndex(samehashDefined, (void *)funcCall, -1)) return E_INTERNAL;
    return SUCCESS;
}

// Function to compare two function calls
enum ERR_CODES _compareTwoFuncCalls(fnCallwTypePtr func1, fnCallwTypePtr func2) {

    // compare return types
    if (func1->returnType != func2->returnType) return E_SEMANTIC_BAD_FUNC_RETURN;
    
    // compare amount of arguments
    unsigned int func1size = getSize(func1->call->data->functionCall->arguments);
    if (func1size != getSize(func2->call->data->functionCall->arguments)) return E_SEMANTIC_INVALID_FUN_PARAM;

    // compare the argument types
    enum DATA_TYPES arg1Type, arg2Type;
    for (unsigned int i = 0; i < func1size; i++) {
        ASTNodePtr arg1 = (ASTNodePtr)getDataAtIndex(func1->call->data->functionCall->arguments, i);
        ASTNodePtr arg2 = (ASTNodePtr)getDataAtIndex(func2->call->data->functionCall->arguments, i);


        // get type 1
        if (arg1->type == AST_NODE_VALUE) {
            arg1Type = arg1->data->value->type;
        } else if(arg1->type == AST_NODE_VARIABLE) {
            arg1Type = arg1->data->variable->type;
        } else {
            return E_INTERNAL;
        }

        // get type 2
        if (arg2->type == AST_NODE_VALUE) {
            arg2Type = arg2->data->value->type;
        } else if(arg2->type == AST_NODE_VARIABLE) {
            arg2Type = arg2->data->variable->type;
        } else {
            return E_INTERNAL;
        }

        if (arg1Type != arg2Type) return E_SEMANTIC_INVALID_FUN_PARAM;
    }

    return SUCCESS;
}

// Function to figure out, if we dont have conflicting function calls
enum ERR_CODES _checkOtherSameFuncCalls(BST *functionCalls, fnCallwTypePtr newFuncCall) {

    unsigned int hash = hashString(newFuncCall->call->data->functionCall->functionName);

    LinkedList *sameHashFuncCalls = _findSameHashFunction(functionCalls, hash);

    // we have not seen the hash before, so need to add it for later validation, and return SUCCESS
    if (sameHashFuncCalls == NULL) return E_NONE;

    // go throught the function definitions, and check, if the functinon calls match
    for (unsigned int i = 0; i < getSize(sameHashFuncCalls); i++) {
        fnCallwTypePtr funCall = (fnCallwTypePtr)getDataAtIndex(sameHashFuncCalls, i);
        if (funCall == NULL) return E_INTERNAL;
        if (funCall->call == NULL) return E_INTERNAL;

        if (strcmp(funCall->call->data->functionCall->functionName, newFuncCall->call->data->functionCall->functionName) == 0) {
            return _compareTwoFuncCalls(funCall, newFuncCall);
        }
    }

    return E_NONE;
}


// Function to add a functionc all and save it
enum ERR_CODES addFunctionCall(fnCallValidatorPtr fnValidator, ASTNodePtr call, enum DATA_TYPES expectedReturnType) {
    
    BST *fnCalls = fnValidator->functionCalls;

    if (fnCalls == NULL || call == NULL) return E_INTERNAL;

    // check for the right type of the call AST
    if (call->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    fnCallwTypePtr callwType = (fnCallwTypePtr)malloc(sizeof(struct fnCallwType));
    if (callwType == NULL) return E_INTERNAL;

    callwType->call = call;
    callwType->returnType = expectedReturnType;

    enum ERR_CODES result = _checkOtherSameFuncCalls(fnCalls, callwType);

    // we found comflicting function calls
    if (result != SUCCESS && result != E_NONE) {
        free(callwType);
        return result;
    }

    // we have the function call in allready, with the same argument tyeps, and expected return type, no need to save it
    if (result == SUCCESS) {
        free(callwType);
        return SUCCESS;
    }

    // in here, we do no have such a function call, try to validate it, or save it for later validation
    result = _handleNewFuncCall(fnValidator, callwType, false);

    if (result != SUCCESS) {
        ASTfreeNode(&call);
        free(callwType);
        return result;
    }

    return SUCCESS;
}


// Function to validate a function call
enum ERR_CODES validateAllFunctrionCalls(fnCallValidatorPtr fnValidator) {

    if (fnValidator == NULL) return E_INTERNAL;

    BST *fnCalls = fnValidator->functionCalls;

    LinkedList *treeData = bstGetNodes(fnCalls);
    if (treeData == NULL) return E_INTERNAL;

    // go through each of the hash sets of the function calls
    for (unsigned int i = 0; i < getSize(treeData); i++) {
        LinkedList *sameHashFuncCalls = (LinkedList *)getDataAtIndex(treeData, i);
        if (sameHashFuncCalls == NULL) {
            removeList(&treeData);
            return E_INTERNAL;
        }
        // validate each of the function calls
        for (unsigned int j = 0; j < getSize(sameHashFuncCalls); j++) {
            fnCallwTypePtr funCall = (fnCallwTypePtr)getDataAtIndex(sameHashFuncCalls, j);
            if (funCall == NULL) {
                removeList(&treeData);
                return E_INTERNAL;
            }

            enum ERR_CODES result = _handleNewFuncCall(fnValidator, funCall, true);
            if (result != SUCCESS) {
                removeList(&treeData);
                return result;
            }
        }
    }
    removeList(&treeData);
    return SUCCESS;
}

// Helper function to free the function call validator
bool _removeFnCallValidator(BST *fnCalls, bool doFuncCalls) {

    if (fnCalls == NULL) return false;

    LinkedList *treeData = bstGetNodes(fnCalls);
    if (treeData == NULL) return false;

    for (unsigned int i = 0; i < getSize(treeData); i++) {
        LinkedList *sameHashFuncCalls = (LinkedList *)getDataAtIndex(treeData, i);
        if (sameHashFuncCalls == NULL) {
            removeList(&treeData);
            return false;
        }

        for (unsigned int j = 0; j < getSize(sameHashFuncCalls); j++) {
            if (doFuncCalls) {
                fnCallwTypePtr funCall = (fnCallwTypePtr)getDataAtIndex(sameHashFuncCalls, j);
                if (funCall == NULL) {
                    removeList(&treeData);
                    return false;
                }
                ASTfreeNode(&funCall->call);
                free(funCall);
            } else {
                ASTNodePtr function = (ASTNodePtr)getDataAtIndex(sameHashFuncCalls, j);
                if (function == NULL) {
                    removeList(&treeData);
                    return false;
                }
                ASTfreeNode(&function);
            }
        }
    }
    removeList(&treeData);
    bstFree(&fnCalls);
    return true;
}

// Function to free the function call validator
bool freeFunctionCallValidator(fnCallValidatorPtr *validator) {

    if (validator == NULL || *validator == NULL) return false;

    fnCallValidatorPtr fnValidator = *validator;

    if (!_removeFnCallValidator(fnValidator->funcDefinitions, false)) return false;
    if (!_removeFnCallValidator(fnValidator->functionCalls, true)) return false;

    free(fnValidator);
    *validator = NULL;
    return true;
}

// ####################### SYMTABLE #######################

// Function to search for a scope in the same hash variables list
bool _searchForVarSameHash(LinkedList *list, char *name) {

    // check if the list is not NULL
    if (list == NULL) {
        return false;
    }

    unsigned int size = getSize(list);
    for (unsigned int i = 0; i < size; i++) {
        SymVariable *variable = (SymVariable *)getDataAtIndex(list, i);
        if (variable != NULL && strcmp(variable->name, name) == 0) {
            return true;
        }
    }
    return false;
}

// Function to init the symbol table
SymTable *symTableInit(void) {

    // create the global scope structures
    SymTable *table = (SymTable *)malloc(sizeof(SymTable));
    SymTableNode *globalScope = (SymTableNode *)malloc(sizeof(SymTableNode));
    
    // check if the memory was allocated
    if (table == NULL || globalScope == NULL) {
        return NULL;
    }

    // fill the global scope
    globalScope->type = SYM_GLOBAL;
    globalScope->key = 0; // key for the global scope
    globalScope->parent = NULL; // no parent
    globalScope->variables = NULL; // link to variables, in global scope disabled
    globalScope->innerScope = NULL; // link to other scopes

    // fill the table
    table->root = globalScope;
    table->currentScope = globalScope;
    table->varCount = 0;
    table->scopeCount = 1;

    return table;
}

// Function to insert a new scope into the symbol table
bool symTableMoveScopeDown(SymTable *table, enum SYMTABLE_NODE_TYPES type) {

    // check if the table is not NULL
    if (table == NULL) {
        return false;
    }

    // this need extra functtion variables

    // create the new scope
    SymTableNode *newScope = (SymTableNode *)malloc(sizeof(SymTableNode));
    BST *variables = bstInit((void (*)(void **))removeList);

    // check if the memory was allocated

    if (newScope == NULL || variables == NULL) {
        return false;
    }

    // fill the new scope
    newScope->type = type;
    newScope->key = table->scopeCount; // key for the new scope
    newScope->parent = table->currentScope; // parent is the current scope
    newScope->variables = variables; // link to variables
    newScope->innerScope = NULL; // link to other scopes

    // insert the new scope into the current scope
    table->currentScope->innerScope = newScope;

    // update the current scope
    table->currentScope = newScope;
    table->scopeCount++;

    return true;
}

// helper function to find if all varaibles were accesed
void _symTableTraverseVariables(TreeNode *node, bool *result) {

    if (node == NULL || *result == false) {
        return;
    }

    // the linked list of the variables, with the same hash
    LinkedList *variables = (LinkedList *)node->data;
    if (variables == NULL) {
        *result = false;
        return;
    }
    // recursive call for the left and right node
    _symTableTraverseVariables(node->left, result);
    _symTableTraverseVariables(node->right, result);

    // check if all the variables in the list ware accesed

    SymVariable *variable = NULL;

    unsigned int size = getSize(variables);
    for (unsigned i = 0; i < size; i++) {
        variable = (SymVariable *)getDataAtIndex(variables, i);
        if (variable == NULL || variable->accesed == false) {
            *result = false;
            return;
        }
    }

    if (variables == NULL) 
        *result = false;

    return;
}


// Function to check if all variables in the symbol table were accesed
bool _symTableAllVariablesAccesed(SymTableNode *node) {

    if (node == NULL || node->type == SYM_GLOBAL || node->variables == NULL) {
        return true;
    }

    bool result = true;
    _symTableTraverseVariables(node->variables->root, &result);
    
    return result == true;
}

// Function to exit the current scope
bool symTableExitScope(SymTable *table, enum ERR_CODES *returnCode) {

    // check if the table is not NULL
    if (table == NULL) {
        return false;
    }

    // in case we are trying to exit the global scope
    if (
        table->currentScope->innerScope == NULL && 
        table->currentScope->type == SYM_GLOBAL) {
        symTableFree(table);
        if (returnCode != NULL) {
            *returnCode = SUCCESS;
        }
        return true;
    }

    // need to remove the current scope
    SymTableNode *currentScope = table->currentScope;

    // check if all variables were accesed
    if (_symTableAllVariablesAccesed(currentScope)) {
        if (returnCode != NULL) {
            *returnCode = SUCCESS;
        }
    } else {
        if (returnCode != NULL) {
            *returnCode = E_SEMANTIC_UNUSED_VAR;
        }
    }

    // update the current scope
    table->currentScope = table->currentScope->parent;
    table->currentScope->innerScope = NULL;

    // free the current scope
    bool result = bstFree(&currentScope->variables);
    free(currentScope);

    return result;
}

// Function to insert a new
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, bool nullable, ASTNodePtr declaration) {
    // Check if the table or current scope is invalid (if global scope declaration is disallowed)
    if (table == NULL || table->currentScope->type == SYM_GLOBAL) {
        return NULL;
    }

    // Check if the variable already exists in the current scope
    SymVariable *var = NULL;
    if (symTableFindVariable(table, name, &var)) {
        return NULL; // Variable already exists
    }

    // Allocate memory for a new variable
    SymVariable *newVariable = (SymVariable *)malloc(sizeof(SymVariable));
    if (newVariable == NULL) {
        return NULL;
    }

    // Duplicate the name and check if strdup succeeded
    newVariable->name = name;
    if (newVariable->name == NULL) {
        free(newVariable); // Cleanup if strdup fails
        return NULL;
    }

    table->varCount++; // Increment the variable count

    // Initialize other fields
    newVariable->type = type;
    newVariable->mutable = mutable;
    newVariable->nullable = nullable;
    newVariable->accesed = false;
    newVariable->declaration = declaration;
    newVariable->id = table->varCount;

    // Get the hash of the variable's name
    unsigned int hash = hashString(name);

    // Get the variables list in the current scope's hash table
    BST *variables = table->currentScope->variables;
    void *sameHashVariables = bstSearchForNode(variables, hash);

    if (sameHashVariables == NULL) {
        // Create a new linked list for variables with the same hash
        sameHashVariables = initLinkedList(true);
        insertNodeAtIndex((LinkedList *)sameHashVariables, (void *)newVariable, -1);
        bstInsertNode(variables, hash, (void *)sameHashVariables);
        return newVariable;    
    } 

    // Insert the new variable into the existing list
    if (!insertNodeAtIndex((LinkedList *)sameHashVariables, (void *)newVariable, -1)) {
        free(newVariable->name);
        free(newVariable); // Cleanup if insertion fails
        return NULL;
    }

    return newVariable; // Success
}

// Function to find a variable in the current scope (including parent scopes)
bool symTableFindVariable(SymTable *table, char *name, SymVariable **returnData) {

    // Check if the table or name is null
    if (table == NULL || name == NULL) 
        return false;

    // Hash the name to find the corresponding variables
    unsigned int hash = hashString(name);

    // Traverse the current and parent scopes
    SymTableNode *currentScope = table->currentScope;
    while (currentScope != NULL) {
        BST *variables = currentScope->variables;
        void *sameHashVariables = bstSearchForNode(variables, hash);

        // If no variables with the same hash, go to the parent scope
        if (sameHashVariables == NULL) {
            currentScope = currentScope->parent;
            continue;
        }

        // Go through the variables with the same hash
        unsigned int size = getSize((LinkedList *)sameHashVariables);
        for (unsigned int i = 0; i < size; i++) {
            SymVariable *variable = (SymVariable *)getDataAtIndex((LinkedList *)sameHashVariables, i);

            // Skip null or mismatched variables
            if (variable == NULL || strcmp(variable->name, name) != 0) 
                continue;

            // Mark the variable as accessed if found
            if (returnData != NULL) 
                *returnData = variable;

            variable->accesed = true; // Only mark if you intend to track access

            return true;
        }
        
        // Move up to the parent scope if not found
        currentScope = currentScope->parent;
    }
    
    return false; // Variable not found
}

// Function to check, if a variable can be mutated
bool symTableCanMutate(SymVariable *variable) {
    if (variable == NULL) {
        return false;
    }

    return variable->mutable;
}

void _symTableFreeNode(SymTableNode *node) {
    if (node == NULL) {
        return;
    }

    _symTableFreeNode(node->innerScope);

    // free the variables
    if (node->variables != NULL) {
        bstFree(&node->variables);
    }

    free(node);
    return;
}

// Function to free the symbol table
bool symTableFree(SymTable *table) {

    if (table == NULL)
        return false;

    // free the inner scopes
    if (table->root != NULL) 
        _symTableFreeNode(table->root);
    
    // free the table
    free(table);
    return true;
}

// Function to copy a variable
SymVariable *copyVariable(SymVariable *variable) {

    // check if the variable is not NULL
    if (variable == NULL) {
        return NULL;
    }

    // allocate memory for the new variable
    SymVariable *newVariable = (SymVariable *)malloc(sizeof(SymVariable));
    if (newVariable == NULL) return NULL;

    // copy the name
    newVariable->name = strdup(variable->name);
    if (newVariable->name == NULL) {
        free(newVariable);
        return NULL;
    }

    // copy the type
    newVariable->type = variable->type;
    newVariable->mutable = variable->mutable;
    newVariable->accesed = variable->accesed;

    return newVariable;

}


