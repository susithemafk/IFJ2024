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

    // ####################### TEST CASE 1 - valid ASTs #######################
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

    /*
    AST 4

    pub fn build(x : []u8, y : []u8) []u8 {
        const res = ifj. concat(x, y);
        return res;
    }
    */ 
    create_test_env(&table, &returnAsts, &test, "AST 4");
    return_asts_4(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 5

    pub fn main() void {
    const a = ifj.string("ahoj ");
    var ct : []u8= ifj.string("svete");
    ct = build(a, ct);
    ifj.write(ct);
}
    */
    create_test_env(&table, &returnAsts, &test, "AST 5");
    return_asts_5(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 6

    pub fn main() void {
    ifj.write("Zadejte cislo pro vypocet faktorialu\n");
    const a = ifj.readi32();
    if (a) |val| {
        if (val < 0) {
            ifj.write("Faktorial "); 
            ifj.write(val);
            ifj.write(" nelze spocitat\n");
        } else {
            var d: f64= ifj.i2f(val);
            var vysl: f64 = 1.0;
            while (d > 0) {
                vysl = vysl * d;
                d = d - 1.0;
            }
            ifj.write("Vysledek: "); ifj.write(vysl); ifj.write(" = ");
            const vysl_i32 = ifj.f2i(vysl);
            ifj.write(vysl_i32); ifj.write("\n");
        }
    } else { // a == null
        ifj.write("Faktorial pro null nelze spocitat\n");
    }
}
    */
    create_test_env(&table, &returnAsts, &test, "AST 6");
    return_asts_6(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 7

    pub fn main() void {
    ifj.write("Zadejte cislo pro vypocet faktorialu: ");
    const inp = ifj.readi32();
    if (inp) |INP| {
        if (INP < 0) {
            ifj.write("Faktorial nelze spocitat!\n");
        } else {
            const vysl = factorial(INP);
            ifj.write("Vysledek: "); ifj.write(vysl);
        }
    } else {
        ifj.write("Chyba pri nacitani celeho cisla!\n");
    }
}
    */
    create_test_env(&table, &returnAsts, &test, "AST 7");
    return_asts_7(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 8

    pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
    */
    create_test_env(&table, &returnAsts, &test, "AST 8");
    return_asts_8(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 9

    pub fn factorial(n: i32) i32 {
    var result: i32 = 0 - 1;
    if (n < 2) {
        result = 1;
    } else {
        const decremented_n = decrement(n, 1);
        const temp_result = factorial(decremented_n);
        result = n * temp_result;
    }
    return result;
    }
    */
    create_test_env(&table, &returnAsts, &test, "AST 9");
    return_asts_9(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);

    /*
    AST 10

    pub fn main() void {
    const str1 = ifj.string("Toto je text v programu jazyka IFJ24");
    var str2 = ifj.string(", ktery jeste trochu obohatime");
    str2 = ifj.concat(str1, str2);
    ifj. write(str1); 
    ifj . write("\n");
    ifj .write(str2); ifj.write("\n");
    ifj.write("Zadejte serazenou posloupnost malych pismen a-h:\n");
    var newInput = ifj.readstr();
    var all: []u8= ifj.string("");
    while (newInput) |inpOK| {
        const abcdefgh = ifj.string("abcdefgh");
        const strcmpResult = ifj.strcmp(inpOK, abcdefgh);
        if (strcmpResult == 0) {
            ifj.write("Spravne zadano!\n");
            ifj.write(all); // vypsat spojene nepodarene vstupy
            newInput = null; // misto break;
        } else {
            ifj.write("Spatne zadana posloupnost, zkuste znovu:\n");
            all = ifj.concat(all, inpOK); // spojuji neplatne vstupy
            newInput = ifj.readstr();
        }
    }
    }
    }
    */
    create_test_env(&table, &returnAsts, &test, "AST 10");
    return_asts_10(table, returnAsts, test);
    free_test_env(&table, &returnAsts, &test);
}

int main(void) {
    run_tests();
    return 0;
}
