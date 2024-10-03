/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symbol_tree.h
 * @date 28.9. 2024
 * @brief Header file for the symbol tree
 */

#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include <stdbool.h>
#include "utility/enumerations.h"
#include "utility/linked_list.h"


// ####################### ABSTRACT SYNTAX TREE #######################

/**
 * @brief Struct for the AST node
 * @param key - key of the node, used for searching in the tree
 * @param data - pointer to the data, in each node
 * @param children - pointer to the children of the node
 * @param parent - pointer to the parent of the node
*/
typedef struct ASTNode {
    unsigned int key;
    void *data; // holds the info, about the block, of the node
    struct LinkedList *children; // holds the children of the node
    struct ASTNode *parent; // holds the parent of the node
} *ASTNodePtr;

/**
 * @brief Struct for the AST
 * @param size - amount of nodes in the tree
 * @param root - root of the tree
 * @param current - current node
 * @param freeFunctionData - Function pointer for freeing node data
*/
typedef struct AST {
    unsigned int size; // amoutn of inner nodes in the tree
    struct ASTNode *root; // root of the tree
    struct ASTNode *current; // current node
    void (*freeFunctionData)(void *data); // Function pointer for freeing node data
} *ASTPtr;


/**
 * Function to create a new node
 * 
 * @param key - key of the node
 * @param data - pointer to the data of the node
 * @note This function is internal
 * @return pointer to the new node
*/
ASTNodePtr _astCreateNewNode(unsigned int key, void *data, ASTNodePtr parent);

/**
 * Init function for the AST
 * 
 * @param freeFunctionData - Function pointer for freeing node data
 * @param startIdx - index of the starting node
 * @note after inti, the tree will be empty, need to use astAddChild to add the first node
 * @return pointer to the initialized AST
*/
ASTPtr astInit(void (*freeFunctionData)(void *data), unsigned int startIdx);

/**
 * Function to add a node to as a child node at the end of the children list
 * 
 * @param tree - pointer to the AST
 * @param data - pointer to the data of the node
 * @param switchTo - boolean value, if true, the current node will be switched to the new node
 * @note if the tree is empty, the first node will be added as the root
 * @return true, if the data was successfully inserted, false otherwise
*/
bool astAddChildToCurrent(ASTPtr tree, void *data, bool switchTo);

/**
 * Function to add a node next to the current node
 * 
 * @param tree - pointer to the AST
 * @param data - pointer to the data of the node
 * @param switchTo - boolean value, if true, the current node will be switched to the new node
 * @note if the tree is empty, nothing will be added
 * @return true, if the data was successfully inserted, false otherwise
*/
bool astAddNextToCurrent(ASTPtr tree, void *data, bool switchTo);

/**
 * Function to switch the current node to the parent node
 * 
 * @param tree - pointer to the AST
 * @return true, if the current node was successfully switched, false otherwise
*/
bool astSwitchToParent(ASTPtr tree);

/**
 * Function to switch the current node to the child node
 * 
 * @param tree - pointer to the AST
 * @param index - index of the child node
 * @return true, if the current node was successfully switched, false otherwise
*/
bool astSwitchToChild(ASTPtr tree, int index);

/**
 * Function to search for a node in the tree
 * 
 * @param tree - pointer to the AST
 * @param key - key of the node
 * @note the search is the current node and will go through the children
 * @return pointer to the data, if the key was found, NULL otherwise
*/
ASTNodePtr astSearchForNode(ASTPtr tree, unsigned int key);

/**
 * Function to free the entire tree
 * 
 * @param tree - pointer to the AST
 * @return true, if the tree was successfully freed, false otherwise
*/
bool astFree(ASTPtr tree);

/**
 * Helper function to free the tree
 * 
 * @param node - pointer to the node
 * @param freeFunctionData - Function pointer for freeing node data
 * @note This function is internal
 * @return true, if the node was successfully freed, false otherwise
*/
bool _astFree(ASTNodePtr node);

#endif // ABSTRACT_SYNTAX_TREE_H
