/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_ast.c
 * @date 20.10. 2024
 * @brief Test file for the AST generation
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantical/abstract_syntax_tree.h"
#include "semantical/sem_enums.h"
#include "utility/enumerations.h"
#include "semantical/symtable.h"

#define TEST_NAME_SIZE 128
#define ERROR_MSG_SIZE 256

// testing the creating and freeing of individual nodes
void test_case_1(void) {

    // Test setup
    TestInstancePtr testInstance = initTestInstance("AST Node Creation with snprintf");

    // Node declarations
    ASTNodePtr node;

    // Declare an array of node types to iterate through
    enum ASTNodeTypes nodeTypes[] = {
        AST_NODE_DECLARE,
        AST_NODE_ASSIGN,
        AST_NODE_TRUTH_EXPRESION,
        AST_NODE_EXPRESION,
        AST_NODE_IF_ELSE,
        AST_NODE_FUNC_CALL,
        AST_NODE_FUNCTION,
        AST_NODE_WHILE,
        AST_NODE_VALUE,
        AST_NODE_VARIABLE,
        AST_NODE_OPERAND
    };

    const char* nodeTypeNames[] = {
        "AST_NODE_DECLARE",       // var a = 6
        "AST_NODE_ASSIGN",        // a = 6
        "AST_NODE_TRUTH_EXPRESION", // a == 6
        "AST_NODE_EXPRESION",     // a + 6
        "AST_NODE_IF_ELSE",       // if (a == 6) { ... } else { ... }
        "AST_NODE_FUNC_CALL",     // func(a, b, c)
        "AST_NODE_FUNCTION",      // func(a, b, c) { ... }
        "AST_NODE_WHILE",         // while (a == 6) { ... }
        "AST_NODE_VALUE",         // 6, 3.14, 'a', "string"
        "AST_NODE_VARIABLE",      // a
        "AST_NODE_OPERAND"        // +, -, *, /
    };

    // Iterate through all node types
    for (unsigned long i = 0; i < sizeof(nodeTypes) / sizeof(nodeTypes[0]); i++) {
        char testName[TEST_NAME_SIZE];
        char errorMsg[ERROR_MSG_SIZE];

        // TEST 1: Node creation
        snprintf(testName, TEST_NAME_SIZE, "Creating node of type: %s", nodeTypeNames[i]);
        node = ASTcreateNode(nodeTypes[i]);
        snprintf(errorMsg, ERROR_MSG_SIZE, "Failed to create node of type: %s (expected), actual: %s (unexpected)",
                nodeTypeNames[i], node != NULL ? nodeTypeNames[node->type] : "NULL");

        testCase(
            testInstance,
            node != NULL,
            testName,
            "Node created successfully (expected)",
            errorMsg
        );

        // TEST 2: Correct type check
        snprintf(testName, TEST_NAME_SIZE, "Checking node type: %s", nodeTypeNames[i]);
        snprintf(errorMsg, ERROR_MSG_SIZE, "Expected type: %s, but got: %s",
                nodeTypeNames[i], node != NULL ? nodeTypeNames[node->type] : "NULL");
        testCase(
            testInstance,
            node != NULL && node->type == nodeTypes[i],
            testName,
            "Node is of the correct type (expected)",
            errorMsg
        );

        // TEST 3: Finished flag check
        snprintf(testName, TEST_NAME_SIZE, "Checking finished flag for node: %s", nodeTypeNames[i]);
        snprintf(errorMsg, ERROR_MSG_SIZE, "Node finished flag is incorrect for type: %s", nodeTypeNames[i]);
        testCase(
            testInstance,
            node != NULL && !node->finished,
            testName,
            "Node is not finished (expected)",
            errorMsg
        );

        // TEST 4: Free the node
        snprintf(testName, TEST_NAME_SIZE, "Freeing node of type: %s", nodeTypeNames[i]);
        snprintf(errorMsg, ERROR_MSG_SIZE, "Failed to free node of type: %s", nodeTypeNames[i]);
        testCase(
            testInstance,
            ASTfreeNode(node),
            testName,
            "Node freed successfully (expected)",
            errorMsg
        );
    }

    // Finish the test instance
    finishTestInstance(testInstance);

}

