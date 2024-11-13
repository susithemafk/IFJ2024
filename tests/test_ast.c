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

/*
Test Overview:
    - Testing the inicialization of every AST node type, and the freeing of the nodes in the test_case_1 func
    - Testing the inicialization of these AST trees in the test_case_2 func

    (prerequsits)
        values:     types:         
            - 6     (I32)
            - 3.14  (F64)
            - hello (U8)
        operands:
            - +     (PLUS)
            - *     (MULTIPLY)
            - >     (GREATERTHAN)
        variables:  
            - a     (I32)
            - b     (I32)
            - c     (F64)
            - d     (F64)
            - notMutableVar (I32, not mutable)      

    - AST tree with value 6
        - create a tree with a single value node with value 6
        - check the type of the node
        - check the value of the node
        - free the node
    - AST tree with value 3.14
        - create a tree with a single value node with value 3.14
        - check the type of the node
        - check the value of the node
        - free the node
    - AST tree with value hello
        - create a tree with a single value node with value hello
        - check the type of the node
        - check the value of the node
        - free the node
    - AST tree for declaration
        - var a
        - var b
        - var c = 3.14 (imutable ..)
    - AST tree for assign
        - a = 6
        - b = 3.14
        - c = 6 (imutable ..)
    - AST tree for function
        - func(a, b, c) { ... }
    - AST tree for function call
        - func(a, func1(b, c), d)
    - AST tree for expresion
        - a + b * 6
    - AST tree for truth expresin
        - a > b
    - AST tree for if else
        - if (a > b) { ... } else { ... }
    - AST tree for while
        - while (a > b) { ... }
*/

void print_token(unsigned int idx, void *data) {

    printf("    %d: ", idx);

    ASTNodePtr node = (ASTNodePtr)data;
    switch(node->type) {
        case AST_NODE_VARIABLE:
            printf("type: variable, name: %s, type: %d", node->data->variable->name, node->data->variable->type);
            break;
        case AST_NODE_VALUE:
            printf("type: value, value: %s, type: %d", node->data->value->value, node->data->value->type);
            break;
        case AST_NODE_OPERAND:
            printf("type: operand, value: %s", node->data->operand->value);
            break;
        default:
            printf("unexpected node type: %d", node->type);
    }
}

// testing the creating and freeing of individual nodes
void test_ast_init(void) {

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
        AST_NODE_OPERAND,
        AST_NODE_RETURN,
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
        "AST_NODE_OPERAND",        // +, -, *, /
        "AST_NODE_RETURN"         // return a
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
            ASTfreeNode(&node),
            testName,
            "Node freed successfully (expected)",
            errorMsg
        );
    }

    // Finish the test instance
    finishTestInstance(testInstance);

}

// values

// 6
struct TOKEN value1 = { .type = TOKEN_I32, .value = "6" }; 
// 3.14
struct TOKEN value2 = { .type = TOKEN_F64, .value = "3.14" };
// "hello"
struct TOKEN value3 = { .type = TOKEN_U8, .value = "hello" };

// operands
// +
struct TOKEN opr1 = { .type = TOKEN_PLUS, .value = "+" };
// *
struct TOKEN opr2 = { .type = TOKEN_MULTIPLY, .value = "*" };
// >
struct TOKEN opr3 = { .type = TOKEN_GREATERTHAN, .value = ">" };

// variables
// a
struct SymVariable var1 = { .name = "a", .type = dTypeI32, .accesed = false, .mutable = true, .nullable = false};
// b
struct SymVariable var2 = { .name = "b", .type = dTypeI32, .accesed = false, .mutable = true, .nullable = false};
// c
struct SymVariable var3 = { .name = "c", .type = dTypeF64, .accesed = false, .mutable = true, .nullable = false};
// d
struct SymVariable var4 = { .name = "d", .type = dTypeF64, .accesed = false, .mutable = true, .nullable = false};
// notMutableVar
struct SymVariable varNotMutable = {.id=1, .name = "notMutableVar", .type = dTypeI32, .accesed = false, .mutable = false };

// aCanBeNull
struct SymVariable varACanBeNull = { .name = "aCanBeNull", .type = dTypeI32, .accesed = false, .mutable = true, .nullable = true };

// aNotNullable 
struct SymVariable varANotNullable = { .name = "aNotNullable", .type = dTypeI32, .accesed = false, .mutable = true, .nullable = false };

// err code
enum ERR_CODES err;
// AST nodes
ASTNodePtr valueNode, variableNode, declareNode, assignNode, functionCallNode, functionNode, whileNode, ifElseNode, expresionNode, truthExpresionNode, operandNode1, operandNode2, operandNode3, operand1, operand2;


// AST for ["6"]
void test_case_1(TestInstancePtr testInstance) {

    ASTNodePtr valueNode = ASTcreateNode(AST_NODE_VALUE);
    // test 1 seting the value
    enum ERR_CODES err = ASTinitNodeValue(valueNode, &value1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeValue",
        "Value node initialized successfully",
        "Failed to initialize value node"
    );

    // test 2 checking the value type
    testCase(
        testInstance,
        valueNode->data->value->type == dTypeI32,
        "Value node type check",
        "Value node type is correct",
        "Value node type is incorrect"
    );

    // test 3 checking the value
    testCase(
        testInstance,
        strcmp(valueNode->data->value->value, "6") == 0,
        "Value node value check",
        "Value node value is correct",
        "Value node value is incorrect"
    );

    // test 4 free the value node
    testCase(
        testInstance,
        ASTfreeNode(&valueNode),
        "ASTfreeNode",
        "Value node freed successfully",
        "Failed to free value node"
    );
}

