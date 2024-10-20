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

// ####################### Function Call Validation #######################

// Function to init the function call validator
BST *initFunctionCallValidator(void) {

    // create the BST
    BST *validator = bstInit((void (*)(void **))removeList);

    return validator;
}

// Function to free the validator
enum ERR_CODES _freeFunctionCallValidator(BST **validator) {

    // check if the validator is not NULL
    if (validator == NULL || *validator == NULL) return E_INTERNAL;

    // free the BST
    if (bstFree(validator) != true) return E_INTERNAL;

    return SUCCESS; 
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

// Function to add a new function definition to the validator
enum ERR_CODES addFunctionDefinition(BST *validator, ASTNodePtr func) {
    
    // check if the call is valid
    if (validator == NULL || func == NULL) return E_INTERNAL;

    // check for right type of function AST
    if (func->type != AST_NODE_FUNCTION) return E_INTERNAL;

    // get the function name
    char *name = func->data->function->functionName;
    unsigned int hash = hashString(name);

    // find the funcitons with the same hash
    LinkedList *sameHashFuncs = _findSameHashFunction(validator, hash);

    // we do not have this hash yet, need to inti
    if (sameHashFuncs == NULL) {
        sameHashFuncs = initLinkedList(false);
        if (sameHashFuncs == NULL) return E_INTERNAL;
        if (!insertNodeAtIndex(sameHashFuncs, (void *)func, 0)) return E_INTERNAL;
        if (!bstInsertNode(validator, hash, (void *)sameHashFuncs)) return E_INTERNAL;
        return SUCCESS;
    }

    // we have the hash, need to find, if we are not redifining the function
    for (unsigned int i = 0; i < getSize(sameHashFuncs); i++) {
        ASTNodePtr function = (ASTNodePtr)getDataAtIndex(sameHashFuncs, i);
        if (function == NULL || function->type != AST_NODE_FUNCTION) return E_INTERNAL;
        if (strcmp(function->data->function->functionName, name) == 0) return E_SEMANTIC_REDIFINITION;
    }

    // insert the function into the list
    if (!insertNodeAtIndex(sameHashFuncs, (void *)func, -1)) return E_INTERNAL;

    return SUCCESS;
}

// Function to validate a function call
enum ERR_CODES validateFunctionCall(BST *validator, ASTNodePtr call, enum DATA_TYPES *returnType) {

    // check if the call is valid
    if (validator == NULL || call == NULL || returnType == NULL) return E_INTERNAL;

    // check for the right type of the call AST
    if (call->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    // get the function name
    char *name = call->data->functionCall->functionName;

    // find the function in the validator
    ASTNodePtr function;
    enum ERR_CODES result = _findFunction(validator, name, &function);

    if (result != SUCCESS) return result;

    // check if the function was found
    if (function == NULL) return E_SEMANTIC_UND_FUNC_OR_VAR;

    // check if the function has the right number of arguments
    unsigned int callArgs = getSize(call->data->functionCall->arguments);
    unsigned int funcArgs = getSize(function->data->function->arguments);

    if (callArgs != funcArgs) return E_SEMANTIC_INVALID_FUN_PARAM;

    // check if the arguments are of the right type
    LinkedList *callArgsList = call->data->functionCall->arguments;
    LinkedList *funcArgsList = function->data->function->arguments;

    for (unsigned int i = 0; i < getSize(funcArgsList); i++) {
        ASTNodePtr callArg = (ASTNodePtr)getDataAtIndex(callArgsList, i);
        ASTNodePtr funcArg = (ASTNodePtr)getDataAtIndex(funcArgsList, i);

        if (callArg == NULL || funcArg == NULL) return E_INTERNAL;

        if (callArg->type != AST_NODE_VARIABLE || funcArg->type != AST_NODE_VARIABLE) return E_INTERNAL;
        if (callArg->data->variable->type != funcArg->data->variable->type) return E_SEMANTIC_INVALID_FUN_PARAM;
    }

    // set the return type
    *returnType = function->data->function->returnType;
    return SUCCESS;
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
    table->scopeCount--;
    table->currentScope->innerScope = NULL;

    // free the current scope
    bool result = bstFree(&currentScope->variables);
    free(currentScope);

    return result;
}

// Function to insert a new
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, ASTNodePtr declaration) {
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

    // Initialize other fields
    newVariable->type = type;
    newVariable->mutable = mutable;
    newVariable->accesed = false;
    newVariable->declaration = declaration;

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


