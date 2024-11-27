/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file bst.c
 * @date 29.9. 2024
 * @brief Test file for the linked list implementation
 */


#include <stdlib.h>
#include <stdio.h>
#include "utility/binary_search_tree.h"
#include "utility/enumerations.h"


/*
// call like this -> (void (*)(unsigned int, void *))print_int
// Function prototype


// Function to print an integer
void print_int(unsigned int idx, void* data) {
printf("    %d: %d", idx,  *(int*)data);
*/

void (*printFunc)(unsigned int, void*);

void print_tree_node(unsigned int key, void *data) {
    TreeNode *node = (TreeNode *)data;
    printf("    %d: %u", key, node->key);
}

void test_case_1(void) {

    TestInstancePtr testInstance = initTestInstance("Binary Search Tree (BST) Test with ints, malloc");

    // Create a new tree with freeData = free (free data when nodes are removed)
    BST *tree = bstInit(freeWrapper);

    // Allocate some integers for testing
    int *data1 = (int *)malloc(sizeof(int));
    int *data2 = (int *)malloc(sizeof(int));
    int *data3 = (int *)malloc(sizeof(int));

    *data1 = 10;
    *data2 = 20;
    *data3 = 5;

    // Insert node with key 10 and data 10
    testCase(
        testInstance,
        bstInsertNode(tree, 10, data1),
        "Inserting node with key 10 and data 10",
        "Successfully inserted node with key 10 (expected)",
        "Failed to insert node with key 10 (unexpected)"
    );

    // Insert node with key 20 and data 20
    testCase(
        testInstance,
        bstInsertNode(tree, 20, data2),
        "Inserting node with key 20 and data 20",
        "Successfully inserted node with key 20 (expected)",
        "Failed to insert node with key 20 (unexpected)"
    );

    // Insert node with key 5 and data 5
    testCase(
        testInstance,
        bstInsertNode(tree, 5, data3),
        "Inserting node with key 5 and data 5",
        "Successfully inserted node with key 5 (expected)",
        "Failed to insert node with key 5 (unexpected)"
    );

    // Test search for key 20
    int *searchResult = (int *)bstSearchForNode(tree, 20);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 20",
        "Found node with key 20 (expected)",
        "Failed to find node with key 20 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 20: %d\n", *searchResult);
    }

    // Test search for key 5
    searchResult = (int *)bstSearchForNode(tree, 5);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 5",
        "Found node with key 5 (expected)",
        "Failed to find node with key 5 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 5: %d\n", *searchResult);
    }

    // Test search for key 10
    searchResult = (int *)bstSearchForNode(tree, 10);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 10",
        "Found node with key 10 (expected)",
        "Failed to find node with key 10 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 10: %d\n", *searchResult);
    }

    // Test search for key 15 (not in the tree)
    searchResult = (int *)bstSearchForNode(tree, 15);
    testCase(
        testInstance,
        searchResult == NULL,
        "Searching for node with key 15 (not in the tree)",
        "Node with key 15 not found (expected)",
        "Found node with key 15 (unexpected)"
    );

    // Test remove node with key 10
    bool removeResult = bstRemoveNode(tree, 10);
    testCase(
        testInstance,
        removeResult,
        "Removing node with key 10",
        "Successfully removed node with key 10 (expected)",
        "Failed to remove node with key 10 (unexpected)"
    );
    printf("$$ Remove result for key 10: %s\n", removeResult ? "Success" : "Failure");

    // Test pop node with key 20
    int *removedData = NULL;
    removeResult = bstPopNode(tree, 20, (void *)&removedData);
    testCase(
        testInstance,
        removeResult,
        "Popping node with key 20",
        "Successfully popped node with key 20 (expected)",
        "Failed to pop node with key 20 (unexpected)"
    );
    if (removedData) {
        printf("$$ Popped node at key 20: %d\n", *removedData);
    }

    // Test search for key 20 after removal
    searchResult = (int *)bstSearchForNode(tree, 20);
    testCase(
        testInstance,
        searchResult == NULL,
        "Searching for node with key 20 after removal",
        "Node with key 20 not found (expected)",
        "Found node with key 20 (unexpected)"
    );

    // Free the entire tree (and its data)
    bool freeResult = bstFree(&tree);
    testCase(
        testInstance,
        freeResult,
        "Freeing the binary search tree",
        "Tree freed successfully (expected)",
        "Failed to free the tree (unexpected)"
    );
    printf("$$ Tree freed: %s\n", freeResult ? "Success" : "Failure");

    // Free the remaining data that was manually popped
    free(removedData);

    finishTestInstance(testInstance);
}


