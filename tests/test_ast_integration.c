/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_ast_full.c
 * @date 20.10. 2024
 * @brief integration tests for symtable and creating ASTs
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "semantical/abstract_syntax_tree.h"
#include "semantical/sem_enums.h"
#include "semantical/symtable.h"
#include "utility/enumerations.h"
#include "ast_assets/good_asts.h"


// ####################### TEST CASES #######################

void create_test_env(SymTable **table, LinkedList **returnAsts, TestInstancePtr *test, char *testName) {

    // create the symbol table
    *table = symTableInit();

    // create the linked list for the return ASTs
    *returnAsts = initLinkedList(false);

    // create the test instance
    *test = initTestInstance(testName);

}

void free_test_env(SymTable **table, LinkedList **returnAsts, TestInstancePtr *test) {

    // free the symbol table
    symTableFree(table);
    
    // free the linked list
    for (unsigned int i = 0; i < getSize(*returnAsts); i++) {
        ASTNodePtr node = (ASTNodePtr)getDataAtIndex(*returnAsts, i);
        ASTfreeNode(&node);
    }
    removeList(returnAsts);

    // finish the test instance
    finishTestInstance(*test);

}

void run_tests(void) {

    SymTable *table;
    LinkedList *returnAsts;
    TestInstancePtr test;

    // ####################### TEST CASE 1 #######################
     /*
    AST 1

    pub fn bar(param : []u8) []u8 {
        const r = foo(param);
        return r;
    }
    */
    create_test_env(&table, &returnAsts, &test, "AST 1");
    return_asts_1(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 2

    pub fn foo(par : []u8) []u8 {
        const ret = bar(par);
        return ret;
    }
    */
    create_test_env(&table, &returnAsts, &test, "AST 2");
    return_asts_2(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 3

    pub fn main() void {
        const par = ifj.string("ahoj");
        _ = bar(par);
    }
    */

    create_test_env(&table, &returnAsts, &test, "AST 3");
    return_asts_3(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    // continue later with other tests
}

int main(void) {
    run_tests();
    return 0;
}