// AST for ["3.14"]
void test_case_2(TestInstancePtr testInstance) {

    ASTNodePtr valueNode = ASTcreateNode(AST_NODE_VALUE);
    enum ERR_CODES err = ASTinitNodeValue(valueNode, &value2);

    // test 5 setting the value node for a float
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeValue",
        "Value node initialized successfully",
        "Failed to initialize value node"
    );

    // test 6 checking the value type for a float
    testCase(
        testInstance,
        valueNode->data->value->type == dTypeF64,
        "Value node type check",
        "Value node type is correct",
        "Value node type is incorrect"
    );

    // test 7 checking the value for a float
    testCase(
        testInstance,
        strcmp(valueNode->data->value->value, "3.14") == 0,
        "Value node value check",
        "Value node value is correct",
        "Value node value is incorrect"
    );

    // test 8 free the value node
    testCase(
        testInstance,
        ASTfreeNode(&valueNode),
        "ASTfreeNode",
        "Value node freed successfully",
        "Failed to free value node"
    );

}

// AST for ["hello"]
void test_case_3(TestInstancePtr testInstance) {

    ASTNodePtr valueNode = ASTcreateNode(AST_NODE_VALUE);
    enum ERR_CODES err = ASTinitNodeValue(valueNode, &value3);

    // test 9 setting the value node for a string
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeValue",
        "Value node initialized successfully",
        "Failed to initialize value node"
    );

    // test 10 checking the value type for a string
    testCase(
        testInstance,
        valueNode->data->value->type == dTypeU8,
        "Value node type check",
        "Value node type is correct",
        "Value node type is incorrect"
    );

    // test 11 checking the value for a string
    testCase(
        testInstance,
        strcmp(valueNode->data->value->value, "hello") == 0,
        "Value node value check",
        "Value node value is correct",
        "Value node value is incorrect"
    );

    // test 12 free the value node
    testCase(
        testInstance,
        ASTfreeNode(&valueNode),
        "ASTfreeNode",
        "Value node freed successfully",
        "Failed to free value node"
    );
}

// AST for varibale [a]
void test_case_4(TestInstancePtr testInstance) {

    ASTNodePtr variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &var1);
    // test 13 AST for variable a
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeVariable",
        "Variable node initialized successfully",
        "Failed to initialize variable node"
    );

    // test 14 checking the variable name
    testCase(
        testInstance,
        strcmp(variableNode->data->variable->name, "a") == 0,
        "Variable node name check",
        "Variable node name is correct",
        "Variable node name is incorrect"
    );

    // test 15 checking the variable type
    testCase(
        testInstance,
        variableNode->data->variable->type == dTypeI32,
        "Variable node type check",
        "Variable node type is correct",
        "Variable node type is incorrect"
    );

    // test 16 checking the variable mutable flag
    testCase(
        testInstance,
        variableNode->data->variable->mutable == true,
        "Variable node mutable flag check",
        "Variable node mutable flag is correct",
        "Variable node mutable flag is incorrect"
    );

    // test 17 free the variable node
    testCase(
        testInstance,
        ASTfreeNode(&variableNode),
        "ASTfreeNode",
        "Variable node freed successfully",
        "Failed to free variable node"
    );

}

// AST for varibale [notMutableVar]
void test_case_5(TestInstancePtr testInstance) {

    // test 18 AST imutable variable
    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &varNotMutable);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeVariable",
        "Variable node initialized successfully",
        "Failed to initialize variable node"
    );

    // test 19 checking the variable name
    testCase(
        testInstance,
        strcmp(variableNode->data->variable->name, "notMutableVar") == 0,
        "Variable node name check",
        "Variable node name is correct",
        "Variable node name is incorrect"
    );

    // test 20 checking the variable type
    testCase(
        testInstance,
        variableNode->data->variable->type == dTypeI32,
        "Variable node type check",
        "Variable node type is correct",
        "Variable node type is incorrect"
    );

    // test 21 checking the variable mutable flag
    testCase(
        testInstance,
        variableNode->data->variable->mutable == false,
        "Variable node mutable flag check",
        "Variable node mutable flag is correct",
        "Variable node mutable flag is incorrect"
    );

    // test 22 free the variable node
    testCase(
        testInstance,
        ASTfreeNode(&variableNode),
        "ASTfreeNode",
        "Variable node freed successfully",
        "Failed to free variable node"
    );

}

