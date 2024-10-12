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

// ####################### HB BINARY SEARCH TREE #######################

/**
 * @brief Struct for the symbol table
 * @param size - amount of nodes in the tree
 * @param root - root of the tree
 * @param freeData - flag, if true, the data of each node will be freed
*/
typedef struct BST {
    unsigned int size;
    struct TreeNode *root;
    void (*freeFunction)(void *data);  // Function pointer for freeing node data
} BST;

/**
 * @brief Struct for the tree node
 * @param key - key of the node, used for searching in the tree
 * @param data - pointer to the data, in each node
 * @param left - pointer to the left child
 * @param right - pointer to the right child
*/
typedef struct TreeNode{
    unsigned int key;
    void *data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/**
 * Initializes the symbol table
 * 
 * @param freeData - flag, if true, the data of each node will be freed
 * @return pointer to the initialized symbol table
*/
BST *bstInit(void (*freeFunction)(void *data));

/**
 * Inserts data into the tree
 * 
 * @param tree - pointer to the symbol table (BST)
 * @param key - key of the node
 * @param data - pointer to the data of the node
 * @return true, if the data was successfully inserted, false otherwise
*/
bool bstInsertNode(BST *tree, unsigned int key, void *data);

/**
 * Searches for data in the tree
 * 
 * @param tree - pointer to the symbol table (BST)
 * @param key - key of the node
 * @return pointer to the data, if the key was found, NULL otherwise
*/
void *bstSearchForNode(BST *tree, unsigned int key);

/**
 * Pops the data from the tree and removes the node
 * 
 * @param tree - pointer to the symbol table (BST)
 * @param key - key of the node
 * @param returnData - pointer to which the data will be stored
 * @return true, if the data was successfully popped, false otherwise
*/
bool bstPopNode(BST *tree, unsigned int key, void **returnData);

/**
 * Removes data from the tree without returning the data
 * 
 * @param tree - pointer to the symbol table (BST)
 * @param key - key of the node
 * @return true, if the data was successfully removed, false otherwise
*/
bool bstRemoveNode(BST *tree, unsigned int key);

/**
 * Frees the symbol table and all of its nodes
 * 
 * @param tree - pointer to the symbol table (BST)
 * @return true, if the bst was successfully freed, false otherwise
*/
bool bstFree(BST *tree);

/**
 * Frees the node and all of its children
 * 
 * @param node - pointer to the tree node
 * @note this function is internal
 * @return true, if the node was successfully freed, false otherwise
*/
bool _bstFreeNode(TreeNode *node, void (*freeFunction)(void *data));

/**
 * Calculates the height of the tree
 * 
 * @param tree - pointer to the BST
 * @return height of the node, or 0 if the node is NULL
*/
int bstCalculateHeight(BST *tree);

/**
 * Calculates the higtht of a node
 * 
 * @param node - pointer to the tree node
 * @note this function is internal
 * @return height of the node, or 0 if the node is NULL
*/
int _bstCalculateHeight(TreeNode *node);

/**
 * @brief Calculates the balance factor of a node.
 * 
 * @param node - pointer to the tree node
 * @return balance factor, positive if left-heavy, negative if right-heavy
 */
int _bstGetBalanceFactor(TreeNode *node);

/**
 * @brief Balances the tree rooted at the given node.
 * 
 * This function checks the balance factor of the node and performs
 * necessary rotations to maintain balance (LL, LR, RL, RR cases).
 * 
 * @param tree - pointer to the BST
 * @return true if the tree was balanced, false otherwise
 */
bool bstBalanceTree(BST *tree);

/**
 * Rotates the tree to the left
 * 
 * @param root - pointer to the root of the subtree
 * @note this function is internal
 * @return pointer to the new root of the subtree
*/
TreeNode *_bstRotLeft(TreeNode *root);

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
    char *name; // the name of the variable
    enum DATA_TYPES type; // the type of the variable
    bool mutable; // if the variable is mutable (constants will have this false)
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
*/
typedef struct SymTable {
    SymTableNode *root;
    unsigned int scopeCount;
    SymTableNode *currentScope;
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
SymVariable *symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable);

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
