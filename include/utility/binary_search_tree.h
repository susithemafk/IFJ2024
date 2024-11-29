/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file function_call_validation.h
 * @date 17.10. 2024
 * @brief Header file for the Binary search tree
 */

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "utility/linked_list.h"
#else
#include "linked_list.h"
#endif

/**
 * @brief Struct for the symbol table
 * @param size - amount of nodes in the tree
 * @param root - root of the tree
 * @param freeData - flag, if true, the data of each node will be freed
*/
typedef struct BST {
    unsigned int size;
    struct TreeNode *root;
    void (*freeFunction)(void **data);  // Function pointer for freeing node data
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
 * Free function wrapper for freeing data in the tree
 * 
 * @param data - pointer to the data
 * @note this function is internal
*/
void freeWrapper(void **data);

/**
 * Initializes the symbol table
 * 
 * @param freeData - flag, if true, the data of each node will be freed
 * @return pointer to the initialized symbol table
*/
BST *bstInit(void (*freeFunction)(void **data));

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
 * @param returnData - pointer to which the data will be stor§ed
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
bool bstFree(BST **tree);

/**
 * Frees the node and all of its children
 * 
 * @param node - pointer to the tree node
 * @note this function is internal
 * @return true, if the node was successfully freed, false otherwise
*/
bool _bstFreeNode(TreeNode *node, void (*freeFunction)(void **data));

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

/**
 * Function to get all the nodes of the tree in a linked list
 * 
 * @param tree - pointer to the BST
 * @return pointer to the linked list of the nodes
 */
LinkedList *bstGetNodes(BST *tree);

/**
 * Function to traverse the tree and add all nodes to the linked list
 * 
 * @param node - pointer to the tree node
 * @param list - pointer to the linked list
 * @note this function is internal, and recursive
 */
void _bstTraverseNodes(TreeNode *node, LinkedList *list);

#endif // BINARY_SEARCH_TREE_H