// AST for declare node [var a = 6]
void test_case_6(TestInstancePtr testInstance) {

    // test 23 AST for declare node
    declareNode = ASTcreateNode(AST_NODE_DECLARE);
    err = ASTeditDeclareNode(declareNode, &var1, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditDeclareNode",
        "Declare node initialized successfully",
        "Failed to initialize declare node"
    );

    // test 24 checking the variable name
    testCase(
        testInstance,
        strcmp(declareNode->data->declare->variable->name, "a") == 0,
        "Declare node name check",
        "Declare node name is correct",
        "Declare node name is incorrect"
    );

    // test 25 checking the variable type
    testCase(
        testInstance,
        declareNode->data->declare->variable->type == dTypeI32,
        "Declare node type check",
        "Declare node type is correct",
        "Declare node type is incorrect"
    );

    // test 26 checking the variable mutable flag
    testCase(
        testInstance,
        declareNode->data->declare->variable->mutable == true,
        "Declare node mutable flag check",
        "Declare node mutable flag is correct",
        "Declare node mutable flag is incorrect"
    );

    // test 27, assing value to the declare node
    valueNode = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode, &value1);
    err = ASTeditDeclareNode(declareNode, NULL, valueNode);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditDeclareNode",
        "Declare node value assigned successfully",
        "Failed to assign value to declare node"
    );

    // test 28 checking the value type
    testCase(
        testInstance,
        declareNode->data->declare->value->data->value->type == dTypeI32,
        "Declare node value type check",
        "Declare node value type is correct",
        "Declare node value type is incorrect"
    );

    // test 29 checking the value
    testCase(
        testInstance,
        strcmp(declareNode->data->declare->value->data->value->value, "6") == 0,
        "Declare node value check",
        "Declare node value is correct",
        "Declare node value is incorrect"
    );

    // test 30 free the declare node
    testCase(
        testInstance,
        ASTfreeNode(&declareNode),
        "ASTfreeNode",
        "Declare node freed successfully",
        "Failed to free declare node"
    );

}

// AST for [const notMutableVar = 6]
void test_case_7(TestInstancePtr testInstance) {

    // test 31 AST for declare node, imutable variable
    declareNode = ASTcreateNode(AST_NODE_DECLARE);
    err = ASTeditDeclareNode(declareNode, &varNotMutable, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditDeclareNode",
        "Declare node initialized successfully",
        "Failed to initialize declare node"
    );

    // test 32 checking the variable name
    testCase(
        testInstance,
        strcmp(declareNode->data->declare->variable->name, "notMutableVar") == 0,
        "Declare node name check",
        "Declare node name is correct",
        "Declare node name is incorrect"
    );

    // test 33 checking the variable type
    testCase(
        testInstance,
        declareNode->data->declare->variable->type == dTypeI32,
        "Declare node type check",
        "Declare node type is correct",
        "Declare node type is incorrect"
    );

    // test 34 checking the variable mutable flag
    testCase(
        testInstance,
        declareNode->data->declare->variable->mutable == false,
        "Declare node mutable flag check",
        "Declare node mutable flag is correct",
        "Declare node mutable flag is incorrect"
    );

    // test 35, assing value to the declare node
    valueNode = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode, &value1);
    err = ASTeditDeclareNode(declareNode, NULL, valueNode);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditDeclareNode",
        "Declare node value assigned successfully",
        "Failed to assign value to declare node"
    );

    // test 36 checking the value type
    testCase(
        testInstance,
        declareNode->data->declare->value->data->value->type == dTypeI32,
        "Declare node value type check",
        "Declare node value type is correct",
        "Declare node value type is incorrect"
    );

    // test 37 checking the value
    testCase(
        testInstance,
        strcmp(declareNode->data->declare->value->data->value->value, "6") == 0,
        "Declare node value check",
        "Declare node value is correct",
        "Declare node value is incorrect"
    );

    // test 38 free the declare node
    testCase(
        testInstance,
        ASTfreeNode(&declareNode),
        "ASTfreeNode",
        "Declare node freed successfully",
        "Failed to free declare node"
    );

}

// AST for assign node [a = 6]
void test_case_8(TestInstancePtr testInstance) {

    // test 39 AST for assign node
    // we have the declaration pointer form the symtable
    assignNode = ASTcreateNode(AST_NODE_ASSIGN);
    err = ASTeditAssignNode(assignNode, &var1, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditAssignNode",
        "Assign node initialized successfully",
        "Failed to initialize assign node"
    );

    // test 40 checking the variable name
    testCase(
        testInstance,
        strcmp(assignNode->data->assign->variable->name, "a") == 0,
        "Assign node name check",
        "Assign node name is correct",
        "Assign node name is incorrect"
    );

    // test 41 checking the variable type
    testCase(
        testInstance,
        assignNode->data->assign->variable->type == dTypeI32,
        "Assign node type check",
        "Assign node type is correct",
        "Assign node type is incorrect"
    );

    // test 42 checking the variable mutable flag
    testCase(
        testInstance,
        assignNode->data->assign->variable->mutable == true,
        "Assign node mutable flag check",
        "Assign node mutable flag is correct",
        "Assign node mutable flag is incorrect"
    );

    // test 43, assing value to the assign node
    valueNode = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode, &value1);

    err = ASTeditAssignNode(assignNode, NULL, valueNode);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditAssignNode",
        "Assign node value assigned successfully",
        "Failed to assign value to assign node"
    );

    // test 44 checking the value type
    testCase(
        testInstance,
        assignNode->data->assign->value->data->value->type == dTypeI32,
        "Assign node value type check",
        "Assign node value type is correct",
        "Assign node value type is incorrect"
    );

    // test 45 checking the value
    testCase(
        testInstance,
        strcmp(assignNode->data->assign->value->data->value->value, "6") == 0,
        "Assign node value check",
        "Assign node value is correct",
        "Assign node value is incorrect"
    );

    // test 46 free the assign node
    testCase(
        testInstance,
        ASTfreeNode(&assignNode),
        "ASTfreeNode",
        "Assign node freed successfully",
        "Failed to free assign node"
    );
}

