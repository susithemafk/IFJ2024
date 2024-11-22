/** AUTHOR
 *
 * @author <247581> Martin Mendl
 * @file symtable.c
 * @date 28.9. 2024
 * @brief main scr file for the symtable
 */

#include "semantical/symtable.h"
#include "semantical/inbuild_funcs.h"
#include "utility/binary_search_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ####################### SYMTABLE #######################

// Wrapper function to get rid of all the function definitions
void freeFuncDefsWrapper(void **data) {

    if (data == NULL || *data == NULL)
        return;

    LinkedList *list = (LinkedList *)*data;
    if (list == NULL)
        return;

    SymFunctionPtr oneFunc;
    unsigned int size = getSize(list);

    for (unsigned int i = 0; i < size; i++) {
        oneFunc = (SymFunctionPtr)getDataAtIndex(list, i);
        symFreeFuncDefinition(&oneFunc);
    }

    removeList(&list);
}

// Function to init an empty function definition
SymFunctionPtr symInitFuncDefinition(void) {

    // allocate the memory for the function definition
    SymFunctionPtr func = (SymFunctionPtr)malloc(sizeof(struct SymFunction));
    if (func == NULL)
        return NULL;

    // init the function definition
    func->funcName = NULL;
    func->returnType = dTypeUndefined;
    func->nullableReturn = false;
    func->paramaters = NULL;

    return func;
}

// Function to free a function definition
void symFreeFuncDefinition(SymFunctionPtr *func) {

    if (func == NULL || *func == NULL)
        return;

    if ((*func)->funcName != NULL)
        free((*func)->funcName);
    if ((*func)->paramaters != NULL)
        removeList(&(*func)->paramaters);

    free(*func);
    *func = NULL;
}

// Function to add a parameter to a function definition
bool symAddParamToFunc(SymFunctionPtr func, enum DATA_TYPES type, bool nullable) {

    if (func == NULL)
        return false;

    // alocate the memory for the parameter
    SymFunctionParamPtr param = (SymFunctionParamPtr)malloc(sizeof(struct SymFunctionParam));
    if (param == NULL)
        return false;

    // fill the parameter
    param->type = type;
    param->nullable = nullable;

    // add the parameter to the arguments list
    if (func->paramaters == NULL) {
        func->paramaters = initLinkedList(true);
        if (func->paramaters == NULL) {
            free(param);
            return false;
        }
    }

    // insert
    if (!insertNodeAtIndex(func->paramaters, (void *)param, -1)) {
        free(param);
        return false;
    }

    return true;
}

// Function to edit a function definition
bool symEditFuncDef(SymFunctionPtr func, char *name, enum DATA_TYPES returnType, int nullable) {

    // check if the call is valid
    if (func == NULL)
        return false;
    if ((name == NULL && returnType == dTypeUndefined && nullable == -1))
        return false;

    // handle name
    if (name != NULL) {
        if (func->funcName != NULL)
            return false;
        func->funcName = malloc(sizeof(char) * (strlen(name) + 1));
        if (func->funcName == NULL)
            return false;
        strcpy(func->funcName, name);
    }

    // handle return type
    if (returnType != dTypeUndefined) {
        if (func->returnType != dTypeUndefined)
            return false;
        func->returnType = returnType;
    }

    // handle nullable
    if (nullable != -1) {
        if (func->nullableReturn != false)
            return false;
        func->nullableReturn = (nullable == 1) ? true : false;
    }

    return true;
}

// Function to insert a new function definition into the symbol table
enum ERR_CODES symTableAddFunction(SymTable *table, SymFunctionPtr function) {

    if (table == NULL || function == NULL)
        return E_INTERNAL;

    // check if the function is already in the table
    if (symTableFindFunction(table, function->funcName) != NULL)
        return E_SEMANTIC_REDIFINITION;

    // insert the function into the table
    unsigned int hash = hashString(function->funcName);

    LinkedList *sameHashFuncs = (LinkedList *)bstSearchForNode(table->functionDefinitions, hash);
    if (sameHashFuncs == NULL) {
        sameHashFuncs = initLinkedList(false);
        if (sameHashFuncs == NULL)
            return E_INTERNAL;
        if (!insertNodeAtIndex(sameHashFuncs, (void *)function, -1)) {
            removeList(&sameHashFuncs);
            return E_INTERNAL;
        }
        if (!bstInsertNode(table->functionDefinitions, hash, (void *)sameHashFuncs)) {
            removeList(&sameHashFuncs);
            return E_INTERNAL;
        }
        return SUCCESS;
    }

    // add the function to the list
    if (!insertNodeAtIndex(sameHashFuncs, (void *)function, -1))
        return E_INTERNAL;

    return SUCCESS;
}

// Function to find a function definition
SymFunctionPtr symTableFindFunction(SymTable *table, char *name) {

    if (table == NULL || name == NULL)
        return NULL;

    unsigned int hash = hashString(name);
    LinkedList *sameHashFuncs = (LinkedList *)bstSearchForNode(table->functionDefinitions, hash);

    if (sameHashFuncs == NULL)
        return NULL;

    unsigned int size = getSize(sameHashFuncs);
    for (unsigned int i = 0; i < size; i++) {
        SymFunctionPtr func = (SymFunctionPtr)getDataAtIndex(sameHashFuncs, i);
        if (func != NULL && strcmp(func->funcName, name) == 0)
            return func;
    }

    return NULL;
}

