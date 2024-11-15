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

void freeFuncDefsWrapper(void **data);
void freeListWrapper(void **data);

/**
 * @brief Struct for the function call validator
 * @param funcDefinitions - the function defintions
 * @param functionCalls - the function calls
 * @note this struct is used to validate the function calls
 */
typedef struct fnDefinitions {
    BST *funcDefinitions; // the function defintions
} *fnDefinitionsPtr;

typedef struct fnCallwType {
    ASTNodePtr call; // pointer to the AST of the function call
    struct SymVariable *assingTo; // pointer to the variable, that the function call is assigned to
} *fnCallwTypePtr;

/**
 * Function to init the funtion call validator
 * 
 * @return pointer to the initialized validator
 * @note this function will return NULL, if the memory allocation fails
 */
fnDefinitionsPtr initFunctionDefinitions(void);

/**
 * Function to add a function definition to the validator
 * 
 * @param validator - pointer to the validator
 * @param func - pointer to the AST of the function
 * @return enum ERR_CODES
*/
enum ERR_CODES addFunctionDefinition(fnDefinitionsPtr defs, ASTNodePtr func);

/**
 * Function to find a function definition in the validator
 * 
 * @param validator - pointer to the validator
 * @param functionName - name of the function to search for
 * @return pointer to the AST of the function, if the function was found, NULL otherwise
*/
ASTNodePtr findFunctionDefinition(fnDefinitionsPtr defs, char *functionName);

/**
 * Function to free the function definitions
 * 
 * @param validator - pointer to the validator
 * @return true, if the function definitions were successfully freed, false otherwise
 * @note this function is internal
*/
void _freeFuncDefSameHahs(LinkedList **list);

/**
 * Function to free the function definitions
 * 
 * @param validator - pointer to the validator
 * @return true, if the function definitions were successfully freed, false otherwise
*/
bool freeFunctionDefinitions(fnDefinitionsPtr *validator);

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
    int nullable; // Indicates if the variable can hold a null value -1 unknown, 0 not nullable, 1 nullable
    bool accesed; // if the variable was accessed
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
 * @param data - linked list, where all the data is stored
*/
typedef struct SymTable {
    SymTableNode *root;
    unsigned int varCount;
    unsigned int scopeCount;
    SymTableNode *currentScope;
    LinkedList *data;
} SymTable;

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
 * @return pointer to the variable, if the variable was successfully inserted, NULL otherwise
*/
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable, int nullable);

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


/**
 * Helper function to free the function definitions
 * 
 * @param list - pointer to the linked list
 * @return bool
*/
void _freeFuncDefinitions(struct LinkedList **listPtr);

/**
 * Helper function to free the function calls
 * 
 * @param list - pointer to the linked list
 * @return bool
*/
void _freeFuncCalls(struct LinkedList **listPtr);



#endif // SYMTABLE_H
