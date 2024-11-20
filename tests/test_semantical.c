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

void test_good_asts(void) {

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
}

void test_bad_asts(void) {

    /*
    what we need to test, and what is allready tested

    what is tested:
        - Usage of undeclared variables (SymTable)
        - Usage of undeclared functions (SymTable)
        - Assigning to a constant (SymTable)

    what we need to test more (mostly differemtn types )
        - Assigning a bad type to a variable
        - expresion with not compatable types 
        - calling a function with the wrong number of arguments, 
        - calling a function with the wrong type of arguments
        - returning a bad type from a function
    */

    SymTable *table;
    TestInstancePtr test;

    test = initTestInstance("AST semantic validation test 2");

    // ======================================================================================

    // testting bad assign type
    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function
    // var a: i32;
    ASTNodePtr assign = ASTcreateNode(AST_NODE_DECLARE);

    SymVariable *var = symTableDeclareVariable(table, "a", dTypeI32, true, false);
    ASTeditDeclareNode(assign, var, NULL);
    // adding a wrong type to the variable
    struct TOKEN token = {.type=TOKEN_STRING, .value="\"ahoj\""};
    ASTNodePtr value = ASTcreateNode(AST_NODE_VALUE);
    ASTinitNodeValue(value, &token);
    ASTeditDeclareNode(assign, NULL, value);

    enum ERR_CODES err = validateAST(assign, table, NULL);
    testCase(
        test,
        err == E_SEMANTIC_INCOMPATABLE_TYPES,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&assign);
    symTableFree(&table);

    // ======================================================================================
    // assigning a bad literal value to variable declaration
    // var a f64 = 2;  -- shoudl be fine

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function

    // var a: f64;
    var = symTableDeclareVariable(table, "a", dTypeF64, true, false);
    assign = ASTcreateNode(AST_NODE_DECLARE);
    ASTeditDeclareNode(assign, var, NULL);

    // adding a wrong type to the variable
    token.type = TOKEN_I32;
    token.value = "2";

    value = ASTcreateNode(AST_NODE_VALUE);
    ASTinitNodeValue(value, &token);
    ASTeditDeclareNode(assign, NULL, value);

    err = validateAST(assign, table, NULL);
    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    testCase(
        test,
        assign->data->declare->variable->type == dTypeF64,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    testCase(
        test,
        strcmp(assign->data->declare->value->data->value->value, "2.0") == 0,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&assign);
    symTableFree(&table);

    // ======================================================================================
    // testing bad expresion type
    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function


    // var a : i32 = 2;
    // var b : f64 = 2.0;
    // a + b; -- should be an error
    var = symTableDeclareVariable(table, "a", dTypeI32, true, false);
    var = symTableDeclareVariable(table, "b", dTypeF64, true, false);

    ASTNodePtr exp = ASTcreateNode(AST_NODE_EXPRESION);
    var = symTableFindVariable(table, "a");
    ASTaddNodeToExpresion(exp, var, NULL, NULL);
    token.type = TOKEN_PLUS;
    token.value = "+";
    ASTaddNodeToExpresion(exp, NULL, NULL, &token);
    var = symTableFindVariable(table, "b");
    ASTaddNodeToExpresion(exp, var, NULL, NULL);

    // need to add the the expresion to a declaraion, so we can validate it
    // var c : i32 = a + b; -- should be an error
    var = symTableDeclareVariable(table, "c", dTypeI32, true, false);
    ASTNodePtr exp1 = ASTcreateNode(AST_NODE_DECLARE);
    ASTeditDeclareNode(exp1, var, exp);

    err = validateAST(exp1, table, NULL);
    testCase(
        test,
        err == E_SEMANTIC_INCOMPATABLE_TYPES,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&exp1);
    symTableFree(&table);

    // ======================================================================================
    // var b : f64 = 3.2
    // testing a special case, where: var c: f64 = b + 2; this should be coverted and should all work, 2 should be converted to 2.0

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function
    var = symTableDeclareVariable(table, "c", dTypeF64, true, false);

    ASTNodePtr dec = ASTcreateNode(AST_NODE_DECLARE);
    ASTeditDeclareNode(dec, var, NULL);
    
    var = symTableDeclareVariable(table, "b", dTypeF64, true, false);

    // create the expresion
    exp = ASTcreateNode(AST_NODE_EXPRESION);
    var = symTableFindVariable(table, "b");
    ASTaddNodeToExpresion(exp, var, NULL, NULL);
    token.type = TOKEN_PLUS;
    token.value = "+";
    ASTaddNodeToExpresion(exp, NULL, NULL, &token);
    token.type = TOKEN_I32;
    token.value = "2";
    ASTaddNodeToExpresion(exp, NULL, &token, NULL);

    ASTeditDeclareNode(dec, NULL, exp);

    err = validateAST(dec, table, NULL);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    LinkedList *expresion_output = dec->data->declare->value->data->expresion->output;
    ASTNodePtr valueNode = (ASTNodePtr)getDataAtIndex(expresion_output, 1);

    testCase(
        test,
        valueNode->data->value->type == dTypeF64,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    testCase(
        test,
        strcmp(valueNode->data->value->value, "2.0") == 0,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );


    ASTfreeNode(&dec);
    symTableFree(&table);

    // ======================================================================================
    // trying to have a nullable value in an expression 
    // var a ?i32 = 2;
    // var b : i32 = a + 2; -- should be an error

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function

    var = symTableDeclareVariable(table, "b", dTypeI32, true, false);
    ASTNodePtr decl = ASTcreateNode(AST_NODE_DECLARE);
    ASTeditDeclareNode(decl, var, NULL);

    var = symTableDeclareVariable(table, "a", dTypeI32, true, true);
    exp = ASTcreateNode(AST_NODE_EXPRESION);
    ASTaddNodeToExpresion(exp, var, NULL, NULL);
    token.type = TOKEN_PLUS;
    token.value = "+";
    ASTaddNodeToExpresion(exp, NULL, NULL, &token);
    token.type = TOKEN_I32;
    token.value = "2";
    ASTaddNodeToExpresion(exp, NULL, &token, NULL);

    ASTeditDeclareNode(decl, NULL, exp);

    err = validateAST(decl, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INCOMPATABLE_TYPES,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&decl);
    symTableFree(&table);

    // ======================================================================================
    // testing a bad function call
    // pub fn foo() i32  -- in the symtable

    table = symTableInit();

    // adding the function definition
    SymFunctionPtr fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeI32, 0);
    symTableAddFunction(table, fnDef);

    // calling the function by itselves
    ASTNodePtr funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL); // shoud be en error, since the funciton has a return type

    err = validateAST(funcCall, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INCOMPATABLE_TYPES,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&funcCall);

    // calling the funcition with a wrong number of arguments (any in this case)

    // _ = foo(2); -- should be an error

    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    token.type = TOKEN_I32;
    token.value = "2";
    ASTeditFunctionCallNode(funcCall, NULL, NULL, &token);

    assign = ASTcreateNode(AST_NODE_ASSIGN);
    var = symTableFindVariable(table, "_");
    ASTeditAssignNode(assign, var, funcCall);

    err = validateAST(assign, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INVALID_FUN_PARAM,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&assign);   
    symTableFree(&table);

    // ======================================================================================
    // testing a bad function call function -> pub fn foo(a: i32) i32

    table = symTableInit();

    // adding the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeI32, 0);
    symAddParamToFunc(fnDef, dTypeI32, false);
    symTableAddFunction(table, fnDef);

    // calling with no args _ = foo(); -- should be an error
    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);

    assign = ASTcreateNode(AST_NODE_ASSIGN);
    var = symTableFindVariable(table, "_");
    ASTeditAssignNode(assign, var, funcCall);

    err = validateAST(assign, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INVALID_FUN_PARAM,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&assign);

    // calling with a wrong type of argument _ = foo("ahoj"); -- should be an error
    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    token.type = TOKEN_STRING;
    token.value = "ahoj";
    ASTeditFunctionCallNode(funcCall, NULL, NULL, &token);

    assign = ASTcreateNode(AST_NODE_ASSIGN);
    var = symTableFindVariable(table, "_");
    ASTeditAssignNode(assign, var, funcCall);

    err = validateAST(assign, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INVALID_FUN_PARAM,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&assign);

    // var a : f64 
    // calling with a bad type of variable _ = foo(a); -- should be an error

    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    var = symTableDeclareVariable(table, "a", dTypeF64, true, false);
    ASTeditFunctionCallNode(funcCall, NULL, var, NULL);

    assign = ASTcreateNode(AST_NODE_ASSIGN);
    var = symTableFindVariable(table, "_");
    ASTeditAssignNode(assign, var, funcCall);

    err = validateAST(assign, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INVALID_FUN_PARAM,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&assign);
    symTableFree(&table);

    // ======================================================================================
    // testing a bad function param, in nullability pub fn foo(a: ?i32) void

    // here we should accept a func call, with a nullable value, and a non nullable value

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function

    // adding the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeVoid, 0);
    symAddParamToFunc(fnDef, dTypeI32, true);
    symTableAddFunction(table, fnDef);

    // calling with a nullable argument var a?i32, foo(a) -- should be valid
    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    var = symTableDeclareVariable(table, "a", dTypeI32, true, true);
    ASTeditFunctionCallNode(funcCall, NULL, var, NULL);

    err = validateAST(funcCall, table, NULL);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&funcCall);

    // calling with a non nullable value, shoudl still be valid
    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    token.type = TOKEN_I32;
    token.value = "2";
    ASTeditFunctionCallNode(funcCall, NULL, NULL, &token);

    err = validateAST(funcCall, table, NULL);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&funcCall);

    // calling with a non nullable variable, should still be valid
    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    var = symTableDeclareVariable(table, "b", dTypeI32, true, false);
    ASTeditFunctionCallNode(funcCall, NULL, var, NULL);

    err = validateAST(funcCall, table, NULL);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&funcCall);
    symTableFree(&table);


    // ======================================================================================
    // calling a functoin with a not nullable arg with a nullable value
    // pub fn foo(a: i32) void

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function

    // adding the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeVoid, 0);
    symAddParamToFunc(fnDef, dTypeI32, false);

    symTableAddFunction(table, fnDef);

    // calling with a nullable argument var a?i32, foo(a) -- should be invalid

    funcCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTeditFunctionCallNode(funcCall, "foo", NULL, NULL);
    var = symTableDeclareVariable(table, "a", dTypeI32, true, true);
    ASTeditFunctionCallNode(funcCall, NULL, var, NULL);

    err = validateAST(funcCall, table, NULL);

    testCase(
        test,
        err == E_SEMANTIC_INVALID_FUN_PARAM,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&funcCall);
    symTableFree(&table);

    // ======================================================================================
    // testing a bad return type
    // function foo() i32

    // returning a string (fail), returning a i32 (pass), 
    // returning a f64 (fail)
    // returnning a void (fail) 
    // returning a nullable i32 (fail) 
    // returning a connstant i32 (pass) 
    // returning a literal i32 (pass)
    // returning a literal f64 3.0 (pass)
    // returning a literal f64 3.14 (fail)
    // returning a expression a : i32 + b : i32 (pass)
    // returning an expression a : i32 + b : f64 (fail)
    // returning an expression a : i32 + b : ?i32 (fail)
    // returning an expression a : f64 + b : f64 (fail)

    table = symTableInit();
    symTableMoveScopeDown(table, SYM_FUNCTION); // we are in a function

    // add the function definition
    fnDef = symInitFuncDefinition();
    symEditFuncDef(fnDef, "foo", dTypeI32, 0);
    symTableAddFunction(table, fnDef);

    // create the AST for the function definition
    ASTNodePtr func = ASTcreateNode(AST_NODE_FUNCTION);
    ASTeditFunctionNode(func, "foo", dTypeI32, 0, NULL);

    // returning a string (fail)
    ASTNodePtr ret = ASTcreateNode(AST_NODE_RETURN);
    token.type = TOKEN_STRING;
    token.value = "\"ahoj\"";
    ASTeditReturnNode(ret, NULL, &token, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == E_SEMANTIC_BAD_FUNC_RETURN,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&ret);

    // trying to return a variable of type u8 (fail)
    ret = ASTcreateNode(AST_NODE_RETURN);
    var = symTableDeclareVariable(table, "a", dTypeU8, true, false);
    ASTeditReturnNode(ret, var, NULL, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == E_SEMANTIC_BAD_FUNC_RETURN,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&ret);

    // trying to return a f64 (fail)
    ret = ASTcreateNode(AST_NODE_RETURN);
    token.type = TOKEN_F64;
    token.value = "3.14";
    ASTeditReturnNode(ret, NULL, &token, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == E_SEMANTIC_BAD_FUNC_RETURN,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&ret);

    // trying to return a f64 3.0 (pass)
    ret = ASTcreateNode(AST_NODE_RETURN);
    token.type = TOKEN_F64;
    token.value = "3.0";

    ASTeditReturnNode(ret, NULL, &token, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&ret);

    // trying to return a void (fail)
    ret = ASTcreateNode(AST_NODE_RETURN);
    token.type = TOKEN_NULL;
    token.value = "null";

    ASTeditReturnNode(ret, NULL, &token, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == E_SEMANTIC_BAD_FUNC_RETURN,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&ret);

    // trying to return a nullable i32 (fail)
    ret = ASTcreateNode(AST_NODE_RETURN);
    var = symTableDeclareVariable(table, "a", dTypeI32, true, true);
    ASTeditReturnNode(ret, var, NULL, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == E_SEMANTIC_BAD_FUNC_RETURN,
        "AST semantic validation",
        "AST is invalid (expected)",
        "AST is valid (unexpected)"
    );

    ASTfreeNode(&ret);

    // trying to return a constant i32 (pass)
    ret = ASTcreateNode(AST_NODE_RETURN);
    token.type = TOKEN_I32;
    token.value = "2";
    ASTeditReturnNode(ret, NULL, &token, NULL);
    
    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&ret);

    // trying to return a1 : i32 + 3.0 (pass)
    ret = ASTcreateNode(AST_NODE_RETURN);
    var = symTableDeclareVariable(table, "a1", dTypeI32, true, false);
    ASTeditReturnNode(ret, var, NULL, NULL);
    token.type = TOKEN_PLUS;
    token.value = "+";
    ASTeditReturnNode(ret, NULL, NULL, &token);
    token.type = TOKEN_F64;
    token.value = "3.0";
    ASTeditReturnNode(ret, NULL, &token, NULL);

    err = validateAST(ret, table, &func);

    testCase(
        test,
        err == SUCCESS,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    // checking if we converted the stuff
    LinkedList *output = ret->data->returnNode->expression->data->expresion->output;

    ASTNodePtr valueNode1 = (ASTNodePtr)getDataAtIndex(output, 1);
    testCase(
        test,
        valueNode1->data->value->type == dTypeI32,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    testCase(
        test,
        strcmp(valueNode1->data->value->value, "3") == 0,
        "AST semantic validation",
        "AST is valid",
        "AST is invalid"
    );

    ASTfreeNode(&ret);
    ASTfreeNode(&func);

    symTableFree(&table);
    finishTestInstance(test);

    // ======================================================================================
}

int main(void) {
    //test_good_asts();
    test_bad_asts();
    return 0;
}
