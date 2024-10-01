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
    BST *tree = bstInit(free);
    
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
    BST *tree = bstInit(NULL);
    
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

void test_case_3(void) {

    // Create a new symbol table
    SymTable *table = symTableInit();

    if (table == NULL) {
        printf("$$ Failed to initialize symbol table\n");
        return;
    }

    // Declare some variables
    printf("$$ Declaring mutable variable 'X' of type I32\n");
    symTableDeclareVariable(table, "X", dTypeI32, true);

    printf("$$ Declaring immutable variable 'Y' of type F64\n");
    symTableDeclareVariable(table, "Y", dTypeF64, false);

    // Find the variables
    SymVariable *varX = NULL;
    SymVariable *varY = NULL;
    SymVariable *varZ = NULL;

    printf("$$ Searching for variable 'X'\n");
    if (symTableFindVariable(table, "X", &varX)) {
        printf("$$ Found variable 'X' of type %d: %s\n", varX->type, varX->type == dTypeI32 ? "(expected)" : "(unexpected)");
    }

    printf("$$ Searching for variable 'Y'\n");
    if (symTableFindVariable(table, "Y", &varY)) {
        printf("$$ Found variable 'Y' of type %d: %s\n", varY->type, varY->type == dTypeF64 ? "(expected)" : "(unexpected)");
    }

    // check for unknown variable
    printf("$$ Searching for unknown variable 'Z'\n");
    if (!symTableFindVariable(table, "Z", NULL)) {
        printf("$$ Variable 'Z' not found\n");
    } else {
        printf("$$ Variable 'Z' found (unexpected)\n");
    }

    printf("$$ Adding a new variable 'Z' of type u8 to the global scope\n");
    if (symTableDeclareVariable(table, "Z", dTypeU8, true)) {
        printf("$$ Declared variable 'Z' in the global scope (expected)\n");
    } else {
        printf("$$ Failed to declare variable 'Z' in the global scope (unexpected)\n");
    }

    // Check if the variables can be mutated
    printf("$$ Variable 'X' is %s : %s\n", symTableCanMutate(varX) ? "mutable" : "immutable", symTableCanMutate(varX) ? "(expected)" : "(unexpected)");
    printf("$$ Variable 'Y' is %s : %s\n", symTableCanMutate(varY) ? "mutable" : "immutable", symTableCanMutate(varY) ? "(unexpected)" : "(expected)");

    // add a new scope
    printf("$$ Adding a new scope\n");
    if (symTableMoveScopeDown(table, SYM_FUNCTION)) {
        printf("$$ Added a new scope (expected)\n");
    } else {
        printf("$$ Failed to add a new scope (unexpected)\n");
    }

    // add the same variable to the new scope
    printf("$$ Declaring variable 'X' in the new scope\n");
    if (symTableDeclareVariable(table, "X", dTypeI32, true)) {
        printf("$$ Declared variable 'X' in the new scope (expected)\n");
    } else {
        printf("$$ Failed to declare variable 'X' in the new scope (unexpected)\n");
    }

    // find the variable in the new scope
    printf("$$ Searching for variable 'X' in the new scope\n");
    if (symTableFindVariable(table, "X", &varX)) {
        printf("$$ Found variable 'X' in the new scope (expected)\n");
    } else {
        printf("$$ Variable 'X' not found in the new scope (unexpacted)\n");
    }

    // add the y variable, but mutable

    printf("$$ Declaring mutable variable 'Y' in the new scope\n");
    if (symTableDeclareVariable(table, "Y", dTypeF64, true)) {
        printf("$$ Declared mutable variable 'Y' in the new scope (expected)\n");
    } else {
        printf("$$ Failed to declare mutable variable 'Y' in the new scope (unexpected)\n");
    }

    // find the Y variable in the new scope
    printf("$$ Searching for variable 'Y' in the new scope\n");
    if (symTableFindVariable(table, "Y", &varY)) {
        printf("$$ Found variable 'Y' in the new scope (expected)\n");
    } else {
        printf("$$ Variable 'Y' not found in the new scope (unexpected)\n");
    }

    // check if the variable can be mutated
    printf("$$ Variable 'Y' is %s : %s\n", symTableCanMutate(varY) ? "mutable" : "immutable", symTableCanMutate(varY) ? "(expected)" : "(unexpected)");

    // try to find the Z variable
    printf("$$ Searching for unknown variable 'Z' in the new scope\n");
    if (symTableFindVariable(table, "Z", &varZ)) {
        printf("$$ Variable 'Z' found in the new scope (expected)\n");
    } else {
        printf("$$ Variable 'Z' not found in the new scope (unexpected)\n");
    }


    printf("$$ Variable 'Z' is of type %d : %s\n", varZ->type, varZ->type == dTypeU8 ? "(expected)" : "(unexpected)");
    printf("$$ Variable 'Z' is %s : %s\n", symTableCanMutate(varZ) ? "mutable" : "immutable", symTableCanMutate(varZ) ? "(expected)" : "(unexpected)");

    // delcare variable Z in the curretn scope, that will not be used
    printf("$$ Declaring variable 'Z' in the current scope\n");
    if (symTableDeclareVariable(table, "Z", dTypeU8, true)) {
        printf("$$ Declared variable 'Z' in the current scope (expected)\n");
    } else {
        printf("$$ Failed to declare variable 'Z' in the current scope (unexpected)\n");
    }

    enum ERR_CODES code;

    // exit the new scope
    printf("$$ Exiting the new scope\n");
    if (symTableExitScope(table, &code)) {
        printf("$$ Exited the new scope (expected)\n");
    } else {
        printf("$$ Failed to exit the new scope (unexpected)\n");
    }

    printf("$$ exit code: %d : %s\n", code, code == E_SEMANTIC_UNUSED_VAR ? "(expected)" : "(unexpected)");

    // free the symbol table
    printf("$$ Freeing the symbol table\n");
    if (symTableFree(table)) {
        printf("$$ Symbol table freed (expected)\n");
    } else {
        printf("$$ Failed to free the symbol table (unexpected)\n");
    }
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