// Function to search for a scope in the same hash variables list
bool _searchForVarSameHash(LinkedList *list, char *name) {

    // check if the list is not NULL
    if (list == NULL)
        return false;

    unsigned int size = getSize(list);
    for (unsigned int i = 0; i < size; i++) {
        SymVariable *variable = (SymVariable *)getDataAtIndex(list, i);
        if (variable != NULL && strcmp(variable->name, name) == 0) {
            return true;
        }
    }
    return false;
}

void freeListWrapper(void **data) {
    removeList((LinkedList **)data);
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
    globalScope->key = 0;                              // key for the global scope
    globalScope->parent = NULL;                        // no parent
    globalScope->variables = bstInit(freeListWrapper); // link to variables, in global scope disabled
    globalScope->innerScope = NULL;                    // link to other scopes

    // check for init problems
    if (globalScope->variables == NULL) {
        free(globalScope);
        free(table);
        return NULL;
    }

    // init global constants
    LinkedList *globalConstants = initLinkedList(false);
    if (globalConstants == NULL) {
        free(globalScope);
        free(table);
        return NULL;
    }

    // fill in global constants
    SymVariable *thorwAway = (SymVariable *)malloc(sizeof(SymVariable));
    if (thorwAway == NULL) {
        free(globalScope);
        free(table);
        free(globalConstants);
        return NULL;
    }

    // init the _ variable
    thorwAway->name = malloc(sizeof(char) * 2);
    if (thorwAway->name == NULL) {
        free(thorwAway);
        free(globalScope);
        free(table);
        free(globalConstants);
        return NULL;
    }
    strcpy(thorwAway->name, "_");
    thorwAway->type = dTypeNone;
    thorwAway->accesed = true;
    thorwAway->id = 0;
    thorwAway->mutable = false;
    thorwAway->nullable = 1;
    if (!insertNodeAtIndex(globalConstants, (void *)thorwAway, -1)) {
        free(thorwAway);
        free(globalScope);
        free(table);
        return NULL;
    }

    // save the global constants
    if (!bstInsertNode(globalScope->variables, hashString("_"), (void *)globalConstants)) {
        free(thorwAway);
        free(globalScope);
        free(table);
        return NULL;
    }

    // alocate the data thing
    LinkedList *data = initLinkedList(false);
    if (data == NULL) {
        free(globalScope);
        free(table);
        return NULL;
    }

    // save the constant to the data
    if (!insertNodeAtIndex(data, (void *)thorwAway, -1)) {
        free(thorwAway);
        free(globalScope);
        free(table);
        return NULL;
    }

    // fill the table
    table->root = globalScope;
    table->currentScope = globalScope;
    table->varCount = 1;
    table->scopeCount = 1;
    table->data = data;
    table->functionDefinitions = bstInit(freeFuncDefsWrapper);
    // need to also add all the function definitions that are build in
    fillInBuildInFuncions(table);

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
    BST *variables = bstInit(freeListWrapper);

    // check if the memory was allocated

    if (newScope == NULL || variables == NULL) {
        return false;
    }

    // fill the new scope
    newScope->type = type;
    newScope->key = table->scopeCount;      // key for the new scope
    newScope->parent = table->currentScope; // parent is the current scope
    newScope->variables = variables;        // link to variables
    newScope->innerScope = NULL;            // link to other scopes

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
enum ERR_CODES symTableExitScope(SymTable *table) {

    // check if the table is not NULL
    if (table == NULL)
        return E_INTERNAL;

    // need to remove the current scope
    SymTableNode *currentScope = table->currentScope;
    enum ERR_CODES returnCode;

    // check if all variables were accesed
    if (_symTableAllVariablesAccesed(currentScope)) {
        #ifdef DEBUG
        printf("Symtable -> All variables in the scope were accesed\n");
        #endif
        returnCode = SUCCESS;
    } else {
        #ifdef DEBUG
        printf("symtable -> Not all variables in the scope were accesed\n");
        #endif
        returnCode = E_SEMANTIC_UND_FUNC_OR_VAR;
    }

    // update the current scope
    table->currentScope = table->currentScope->parent;
    table->currentScope->innerScope = NULL;

    // free the current scope
    bool result = bstFree(&currentScope->variables);
    free(currentScope);

    if (!result) return E_INTERNAL;
    return returnCode;
}

// Function to insert a new
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, bool nullable) {
    // Check if the table or current scope is invalid (if global scope declaration is disallowed)
    if (table == NULL || table->currentScope->type == SYM_GLOBAL) {
        return NULL;
    }

    // Check if the variable already exists in the current scope
    SymVariable *var = symTableFindVariable(table, name);
    if (var != NULL)
        return NULL; // Variable already exists

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
SymVariable *symTableFindVariable(SymTable *table, char *name) {

    // Check if the table or name is null
    if (table == NULL || name == NULL)
        return NULL;

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

            variable->accesed = true; // Mark, that the variable was accessed

            #ifdef DEBUG
            printf("Variable %s found in scope %d\n", name, currentScope->key);
            printf("Variable ID: %d\n", variable->id);
            printf("Variable name: %s\n", variable->name);
            printf("Variable type: %d\n", variable->type);
            printf("Variable mutable: %d\n", variable->mutable);
            printf("Variable nullable: %d\n", variable->nullable);
            #endif
            return variable;
        }

        // Move up to the parent scope if not found
        currentScope = currentScope->parent;
    }

    return NULL; // Variable not found
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
    bstFree(&(*table)->functionDefinitions);

    // free the table
    free(tTable);
    *table = NULL;
    return true;
}
