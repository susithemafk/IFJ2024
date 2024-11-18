/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file sem_validator.h
 * @date 25.10.2024
 * @brief Header file for the semantic validator
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
#include "semantical/sem_validator.h"

// ####################### Semantic validation of ASTs #######################

int main(void) {

    SymTable *table = symTableInit();
    LinkedList *returnAsts = initLinkedList(false);
    TestInstancePtr test = initTestInstance("AST semantic validation test 1");
    enum ERR_CODES err;
    ASTNodePtr ast;
    SymFunctionPtr fnDef;
    /*
    AST 1

    pub fn bar(param : []u8) []u8 {
        const r = foo(param);
        return r;
    }
    */
    // first pass, add the function definitions
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "bar", dTypeU8, 0);
    symAddParamToFunc(fnDef, dTypeU8, false);
    symTableAddFunction(table, fnDef);

    // generate the ASTs for the 1st function
    return_asts_1(table, returnAsts, NULL);

    /*
    AST 2

    pub fn foo(par : []u8) []u8 {
        const ret = bar(par);
        return ret;
    }
    */
    // first add the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeU8, 0);
    symAddParamToFunc(fnDef, dTypeU8, false);
    symTableAddFunction(table, fnDef);

    // generate the ASTs for the 2nd function
    return_asts_2(table, returnAsts, NULL);

    /*
    AST 3

    pub fn main() void {
        const par = ifj.string("ahoj");
        _ = bar(par);
    }
    */
    // first add the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "main", dTypeVoid, 0);
    symTableAddFunction(table, fnDef);

    // gnerate the ASTs for the 3rd function
    return_asts_3(table, returnAsts, NULL);

    // dont need to add the main function, since it will never be called, just by the tests

    // now go through the ASTs and validate them
    ASTNodePtr currentFunc;
    unsigned int size = getSize(returnAsts);
    for (unsigned int i = 0; i < size; i++) {
        printf("test: %u/%u\n", i, size);
        ast = (ASTNodePtr)getDataAtIndex(returnAsts, i);
        err = validateAST(ast, table, &currentFunc);
        testCase(
            test,
            err == SUCCESS,
            "AST validation",
            "AST is valid",
            "AST is invalid"
        );
        if (err != SUCCESS) printErrCode(err);
    }

    for (unsigned int i = 0; i < size; i++) {
        ast = (ASTNodePtr)getDataAtIndex(returnAsts, i);
        ASTfreeNode(&ast);
    }

    // free the function definitions
    symTableFree(&table);
    finishTestInstance(test);
    removeList(&returnAsts);
    return 0;
}