// AST for assign node [notMutableVar = 6]
void test_case_9(TestInstancePtr testInstance) {
    // test 47 AST for assign node, imutable variable
    assignNode = ASTcreateNode(AST_NODE_ASSIGN);
    err = ASTeditAssignNode(assignNode, &varNotMutable, NULL);

    testCase(
        testInstance,
        err == E_SEMANTIC_REDIFINITION,
        "ASTeditAssignNode - imutable variable",
        "failed to assign value to imutable variable (expected)",
        "assigned value to imutable variable (unexpected)"
    );

    // test 48 free the declare node
    testCase(
        testInstance,
        ASTfreeNode(&assignNode),
        "ASTedtAssignNode - imutable variable (bad assing)",
        "Declare node freed successfully",
        "Failed to free declare node"
    );
}

// AST for function node [func(a, b, c) { ... }]
void test_case_10(TestInstancePtr testInstance) {

    // test 49 AST for function node, adding name
    functionNode = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(functionNode, "func", dTypeUndefined, -1, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionNode",
        "Function node initialized successfully",
        "Failed to initialize function node"
    );

    // test 50 checking the function name
    testCase(
        testInstance,
        strcmp(functionNode->data->function->functionName, "func") == 0,
        "Function node name check",
        "Function node name is correct",
        "Function node name is incorrect"
    );

    // test 51 adding aruguments to the function node
    err = ASTeditFunctionNode(functionNode, NULL, dTypeUndefined, -1, &var1);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionNode - adding arguments",
        "Function node arguments added successfully",
        "Failed to add arguments to function node"
    );

    // test 52 checking the function argument amount
    LinkedList *arguments = functionNode->data->function->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 1,
        "Function node argument amount check",
        "Function node argument amount is correct",
        "Function node argument amount is incorrect"
    );

    // test 53 checking the function argument name
    ASTNodePtr argument = (ASTNodePtr)getDataAtIndex(arguments, 0);
    testCase(
        testInstance,
        strcmp(argument->data->variable->name, "a") == 0,
        "Function node argument name check",
        "Function node argument name is correct",
        "Function node argument name is incorrect"
    );

    // test 54 checking the function argument type
    testCase(
        testInstance,
        argument->data->variable->type == dTypeI32,
        "Function node argument type check",
        "Function node argument type is correct",
        "Function node argument type is incorrect"
    );

    // teset 55 adding a 2nd argument to the function node
    err = ASTeditFunctionNode(functionNode, NULL, dTypeUndefined, -1, &var2);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionNode - adding 2nd argument",
        "Function node 2nd argument added successfully",
        "Failed to add 2nd argument to function node"
    );

    // test 56 checking the function argument amount
    arguments = functionNode->data->function->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 2,
        "Function node argument amount check",
        "Function node argument amount is correct",
        "Function node argument amount is incorrect"
    );

    // test 57 checking the function argument name
    argument = (ASTNodePtr)getDataAtIndex(arguments, 1);

    testCase(
        testInstance,
        strcmp(argument->data->variable->name, "b") == 0,
        "Function node argument name check",
        "Function node argument name is correct",
        "Function node argument name is incorrect"
    );

    // test 58 checking the function argument type
    testCase(
        testInstance,
        argument->data->variable->type == dTypeI32,
        "Function node argument type check",
        "Function node argument type is correct",
        "Function node argument type is incorrect"
    );

    // test 59 adding a return type to the function node
    err = ASTeditFunctionNode(functionNode, NULL, dTypeI32, -1, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionNode - adding return type",
        "Function node return type added successfully",
        "Failed to add return type to function node"
    );

    // test 60 checking the function return type
    testCase(
        testInstance,
        functionNode->data->function->returnType == dTypeI32,
        "Function node return type check",
        "Function node return type is correct",
        "Function node return type is incorrect"
    );

    // test 62 checking the function nullable return type
    err = ASTeditFunctionNode(functionNode, NULL, dTypeUndefined, 0, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionNode - nullable return type",
        "Function node nullable return type set successfully",
        "Failed to set nullable return type to function node"
    );

    // test 63 checking the function nullable return type
    testCase(
        testInstance,
        functionNode->data->function->nullable == 0,
        "Function node nullable return type check",
        "Function node nullable return type is correct",
        "Function node nullable return type is incorrect"
    );

    // test 61 free the function node
    testCase(
        testInstance,
        ASTfreeNode(&functionNode),
        "ASTfreeNode",
        "Function node freed successfully",
        "Failed to free function node"
    );
}

