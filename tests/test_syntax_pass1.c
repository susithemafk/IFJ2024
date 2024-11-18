/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  <xsvobov00> Veronika Svobodová
 * @file parser.c
 * @date 17.11. 2024
 * @brief main src file for the syntaxical parser
 */

#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "syntaxical/parser_pass1.h"

#define MAX_TOKEN_LEN 100

void compareToknes(FILE *expectedFile, LinkedList *buffer) {

    TestInstancePtr test = initTestInstance("First pass toknes saving test");

    unsigned int size = getSize(buffer);
    TOKEN_PTR token;
    char expected_token[MAX_TOKEN_LEN];

    for (unsigned int i = 0; i < size; i++) {

        token = (TOKEN_PTR)getDataAtIndex(buffer, i);
        if (fgets(expected_token, MAX_TOKEN_LEN, expectedFile) != NULL) {
            size_t len = strlen(expected_token);
            if (len > 0 && expected_token[len - 1] == '\n') {
                expected_token[len - 1] = '\0';
            }
        }

        testCase(
            test,
            strcmp(token->value, expected_token) == 0,
            "First pass tokens saving",
            "Token saved correctly",
            "Token not saved correctly"
        );
        printf("expected: %s\n", expected_token);
        printf("actual: %s\n", token->value);
    }

    finishTestInstance(test);
    freeBuffer(&buffer);
}

int main(void) {

    SymTable *table = symTableInit();
    LinkedList *buffer = initLinkedList(false);

    enum ERR_CODES err = firstPass(table, stdin, buffer);
    FILE *expected_file = fopen("./tests/lexical/expected_output.txt", "r");

    printf("first pass done\n");
    printf("buffer size: %d\n", getSize(buffer));

    compareToknes(expected_file, buffer);

    TestInstancePtr test = initTestInstance("First pass testing");

    testCase(
        test,
        err == SUCCESS,
        "First pass",
        "First pass completed successfully",
        "First pass failed"
    );  
    if (err != SUCCESS) {
        printErrCode(err);
    }

    finishTestInstance(test);

    fclose(expected_file);

    test = initTestInstance("First pass testing");

    // try to find the function definitions
    SymFunctionPtr func = symTableFindFunction(table, "main");

    testCase(
        test,
        func != NULL,
        "Find main function",
        "Main function found",
        "Main function not found"
    );

    testCase(
        test,
        func->returnType == dTypeVoid,
        "Main function return type",
        "Main function return type is void",
        "Main function return type is not void"
    );

    testCase(
        test,
        func->paramaters == NULL,
        "Main function params",
        "Main function has no params",
        "Main function has params"
    );

    // try to find the build function
    func = symTableFindFunction(table, "build");

    testCase(
        test,
        func != NULL,
        "Find build function",
        "Build function found",
        "Build function not found"
    );

    testCase(
        test,
        func->returnType == dTypeU8,
        "Build function return type",
        "Build function return type is i32",
        "Build function return type is not i32"
    );

    testCase(
        test,
        func->nullableReturn == false,
        "Build function nullable return",
        "Build function return is nullable",
        "Build function return is not nullable"
    );

    testCase(
        test,
        func->paramaters != NULL,
        "Build function params",
        "Build function has params",
        "Build function has no params"
    );

    testCase(
        test,
        getSize(func->paramaters) == 2,
        "Build function params count",
        "Build function has 2 params",
        "Build function has not 2 params"
    );

    LinkedList *args = func->paramaters;
    SymFunctionParamPtr var;
    
    var = (SymFunctionParamPtr)getDataAtIndex(args, 0);

    testCase(
        test,
        var != NULL,
        "Build function param 1",
        "Build function param 1 found",
        "Build function param 1 not found"
    );

    testCase(
        test,
        var->type == dTypeU8,
        "Build function param 1",
        "Build function param 1 is u8",
        "Build function param 1 is not u8"
    );

    if (var->type != dTypeU8) {
        printf("data type: %d\n", var->type);
    }

    var = (SymFunctionParamPtr)getDataAtIndex(args, 1);

    testCase(
        test,
        var != NULL,
        "Build function param 2",
        "Build function param 2 found",
        "Build function param 2 not found"
    );

    testCase(
        test,
        var->type == dTypeU8,
        "Build function param 2",
        "Build function param 2 is u8",
        "Build function param 2 is not u8"
    );

    if (var->type != dTypeU8) {
        printf("data type: %d\n", var->type);
    }

    func = symTableFindFunction(table, "add");

    testCase(
        test,
        func != NULL,
        "Find add function",
        "Add function found",
        "Add function not found"
    );

    testCase(
        test,
        func->returnType == dTypeI32,
        "Add function return type",
        "Add function return type is i32",
        "Add function return type is not i32"
    );

    testCase(
        test,
        func->nullableReturn == false,
        "Add function nullable return",
        "Add function return is nullable",
        "Add function return is not nullable"
    );

    testCase(
        test,
        func->paramaters != NULL,
        "Add function params",
        "Add function has params",
        "Add function has no params"
    );

    testCase(
        test,
        getSize(func->paramaters) == 2,
        "Add function params count",
        "Add function has 2 params",
        "Add function has not 2 params"
    );

    args = func->paramaters;

    var = (SymFunctionParamPtr)getDataAtIndex(args, 0);

    testCase(
        test,
        var != NULL,
        "Add function param 1",
        "Add function param 1 found",
        "Add function param 1 not found"
    );

    testCase(
        test,
        var->type == dTypeI32,
        "Add function param 1",
        "Add function param 1 is i32",
        "Add function param 1 is not i32"
    );

    var = (SymFunctionParamPtr)getDataAtIndex(args, 1);

    testCase(
        test,
        var != NULL,
        "Add function param 2",
        "Add function param 2 found",
        "Add function param 2 not found"
    );

    finishTestInstance(test);
    symTableFree(&table);
    return 0;
}

