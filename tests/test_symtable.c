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
#include "utility/enumerations.h"


void test_case_1(void) {
    unsigned int testNumber = 1;

    // Create a new tree with freeData = free (free data when nodes are removed)
    BST *tree = bstInit(free);

    // Allocate some integers for testing
    int *data1 = (int *)malloc(sizeof(int));
    int *data2 = (int *)malloc(sizeof(int));
    int *data3 = (int *)malloc(sizeof(int));

    *data1 = 10;
    *data2 = 20;
    *data3 = 5;

    // Insert node with key 10 and data 10
    testCase(
        &testNumber,
        bstInsertNode(tree, 10, data1),
        "Inserting node with key 10 and data 10",
        "Successfully inserted node with key 10 (expected)",
        "Failed to insert node with key 10 (unexpected)"
    );

    // Insert node with key 20 and data 20
    testCase(
        &testNumber,
        bstInsertNode(tree, 20, data2),
        "Inserting node with key 20 and data 20",
        "Successfully inserted node with key 20 (expected)",
        "Failed to insert node with key 20 (unexpected)"
    );

    // Insert node with key 5 and data 5
    testCase(
        &testNumber,
        bstInsertNode(tree, 5, data3),
        "Inserting node with key 5 and data 5",
        "Successfully inserted node with key 5 (expected)",
        "Failed to insert node with key 5 (unexpected)"
    );

    // Test search for key 20
    int *searchResult = (int *)bstSearchForNode(tree, 20);
    testCase(
        &testNumber,
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
        &testNumber,
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
        &testNumber,
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
        &testNumber,
        searchResult == NULL,
        "Searching for node with key 15 (not in the tree)",
        "Node with key 15 not found (expected)",
        "Found node with key 15 (unexpected)"
    );

    // Test remove node with key 10
    bool removeResult = bstRemoveNode(tree, 10);
    testCase(
        &testNumber,
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
        &testNumber,
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
        &testNumber,
        searchResult == NULL,
        "Searching for node with key 20 after removal",
        "Node with key 20 not found (expected)",
        "Found node with key 20 (unexpected)"
    );

    // Free the entire tree (and its data)
    bool freeResult = bstFree(tree);
    testCase(
        &testNumber,
        freeResult,
        "Freeing the binary search tree",
        "Tree freed successfully (expected)",
        "Failed to free the tree (unexpected)"
    );
    printf("$$ Tree freed: %s\n", freeResult ? "Success" : "Failure");

    // Free the remaining data that was manually popped
    free(removedData);
}


void test_case_2(void) {
    unsigned int testNumber = 1;

    // Create a new tree with freeData = NULL (no free function for data)
    BST *tree = bstInit(NULL);

    // Insert nodes with character data
    char data1 = 'A';
    char data2 = 'B';
    char data3 = 'C';

    // Insert node 1 with 'A'
    testCase(
        &testNumber,
        bstInsertNode(tree, 1, &data1),
        "Inserting node with key 1 and data 'A'",
        "Successfully inserted node with key 1 (expected)",
        "Failed to insert node with key 1 (unexpected)"
    );

    // Insert node 2 with 'B'
    testCase(
        &testNumber,
        bstInsertNode(tree, 2, &data2),
        "Inserting node with key 2 and data 'B'",
        "Successfully inserted node with key 2 (expected)",
        "Failed to insert node with key 2 (unexpected)"
    );

    // Insert node 3 with 'C'
    testCase(
        &testNumber,
        bstInsertNode(tree, 3, &data3),
        "Inserting node with key 3 and data 'C'",
        "Successfully inserted node with key 3 (expected)",
        "Failed to insert node with key 3 (unexpected)"
    );

    // Test search for key 1
    char *searchResult = (char *)bstSearchForNode(tree, 1);
    testCase(
        &testNumber,
        searchResult != NULL,
        "Searching for node with key 1",
        "Found node with key 1 (expected)",
        "Failed to find node with key 1 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 1: %c\n", *searchResult);
    }

    // Test search for key 2
    searchResult = (char *)bstSearchForNode(tree, 2);
    testCase(
        &testNumber,
        searchResult != NULL,
        "Searching for node with key 2",
        "Found node with key 2 (expected)",
        "Failed to find node with key 2 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 2: %c\n", *searchResult);
    }

    // Test search for key 3
    searchResult = (char *)bstSearchForNode(tree, 3);
    testCase(
        &testNumber,
        searchResult != NULL,
        "Searching for node with key 3",
        "Found node with key 3 (expected)",
        "Failed to find node with key 3 (unexpected)"
    );
    if (searchResult) {
        printf("$$ Search result for key 3: %c\n", *searchResult);
    }

    // Test remove node with key 1
    bool removeResult = bstRemoveNode(tree, 1);
    testCase(
        &testNumber,
        removeResult,
        "Removing node with key 1",
        "Successfully removed node with key 1 (expected)",
        "Failed to remove node with key 1 (unexpected)"
    );
    printf("$$ Remove result for key 1: %s\n", removeResult ? "Success" : "Failure");

    // Test pop node with key 2
    char *removedData = NULL;
    removeResult = bstPopNode(tree, 2, (void *)&removedData);
    testCase(
        &testNumber,
        removeResult,
        "Popping node with key 2",
        "Successfully popped node with key 2 (expected)",
        "Failed to pop node with key 2 (unexpected)"
    );
    if (removedData) {
        printf("$$ Popped node at key 2: %c\n", *removedData);
    }

    // Test search for key 2 after removal
    searchResult = (char *)bstSearchForNode(tree, 2);
    testCase(
        &testNumber,
        searchResult == NULL,
        "Searching for node with key 2 after removal",
        "Node with key 2 not found (expected)",
        "Found node with key 2 (unexpected)"
    );

    // Free the tree (without freeing data)
    bool freeResult = bstFree(tree);
    testCase(
        &testNumber,
        freeResult,
        "Freeing the binary search tree",
        "Tree freed successfully (expected)",
        "Failed to free the tree (unexpected)"
    );
    printf("$$ Tree freed: %s\n", freeResult ? "Success" : "Failure");
}