void test_case_2(void) {

    TestInstancePtr testInstance = initTestInstance("Binary Search Tree (BST) Test with chars, no malloc");

    // Create a new tree with freeData = NULL (no free function for data)
    BST *tree = bstInit(NULL);

    // Insert nodes with character data
    char data1 = 'A';
    char data2 = 'B';
    char data3 = 'C';
    char data4 = 'D';
    char data5 = 'E';
    char data6 = 'F';
    char data7 = 'G';
    char data8 = 'H';
    char data9 = 'I';

    // Insert node 1 with 'A'
    testCase(
        testInstance,
        bstInsertNode(tree, 1, &data1),
        "Inserting node with key 1 and data 'A'",
        "Successfully inserted node with key 1 (expected)",
        "Failed to insert node with key 1 (unexpected)"
    );

    // Insert node 2 with 'B'
    testCase(
        testInstance,
        bstInsertNode(tree, 2, &data2),
        "Inserting node with key 2 and data 'B'",
        "Successfully inserted node with key 2 (expected)",
        "Failed to insert node with key 2 (unexpected)"
    );

    // Insert node 3 with 'C'
    testCase(
        testInstance,
        bstInsertNode(tree, 3, &data3),
        "Inserting node with key 3 and data 'C'",
        "Successfully inserted node with key 3 (expected)",
        "Failed to insert node with key 3 (unexpected)"
    );

    // Insert node 4 with 'D'
    testCase(
        testInstance,
        bstInsertNode(tree, 4, &data4),
        "Inserting node with key 4 and data 'D'",
        "Successfully inserted node with key 4 (expected)",
        "Failed to insert node with key 4 (unexpected)"
    );

    // Insert node 5 with 'E'
    testCase(
        testInstance,
        bstInsertNode(tree, 5, &data5),
        "Inserting node with key 5 and data 'E'",
        "Successfully inserted node with key 5 (expected)",
        "Failed to insert node with key 5 (unexpected)"
    );

    // Insert node 6 with 'F'
    testCase(
        testInstance,
        bstInsertNode(tree, 6, &data6),
        "Inserting node with key 6 and data 'F'",
        "Successfully inserted node with key 6 (expected)",
        "Failed to insert node with key 6 (unexpected)"
    );

    // Insert node 7 with 'G'
    testCase(
        testInstance,
        bstInsertNode(tree, 7, &data7),
        "Inserting node with key 7 and data 'G'",
        "Successfully inserted node with key 7 (expected)",
        "Failed to insert node with key 7 (unexpected)"
    );

    // Insert node 8 with 'H'
    testCase(
        testInstance,
        bstInsertNode(tree, 8, &data8),
        "Inserting node with key 8 and data 'H'",
        "Successfully inserted node with key 8 (expected)",
        "Failed to insert node with key 8 (unexpected)"
    );

    // Insert node 9 with 'I'
    testCase(
        testInstance,
        bstInsertNode(tree, 9, &data9),
        "Inserting node with key 9 and data 'I'",
        "Successfully inserted node with key 9 (expected)",
        "Failed to insert node with key 9 (unexpected)"
    );

    // Test search for key 4 (D)
    char *searchResult = (char *)bstSearchForNode(tree, 4);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 4",
        "Found node with key 4 (expected)",
        "Failed to find node with key 4 (unexpected)"
    );
    testCase(
        testInstance,
        searchResult && *searchResult == data4,
        "Verifying data for node with key 4",
        "Data for node with key 4 matches 'D' (expected)",
        "Data for node with key 4 does not match 'D' (unexpected)"
    );

    // Test search for key 5 (E)
    searchResult = (char *)bstSearchForNode(tree, 5);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 5",
        "Found node with key 5 (expected)",
        "Failed to find node with key 5 (unexpected)"
    );
    testCase(
        testInstance,
        searchResult && *searchResult == data5,
        "Verifying data for node with key 5",
        "Data for node with key 5 matches 'E' (expected)",
        "Data for node with key 5 does not match 'E' (unexpected)"
    );

    // Test search for key 6 (F)
    searchResult = (char *)bstSearchForNode(tree, 6);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 6",
        "Found node with key 6 (expected)",
        "Failed to find node with key 6 (unexpected)"
    );
    testCase(
        testInstance,
        searchResult && *searchResult == data6,
        "Verifying data for node with key 6",
        "Data for node with key 6 matches 'F' (expected)",
        "Data for node with key 6 does not match 'F' (unexpected)"
    );

    // Test search for key 7 (G)
    searchResult = (char *)bstSearchForNode(tree, 7);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 7",
        "Found node with key 7 (expected)",
        "Failed to find node with key 7 (unexpected)"
    );
    testCase(
        testInstance,
        searchResult && *searchResult == data7,
        "Verifying data for node with key 7",
        "Data for node with key 7 matches 'G' (expected)",
        "Data for node with key 7 does not match 'G' (unexpected)"
    );

    // Test remove node with key 5 (E)
    bool removeResult = bstRemoveNode(tree, 5);
    testCase(
        testInstance,
        removeResult,
        "Removing node with key 5",
        "Successfully removed node with key 5 (expected)",
        "Failed to remove node with key 5 (unexpected)"
    );

    // Test pop node with key 4 (D)
    char *removedData = NULL;
    removeResult = bstPopNode(tree, 4, (void *)&removedData);
    testCase(
        testInstance,
        removeResult,
        "Popping node with key 4",
        "Successfully popped node with key 4 (expected)",
        "Failed to pop node with key 4 (unexpected)"
    );
    testCase(
        testInstance,
        removedData && *removedData == data4,
        "Verifying popped data for node with key 4",
        "Popped data matches 'D' (expected)",
        "Popped data does not match 'D' (unexpected)"
    );

    // Test search for key 4 after removal
    searchResult = (char *)bstSearchForNode(tree, 4);
    testCase(
        testInstance,
        searchResult == NULL,
        "Searching for node with key 4 after removal",
        "Node with key 4 not found (expected)",
        "Found node with key 4 (unexpected)"
    );

    // Test search for key 8 (H)
    searchResult = (char *)bstSearchForNode(tree, 8);
    testCase(
        testInstance,
        searchResult != NULL,
        "Searching for node with key 8",
        "Found node with key 8 (expected)",
        "Failed to find node with key 8 (unexpected)"
    );
    testCase(
        testInstance,
        searchResult && *searchResult == data8,
        "Verifying data for node with key 8",
        "Data for node with key 8 matches 'H' (expected)",
        "Data for node with key 8 does not match 'H' (unexpected)"
    );


    LinkedList *treeNodes = bstGetNodes(tree);
    printf("$$ Tree nodes:\n");
    // print the tree
    printList(treeNodes,  (void (*)(unsigned int, void *))print_tree_node);
    removeList(&treeNodes); // free the memory

    // Free the tree (without freeing data)
    bool freeResult = bstFree(&tree);
    testCase(
        testInstance,
        freeResult,
        "Freeing the binary search tree",
        "Tree freed successfully (expected)",
        "Failed to free the tree (unexpected)"
    );

    finishTestInstance(testInstance);
}


int main(void) {

    test_case_1();
    test_case_2();

    return 0;
}
