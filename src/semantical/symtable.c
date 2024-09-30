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
#include "semantical/symtable.h"

BST *bstInit(bool freeData) {

    BST *tree = (BST *)malloc(sizeof(BST));

    if (tree == NULL) {
        return NULL;
    }

    tree->size = 0;
    tree->root = NULL;
    tree->freeData = freeData;

    return tree;
}

// Internal function to calculate the height of a node.
int _bstCalculateHeight(TreeNode *node) {
    if (node == NULL) {
        return -1; // Return -1 for an empty node (base case for height)
    }

    // Recursively calculate height of left and right subtrees
    int leftHeight = _bstCalculateHeight(node->left);
    int rightHeight = _bstCalculateHeight(node->right);

    // Return the greater of the two heights plus 1 (for the current node)
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// External function to calculate the height of the tree
int bstCalculateHeight(BST *tree) {
    if (tree == NULL) {
        return -1; // Empty tree, return -1
    }

    // Internal call to calculate height from the root
    return _bstCalculateHeight(tree->root);
}

// Internal function to perform a left rotation
TreeNode * _bstRotLeft(TreeNode *root) {
    TreeNode *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;

    return newRoot;
}

// Internal function to perform a right rotation
TreeNode * _bstRotRight(TreeNode *root) {
    TreeNode *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;

    return newRoot;
}

// Internal function to get the balance factor of a node
int _bstGetBalanceFactor(TreeNode *node) {
    if (node == NULL) {
        return 0; // Balance factor for an empty node is 0
    }

    // Difference between left and right subtree heights
    return _bstCalculateHeight(node->left) - _bstCalculateHeight(node->right);
}


// Function to balance the tree (after insertion/deletion)
bool bstBalanceTree(BST *tree) {
    if (tree == NULL) {
        return false; // Handle null pointer case
    }

    int balanceFactor = _bstGetBalanceFactor(tree->root);

    // Left-heavy case (balance factor > 1)
    if (balanceFactor > 1) {
        if (_bstGetBalanceFactor(tree->root->left) < 0) {
            // Left-Right (LR) case
            tree->root->left = _bstRotLeft(tree->root->left);
        }
        // Left-Left (LL) case
        tree->root = _bstRotRight(tree->root);
    }

    // Right-heavy case (balance factor < -1)
    else if (balanceFactor < -1) {
        if (_bstGetBalanceFactor(tree->root->right) > 0) {
            // Right-Left (RL) case
            tree->root->right = _bstRotRight(tree->root->right);
        }
        // Right-Right (RR) case
        tree->root = _bstRotLeft(tree->root);
    }

    return true; // Tree is balanced
}

// Internal function for freeing a node
bool _bstFreeNode(TreeNode *node, bool freeData) {
    if (node == NULL) {
        return true; // Base case: nothing to free
    }

    // Recursively free the left and right subtrees
    if (!_bstFreeNode(node->left, freeData) || !_bstFreeNode(node->right, freeData)) {
        return false; // Return false if any recursive call fails
    }

    // Free the data if needed
    if (freeData && node->data != NULL) {
        free(node->data);
    }

    // Free the node itself
    free(node);

    return true; // Success
}

// Function to free the whole tree
bool bstFree(BST *tree) {
    if (tree == NULL) {
        return false; // Handle null tree pointer
    }

    // Free the root node (and all sub-nodes)
    bool result = _bstFreeNode(tree->root, tree->freeData);

    // Free the tree structure itself
    free(tree);

    return result; // Return the result of freeing nodes
}


// Function to insert a node, to the tree
bool bstInsertNode(BST *tree, unsigned int key, void *data) {

    if (tree == NULL) {
        return false;
    }

    // Allocate memory for the new node
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

    if (newNode == NULL) {
        return false;
    }

    // Set the key and data in the new node
    newNode->key = key;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    // If the tree is empty, set the new node as the root
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->size++;
        return true;
    }

    // Traverse the tree to find the correct insertion point
    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    while (current != NULL) {
        parent = current;

        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            free(newNode);
            return false; // Key already exists in the tree
        }
    }

    // Insert the new node at the correct position
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    tree->size++;

    return bstBalanceTree(tree);
}

bool bstPopNode(BST *tree, unsigned int key, void **returnData) {
    if (tree == NULL || tree->root == NULL) {
        return false; // Handle null tree
    }

    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    // Find the node to be removed
    while (current != NULL) {
        if (key == current->key) {
            break;
        }

        parent = current;

        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        return false; // Key not found
    }

    *returnData = current->data; // Set the returnData to the node's data

    // Now unlink the node from the tree
    TreeNode *replacement = NULL;

    // Case 1: Node has no children
    if (current->left == NULL && current->right == NULL) {
        replacement = NULL;

    // Case 2: Node has one child
    } else if (current->left == NULL || current->right == NULL) {
        replacement = (current->left != NULL) ? current->left : current->right;

    // Case 3: Node has two children
    } else {
        // Find in-order successor (smallest node in the right subtree)
        TreeNode *successor = current->right;
        TreeNode *successorParent = current;

        while (successor->left != NULL) {
            successorParent = successor;
            successor = successor->left;
        }

        // Swap values with successor (no need to swap the actual nodes)
        current->key = successor->key;
        current->data = successor->data;

        // Remove the successor (it will have at most one child)
        current = successor;
        parent = successorParent;
        replacement = current->right; // Successor has no left child, so it's right child or NULL
    }

    // Unlink the node
    if (parent == NULL) {
        tree->root = replacement; // If no parent, we're removing the root
    } else if (parent->left == current) {
        parent->left = replacement;
    } else {
        parent->right = replacement;
    }

    free(current); // Free the removed node
    tree->size--;

    // Now balance the tree
    return bstBalanceTree(tree);
}

// Function to remove a node from the tree
bool bstRemoveNode(BST *tree, unsigned int key) {

    void *data = NULL;
    bool result = bstPopNode(tree, key, &data);

    if (result && tree->freeData) {
        free(data);
    }

    return result;
}

// Function to search for a node in the tree
void * bstSearchForNode(BST *tree, unsigned int key) {

    if (tree == NULL || tree->root == NULL) {
        return NULL; // Handle null tree
    }

    TreeNode *current = tree->root;

    // Traverse the tree to find the node
    while (current != NULL) {
        if (key == current->key) {
            return current->data; // Return the data if the key matches
        }

        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL; // Key not found
} 