// AST for function call node [func(6, a)]
void test_case_11(TestInstancePtr testInstance) {

    // test 64 AST for function call node
    functionCallNode = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(functionCallNode, "func", NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode",
        "Function call node initialized successfully",
        "Failed to initialize function call node"
    );

    // test 65 checking the function call name
    testCase(
        testInstance,
        strcmp(functionCallNode->data->functionCall->functionName, "func") == 0,
        "Function call node name check",
        "Function call node name is correct",
        "Function call node name is incorrect"
    );

    // test 66 adding arguments to the function call node
    err = ASTeditFunctionCallNode(functionCallNode, NULL, NULL, &value1);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode - adding arguments",
        "Function call node arguments added successfully",
        "Failed to add arguments to function call node"
    );

    // test 67 checking the function call argument amount
    LinkedList *arguments = functionCallNode->data->functionCall->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 1,
        "Function call node argument amount check",
        "Function call node argument amount is correct",
        "Function call node argument amount is incorrect"
    );

    // test 68 checking the function call argument type
    ASTNodePtr argument = (ASTNodePtr)getDataAtIndex(arguments, 0);

    testCase(
        testInstance,
        argument->data->value->type == dTypeI32,
        "Function call node argument type check",
        "Function call node argument type is correct",
        "Function call node argument type is incorrect"
    );

    // test 69 checking the function call argument value
    testCase(
        testInstance,
        strcmp(argument->data->value->value, "6") == 0,
        "Function call node argument value check",
        "Function call node argument value is correct",
        "Function call node argument value is incorrect"
    );

    // test 70 adding a 2nd argument to the function call node
    err = ASTeditFunctionCallNode(functionCallNode, NULL, &var2, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode - adding 2nd argument",
        "Function call node 2nd argument added successfully",
        "Failed to add 2nd argument to function call node"
    );

    // test 71 checking the function call argument amount
    arguments = functionCallNode->data->functionCall->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 2,
        "Function call node argument amount check",
        "Function call node argument amount is correct",
        "Function call node argument amount is incorrect"
    );

    // test 72 checking the function call argument name
    argument = (ASTNodePtr)getDataAtIndex(arguments, 1);

    testCase(
        testInstance,
        strcmp(argument->data->variable->name, "b") == 0,
        "Function call node argument name check",
        "Function call node argument name is correct",
        "Function call node argument name is incorrect"
    );

    // test 73 checking the function call argument type
    testCase(
        testInstance,
        argument->data->variable->type == dTypeI32,
        "Function call node argument type check",
        "Function call node argument type is correct",
        "Function call node argument type is incorrect"
    );
}

// AST for expresion node [a+b*6]
void test_case_12(TestInstancePtr testInstance) {

    // test 74 AST for expresion node a+b*6
    expresionNode = ASTcreateNode(AST_NODE_EXPRESION);
    err = ASTaddNodeToExpresion(expresionNode, &var1, NULL, NULL);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 1 added to expresion node successfully",
        "Failed to add operand node 1 to expresion node"
    );

    // test 76 adding the + operand to the expresion node
    err = ASTaddNodeToExpresion(expresionNode, NULL, NULL, &opr1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 1 added to expresion node successfully",
        "Failed to add operand node 1 to expresion node"
    );

    // test 77 adding the b variable to the expresion node
    err = ASTaddNodeToExpresion(expresionNode, &var2, NULL, NULL);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 2 added to expresion node successfully",
        "Failed to add operand node 2 to expresion node"
    );

    // test 79 adding the * operand to the expresion node
    err = ASTaddNodeToExpresion(expresionNode, NULL, NULL, &opr2);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 2 added to expresion node successfully",
        "Failed to add operand node 2 to expresion node"
    );

    err = ASTaddNodeToExpresion(expresionNode, NULL, &value1, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 3 added to expresion node successfully",
        "Failed to add operand node 3 to expresion node"
    );

    // test 81 finish the expresion node
    err = ASTfinishExpresion(expresionNode);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTfinishExpresion",
        "Expresion node finished successfully",
        "Failed to finish expresion node"
    );

    // checking the finished expresion node, should be a+b*6, but in postfix so ab6*+
    LinkedList *expresionOut = expresionNode->data->expresion->output;

    // test 82 check the size of the output
    testCase(
        testInstance,
        getSize(expresionOut) == 5,
        "Expresion node output size check",
        "Expresion node output size is correct",
        "Expresion node output size is incorrect"
    );

    // test 83 check the first element
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 0);
    printList(expresionOut, (void (*)(unsigned int, void *))print_token);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion node output first element check",
        "Expresion node output first element is correct",
        "Expresion node output first element is incorrect"
    );

    // test 84 check the value of the first element
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Expresion node output first element value check",
        "Expresion node output first element value is correct",
        "Expresion node output first element value is incorrect"
    );

    // test 85 check the second element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 1);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion node output second element check",
        "Expresion node output second element is correct",
        "Expresion node output second element is incorrect"
    );

    // test 86 check the value of the second element
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "Expresion node output second element value check",
        "Expresion node output second element value is correct",
        "Expresion node output second element value is incorrect"
    );

    // test 87 check the third element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 2);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VALUE,
        "Expresion node output third element check",
        "Expresion node output third element is correct",
        "Expresion node output third element is incorrect"
    );

    // test 88 check the value of the third element

    testCase(
        testInstance,
        strcmp(outputNode->data->value->value, "6") == 0,
        "Expresion node output third element value check",
        "Expresion node output third element value is correct",
        "Expresion node output third element value is incorrect"
    );

    // test 89 check the fourth element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 3);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_OPERAND,
        "Expresion node output fourth element check",
        "Expresion node output fourth element is correct",
        "Expresion node output fourth element is incorrect"
    );

    // test 90 check the value of the fourth element
    testCase(
        testInstance,
        outputNode->data->operand->type == opr2.type,
        "Expresion node output fourth element value check",
        "Expresion node output fourth element value is correct",
        "Expresion node output fourth element value is incorrect"
    );

    // test 91 check the fifth element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 4);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_OPERAND,
        "Expresion node output fifth element check",
        "Expresion node output fifth element is correct",
        "Expresion node output fifth element is incorrect"
    );

    // test 92 check the value of the fifth element
    testCase(
        testInstance,
        outputNode->data->operand->type == opr1.type,
        "Expresion node output fifth element value check",
        "Expresion node output fifth element value is correct",
        "Expresion node output fifth element value is incorrect"
    );

    // test 93 free the expresion node
    testCase(
        testInstance,
        ASTfreeNode(&expresionNode),
        "ASTfreeNode",
        "Expresion node freed",
        "Failed to free expresion node"
    );
}


