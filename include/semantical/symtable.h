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

/**
 * @brief Struct for the symbol table
 * @param size - amount of nodes in the tree
 * @param root - root of the tree
 * @param freeData - flag, if true, the data of each node will be freed
*/
typedef struct BST {
    unsigned int size;
    struct TreeNode *root;
    bool freeData;
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
BST *bstInit(bool freeData);

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
bool _bstFreeNode(TreeNode *node, bool freeData);

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
 * Rotates the tree to the right
 * 
 * @param root - pointer to the root of the subtree
 * @note this function is internal
 * @return pointer to the new root of the subtree
*/
TreeNode *_bstRotRight(TreeNode *root);

#endif // SYMTABLE_H
