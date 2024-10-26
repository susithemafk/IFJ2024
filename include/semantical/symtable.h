/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symtable.h
 * @date 28.9. 2024
 * @brief Header file for the symbol table (BST)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "utility/binary_search_tree.h"
#include "semantical/sem_enums.h"

// ####################### Function Call Validation #######################

/**
 * Function to initialize the function call validator
 * 
 * @return pointer to the initialized function call validator
*/
BST *initFunctionCallValidator(void);

/**
 * Function to free the Function Call Validator
 * 
 * @param validator - pointer to the validator
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES _freeFunctionCallValidator(BST **validator);

/**
 * Function to add a new function definition to the validator
 * 
 * @param validator - pointer to the validator
 * @param func - pointer to the AST of the definition
 * @return enum ERR_CODES
*/
enum ERR_CODES addFunctionDefinition(BST *validator, ASTNodePtr func);

/**
 * Function to find the same hash function in the validator
 * 
 * @param validator - pointer to the validator
 * @param name - name of the function
 * @return LinkedList of the functions with the same hash
 * @note this function is internal
*/
LinkedList *_findSameHashFunction(BST *validator, unsigned int hash);

/**
 * Function to find a function by name
 * 
 * @param validator - pointer to the validator
 * @param name - name of the function
 * @return err-code
 * @note this function is internal
*/
enum ERR_CODES _findFunction(BST *validator, char *name, ASTNodePtr *result);

/**
 * Function to validate a function call
 * 
 * @param validator - pointer to the validator
 * @param call - pointer to the AST of the function call
 * @param returnType - pointer to the return type of the function
 * @return enum ERR_CODES
*/
enum ERR_CODES validateFunctionCall(BST *validator, ASTNodePtr call, enum DATA_TYPES *returnType);

// ####################### SYMTABLE #######################

// this might hvae to change in the future
enum SYMTABLE_NODE_TYPES {
    SYM_GLOBAL = 1,
    SYM_FUNCTION = 2,
    SYM_IF = 3,
    SYM_WHILE = 4,
    SYM_FOR = 5,
    SYM_SWITCH = 6,
};

typedef struct SymVariable {
    unsigned int id; // id of the variable (id is valid, inside of the scope)
    char *name; // the name of the variable
    enum DATA_TYPES type; // the type of the variable
    bool mutable; // if the variable is mutable (constants will have this false)
    bool accesed; // if the variable was accessed
    struct ASTNode *declaration; // pointer to the declaration node
} SymVariable;

/**
 * @brief Struct for the symTable Node
 * @param type - type of the node
 * @param key - key of the node
 * @param parent - pointer to the parent node
 * @param variables - pointer to the BST of the variables
 * @param innerScopes - pointer to the linked list of the inner scopes
*/
typedef struct SymTableNode {
    enum SYMTABLE_NODE_TYPES type;
    unsigned int key;
    struct SymTableNode *parent;
    BST *variables;
    struct SymTableNode *innerScope;
} SymTableNode;

/**
 * @brief Struct for the symbol table
 * @param root - pointer to the root of the tree
 * @param innerScopesCount - amount of inner scopes in the tree
*/
typedef struct SymTable {
    SymTableNode *root;
    unsigned int varCount;
    unsigned int scopeCount;
    SymTableNode *currentScope;
} SymTable;

/**
 * Function to create a copy of a variable
 * 
 * @param variable - pointer to the variable
 * @return pointer to the new variable
*/
SymVariable *copyVariable(SymVariable *variable);

/**
 * Initializes the symbol table
 * @return pointer to the initialized symbol table
*/
SymTable *symTableInit(void);

/**
 * Insert a new scope into the symbol table, at the curretn location
 * 
 * @param table - pointer to the symbol table
 * @param type - type of the scope
 * @return true, if the scope was successfully inserted, false otherwise
*/
bool symTableMoveScopeDown(SymTable *table, enum SYMTABLE_NODE_TYPES type);

/**
 * Function to exit the current scope
 * 
 * @param table - pointer to the symbol table
 * @note in case we try to exit the global scope, the table will be freed
 * @return true, if the scope was successfully exited, false otherwise
*/
bool symTableExitScope(SymTable *table, enum ERR_CODES *returnCode);

/**
 * Insert a new variable to the current scope
 * 
 * @param table - pointer to the symbol table
 * @param name - name of the variable to insert
 * @param type - type of the variable to insert
 * @param mutable - flag, if the variable is mutable
 * @return pointer to the variable, if the variable was successfully inserted, NULL otherwise
*/
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, ASTNodePtr declaration);

/**
 * Search for a vairable based on its name, in same hash variables
 * 
 * @param list - pointer to the linked_list
 * @param name - name of the variable to search for
 * @return true if the variable was found, false otherwise
*/
bool _searchForVarSameHash(LinkedList *list, char *name);


/**
 * Function to check, if a variable can be found in the current scope
 * 
 * @param table - pointe to the symbol table
 * @param name - name of the variable to search for
 * @param returnData - pointer to which the data will be stored, if not NULL
 * @return true, if the variable was found, false otherwise
*/
bool symTableFindVariable(SymTable *table, char *name, SymVariable **returnData);

/**
 * Function to determin, if a variable can be mutated
 * 
 * @param variable - pointer to the variable
 * @return true, if the variable can be mutated, false otherwise
*/
bool symTableCanMutate(SymVariable *variable);

/**
 * Function to free the symbol table
 * 
 * @param table - pointer to the symbol table
 * @return true, if the symbol table was successfully freed, false otherwise
*/
bool symTableFree(SymTable *table);

/**
 * Function to free the symTableNode
 * 
 * @param node - pointer to the symTableNode
 * @return true, if the node was successfully freed, false otherwise
*/
void _symTableFreeNode(SymTableNode *node);

/**
 * Function to check, if all the variables in the node ware accessed
 * 
 * @param node - pointer to the symTableNode
*/
bool _symTableAllVariablesAccesed(SymTableNode *node);

/**
 * Function to tranarse the variable tree
 * 
 * @param node - pointer to the symTableNode
 * @param result - pointer to the result
 * @note this function is internal
 * @return void
*/
void _symTableTraverseVariables(TreeNode *node, bool *result);


#endif // SYMTABLE_H