// AST for truth expresion node [a>b]
void test_case_13(TestInstancePtr testInstance) {

    // test 94 AST for truth expresion node a>b
    truthExpresionNode = ASTcreateNode(AST_NODE_TRUTH_EXPRESION);

    err = ASTeditTruthExpresion(truthExpresionNode, &var1, NULL, NULL);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 1 added to truth expresion node successfully",
        "Failed to add operand node 1 to truth expresion node"
    );

    // test 95 test the operand in the truth expresion
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->operator == TOKEN_NONE,
        "Truth expresion node operator 1 check",
        "Truth expresion node operator 1 is correct",
        "Truth expresion node operator 1 is incorrect"
    );

    // test 96 test the left part of the truth expresion in the truth expresion (type)
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->left->type == AST_NODE_EXPRESION,
        "Truth expresion node left part type 1 check",
        "Truth expresion node left part type 1 is correct",
        "Truth expresion node left part type 1 is incorrect"
    );

    // test 97 test the left inner node type
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node left part value 1 check",
        "Truth expresion node left part value 1 is correct",
        "Truth expresion node left part value 1 is incorrect"
    );

    // test 98 test left inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Truth expresion node left part value 1 check",
        "Truth expresion node left part value 1 is correct",
        "Truth expresion node left part value 1 is incorrect"
    );

    // test 100 adding the > operand to the truth expresion
    err = ASTeditTruthExpresion(truthExpresionNode, NULL, NULL, &opr3);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 2 added to truth expresion node successfully",
        "Failed to add operand node 2 to truth expresion node"
    );

    // test 101 test the operand in the truth expresion
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "Truth expresion node operator 2 check",
        "Truth expresion node operator 2 is correct",
        "Truth expresion node operator 2 is incorrect"
    );

    // test 102 adding the b variable to the truth expresion
    err = ASTeditTruthExpresion(truthExpresionNode, &var2, NULL, NULL);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 2 added to truth expresion node successfully",
        "Failed to add operand node 2 to truth expresion node"
    );

    // test 103 test the right part of the truth expresion in the truth expresion (type)
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->right->type == AST_NODE_EXPRESION,
        "Truth expresion node right part type 2 check",
        "Truth expresion node right part type 2 is correct",
        "Truth expresion node right part type 2 is incorrect"
    );

    // test 104 test the right inner node type
    outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->right->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node right part value 2 check",
        "Truth expresion node right part value 2 is correct",
        "Truth expresion node right part value 2 is incorrect"
    );

    // test 105 test right inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "Truth expresion node right part value 2 check",
        "Truth expresion node right part value 2 is correct",
        "Truth expresion node right part value 2 is incorrect"
    );

    // test 106 if the switch did not change the left side
    outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node left part value 2 check",
        "Truth expresion node left part value 2 is correct",
        "Truth expresion node left part value 2 is incorrect"
    );

    // test 107 test left inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Truth expresion node left part value 2 check",
        "Truth expresion node left part value 2 is correct",
        "Truth expresion node left part value 2 is incorrect"
    );

    // test 108 free the truth expresion node
    testCase(
        testInstance,
        ASTfreeNode(&truthExpresionNode),
        "ASTfreeNode",
        "Truth expresion node freed",
        "Failed to free truth expresion node"
    );

}

