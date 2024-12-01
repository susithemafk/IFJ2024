/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symtable.c
 * @date 29.9. 2024
 * @brief Test file for the linked list implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantical/symtable.h"
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

    // 2. Test finding the value fohrow away variable in global scope
    SymVariable *var = symTableFindVariable(table, "_");

    testCase(
        testInstance,
        var != NULL,
        "Searching for the throw away variable '_' in the global scope",
        "Found the throw away variable '_' in the global scope (expected)",
        "Failed to find the throw away variable '_' in the global scope (unexpected)"
    );

    // 3. Test if the throw away variable is mutable
    testCase(
        testInstance,
        !symTableCanMutate(var),
        "Testing if the throw away variable '_' is mutable",
        "The throw away variable '_' is immutable (expected)",
        "The throw away variable '_' is mutable (unexpected)"
    );

    // 4. Test the name of the throw away variable
    testCase(
        testInstance,
        strcmp(var->name, "_") == 0,
        "Checking the name of the throw away variable",
        "The name of the throw away variable is '_' (expected)",
        "The name of the throw away variable is not '_' (unexpected)"
    );

    // 5. Test that declaring variables in the global scope fails
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, false) == NULL,
        "Attempting to declare variable 'X' in the global scope (should fail)",
        "Correctly failed to declare 'X' in the global scope (expected)",
        "Unexpectedly succeeded in declaring 'X' in the global scope (unexpected)"
    );

    // 6. Add a new scope (e.g., function)
    testCase(
        testInstance,
        symTableMoveScopeDown(table, SYM_FUNCTION),
        "Adding a new function scope",
        "Successfully added a new function scope (expected)",
        "Failed to add function scope (unexpected)"
    );

    // 7. Declare variable 'X' inside the function scope
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, false) != NULL,
        "Declaring variable 'X' in the function scope",
        "Successfully declared 'X' in the function scope (expected)",
        "Failed to declare 'X' in the function scope (unexpected)"
    );

    // 8. Move down to a new scope (e.g., inside a block)
    testCase(
        testInstance,
        symTableMoveScopeDown(table, SYM_IF),
        "Adding a new block scope inside the function",
        "Successfully added a block scope (expected)",
        "Failed to add block scope (unexpected)"
    );

    // 9. Redeclare variable 'X' in the block scope (should fail)
    testCase(
        testInstance,
        symTableDeclareVariable(table, "X", dTypeI32, true, false, false) == NULL,
        "Attempting to redeclare variable 'X' in the block scope (should fail)",
        "Correctly failed to redeclare 'X' in the block scope (expected)",
        "Unexpectedly succeeded in redeclaring 'X' in the block scope (unexpected)"
    );

    // 10. Declare a new variable 'Y' in the block scope
    testCase(
        testInstance,
        symTableDeclareVariable(table, "Y", dTypeF64, true, false, false) != NULL,
        "Declaring variable 'Y' in the block scope",
        "Successfully declared 'Y' in the block scope (expected)",
        "Failed to declare 'Y' in the block scope (unexpected)"
    );

    // 11. Test finding 'Y' in the current scope
    SymVariable *varY = symTableFindVariable(table, "Y");
    testCase(
        testInstance,
        varY != NULL,
        "Searching for variable 'Y' in the block scope",
        "Found 'Y' in the block scope (expected)",
        "Failed to find 'Y' in the block scope (unexpected)"
    );

    varY->modified = true; // similating modification

    // 12. Test finding 'X' from the function scope
    SymVariable *varX = symTableFindVariable(table, "X");
    testCase(
        testInstance,
        varX != NULL,
        "Searching for variable 'X' from the function scope",
        "Found 'X' from the function scope (expected)",
        "Failed to find 'X' from the function scope (unexpected)"
    );

    varX->modified = true; // similating modification

    // 13. Check if 'X' is mutable
    testCase(
        testInstance,
        symTableCanMutate(varX),
        "Testing if variable 'X' is mutable",
        "Variable 'X' is mutable (expected)",
        "Variable 'X' is immutable (unexpected)"
    );

    // 14. Test finding the value fohrow away variable in if scope
    var = symTableFindVariable(table, "_");

    testCase(
        testInstance,
        var != NULL,
        "Searching for the throw away variable '_' in the global scope",
        "Found the throw away variable '_' in the global scope (expected)",
        "Failed to find the throw away variable '_' in the global scope (unexpected)"
    );

    // 15. Test if the throw away variable is mutable
    testCase(
        testInstance,
        !symTableCanMutate(var),
        "Testing if the throw away variable '_' is mutable",
        "The throw away variable '_' is immutable (expected)",
        "The throw away variable '_' is mutable (unexpected)"
    );

    // 16. Test the name of the throw away variable
    testCase(
        testInstance,
        strcmp(var->name, "_") == 0,
        "Checking the name of the throw away variable",
        "The name of the throw away variable is '_' (expected)",
        "The name of the throw away variable is not '_' (unexpected)"
    );

    // 17. Check for unused variables
    testCase(
        testInstance,
        symTableExitScope(table) == SUCCESS,
        "Checking for unused variables in exited block scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // 18. Check for unused variables in function scope
    testCase(
        testInstance,
        symTableExitScope(table) == SUCCESS,
        "Checking for unused variables in exited function scope",
        "All variables were used (expected)",
        "Not all variables were used (unexpected)"
    );

    // 19. Free the symbol table
    testCase(
        testInstance,
        symTableFree(&table),
        "Freeing the symbol table",
        "Successfully freed the symbol table (expected)",
        "Failed to free the symbol table (unexpected)"
    );

    finishTestInstance(testInstance);

}

int main(void) {
    test_variables();
    return 0;
}