void test_case_3(void) {
    unsigned int testNumber = 1;

    // Initialize a new symbol table
    SymTable *table = symTableInit();
    testCase(
        &testNumber,
        table != NULL,
        "Initializing a new symbol table",
        "Successfully initialized a new symbol table (expected)",
        "Failed to initialize a new symbol table (unexpected)"
    );

    // Test that declaring variables in the global scope fails
    testCase(
        &testNumber,
        !symTableDeclareVariable(table, "X", dTypeI32, true),
        "Attempting to declare variable 'X' in the global scope (should fail)",
        "Correctly failed to declare 'X' in the global scope (expected)",
        "Unexpectedly succeeded in declaring 'X' in the global scope (unexpected)"
    );

    // Add a new scope (e.g., function)
    testCase(
        &testNumber,
        symTableMoveScopeDown(table, SYM_FUNCTION),
        "Adding a new function scope",
        "Successfully added a new function scope (expected)",
        "Failed to add function scope (unexpected)"
    );

    // Declare variable 'X' inside the function scope
    testCase(
        &testNumber,
        symTableDeclareVariable(table, "X", dTypeI32, true),
        "Declaring variable 'X' in the function scope",
        "Successfully declared 'X' in the function scope (expected)",
        "Failed to declare 'X' in the function scope (unexpected)"
    );

    // Move down to a new scope (e.g., inside a block)
    testCase(
        &testNumber,
        symTableMoveScopeDown(table, SYM_IF),
        "Adding a new block scope inside the function",
        "Successfully added a block scope (expected)",
        "Failed to add block scope (unexpected)"
    );

    // Redeclare variable 'X' in the block scope (should fail)
    testCase(
        &testNumber,
        !symTableDeclareVariable(table, "X", dTypeI32, true),
        "Attempting to redeclare variable 'X' in the block scope (should fail)",
        "Correctly failed to redeclare 'X' in the block scope (expected)",
        "Unexpectedly succeeded in redeclaring 'X' in the block scope (unexpected)"
    );

    // Declare a new variable 'Y' in the block scope
    testCase(
        &testNumber,
        symTableDeclareVariable(table, "Y", dTypeF64, true),
        "Declaring variable 'Y' in the block scope",
        "Successfully declared 'Y' in the block scope (expected)",
        "Failed to declare 'Y' in the block scope (unexpected)"
    );

    // Test finding 'Y' in the current scope
    SymVariable *varY = NULL;
    testCase(
        &testNumber,
        symTableFindVariable(table, "Y", &varY),
        "Searching for variable 'Y' in the block scope",
        "Found 'Y' in the block scope (expected)",
        "Failed to find 'Y' in the block scope (unexpected)"
    );

    // Test finding 'X' from the function scope
    SymVariable *varX = NULL;
    testCase(
        &testNumber,
        symTableFindVariable(table, "X", &varX),
        "Searching for variable 'X' from the function scope",
        "Found 'X' from the function scope (expected)",
        "Failed to find 'X' from the function scope (unexpected)"
    );

    // Check if 'X' is mutable
    testCase(
        &testNumber,
        symTableCanMutate(varX),
        "Testing if variable 'X' is mutable",
        "Variable 'X' is mutable (expected)",
        "Variable 'X' is immutable (unexpected)"
    );

    // Exit block scope and test variable access
    enum ERR_CODES code;
    testCase(
        &testNumber,
        symTableExitScope(table, &code),
        "Exiting block scope (we have searched for all the variables)",
        "Exited block scope (expected)",
        "Failed to exit block scope (unexpected)"
    );

    // Check for unused variables
    testCase(
        &testNumber,
        code == E_NONE,
        "Checking for unused variables in exited block scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // Exit function scope and test variable access
    testCase(
        &testNumber,
        symTableExitScope(table, &code),
        "Exiting function scope",
        "Exited function scope (expected)",
        "Failed to exit function scope (unexpected)"
    );

    // Check for unused variables in function scope
    testCase(
        &testNumber,
        code == E_NONE,
        "Checking for unused variables in exited function scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // Exit global scope and test variable access
    testCase(
        &testNumber,
        symTableExitScope(table, &code),
        "Exiting global scope",
        "Exited global scope (expected)",
        "Failed to exit global scope (unexpected)"
    );

    // Check for unused variables in global scope
    testCase(
        &testNumber,
        code == E_NONE,
        "Checking for unused variables in exited global scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // Free the symbol table
    printf("-- Freeing the symbol table (when exiting the global scope)\n");
}

int main(void) {

    // Run the test cases
    printf("\n=========== Test BST malloced ==========\n");
    test_case_1();
    printf("\n=========== Test BST not malloced ==========\n");
    test_case_2();
    printf("\n=========== Test SymTable ==========\n");
    test_case_3();

    return 0;
}
