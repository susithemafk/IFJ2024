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
#include "ast_assets/inbuild_funcs.h"

// ####################### Function Call Validation #######################

// Function to init the funtion call validator
fnDefinitionsPtr initFunctionDefinitions(void) {

    // create the validator
    fnDefinitionsPtr validator = (fnDefinitionsPtr)malloc(sizeof(struct fnDefinitions));
    if (validator == NULL) return NULL;

    // init the BST
    validator->funcDefinitions = bstInit((void (*)(void **))_freeFuncDefSameHahs);
    if (validator->funcDefinitions == NULL) {
        free(validator);
        return NULL;
    }

    // add the inbuild functions
    fillInBuildInFuncions(validator);

    return validator;
}

// Function to add a function definition to the validator
enum ERR_CODES addFunctionDefinition(fnDefinitionsPtr validator, ASTNodePtr func) {

    // internal error handeling
    if (validator == NULL || func == NULL) return E_INTERNAL;
    if (func->type != AST_NODE_FUNCTION) return E_INTERNAL;

    unsigned int hash = hashString(func->data->function->functionName);

    // check for redefinition
    LinkedList *sameHashFunctions = bstSearchForNode(validator->funcDefinitions, hash);

    // save the new function
    if (sameHashFunctions == NULL) {
        sameHashFunctions = initLinkedList(false);
        if (sameHashFunctions == NULL) return E_INTERNAL;
        if (!insertNodeAtIndex(sameHashFunctions, (void *)func, -1)) return E_INTERNAL;
        if (!bstInsertNode(validator->funcDefinitions, hash, (void *)sameHashFunctions)) {
            removeList(&sameHashFunctions);
            return E_INTERNAL;
        }
        return SUCCESS;
    }

    // check for redefinition
    for (unsigned int i = 0; i < getSize(sameHashFunctions); i++) {
        ASTNodePtr function = (ASTNodePtr)getDataAtIndex(sameHashFunctions, i);
        if (function == NULL) return E_INTERNAL;
        if (strcmp(function->data->function->functionName, func->data->function->functionName) == 0) {
            return E_SEMANTIC_REDIFINITION;
        }
    }

    // save the new function
    if (!insertNodeAtIndex(sameHashFunctions, (void *)func, -1)) return E_INTERNAL;

    return SUCCESS;
}

// Function to find a function definition based on a function name
ASTNodePtr findFunctionDefinition(fnDefinitionsPtr validator, char *functionName) {

    // internal error handeling
    if (validator == NULL || functionName == NULL) return NULL;

    unsigned int hash = hashString(functionName);

    // search for the function
    LinkedList *sameHashFunctions = bstSearchForNode(validator->funcDefinitions, hash);
    if (sameHashFunctions == NULL) return NULL;

    // search for the function in the list
    for (unsigned int i = 0; i < getSize(sameHashFunctions); i++) {
        ASTNodePtr function = (ASTNodePtr)getDataAtIndex(sameHashFunctions, i);
        if (function == NULL) return NULL;
        if (strcmp(function->data->function->functionName, functionName) == 0) {
            return function;
        }
    }

    return NULL;
}

// helper functio to free a BST node
void _freeFuncDefSameHahs(LinkedList **list) {
    if (list == NULL || *list == NULL) return;

    for (unsigned int i = 0; i < getSize(*list); i++) {
        ASTNodePtr function = (ASTNodePtr)getDataAtIndex(*list, i);
        if (function == NULL) continue;
        ASTfreeNode(&function);
    }

    removeList(list);
}

// Function to free the function definitions
bool freeFunctionDefinitions(fnDefinitionsPtr *validator) {

    // internal error handeling
    if (validator == NULL || *validator == NULL) return false;

    // free the BST
    bool result = bstFree(&(*validator)->funcDefinitions);

    // free the validator
    free(*validator);
    *validator = NULL;

    return result;
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

    // alocate the data thing
    LinkedList *data = initLinkedList(false);
    if (data == NULL) {
        free(globalScope);
        free(table);
        return NULL;
    }

    // fill the table
    table->root = globalScope;
    table->currentScope = globalScope;
    table->varCount = 0;
    table->scopeCount = 1;
    table->data = data;

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
        symTableFree(&table);
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
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, bool nullable) {
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

    // Allocate memory for the name and copy it
    newVariable->name = (char *)malloc(strlen(name) + 1);
    if (newVariable->name == NULL) {
        free(newVariable); // Cleanup if malloc fails
        return NULL;
    }
    strcpy(newVariable->name, name);

    table->varCount++; // Increment the variable count

    // Initialize other fields
    newVariable->type = type;
    newVariable->mutable = mutable;
    newVariable->nullable = nullable;
    newVariable->accesed = false;
    newVariable->id = table->varCount;

    // Get the hash of the variable's name
    unsigned int hash = hashString(name);

    // Get the variables list in the current scope's hash table
    BST *variables = table->currentScope->variables;
    void *sameHashVariables = bstSearchForNode(variables, hash);

    // save the data into the linked list dedicated for data
    if (!insertNodeAtIndex(table->data, (void *)newVariable, -1)) {
        free(newVariable->name);
        free(newVariable); // Cleanup if insertion fails
        return NULL;
    }

    if (sameHashVariables == NULL) {
        // Create a new linked list for variables with the same hash
        sameHashVariables = initLinkedList(false);
        if (!insertNodeAtIndex((LinkedList *)sameHashVariables, (void *)newVariable, -1)) {
            free(newVariable->name);
            free(newVariable); // Cleanup if insertion fails
            return NULL;
        }
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
bool symTableFree(SymTable **table) {

    SymTable *tTable = *table;

    if (table == NULL)
        return false;

    // free the inner scopes
    if (tTable->root != NULL) 
        _symTableFreeNode(tTable->root);

    // free the variables
    for (unsigned int i = 0; i < getSize(tTable->data); i++) {
        SymVariable *variable = (SymVariable *)getDataAtIndex(tTable->data, i);
        if (variable == NULL) {
            return false;
        }
        free(variable->name);
        free(variable);
    }

    removeList(&tTable->data);
    
    // free the table
    free(tTable);
    *table = NULL;
    return true;
}