// AST for if node [if (a > b) { ... }]
void test_case_14(TestInstancePtr testInstance) {

    // test 109 AST for if node for if (a > b)
    ifElseNode = ASTcreateNode(AST_NODE_IF_ELSE);
    err = ASTeditIfNode(ifElseNode, &var1, NULL, NULL, NULL, NULL);
    
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditifElseNode",
        "If node initialized successfully",
        "Failed to initialize if node"
    );

    // test 110 test the if node condition
    testCase(
        testInstance,
        ifElseNode->data->ifElse->condition->type == AST_NODE_TRUTH_EXPRESION,
        "If node condition type check",
        "If node condition type is correct",
        "If node condition type is incorrect"
    );

    // test 111 test the if node condition value
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(ifElseNode->data->ifElse->condition->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 112 test the if node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 113 test the if node condition value type (a)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "If node condition value type check",
        "If node condition value type is correct",
        "If node condition value type is incorrect"
    );

    // test 114 test adding the > operand to the if node condition
    err = ASTeditIfNode(ifElseNode, NULL, NULL, &opr3, NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditifElseNode",
        "Operand added to if node condition successfully",
        "Failed to add operand to if node condition"
    );

    // test 113 test the if node condition value (it should be auto negated)
    testCase(
        testInstance,
        ifElseNode->data->ifElse->condition->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 114 test adding variable b to the if node condition
    err = ASTeditIfNode(ifElseNode, &var2, NULL, NULL, NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditifElseNode",
        "Variable added to if node condition successfully",
        "Failed to add variable to if node condition"
    );

    // test 115 test the if node condition value

    outputNode = (ASTNodePtr)getDataAtIndex(ifElseNode->data->ifElse->condition->data->truthExpresion->right->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 116 test the if node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 117 test if the nod econdition value type (b)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "If node condition value type check",
        "If node condition value type is correct",
        "If node condition value type is incorrect"
    );

    // test 118 free the if node
    testCase(
        testInstance,
        ASTfreeNode(&ifElseNode),
        "ASTfreeNode",
        "If node freed",
        "Failed to free if node"
    );

}

// AST for if node [if (a) |aNotNull| {...}]
void test_case_15(TestInstancePtr testInstance) {

    ifElseNode = ASTcreateNode(AST_NODE_IF_ELSE);

    err = ASTeditIfNode(ifElseNode, NULL, NULL, NULL, &varACanBeNull, NULL);
    // test 119 test the if node condition)
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditifElseNode",
        "If node initialized successfully",
        "Failed to initialize if node"
    );

    // test 120 test the if node condition type
    testCase(
        testInstance,
        ifElseNode->data->ifElse->truVar->type == dTypeI32,
        "If node condition type check",
        "If node condition type is correct",
        "If node condition type is incorrect"
    );

    // test 121 test the if node condition value
    testCase(
        testInstance,
        strcmp(ifElseNode->data->ifElse->truVar->name, "aCanBeNull") == 0,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 122 test the if node condition value type (a)
    err = ASTeditIfNode(ifElseNode, NULL, NULL, NULL, NULL, &varANotNullable);
    
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditifElseNode",
        "Variable added to if node condition successfully",
        "Failed to add variable to if node condition"
    );

    // test 123 test the if node condition value
    testCase(
        testInstance,
        ifElseNode->data->ifElse->nulVar->type == dTypeI32,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 124 test the if node condition value
    testCase(
        testInstance,
        strcmp(ifElseNode->data->ifElse->nulVar->name, "aNotNullable") == 0,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 125 free the if node
    testCase(
        testInstance,
        ASTfreeNode(&ifElseNode),
        "ASTfreeNode",
        "If node freed",
        "Failed to free if node"
    );
}

// AST for while node [while (a > b) { ... }]
void test_case_16(TestInstancePtr testInstance) {
    // test 126 AST for while node for while (a > b)
    whileNode = ASTcreateNode(AST_NODE_WHILE);
    err = ASTeditWhileNode(whileNode, &var1, NULL, NULL, NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "While node initialized successfully",
        "Failed to initialize while node"
    );

    // test 127 test the while node condition type
    testCase(
        testInstance,
        whileNode->data->whileLoop->condition->type == AST_NODE_TRUTH_EXPRESION,
        "While node condition type check",
        "While node condition type is correct",
        "While node condition type is incorrect"
    );

    // test 128 test the while node condition value
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(whileNode->data->whileLoop->condition->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 129 test the while node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 130 test the while node condition value type (a)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "While node condition value type check",
        "While node condition value type is correct",
        "While node condition value type is incorrect"
    );

    // test 131 test adding the > operand to the while node condition
    err = ASTeditWhileNode(whileNode, NULL, NULL, &opr3, NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "Operand added to while node condition successfully",
        "Failed to add operand to while node condition"
    );

    // test 132 test the while node condition value (it should be auto negated)
    testCase(
        testInstance,
        whileNode->data->whileLoop->condition->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 133 test adding variable b to the while node condition
    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTeditWhileNode(whileNode, &var2, NULL, NULL, NULL, NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "Variable added to while node condition successfully",
        "Failed to add variable to while node condition"
    );

    // test 134 test the while node condition value
    outputNode = (ASTNodePtr)getDataAtIndex(whileNode->data->whileLoop->condition->data->truthExpresion->right->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 135 test the while node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 136 test if the nod econdition value type (b)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "While node condition value type check",
        "While node condition value type is correct",
        "While node condition value type is incorrect"
    );

    // test 137 free the while node
    testCase(
        testInstance,
        ASTfreeNode(&whileNode),
        "ASTfreeNode",
        "While node freed",
        "Failed to free while node"
    );
}

// AST for while node [while (a) |aNotNull| {...}]
void test_case_17(TestInstancePtr testInstance) {

    whileNode = ASTcreateNode(AST_NODE_WHILE);

    err = ASTeditWhileNode(whileNode, NULL, NULL, NULL, &varACanBeNull, NULL);
    // test 138 test the while node condition)
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "While node initialized successfully",
        "Failed to initialize while node"
    );
    // test 139 test the while node condition type
    testCase(
        testInstance,
        whileNode->data->whileLoop->truVar->type == dTypeI32,
        "While node condition type check",
        "While node condition type is correct",
        "While node condition type is incorrect"
    );

    // test 140 test the while node nulability of the var
    testCase(
        testInstance,
        whileNode->data->whileLoop->truVar->nullable,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 141 test the while node condition value
    testCase(
        testInstance,
        strcmp(whileNode->data->whileLoop->truVar->name, "aCanBeNull") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 142 test the while node condition value type (a)
    err = ASTeditWhileNode(whileNode, NULL, NULL, NULL, NULL, &varANotNullable);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "Variable added to while node condition successfully",
        "Failed to add variable to while node condition"
    );

    // test 143 test the while node condition value
    testCase(
        testInstance,
        whileNode->data->whileLoop->nulVar->type == dTypeI32,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 144 test the while node condition value
    testCase(
        testInstance,
        strcmp(whileNode->data->whileLoop->nulVar->name, "aNotNullable") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 145 free the while node
    testCase(
        testInstance,
        ASTfreeNode(&whileNode),
        "ASTfreeNode",
        "While node freed",
        "Failed to free while node"
    );

}

// test case for AST return node [return a+b]
void test_case_18(TestInstancePtr testInstance) {

    // create the return node
    ASTNodePtr returnNode = ASTcreateNode(AST_NODE_RETURN);

    // add the variable node to the return node
    err = ASTeditReturnNode(returnNode, &var1, NULL, NULL);

    // test 146 check if the return node was initialized successfully
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditReturnNode",
        "Return node initialized successfully",
        "Failed to initialize return node"
    );

    // test 147 addint the operand to the return node
    err = ASTeditReturnNode(returnNode, NULL, NULL, &opr1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditReturnNode",
        "Operand added to return node successfully",
        "Failed to add operand to return node"
    );

    // test 148 add the variable b to the return node
    err = ASTeditReturnNode(returnNode, &var2, NULL, NULL);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditReturnNode",
        "Variable added to return node successfully",
        "Failed to add variable to return node"
    );

    // teset 150 check if the expresion was compiled correctly
    err = ASTfinishExpresion(returnNode->data->returnNode->expression);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTfinishExpresion",
        "Expresion finished successfully",
        "Failed to finish expresion"
    );

    // check each of the expresion nodes
    LinkedList *expresionOut = returnNode->data->returnNode->expression->data->expresion->output;

    // test 151 check the size of the output
    testCase(
        testInstance,
        getSize(expresionOut) == 3,
        "Expresion output size check",
        "Expresion output size is correct",
        "Expresion output size is incorrect"
    );

    // test 152 check the first element
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion output first element check",
        "Expresion output first element is correct",
        "Expresion output first element is incorrect"
    );

    // test 153 check the value of the first element
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Expresion output first element value check",
        "Expresion output first element value is correct",
        "Expresion output first element value is incorrect"
    );

    // test 154 check the second element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 1);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion output second element check",
        "Expresion output second element is correct",
        "Expresion output second element is incorrect"
    );

    // test 156 check the value of the second element
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "Expresion output second element value check",
        "Expresion output second element value is correct",
        "Expresion output second element value is incorrect"
    );

    // test 157 check the third element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 2);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_OPERAND,
        "Expresion output third element check",
        "Expresion output third element is correct",
        "Expresion output third element is incorrect"
    );

    // test 158 check the value of the third element
    testCase(
        testInstance,
        outputNode->data->operand->type == opr1.type,
        "Expresion output third element value check",
        "Expresion output third element value is correct",
        "Expresion output third element value is incorrect"
    );

    // test 159 free the return node
    testCase(
        testInstance,
        ASTfreeNode(&returnNode),
        "ASTfreeNode",
        "Return node freed",
        "Failed to free return node"
    );
}

