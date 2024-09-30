/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_linked_list.c
 * @date 29.9. 2024
 * @brief Test file for the linked list implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include "semantical/symtable.h"

void test_case_1(void) {
    // Create a new tree with freeData = true
    BST *tree = bstInit(true);
    
    // Allocate some integers for testing
    int *data1 = (int *)malloc(sizeof(int));
    int *data2 = (int *)malloc(sizeof(int));
    int *data3 = (int *)malloc(sizeof(int));
    
    *data1 = 10;
    *data2 = 20;
    *data3 = 5;

    // Insert nodes
    bstInsertNode(tree, 10, data1);
    bstInsertNode(tree, 20, data2);
    bstInsertNode(tree, 5, data3);

    // Test search
    int *searchResult = (int *)bstSearchForNode(tree, 20);
    printf("$$ Search result for key 20: %d\n", *searchResult);

    searchResult = (int *)bstSearchForNode(tree, 5);
    printf("$$ Search result for key 5: %d\n", *searchResult);

    searchResult = (int *)bstSearchForNode(tree, 10);
    printf("$$ Search result for key 10: %d\n", *searchResult);

    searchResult = (int *)bstSearchForNode(tree, 15);   
    printf("$$ Search result for key 15 (not in the tree): %s\n", searchResult == NULL ? "Not found" : "Found");

    // Test remove node
    bool removeResult = bstRemoveNode(tree, 10);
    printf("$$ Remove result for key 10: %s\n", removeResult ? "Success" : "Failure");

    int *removedData = NULL;
    removeResult = bstPopNode(tree, 20, (void *)&removedData);
    printf("$$ Popped node at key 20: %d\n", *removedData);

    searchResult = (int *)bstSearchForNode(tree, 20);
    printf("$$ Search result for key 20 (after removal): %s\n", searchResult == NULL ? "Not found" : "Found");

    // Free the entire tree (and its data)
    bool freeResult = bstFree(tree);
    printf("$$ Tree freed: %s\n", freeResult ? "Success" : "Failure");

    free(data2);
}

void test_case_2(void) {
    // Create a new tree with freeData = false
    BST *tree = bstInit(false);
    
    // Insert nodes with character data
    char data1 = 'A';
    char data2 = 'B';
    char data3 = 'C';

    // Insert nodes
    bstInsertNode(tree, 1, &data1);
    bstInsertNode(tree, 2, &data2);
    bstInsertNode(tree, 3, &data3);

    // Test search
    char *searchResult = (char *)bstSearchForNode(tree, 1);
    printf("$$ Search result for key 1: %c\n", *searchResult);

    searchResult = (char *)bstSearchForNode(tree, 2);
    printf("$$ Search result for key 2: %c\n", *searchResult);

    searchResult = (char *)bstSearchForNode(tree, 3);
    printf("$$ Search result for key 3: %c\n", *searchResult);

    // Test remove node
    bool removeResult = bstRemoveNode(tree, 1);
    printf("$$ Remove result for key 1: %s\n", removeResult ? "Success" : "Failure");

    // Test pop node
    char *removedData = NULL;
    removeResult = bstPopNode(tree, 2, (void *)&removedData);
    printf("$$ Popped node at key 2: %c\n", *removedData);

    searchResult = (char *)bstSearchForNode(tree, 2);
    printf("$$ Search result for key 2 (after removal): %s\n", searchResult == NULL ? "Not found" : "Found");

    // Free the tree (without freeing data)
    bool freeResult = bstFree(tree);
    printf("$$ Tree freed: %s\n", freeResult ? "Success" : "Failure");
}


int main(void) {

    // Run the test cases
    test_case_1();
    test_case_2();

    return 0;
}
