/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symbol_tree.c
 * @date 28.9. 2024
 * @brief Implementation of the symbol tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/abstract_syntax_tree.h"
#include "utility/linked_list.h"
#include "utility/enumerations.h"

// Internal function to create a new node
ASTNodePtr _astCreateNewNode(unsigned int key, void *data, ASTNodePtr parent) {

    // alocate the memeory for the new node
    ASTNodePtr newNode = (ASTNodePtr)malloc(sizeof(struct ASTNode));

    // check if the allocation was successful
    if (newNode == NULL) {
        return NULL;
    }

    // set the node data
    newNode->key = key;
    newNode->data = data;
    newNode->parent = parent;

    // init the linked list
    newNode->children = initLinkedList(true);

    // check if the list was initialized
    if (newNode->children == NULL) {
        free(newNode);
        return NULL;
    }

    return newNode;
}

// Function to init the AST
ASTPtr astInit(void (*freeFunctionData)(void *data), unsigned int startIdx) {

    // allocate the mem for the tree
    ASTPtr tree = (ASTPtr)malloc(sizeof(struct AST));

    // check if the allocation was successful
    if (tree == NULL) {
        return NULL;
    }

    // set the tree data
    tree->current = NULL;
    tree->root = NULL;
    tree->size = 0;
    tree->freeFunctionData = freeFunctionData;

    return tree;
}

// Function to add a child to the tree, at the current node
bool astAddChildToCurrent(ASTPtr tree, void *data, bool switchTo) {

    // chekc if the tree is valid
    if (tree == NULL) {
        return false;
    }

    // create the new node
    ASTNodePtr newNode = _astCreateNewNode(tree->size, data, tree->current);

    // check if the node was created
    if (newNode == NULL) {
        return false;
    }

    // check if the tree is empty
    if (switchTo && tree->current == NULL && tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        tree->size++;
        return true;
    }

    // add the new node to the childrens list
    if (!insertNodeAtIndex(tree->current->children, (void *)newNode, -1))
        return false;

    // check if the current node should be switched
    if (switchTo) {
        tree->current = newNode;
    }

    tree->size++;

    return true;
}

// Function to add a node next to the current node
bool astAddNextToCurrent(ASTPtr tree, void *data, bool switchTo) {

    // check, if the tree is valid
    if (tree == NULL || tree->current == NULL || tree->size == 0) {
        return false;
    }

    // create the new node
    ASTNodePtr newNode = _astCreateNewNode(tree->size, data, tree->current->parent);

    // check if the node was created
    if (newNode == NULL) {
        return false;
    }

    // add the new node to the children list, of the parent node
    if (!insertNodeAtIndex(tree->current->parent->children, (void *)newNode, -1))
        return false;   

    // check if the current node should be switched
    if (switchTo) {
        tree->current = newNode;
    }

    tree->size++;

    return true;

}


// Function to switch the current node to the parent node
bool astSwitchToParent(ASTPtr tree) {
    
    // check if the tree is valid
    if (tree == NULL) {
        return false;
    }

    // check if the current node is the root
    if (tree->current == tree->root) {
        return false;
    }

    // set the current node to the parent
    tree->current = tree->current->parent;

    return true;

}

// Function to switch the current node to the child node
bool astSwitchToChild(ASTPtr tree, int index) {

    // check if the tree is valid
    if (tree == NULL) {
        return false;
    }

    // get the node from the children list
    ASTNodePtr node = (ASTNodePtr)getDataAtIndex(tree->current->children, index);

    // invalid index
    if (node == NULL) {
        return false;
    }

    // set the current node
    tree->current = node;

    return true;
}

// Function to search for a node in the tree
ASTNodePtr astSearchForNode(ASTPtr tree, unsigned int key) {

    // check if the tree is valid
    if (tree == NULL) {
        return NULL;
    }

    // check if the tree is empty
    if (tree->root == NULL || tree->size == 0) {
        return NULL;
    }

    // go throught the current children list and search for the key
    LinkedList *list = tree->current->children;

    unsigned int size = getSize(list);
    for (unsigned int i = 0; i < size; i++) {
        ASTNodePtr node = (ASTNodePtr)getDataAtIndex(list, i);
        if (node->key == key) {
            return node->data;
        }
    }

    return NULL;
}

bool _astFree(ASTNodePtr node) {

    // check if the node is valid
    if (node == NULL) {
        return false;
    }

    // free the children list, utilizing recursion
    if (node->children != NULL && getSize(node->children) > 0) {
        for (unsigned int i = 0; i < getSize(node->children); i++) {
            ASTNodePtr child = (ASTNodePtr)getDataAtIndex(node->children, i);
            _astFree(child);
        }
    }

    // at this point, we know, that all the children are freed
    removeList(node->children);

    // free the data
    if (node->data != NULL) {
        free(node->data);
    }

    // free the node
    free(node);

    return true;
}

bool astFree(ASTPtr tree) {

    // check if the tree is valid
    if (tree == NULL) {
        return false;
    }

    // free the tree, utilizing recursion
    bool result = false;
    if (tree->root != NULL) {
        result = _astFree(tree->root);
    }

    // free the tree
    free(tree);

    return true;
}

