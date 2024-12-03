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

#ifdef USE_CUSTOM_STRUCTURE
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "utility/binary_search_tree.h"
#else
#include "linked_list.h"
#include "enumerations.h"
#include "binary_search_tree.h"
#endif

// ####################### SYMTABLE #######################

// types of scopes
enum SYMTABLE_NODE_TYPES {
    SYM_GLOBAL = 1,
    SYM_FUNCTION = 2,
    SYM_IF = 3,
    SYM_WHILE = 4,
};

// Function definition
typedef struct SymFunction {
    char *funcName;
    enum DATA_TYPES returnType;
    bool nullableReturn;
    LinkedList *paramaters; 
} *SymFunctionPtr;

// Function Param
typedef struct SymFunctionParam {
    enum DATA_TYPES type;
    bool nullable;
} *SymFunctionParamPtr;

// SymTable node
typedef struct SymTableNode {
    enum SYMTABLE_NODE_TYPES type;
    unsigned int key;
    struct SymTableNode *parent;
    BST *variables;
    struct SymTableNode *innerScope;
} SymTableNode;

// Symbol table ADT
typedef struct SymTable {
    SymTableNode *root; // root of the tree
    unsigned int varCount; // amount of variables (for making unique ids)
    unsigned int scopeCount; // amount of scopes in the tree
    SymTableNode *currentScope; // pointer to the current scope
    BST *functionDefinitions; // pointer to the function definitions BST
    LinkedList *data; // for storing variables
    LinkedList *tokenBuffer; // for storing the tokens
} SymTable;

/**
 * Wrapper function to get rid of all the function definitions
 * 
 * @param data - pointer to the data
*/
void freeFuncDefsWrapper(void **data);

/**
 * Function to init an empty funtion definition
 * 
 * @return pointer to the function definition
*/
SymFunctionPtr symInitFuncDefinition(void);

/**
 * Function to free a function definition
 * 
 * @param func - double ptr to the function defintion
 * @return void
*/
void symFreeFuncDefinition(SymFunctionPtr *func);

/**
 * Function to add a paramater to a function definition
 * 
 * @param func - pointer to the function definition
 * @param type - type of the paramater
 * @param nullable - if the paramater can be null
 * @return true, if the paramater was successfully added, false otherwise
*/
bool symAddParamToFunc(SymFunctionPtr func, enum DATA_TYPES type, bool nullable);

/**
 * Function to edit a function definition
 * 
 * @param func - pointer to the function definition
 * @param name - name of the function
 * @param returnType - return type of the function
 * @param nullable - if the return value can be null
 * @return true, if the name was successfully added, false otherwise
*/
bool symEditFuncDef(SymFunctionPtr func, char *name, enum DATA_TYPES returnType, bool nullable);

// SymTable Variable
typedef struct SymVariable {
    unsigned int id; // id of the variable (id is valid, inside of the scope)
    char *name; // the name of the variable
    enum DATA_TYPES type; // the type of the variable
    bool mutable; // if the variable is mutable (constants will have this false)
    int nullable; // Indicates if the variable can hold a null value -1 unknown, 0 not nullable, 1 nullable
    bool accesed; // if the variable was accessed
    bool modified; // if the variable was modified
    bool valueKnonwAtCompileTime; // if the value of the variable is known at compile time and can be converted to i32
} SymVariable;

/**
 * Function to add a function definition to the symbol table
 * 
 * @param table - pointer to the symbol table
 * @param function - pointer to the function definition
 * @return err codes
*/
enum ERR_CODES symTableAddFunction(SymTable *table, SymFunctionPtr function);

/**
 * Function to find a function definition
 * 
 * @param table - pointer to the symbol table
 * @param name - name of the called function
 * @return pointer to the function definition, if the function was found, NULL otherwise
*/
SymFunctionPtr symTableFindFunction(SymTable *table, char *name);

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
enum ERR_CODES symTableExitScope(SymTable *table);

/**
 * Insert a new variable to the current scope
 * 
 * @param table - pointer to the symbol table
 * @param name - name of the variable to insert
 * @param type - type of the variable to insert
 * @param mutable - flag, if the variable is mutable
 * @param nullable - flag, if the variable can be null
 * @param canBeConvertedToI32 - flag, if the variable can be converted to i32
 * @return pointer to the variable, if the variable was successfully inserted, NULL otherwise
*/
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, bool nullable, bool canBeConvertedToI32);

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
 * @return true, if the variable was found, false otherwise
*/
SymVariable *symTableFindVariable(SymTable *table, char *name);

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
bool symTableFree(SymTable **table);

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
 * @return true, if all the variables were accessed, false otherwise
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
