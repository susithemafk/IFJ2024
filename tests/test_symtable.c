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
#include "semantical/sem_enums.h"
#include "utility/enumerations.h"

void test_variables(void) {

    TestInstancePtr testInstance = initTestInstance("Symbol Table Tests");

    // 1. Initialize a new symbol table
    SymTable *table = symTableInit();
    testCase(
        testInstance,
        table != NULL,
        "Initializing a new symbol table",
        "Successfully initialized a new symbol table (expected)",
        "Failed to initialize a new symbol table (unexpected)"
    );

    // 2. Test that declaring variables in the global scope fails
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, NULL) == NULL,
        "Attempting to declare variable 'X' in the global scope (should fail)",
        "Correctly failed to declare 'X' in the global scope (expected)",
        "Unexpectedly succeeded in declaring 'X' in the global scope (unexpected)"
    );

    // 3. Add a new scope (e.g., function)
    testCase(
        testInstance,
        symTableMoveScopeDown(table, SYM_FUNCTION),
        "Adding a new function scope",
        "Successfully added a new function scope (expected)",
        "Failed to add function scope (unexpected)"
    );

    // 4. Declare variable 'X' inside the function scope
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, NULL) != NULL,
        "Declaring variable 'X' in the function scope",
        "Successfully declared 'X' in the function scope (expected)",
        "Failed to declare 'X' in the function scope (unexpected)"
    );

    // 5. Move down to a new scope (e.g., inside a block)
    testCase(
        testInstance,
        symTableMoveScopeDown(table, SYM_IF),
        "Adding a new block scope inside the function",
        "Successfully added a block scope (expected)",
        "Failed to add block scope (unexpected)"
    );

    // 6. Redeclare variable 'X' in the block scope (should fail)
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, NULL) == NULL,
        "Attempting to redeclare variable 'X' in the block scope (should fail)",
        "Correctly failed to redeclare 'X' in the block scope (expected)",
        "Unexpectedly succeeded in redeclaring 'X' in the block scope (unexpected)"
    );

    // 7. Declare a new variable 'Y' in the block scope
    testCase(
        testInstance,
        symTableDeclareVariable(table, "Y", dTypeF64, true, false, NULL) != NULL,
        "Declaring variable 'Y' in the block scope",
        "Successfully declared 'Y' in the block scope (expected)",
        "Failed to declare 'Y' in the block scope (unexpected)"
    );

    // 8. Test finding 'Y' in the current scope
    SymVariable *varY = NULL;
    testCase(
        testInstance,
        symTableFindVariable(table, "Y", &varY),
        "Searching for variable 'Y' in the block scope",
        "Found 'Y' in the block scope (expected)",
        "Failed to find 'Y' in the block scope (unexpected)"
    );

    // 9. Test finding 'X' from the function scope
    SymVariable *varX = NULL;
    testCase(
        testInstance,
        symTableFindVariable(table, "X", &varX),
        "Searching for variable 'X' from the function scope",
        "Found 'X' from the function scope (expected)",
        "Failed to find 'X' from the function scope (unexpected)"
    );

    // 10. Check if 'X' is mutable
    testCase(
        testInstance,
        symTableCanMutate(varX),
        "Testing if variable 'X' is mutable",
        "Variable 'X' is mutable (expected)",
        "Variable 'X' is immutable (unexpected)"
    );

    // 11. Exit block scope and test variable access
    enum ERR_CODES code;
    testCase(
        testInstance,
        symTableExitScope(table, &code),
        "Exiting block scope (we have searched for all the variables)",
        "Exited block scope (expected)",
        "Failed to exit block scope (unexpected)"
    );

    // 12. Check for unused variables
    testCase(
        testInstance,
        code == SUCCESS,
        "Checking for unused variables in exited block scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // 13. Exit function scope and test variable access
    testCase(
        testInstance,
        symTableExitScope(table, &code),
        "Exiting function scope",
        "Exited function scope (expected)",
        "Failed to exit function scope (unexpected)"
    );

    // 14. Check for unused variables in function scope
    testCase(
        testInstance,
        code == SUCCESS,
        "Checking for unused variables in exited function scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // 15. Exit global scope and test variable access
    testCase(
        testInstance,
        symTableExitScope(table, &code),
        "Exiting global scope",
        "Exited global scope (expected)",
        "Failed to exit global scope (unexpected)"
    );

    // 16. Check for unused variables in global scope
    testCase(
        testInstance,
        code == SUCCESS,
        "Checking for unused variables in exited global scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // Free the symbol table
    printf("-- Freeing the symbol table (when exiting the global scope)\n");

    finishTestInstance(testInstance);

}

int main(void) {
    test_variables();
    return 0;
}
