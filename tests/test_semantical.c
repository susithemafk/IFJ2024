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

    #ifdef DEBUG
    DEBUG_MSG("Starting the AST semantic validation test 1");
    #endif

    SymTable *table = symTableInit();
    LinkedList *returnAsts = initLinkedList(false);
    fnDefinitionsPtr fnDefs = initFunctionDefinitions();
    TestInstancePtr test = initTestInstance("AST semantic validation test 1");
    enum ERR_CODES err;
    bool result;
    ASTNodePtr ast;
    /*
    AST 1

    pub fn bar(param : []u8) []u8 {
        const r = foo(param);
        return r;
    }
    */
    // generate the ASTs for the 1st function
    return_asts_1(table, returnAsts, NULL);

    /*
    AST 2

    pub fn foo(par : []u8) []u8 {
        const ret = bar(par);
        return ret;
    }
    */
    // generate the ASTs for the 2nd function
    return_asts_2(table, returnAsts, NULL);

    /*
    AST 3

    pub fn main() void {
        const par = ifj.string("ahoj");
        _ = bar(par);
    }
    */
    // gnerate the ASTs for the 3rd function
    return_asts_3(table, returnAsts, NULL);

    // save the function definitions
    for (unsigned int i = 0; i < getSize(returnAsts); i++) {
        ast = (ASTNodePtr)getDataAtIndex(returnAsts, i);
        if (ast->type == AST_NODE_FUNCTION) {
            err = addFunctionDefinition(fnDefs, ast);
            testCase(
                test,
                err == SUCCESS,
                "Function definition add",
                "Function definition added",
                "Function definition failed to add"
            );
        }
    }

    // dont need to add the main function, since it will never be called, just by the tests

    // now go through the ASTs and validate them
    ASTNodePtr currentFunc;
    for (unsigned int i = 0; i < getSize(returnAsts); i++) {
        ast = (ASTNodePtr)getDataAtIndex(returnAsts, i);
        err = validateAST(ast, fnDefs, &currentFunc);
        testCase(
            test,
            err == SUCCESS,
            "AST validation",
            "AST is valid",
            "AST is invalid"
        );
        if (err != SUCCESS) printErrCode(err);
        if (ast->type == AST_NODE_FUNCTION) continue;
        result = ASTfreeNode(&ast);  
        testCase(
            test,
            result,
            "AST free",
            "AST freed",
            "AST failed to free"
        );
    }

    // free the function definitions
    result = freeFunctionDefinitions(&fnDefs);
    testCase(
        test,
        result,
        "Function definitions free",
        "Function definitions freed",
        "Function definitions failed to free"
    );

    symTableFree(&table);
    finishTestInstance(test);
    removeList(&returnAsts);
    return 0;
}
