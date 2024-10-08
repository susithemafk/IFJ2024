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
#include <string.h>
#include "semantical/symtable.h"

// ####################### HB BINARY SEARCH TREE #######################


BST *bstInit(void (*freeFunction)(void *data)) {

    BST *tree = (BST *)malloc(sizeof(BST));

    if (tree == NULL) {
        return NULL;
    }

    tree->size = 0;
    tree->root = NULL;
    tree->freeFunction = freeFunction;

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

    if (result && tree->freeFunction != NULL) {
        tree->freeFunction(data); // Free the data if a free function is provided
    }

    return result;
}

// Function to search for a node in the tree
void *bstSearchForNode(BST *tree, unsigned int key) {

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


// Internal function for freeing a node
bool _bstFreeNode(TreeNode *node, void (*freeFunction)(void *data)) {
    if (node == NULL) {
        return true; // Base case: nothing to free
    }

    // Recursively free the left and right subtrees
    if (!_bstFreeNode(node->left, freeFunction) || !_bstFreeNode(node->right, freeFunction)) {
        return false; // Return false if any recursive call fails
    }

    // Free the data if a free function is provided
    if (freeFunction != NULL && node->data != NULL) {
        freeFunction(node->data);  // Call the function pointer to free the data
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
    bool result = _bstFreeNode(tree->root, tree->freeFunction);

    // Free the tree structure itself
    free(tree);

    return result; // Return the result of freeing nodes
}

// ####################### SYMTABLE #######################


void (*freeDataFunction)(void *);

// Function to search for a scope in the same hash variables list
bool _searchForVarSameHash(LinkedList *list, char *name) {

    // check if the list is not NULL
    if (list == NULL) {
        return false;
    }

    unsigned int size = getSize(list);
    for (unsigned int i = 0; i < size; i++) {
        SymVariable *variable = (SymVariable *)getDataAtIndex(list, i);
        if (variable != NULL && strcmp(variable->name, name) == 0) {
            return true;
        }
    }
    return false;
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
    globalScope->key = 0; // key for the global scope
    globalScope->parent = NULL; // no parent
    globalScope->variables = NULL; // link to variables, in global scope disabled
    globalScope->innerScope = NULL; // link to other scopes

    // fill the table
    table->root = globalScope;
    table->currentScope = globalScope;
    table->scopeCount = 1;

    return table;
}

// Function to insert a new scope into the symbol table
bool symTableMoveScopeDown(SymTable *table, enum SYMTABLE_NODE_TYPES type) {

    // check if the table is not NULL
    if (table == NULL) {
        return false;
    }

    // create the new scope
    SymTableNode *newScope = (SymTableNode *)malloc(sizeof(SymTableNode));
    BST *variables = bstInit((void (*)(void *))removeList);

    // check if the memory was allocated

    if (newScope == NULL || variables == NULL) {
        return false;
    }

    // fill the new scope
    newScope->type = type;
    newScope->key = table->scopeCount; // key for the new scope
    newScope->parent = table->currentScope; // parent is the current scope
    newScope->variables = variables; // link to variables
    newScope->innerScope = NULL; // link to other scopes

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
bool symTableExitScope(SymTable *table, enum ERR_CODES *returnCode) {

    // check if the table is not NULL
    if (table == NULL) {
        return false;
    }

    // in case we are trying to exit the global scope
    if (
        table->currentScope->innerScope == NULL && 
        table->currentScope->type == SYM_GLOBAL) {
        symTableFree(table);
        if (returnCode != NULL) {
            *returnCode = E_NONE;
        }
        return true;
    }

    // need to remove the current scope
    SymTableNode *currentScope = table->currentScope;

    // check if all variables were accesed
    if (_symTableAllVariablesAccesed(currentScope)) {
        if (returnCode != NULL) {
            *returnCode = E_NONE;
        }
    } else {
        if (returnCode != NULL) {
            *returnCode = E_SEMANTIC_UNUSED_VAR;
        }
    }

    // update the current scope
    table->currentScope = table->currentScope->parent;
    table->scopeCount--;
    table->currentScope->innerScope = NULL;

    // free the current scope
    bool result = bstFree(currentScope->variables);
    free(currentScope);

    return result;
}

// Function to insert a new
bool symTableDeclareVariable(SymTable *table, char *name, enum DATA_TYPES type, bool mutable) {
    // Check if the table or current scope is invalid (if global scope declaration is disallowed)
    if (table == NULL || table->currentScope->type == SYM_GLOBAL) {
        return false;
    }

    // Check if the variable already exists in the current scope
    SymVariable *var = NULL;
    if (symTableFindVariable(table, name, &var)) {
        return false; // Variable already exists
    }

    // Allocate memory for a new variable
    SymVariable *newVariable = (SymVariable *)malloc(sizeof(SymVariable));
    if (newVariable == NULL) {
        return false;
    }

    // Duplicate the name and check if strdup succeeded
    newVariable->name = name;
    if (newVariable->name == NULL) {
        free(newVariable); // Cleanup if strdup fails
        return false;
    }

    // Initialize other fields
    newVariable->type = type;
    newVariable->mutable = mutable;
    newVariable->accesed = false;

    // Get the hash of the variable's name
    unsigned int hash = hashString(name);

    // Get the variables list in the current scope's hash table
    BST *variables = table->currentScope->variables;
    void *sameHashVariables = bstSearchForNode(variables, hash);

    if (sameHashVariables == NULL) {
        // Create a new linked list for variables with the same hash
        sameHashVariables = initLinkedList(true);
        insertNodeAtIndex((LinkedList *)sameHashVariables, (void *)newVariable, -1);
        bstInsertNode(variables, hash, (void *)sameHashVariables);
        return true;    
    } 

    // Insert the new variable into the existing list
    if (!insertNodeAtIndex((LinkedList *)sameHashVariables, (void *)newVariable, -1)) {
        free(newVariable->name);
        free(newVariable); // Cleanup if insertion fails
        return false;
    }

    return true; // Success
}

// Function to find a variable in the current scope (including parent scopes)
bool symTableFindVariable(SymTable *table, char *name, SymVariable **returnData) {

    // Check if the table or name is null
    if (table == NULL || name == NULL) 
        return false;

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
            if (returnData != NULL) 
                *returnData = variable;

            variable->accesed = true; // Only mark if you intend to track access

            return true;
        }
        
        // Move up to the parent scope if not found
        currentScope = currentScope->parent;
    }
    
    return false; // Variable not found
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
        bstFree(node->variables);
    }

    free(node);
    return;
}

// Function to free the symbol table
bool symTableFree(SymTable *table) {

    if (table == NULL)
        return false;

    // free the inner scopes
    if (table->root != NULL) 
        _symTableFreeNode(table->root);
    
    // free the table
    free(table);
    return true;
}