// main function for testing
int main(void) {
    test_ast_init(); // testing the init function for each AST node type
    TestInstancePtr testInstance = initTestInstance("Testing AST");
    test_case_1(testInstance); // AST for value node [6]
    test_case_2(testInstance); // AST for value node [3.14]
    test_case_3(testInstance); // AST for value node ["hello"]
    test_case_4(testInstance); // AST for variable [a]
    test_case_5(testInstance); // AST for variable [notMutableVar]
    test_case_6(testInstance); // AST for declare node [var a = 6]
    test_case_7(testInstance); // AST for declare node [const notMutableVar = 6]
    test_case_8(testInstance); // AST for assing node [a = 6]
    test_case_9(testInstance); // AST for assing node [notMutableVar = 6]
    test_case_10(testInstance); // AST for function node [func(a, b, c) { ... }]
    test_case_11(testInstance); // AST for function call node [func(6, a)]
    test_case_12(testInstance); // AST for expresion node [a+b*6]
    test_case_13(testInstance); // AST for truth expresion node [a>b]
    test_case_14(testInstance); // AST for if node [if (a > b) { ... }]
    test_case_15(testInstance); // AST for if node [if (a) |aNotNull| {...}]
    test_case_16(testInstance); // AST for while node [while (a > b) { ... }]
    test_case_17(testInstance); // AST for while node [while (a) |aNotNull| {...}]
    test_case_18(testInstance); // test case for AST return node [return a+b]
    finishTestInstance(testInstance);
    return 0;
}  // end of tests