/*
Now since we have tested all the individual nodes, we can test the AST tree creation
what we need to be able to create is:
    declaration
    assignment
    truth expresion
    expresion
    if else
    function call
    function
    while
    value
    variable
    operand

at this point, we will just test the indivudial functions in the simpliest way possible, test case 3 will have
the hard test, since i have to create a general free AST function
*/
void test_case_2(void) {

    TestInstancePtr testInstance = initTestInstance("AST Tree Creation");

    // test 1 AST for value 6

    struct TOKEN value;
    value.type = TOKEN_I32;
    value.value = "6";

    struct SymVariable var;
    var.name = "a";
    var.type = dTypeI32;
    var.accesed = false;
    var.mutable = true;

    ASTNodePtr valueNode = ASTcreateNode(AST_NODE_VALUE);

    // initing the value
    testCase(
        testInstance,
        valueNode != NULL,
        "Creating AST Node for value 6",
        "Node created successfully (expected)",
        "Failed to create node for value 6 (unexpected)"
    );
    
    // editing the value node
    testCase(
        testInstance,
        ASTeditNodeValue(valueNode, &value) == SUCCESS,
        "Editing AST Node for value 6",
        "Node edited successfully (expected)",
        "Failed to edit node for value 6 (unexpected)"
    );

    // checking the type of the node
    testCase(
        testInstance,
        valueNode->type == AST_NODE_VALUE,
        "Checking node type for value 6",
        "Node is of the correct type (expected)",
        "Node is not of the correct type (unexpected)"
    );

    // checking the finished flag
    testCase(
        testInstance,
        valueNode->finished == false,
        "Checking finished flag for value 6",
        "Node is not finished (expected)",
        "Node is finished (unexpected)"
    );

    // checking the value of the node
    testCase(
        testInstance,
        valueNode->data->value->type == dTypeI32,
        "Checking value type for node value 6",
        "Node has the correct value (expected)",
        "Node has an incorrect value (unexpected)"
    );

    // checking the value of the node
    testCase(
        testInstance,
        strcmp(valueNode->data->value->value, "6") == 0,
        "Checking value for node value 6",
        "Node has the correct value (expected)",
        "Node has an incorrect value (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(valueNode),
        "Freeing node for value 6",
        "Node freed successfully (expected)",
        "Failed to free node for value 6 (unexpected)"
    );

    // test 2. AST for var a = 6;
    ASTNodePtr declareNode = ASTcreateNode(AST_NODE_DECLARE);
    valueNode = ASTcreateNode(AST_NODE_VALUE);
    var.declaration = declareNode;

    // bad func call
    testCase(
        testInstance,
        ASTeditDeclareNode(declareNode, NULL, NULL) == E_INTERNAL,
        "Bad calling of the edit func",
        "Function faild as expected (expected)",
        "Function did not fail as expected (unexpected)"
    );

    // saving the variable to the declare node
    testCase(
        testInstance,
        ASTeditDeclareNode(declareNode, &var, NULL) == SUCCESS,
        "Saving variable to the declare node",
        "Variable saved successfully (expected)",
        "Failed to save variable to the declare node (unexpected)"
    );

    // saving the value to the declare node
    testCase(
        testInstance,
        ASTeditDeclareNode(declareNode, NULL, valueNode) == SUCCESS,
        "Saving value to the declare node",
        "Value saved successfully (expected)",
        "Failed to save value to the declare node (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(declareNode),
        "Freeing node for var a = 6",
        "Node freed successfully (expected)",
        "Failed to free node for var a = 6 (unexpected)"
    );

    // testing declaration of an imutable variable
    var.mutable = false;

    declareNode = ASTcreateNode(AST_NODE_DECLARE);

    // saving the variable to the declare node
    testCase(
        testInstance,
        ASTeditDeclareNode(declareNode, &var, NULL) == SUCCESS,
        "Assigning to an imutable variable",
        "Function faild as expected (expected)",
        "Function did not fail as expected (unexpected)"
    );

    // saving the value to the declare node
    testCase(
        testInstance,
        ASTeditDeclareNode(declareNode, NULL, valueNode) == SUCCESS,
        "Saving value to the declare node for an imutable variable",
        "Function faild as expected (expected)",
        "Function did not fail as expected (unexpected)"
    );

    ASTfreeNode(valueNode);

    // test 3. AST for a = 6;
    ASTNodePtr assignNode = ASTcreateNode(AST_NODE_ASSIGN);
    valueNode = ASTcreateNode(AST_NODE_VALUE);

    // first sub test, assing to mutable variable
    declareNode->data->declare->variable->mutable = true;

    // saving the variable to the declare node
    testCase(
        testInstance,
        ASTeditAssignNode(assignNode, declareNode, NULL) == SUCCESS,
        "Assigning to a mutable variable",
        "Variable saved successfully (expected)",
        "Failed to save variable to the assign node (unexpected)"
    );

    // assingin value to mutable variable
    testCase(
        testInstance,
        ASTeditAssignNode(assignNode, NULL, valueNode) == SUCCESS,
        "Assigning value to a mutable variable",
        "Value saved successfully (expected)",
        "Failed to save value to the assign node (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(assignNode),
        "Freeing node for a = 6",
        "Node freed successfully (expected)",
        "Failed to free node for a = 6 (unexpected)"
    );

    // changing the varible to imutable
    declareNode->data->declare->variable->mutable = false;

    assignNode = ASTcreateNode(AST_NODE_ASSIGN);
    valueNode = ASTcreateNode(AST_NODE_VALUE);

    // saving the variable to the declare node
    testCase(
        testInstance,
        ASTeditAssignNode(assignNode, declareNode, NULL) == E_SEMANTIC_REDIFINITION,
        "Assigning to an imutable variable",
        "Function faild as expected (expected)",
        "Function did not fail as expected (unexpected)"
    );

    testCase(
        testInstance,
        ASTfreeNode(assignNode),
        "Freeing node for a = 6",
        "Node freed successfully (expected)",
        "Failed to free node for a = 6 (unexpected)"
    );

    ASTfreeNode(valueNode);


    // test 4. AST for a+6;
    ASTNodePtr expresion = ASTcreateNode(AST_NODE_EXPRESION);
    ASTNodePtr var1 = ASTcreateNode(AST_NODE_VARIABLE);
    var1->data->variable = &var;

    // adding the variable to the expresion
    testCase(
        testInstance,
        ASTaddNodeToExpresion(expresion, var1) == SUCCESS,
        "Adding variable 'a' to the expresion 'a+6'",
        "Variable added successfully (expected)",
        "Failed to add variable to the expresion (unexpected)"
    );

    // adding + to the expresion
    ASTNodePtr operand = ASTcreateNode(AST_NODE_OPERAND);
    struct TOKEN plus;
    plus.type = TOKEN_PLUS;
    plus.value = "+";
    operand->data->operand = &plus;

    // adding the operand to the expresion
    testCase(
        testInstance,
        ASTaddNodeToExpresion(expresion, operand) == SUCCESS,
        "Adding operand '+' to the expresion: 'a+6'",
        "Operand added successfully (expected)",
        "Failed to add operand to the expresion (unexpected)"
    );

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    ASTeditNodeValue(valueNode, &value);

    // adding the value to the expresion
    testCase(
        testInstance,
        ASTaddNodeToExpresion(expresion, valueNode) == SUCCESS,
        "Adding value '6 'to the expresion 'a+6'",
        "Value added successfully (expected)",
        "Failed to add value to the expresion (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(expresion),
        "Freeing node for a+6",
        "Node freed successfully (expected)",
        "Failed to free node for a+6 (unexpected)"
    );

    // test 5 AST for a+6 == 6
    ASTNodePtr truthExpresion = ASTcreateNode(AST_NODE_TRUTH_EXPRESION);
    var1 = ASTcreateNode(AST_NODE_VARIABLE);
    var1->data->variable = &var;
    // adding the expresion part to the truth expresion
    testCase(
        testInstance,
        ASTeditTruthExpresion(truthExpresion, var1, TOKEN_NONE) == SUCCESS,
        "Adding variable 'a' to the truth expresion 'a+6 == 6'",
        "Variable added successfully (expected)",
        "Failed to add variable to the truth expresion (unexpected)"
    );

    // adding + to the expresion
    operand = ASTcreateNode(AST_NODE_OPERAND);
    operand->data->operand = &plus;

    testCase(
        testInstance,
        ASTeditTruthExpresion(truthExpresion, operand, TOKEN_NONE) == SUCCESS,
        "Adding operand '+' to the truth expresion 'a+6 == 6'",
        "Operand added successfully (expected)",
        "Failed to add operand to the truth expresion (unexpected)"
    );

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    ASTeditNodeValue(valueNode, &value);

    // adding the value to the expresion
    testCase(
        testInstance,
        ASTeditTruthExpresion(truthExpresion, valueNode, TOKEN_NONE) == SUCCESS,
        "Adding value '6' to the truth expresion 'a+6 == 6'",
        "Value added successfully (expected)",
        "Failed to add value to the truth expresion (unexpected)"
    );

    // adding the operand to the truth expresion
    operand = ASTcreateNode(AST_NODE_OPERAND);
    struct TOKEN equals;
    equals.type = TOKEN_EQUALS;
    equals.value = "==";
    operand->data->operand = &equals;

    // adding the operand to the truth expresion
    testCase(
        testInstance,
        ASTeditTruthExpresion(truthExpresion, operand, TOKEN_NONE) == SUCCESS,
        "Adding operand '==' to the truth expresion 'a+6 == 6'",
        "Operand added successfully (expected)",
        "Failed to add operand to the truth expresion (unexpected)"
    );

    // adding the value to the expresion
    testCase(
        testInstance,
        ASTeditTruthExpresion(truthExpresion, valueNode, TOKEN_NONE) == SUCCESS,
        "Adding value '6' to the truth expresion 'a+6 == 6'",
        "Value added successfully (expected)",
        "Failed to add value to the truth expresion (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(truthExpresion),
        "Freeing node for a+6 == 6",
        "Node freed successfully (expected)",
        "Failed to free node for a+6 == 6 (unexpected)"
    );

    ASTfreeNode(valueNode);
    //ASTfreeNode(var1);
    //ASTfreeNode(operand);

    // test 6 AST for if (a == 6)
    ASTNodePtr ifNode = ASTcreateNode(AST_NODE_IF_ELSE);

    var1 = ASTcreateNode(AST_NODE_VARIABLE);
    var1->data->variable = &var;

    // adding the expresion part to the if node
    testCase(
        testInstance,
        ASTeditIfNode(ifNode, var1) == SUCCESS,
        "Adding variable 'a' to the if node 'if (a == 6)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the if node (unexpected)"
    );

    // adding the operand to the if node
    operand = ASTcreateNode(AST_NODE_OPERAND);
    operand->data->operand = &equals;

    testCase(
        testInstance,
        ASTeditIfNode(ifNode, operand) == SUCCESS,
        "Adding operand '==' to the if node 'if (a == 6)'",
        "Operand added successfully (expected)",
        "Failed to add operand to the if node (unexpected)"
    );

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    ASTeditNodeValue(valueNode, &value);

    // adding the value to the if node
    testCase(
        testInstance,
        ASTeditIfNode(ifNode, valueNode) == SUCCESS,
        "Adding value '6' to the if node 'if (a == 6)'",
        "Value added successfully (expected)",
        "Failed to add value to the if node (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(ifNode),
        "Freeing node for if (a == 6)",
        "Node freed successfully (expected)",
        "Failed to free node for if (a == 6) (unexpected)"
    );

    ASTfreeNode(valueNode);
    //ASTfreeNode(var1);
    //ASTfreeNode(operand);

    // test 7 AST for while (a > 6)
    ASTNodePtr whileNode = ASTcreateNode(AST_NODE_WHILE);

    var1 = ASTcreateNode(AST_NODE_VARIABLE);
    var1->data->variable = &var;

    // adding the expresion part to the while node
    testCase(
        testInstance,
        ASTeditWhileNode(whileNode, var1) == SUCCESS,
        "Adding variable 'a' to the while node 'while (a > 6)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the while node (unexpected)"
    );

    struct TOKEN greater;
    greater.type = TOKEN_GREATERTHAN;
    greater.value = ">";

    operand = ASTcreateNode(AST_NODE_OPERAND);
    operand->data->operand = &greater;

    // adding the operand to the while node
    testCase(
        testInstance,
        ASTeditWhileNode(whileNode, operand) == SUCCESS,
        "Adding operand '>' to the while node 'while (a > 6)'",
        "Operand added successfully (expected)",
        "Failed to add operand to the while node (unexpected)"
    );

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    ASTeditNodeValue(valueNode, &value);

    // adding the value to the while node
    testCase(
        testInstance,
        ASTeditWhileNode(whileNode, valueNode) == SUCCESS,
        "Adding value '6' to the while node 'while (a > 6)'",
        "Value added successfully (expected)",
        "Failed to add value to the while node (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(whileNode),
        "Freeing node for while (a > 6)",
        "Node freed successfully (expected)",
        "Failed to free node for while (a > 6) (unexpected)"
    );

    ASTfreeNode(valueNode);
    //ASTfreeNode(var1);
    //ASTfreeNode(operand);

    // test 8 AST for func(a, b, c) i32; (function declaration)
    ASTNodePtr funcDefNode = ASTcreateNode(AST_NODE_FUNCTION);

    ASTNodePtr funcParam = ASTcreateNode(AST_NODE_VARIABLE);
    funcParam->data->variable = &var;

    char *functionName = malloc(sizeof(char) * 5);
    strcpy(functionName, "func");

    // adding the function name to the function node
    testCase(
        testInstance,
        ASTeditFunctionNode(funcDefNode, functionName, dTypeNone, NULL) == SUCCESS,
        "Adding function name 'func' to the function node 'func(a, b, c) i32;'",
        "Function name added successfully (expected)",
        "Failed to add function name to the function node (unexpected)"
    );

    // adding the variable to the function node
    testCase(
        testInstance,
        ASTeditFunctionNode(funcDefNode, NULL, dTypeNone, funcParam) == SUCCESS,
        "Adding variable 'a' to the function node 'func(a, b, c) i32;'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function node (unexpected)"
    );

    // adding the variable to the function node
    testCase(
        testInstance,
        ASTeditFunctionNode(funcDefNode, NULL, dTypeNone, funcParam) == SUCCESS,
        "Adding variable 'b' to the function node 'func(a, b, c) i32;'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function node (unexpected)"
    );

    // adding the variable to the function node
    testCase(
        testInstance,
        ASTeditFunctionNode(funcDefNode, NULL, dTypeNone, funcParam) == SUCCESS,
        "Adding variable 'c' to the function node 'func(a, b, c) i32;'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function node (unexpected)"
    );

    // adding the return type to the function node

    testCase(
        testInstance,
        ASTeditFunctionNode(funcDefNode, NULL, dTypeI32, NULL) == SUCCESS,
        "Adding return type 'i32' to the function node 'func(a, b, c) i32;'",
        "Return type added successfully (expected)",
        "Failed to add return type to the function node (unexpected)"
    );

    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(funcDefNode),
        "Freeing node for func(a, b, c) i32;",
        "Node freed successfully (expected)",
        "Failed to free node for func(a, b, c) i32; (unexpected)"
    );

    // test 9 AST for function call func(a, b, c)

    ASTNodePtr funcCallNode = ASTcreateNode(AST_NODE_FUNC_CALL);
    ASTNodePtr funcCallParam = ASTcreateNode(AST_NODE_VARIABLE);
    funcCallParam->data->variable = &var;

    // adding the function name to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode, functionName, NULL) == SUCCESS,
        "Adding function name 'func' to the function call node 'func(a, b, c)'",
        "Function name added successfully (expected)",
        "Failed to add function name to the function call node (unexpected)"
    );

    // adding the variable to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'a' to the function call node 'func(a, b, c)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    // adding the variable to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'b' to the function call node 'func(a, b, c)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    // adding the variable to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'c' to the function call node 'func(a, b, c)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    /*
    // freeing the node
    testCase(
        testInstance,
        ASTfreeNode(funcCallNode),
        "Freeing node for func(a, b, c)",
        "Node freed successfully (expected)",
        "Failed to free node for func(a, b, c) (unexpected)"
    );
    */

    // test 10 AST for func1(a, func2(b, c), d)
    char *funcName1 = malloc(sizeof(char) * 6);
    strcpy(funcName1, "func1");
    ASTNodePtr funcCallNode1 = ASTcreateNode(AST_NODE_FUNC_CALL);
    funcCallParam = ASTcreateNode(AST_NODE_VARIABLE);
    funcCallParam->data->variable = &var;

    // addng the function name to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, funcName1, NULL) == SUCCESS,
        "Adding function name 'func1' to the function call node 'func1(a, func2(b, c), d)'",
        "Function name added successfully (expected)",
        "Failed to add function name to the function call node (unexpected)"
    );

    // adding the variable to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'a' to the function call node 'func1(a, func2(b, c), d)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    ASTNodePtr funcCallNode2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    char *funcName2 = malloc(sizeof(char) * 6);
    strcpy(funcName2, "func2");    
    // adding a new function call as a parameter
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, NULL, funcCallNode2) == SUCCESS,
        "Adding function call 'func1(a, b)' to the function call node 'func(a, func1(a, b), d)'",
        "Function call added successfully (expected)",
        "Failed to add function call to the function call node (unexpected)"
    );

    // changing the 2nd function call name
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode2, funcName2, NULL) == SUCCESS,
        "Changing function name 'func2' to 'func2' in the function call node 'func(a, func1(a, b), d)'",
        "Function name changed successfully (expected)",
        "Failed to change function name in the function call node (unexpected)"
    );

    // adding the variable to the inner function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'b' to the function call node 'func1(a, func2(b, c), d)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    // adding the variable to the inner function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'c' to the function call node 'func1(a, func2(b, c), d)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    // exiting the inner func
    testCase(
        testInstance,
        ASTswitchToOuterFunctionCall(funcCallNode1) == SUCCESS,
        "Exiting the inner function call 'func2(b, c)' in the function call node 'func1(a, func2(b, c), d)'",
        "Function call exited successfully (expected)",
        "Failed to exit the function call (unexpected)"
    );

    // adding the variable to the function call node
    testCase(
        testInstance,
        ASTeditFunctionCallNode(funcCallNode1, NULL, funcCallParam) == SUCCESS,
        "Adding variable 'd' to the function call node 'func1(a, func2(b, c), d)'",
        "Variable added successfully (expected)",
        "Failed to add variable to the function call node (unexpected)"
    );

    finishTestInstance(testInstance);
}

int main(void) {
    test_case_1();
    test_case_2();
    return 0;
}  // end of tests
