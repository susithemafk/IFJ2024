/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_functin_call_validation.c
 * @date 28.9. 2024
 * @brief test file for the function call validation
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>    
#include "semantical/function_call_validation.h"
#include "utility/enumerations.h"


void test_case1(void) {

    TestInstancePtr testInstance = initTestInstance("Function Call Validation Test");

    // Initialize the function call validator
    FunctionCallValidatorPtr validator = initFunctionCallValidator();

    // test if the validator was initialized
    testCase(
        testInstance,
        validator != NULL,
        "Initializing the function call validator",
        "Function call validator initialized successfully (expected)",
        "Failed to initialize function call validator (unexpected)"
    );

    // Add a called function
    testCase(
        testInstance,
        addCalledFunction(validator, "func1"),
        "Adding a called function 'func1'",
        "Successfully added called function 'func1' (expected)",
        "Failed to add called function 'func1' (unexpected)"
    );

    // Add an allready called function
    testCase(
        testInstance,
        addCalledFunction(validator, "func1"),
        "Adding an already called function 'func1'",
        "Successfully added already called function 'func1' (expected)",
        "Failed to add already called function 'func1' (unexpected)"
    );

    // Add a new called function 
    testCase(
        testInstance,
        addCalledFunction(validator, "func3"),
        "Adding a called function 'func2'",
        "Successfully added called function 'func2' (expected)",
        "Failed to add called function 'func2' (unexpected)"
    );

    // Add a defined function
    testCase(
        testInstance,
        addDefinedFunction(validator, "func2"),
        "Adding a defined function 'func2'",
        "Successfully added defined function 'func2' (expected)",
        "Failed to add defined function 'func2' (unexpected)"
    );

    // Add an allready defined function
    testCase(
        testInstance,
        addDefinedFunction(validator, "func2"),
        "Adding an already defined function 'func2'",
        "Successfully added already defined function 'func2' (expected)",
        "Failed to add already defined function 'func2' (unexpected)"
    );

    // Validate all function calls (shoud fail this time)
    testCase(
        testInstance,
        !validateALLFunctionCalls(validator),
        "Validating all function calls",
        "Successfully found, that not all calls are valid (expected)",
        "Validated All function calls (unexpected)"
    );    

    // Free the function call validator
    freeFunctionCallValidator(validator);

    // Create a new validator
    validator = initFunctionCallValidator();

    // check if the validator was initialized
    testCase(
        testInstance,
        validator != NULL,
        "Reinitializing the function call validator",
        "Function call validator reinitialized successfully (expected)",
        "Failed to reinitialize function call validator (unexpected)"
    );

    // Add a called function
    testCase(
        testInstance,
        addCalledFunction(validator, "func1"),
        "Adding a called function 'func1'",
        "Successfully added called function 'func1' (expected)",
        "Failed to add called function 'func1' (unexpected)"
    );

    // Add a defined function
    testCase(
        testInstance,
        addDefinedFunction(validator, "func1"),
        "Adding a defined function 'func1'",
        "Successfully added defined function 'func1' (expected)",
        "Failed to add defined function 'func1' (unexpected)"
    );

    // Validate all function calls (shoud pass this time)
    testCase(
        testInstance,
        validateALLFunctionCalls(validator),
        "Validating all function calls",
        "Successfully validated all function calls (expected)",
        "Failed to validate all function calls (unexpected)"
    );

    // Free the function call validator
    freeFunctionCallValidator(validator);

    // Finish the test instance
    finishTestInstance(testInstance);
}

int main(void) {
    test_case1();
    return 0;
}   
