/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symbol_tree.h
 * @date 28.9. 2024
 * @brief Header file for the symbol tree
 */

#ifndef SYMBOL_TREE_H
#define SYMBOL_TREE_H

#include <stdbool.h>
#include "utility/enumerations.h"
#include "utility/linked_list.h"


// ####################### SYMBOL TREE #######################

// Prototype, need to thnk this throught
typedef struct SMTree {
    unsigned int amount_of_nodes;
    struct SMTTreeNode *root;
    void (*freeFunction)(void *data);  // Function pointer for freeing node data
} SMTree;

typedef struct SMTTreeNode {
    unsigned int key;
    TOKEN *token;
    struct SMTTreeNode *left;
    struct SMTTreeNode *right;
} SMTTreeNode;


#endif // SYMBOL_TREE_H
