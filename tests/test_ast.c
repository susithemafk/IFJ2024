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
        printf("strings compiled\n");
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

void test_case_2(void) {
    // test 1 AST for value 6

    struct TOKEN value1;
    value1.type = TOKEN_I32;
    value1.value = "6";

    struct TOKEN value2;
    value2.type = TOKEN_F64;
    value2.value = "3.14";

    struct TOKEN opr1;
    opr1.type = TOKEN_PLUS;
    opr1.value = "+";

    struct TOKEN opr2;
    opr2.type = TOKEN_MULTIPLY;
    opr2.value = "*";

    struct TOKEN opr3;
    opr3.type = TOKEN_GREATERTHAN;
    opr3.value = ">";

    struct TOKEN value3;
    value3.type = TOKEN_U8;
    value3.value = "hello";

    struct SymVariable var1;
    var1.name = "a";
    var1.type = dTypeI32;
    var1.accesed = false;
    var1.mutable = true;

    struct SymVariable var2;
    var2.name = "b";
    var2.type = dTypeI32;
    var2.accesed = false;
    var2.mutable = true;

    struct SymVariable var3;
    var3.name = "c";
    var3.type = dTypeF64;
    var3.accesed = false;
    var3.mutable = true;

    struct SymVariable var4;
    var4.name = "d";
    var4.type = dTypeF64;
    var4.accesed = false;
    var4.mutable = true;

    struct SymVariable varNotMutable;
    varNotMutable.name = "notMutableVar";
    varNotMutable.type = dTypeI32;
    varNotMutable.accesed = false;
    varNotMutable.mutable = false;

    TestInstancePtr testInstance = initTestInstance("AST Tree Creation");

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

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode, &value2);

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

    valueNode = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode, &value3);

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

    // test 23 AST for declare node
    ASTNodePtr declareNode = ASTcreateNode(AST_NODE_DECLARE);
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
    printf("err code -> %d\n", err);
    err = ASTeditDeclareNode(declareNode, NULL, valueNode);
    printf("err code -> %d\n", err);
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

    // test 39 AST for assign node
    // we have the declaration pointer form the symtable
    ASTNodePtr declareNode2 = ASTcreateNode(AST_NODE_DECLARE);
    err = ASTeditDeclareNode(declareNode2, &var1, NULL);

    ASTNodePtr assignNode = ASTcreateNode(AST_NODE_ASSIGN);

    err = ASTeditAssignNode(assignNode, declareNode2, NULL);

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

    ASTfreeNode(&declareNode2);

    // test 47 AST for assign node, imutable variable
    declareNode2 = ASTcreateNode(AST_NODE_DECLARE);
    err = ASTeditDeclareNode(declareNode2, &varNotMutable, NULL);

    assignNode = ASTcreateNode(AST_NODE_ASSIGN);
    err = ASTeditAssignNode(assignNode, declareNode2, NULL);

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

    ASTfreeNode(&declareNode2);


    // test 49 AST for function node, adding name
    ASTNodePtr functionNode = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(functionNode, "func", dTypeUndefined, NULL); // dont know the type yet, so pass in undefined

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
    ASTNodePtr funcParam1 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(funcParam1, &var1);

    err = ASTeditFunctionNode(functionNode, NULL, dTypeUndefined, funcParam1);

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
    ASTNodePtr funcParam2 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(funcParam2, &var2);

    err = ASTeditFunctionNode(functionNode, NULL, dTypeUndefined, funcParam2);

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
    err = ASTeditFunctionNode(functionNode, NULL, dTypeI32, NULL);

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

    // test 61 free the function node
    testCase(
        testInstance,
        ASTfreeNode(&functionNode),
        "ASTfreeNode",
        "Function node freed successfully",
        "Failed to free function node"
    );

    // test 62 AST for function call node
    ASTNodePtr functionCallNode = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(functionCallNode, "func", NULL);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode",
        "Function call node initialized successfully",
        "Failed to initialize function call node"
    );

    // test 63 checking the function call name
    testCase(
        testInstance,
        strcmp(functionCallNode->data->functionCall->functionName, "func") == 0,
        "Function call node name check",
        "Function call node name is correct",
        "Function call node name is incorrect"
    );

    // test 64 adding arguments to the function call node
    ASTNodePtr funcCallArg1 = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(funcCallArg1, &value1);

    err = ASTeditFunctionCallNode(functionCallNode, NULL, funcCallArg1);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode - adding arguments",
        "Function call node arguments added successfully",
        "Failed to add arguments to function call node"
    );

    // test 65 checking the function call argument amount
    arguments = functionCallNode->data->functionCall->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 1,
        "Function call node argument amount check",
        "Function call node argument amount is correct",
        "Function call node argument amount is incorrect"
    );

    // test 66 checking the function call argument type
    argument = (ASTNodePtr)getDataAtIndex(arguments, 0);

    testCase(
        testInstance,
        argument->data->value->type == dTypeI32,
        "Function call node argument type check",
        "Function call node argument type is correct",
        "Function call node argument type is incorrect"
    );

    // test 67 checking the function call argument value
    testCase(
        testInstance,
        strcmp(argument->data->value->value, "6") == 0,
        "Function call node argument value check",
        "Function call node argument value is correct",
        "Function call node argument value is incorrect"
    );

    // test 68 adding a 2nd argument to the function call node
    ASTNodePtr funcCallArg2 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(funcCallArg2, &var2);

    err = ASTeditFunctionCallNode(functionCallNode, NULL, funcCallArg2);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditFunctionCallNode - adding 2nd argument",
        "Function call node 2nd argument added successfully",
        "Failed to add 2nd argument to function call node"
    );

    // test 69 checking the function call argument amount
    arguments = functionCallNode->data->functionCall->arguments;

    testCase(
        testInstance,
        getSize(arguments) == 2,
        "Function call node argument amount check",
        "Function call node argument amount is correct",
        "Function call node argument amount is incorrect"
    );

    // test 70 checking the function call argument name
    argument = (ASTNodePtr)getDataAtIndex(arguments, 1);

    testCase(
        testInstance,
        strcmp(argument->data->variable->name, "b") == 0,
        "Function call node argument name check",
        "Function call node argument name is correct",
        "Function call node argument name is incorrect"
    );

    // test 71 checking the function call argument type
    testCase(
        testInstance,
        argument->data->variable->type == dTypeI32,
        "Function call node argument type check",
        "Function call node argument type is correct",
        "Function call node argument type is incorrect"
    );

    // test 72 AST for expresion node a+b*6
    ASTNodePtr expresionNode = ASTcreateNode(AST_NODE_EXPRESION);

    ASTNodePtr operandNode1 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(operandNode1, &var1);

    err = ASTaddNodeToExpresion(expresionNode, operandNode1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 1 added to expresion node successfully",
        "Failed to add operand node 1 to expresion node"
    );

    // test 73 creating the + operand
    ASTNodePtr operand1 = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(operand1, opr1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeOperand",
        "Operand node 1 initialized successfully",
        "Failed to initialize operand node 1"
    );

    // test 74 adding the + operand to the expresion node
    err = ASTaddNodeToExpresion(expresionNode, operand1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 1 added to expresion node successfully",
        "Failed to add operand node 1 to expresion node"
    );

    // test 75 adding the b variable to the expresion node
    ASTNodePtr operandNode2 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(operandNode2, &var2);

    err = ASTaddNodeToExpresion(expresionNode, operandNode2);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 2 added to expresion node successfully",
        "Failed to add operand node 2 to expresion node"
    );

    // test 76 creating the * operand
    ASTNodePtr operand2 = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(operand2, opr2);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTinitNodeOperand",
        "Operand node 2 initialized successfully",
        "Failed to initialize operand node 2"
    );

    // test 77 adding the * operand to the expresion node
    err = ASTaddNodeToExpresion(expresionNode, operand2);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 2 added to expresion node successfully",
        "Failed to add operand node 2 to expresion node"
    );

    // test 78 adding the 6 value to the expresion node
    ASTNodePtr operandNode3 = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(operandNode3, &value1);

    err = ASTaddNodeToExpresion(expresionNode, operandNode3);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToExpresion",
        "Operand node 3 added to expresion node successfully",
        "Failed to add operand node 3 to expresion node"
    );

    // test 79 finish the expresion node
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

    // test 80 check the size of the output
    testCase(
        testInstance,
        getSize(expresionOut) == 5,
        "Expresion node output size check",
        "Expresion node output size is correct",
        "Expresion node output size is incorrect"
    );

    // test 81 check the first element
    ASTNodePtr outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 0);
    printList(expresionOut, (void (*)(unsigned int, void *))print_token);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion node output first element check",
        "Expresion node output first element is correct",
        "Expresion node output first element is incorrect"
    );

    // test 82 check the value of the first element
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Expresion node output first element value check",
        "Expresion node output first element value is correct",
        "Expresion node output first element value is incorrect"
    );

    // test 83 check the second element
    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 1);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Expresion node output second element check",
        "Expresion node output second element is correct",
        "Expresion node output second element is incorrect"
    );

    // test 84 check the value of the second element

    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "Expresion node output second element value check",
        "Expresion node output second element value is correct",
        "Expresion node output second element value is incorrect"
    );

    // test 85 check the third element

    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 2);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VALUE,
        "Expresion node output third element check",
        "Expresion node output third element is correct",
        "Expresion node output third element is incorrect"
    );

    // test 86 check the value of the third element

    testCase(
        testInstance,
        strcmp(outputNode->data->value->value, "6") == 0,
        "Expresion node output third element value check",
        "Expresion node output third element value is correct",
        "Expresion node output third element value is incorrect"
    );

    // test 87 check the fourth element

    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 3);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_OPERAND,
        "Expresion node output fourth element check",
        "Expresion node output fourth element is correct",
        "Expresion node output fourth element is incorrect"
    );

    // test 88 check the value of the fourth element

    testCase(
        testInstance,
        outputNode->data->operand->type == opr2.type,
        "Expresion node output fourth element value check",
        "Expresion node output fourth element value is correct",
        "Expresion node output fourth element value is incorrect"
    );

    // test 89 check the fifth element

    outputNode = (ASTNodePtr)getDataAtIndex(expresionOut, 4);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_OPERAND,
        "Expresion node output fifth element check",
        "Expresion node output fifth element is correct",
        "Expresion node output fifth element is incorrect"
    );

    // test 90 check the value of the fifth element

    testCase(
        testInstance,
        outputNode->data->operand->type == opr1.type,
        "Expresion node output fifth element value check",
        "Expresion node output fifth element value is correct",
        "Expresion node output fifth element value is incorrect"
    );

    // test 91 free the expresion node
    testCase(
        testInstance,
        ASTfreeNode(&expresionNode),
        "ASTfreeNode",
        "Expresion node freed",
        "Failed to free expresion node"
    );

    // test 92 AST for truth expresion node a>b
    ASTNodePtr truthExpresionNode = ASTcreateNode(AST_NODE_TRUTH_EXPRESION);

    operandNode1 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(operandNode1, &var1);

    err = ASTeditTruthExpresion(truthExpresionNode, operandNode1);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 1 added to truth expresion node successfully",
        "Failed to add operand node 1 to truth expresion node"
    );

    // test 93 test the operand in the truth expresion
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->operator == TOKEN_NONE,
        "Truth expresion node operator 1 check",
        "Truth expresion node operator 1 is correct",
        "Truth expresion node operator 1 is incorrect"
    );

    // test 94 test the left part of the truth expresion in the truth expresion (type)
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->left->type == AST_NODE_EXPRESION,
        "Truth expresion node left part type 1 check",
        "Truth expresion node left part type 1 is correct",
        "Truth expresion node left part type 1 is incorrect"
    );

    // test 95 test the left inner node type

    outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node left part value 1 check",
        "Truth expresion node left part value 1 is correct",
        "Truth expresion node left part value 1 is incorrect"
    );

    // test 96 test left inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Truth expresion node left part value 1 check",
        "Truth expresion node left part value 1 is correct",
        "Truth expresion node left part value 1 is incorrect"
    );

    ASTNodePtr opr = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(opr, opr1);

    // test 97 adding the + operand to the truth expresion (should fail)
    err = ASTeditTruthExpresion(truthExpresionNode, opr);
    testCase(
        testInstance,
        err == E_SYNTAX,
        "ASTaddNodeToTruthExpresion",
        "Operand node 1 added to truth expresion node successfully",
        "Failed to add operand node 1 to truth expresion node"
    );

    ASTfreeNode(&opr);

    // test 98 adding the > operand to the truth expresion
    opr = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(opr, opr3);

    err = ASTeditTruthExpresion(truthExpresionNode, opr);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 2 added to truth expresion node successfully",
        "Failed to add operand node 2 to truth expresion node"
    );

    // test 99 test the operand in the truth expresion
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "Truth expresion node operator 2 check",
        "Truth expresion node operator 2 is correct",
        "Truth expresion node operator 2 is incorrect"
    );

    // test 100 adding the b variable to the truth expresion
    operandNode2 = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(operandNode2, &var2);

    err = ASTeditTruthExpresion(truthExpresionNode, operandNode2);
    testCase(
        testInstance,
        err == SUCCESS,
        "ASTaddNodeToTruthExpresion",
        "Operand node 2 added to truth expresion node successfully",
        "Failed to add operand node 2 to truth expresion node"
    );

    // test 101 test the right part of the truth expresion in the truth expresion (type)
    testCase(
        testInstance,
        truthExpresionNode->data->truthExpresion->right->type == AST_NODE_EXPRESION,
        "Truth expresion node right part type 2 check",
        "Truth expresion node right part type 2 is correct",
        "Truth expresion node right part type 2 is incorrect"
    );

    // test 102 test the right inner node type
    outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->right->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node right part value 2 check",
        "Truth expresion node right part value 2 is correct",
        "Truth expresion node right part value 2 is incorrect"
    );

    // test 103 test right inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "Truth expresion node right part value 2 check",
        "Truth expresion node right part value 2 is correct",
        "Truth expresion node right part value 2 is incorrect"
    );

    // test 104 if the switch did not change the left side
    outputNode = (ASTNodePtr)getDataAtIndex(truthExpresionNode->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "Truth expresion node left part value 2 check",
        "Truth expresion node left part value 2 is correct",
        "Truth expresion node left part value 2 is incorrect"
    );

    // test 105 test left inner node value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "Truth expresion node left part value 2 check",
        "Truth expresion node left part value 2 is correct",
        "Truth expresion node left part value 2 is incorrect"
    );

    // test 106 free the truth expresion node
    testCase(
        testInstance,
        ASTfreeNode(&truthExpresionNode),
        "ASTfreeNode",
        "Truth expresion node freed",
        "Failed to free truth expresion node"
    );

    // test 107 AST for if node for if (a > b)
    ASTNodePtr ifNode = ASTcreateNode(AST_NODE_IF_ELSE);

    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &var1);

    err = ASTeditIfNode(ifNode, variableNode);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditIfNode",
        "If node initialized successfully",
        "Failed to initialize if node"
    );

    // test 108 test the if node condition

    testCase(
        testInstance,
        ifNode->data->ifElse->condition->type == AST_NODE_TRUTH_EXPRESION,
        "If node condition type check",
        "If node condition type is correct",
        "If node condition type is incorrect"
    );

    // test 109 test the if node condition value
    outputNode = (ASTNodePtr)getDataAtIndex(ifNode->data->ifElse->condition->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 110 test the if node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 111 test the if node condition value type (a)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "If node condition value type check",
        "If node condition value type is correct",
        "If node condition value type is incorrect"
    );

    // test 112 test adding the > operand to the if node condition
    opr = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(opr, opr3);

    err = ASTeditIfNode(ifNode, opr);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditIfNode",
        "Operand added to if node condition successfully",
        "Failed to add operand to if node condition"
    );

    // test 113 test the if node condition value (it should be auto negated)
    testCase(
        testInstance,
        ifNode->data->ifElse->condition->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "If node condition value check",
        "If node condition value is correct",
        "If node condition value is incorrect"
    );

    // test 114 test adding variable b to the if node condition
    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &var2);

    err = ASTeditIfNode(ifNode, variableNode);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditIfNode",
        "Variable added to if node condition successfully",
        "Failed to add variable to if node condition"
    );

    // test 115 test the if node condition value

    outputNode = (ASTNodePtr)getDataAtIndex(ifNode->data->ifElse->condition->data->truthExpresion->right->data->expresion->output, 0);

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
        ASTfreeNode(&ifNode),
        "ASTfreeNode",
        "If node freed",
        "Failed to free if node"
    );

    // test 119 AST for while node for while (a > b)
    ASTNodePtr whileNode = ASTcreateNode(AST_NODE_WHILE);

    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &var1);

    err = ASTeditWhileNode(whileNode, variableNode);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "While node initialized successfully",
        "Failed to initialize while node"
    );

    // test 120 test the while node condition type
    testCase(
        testInstance,
        whileNode->data->whileLoop->condition->type == AST_NODE_TRUTH_EXPRESION,
        "While node condition type check",
        "While node condition type is correct",
        "While node condition type is incorrect"
    );

    // test 121 test the while node condition value
    outputNode = (ASTNodePtr)getDataAtIndex(whileNode->data->whileLoop->condition->data->truthExpresion->left->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 122 test the while node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "a") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 123 test the while node condition value type (a)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "While node condition value type check",
        "While node condition value type is correct",
        "While node condition value type is incorrect"
    );

    // test 124 test adding the > operand to the while node condition
    opr = ASTcreateNode(AST_NODE_OPERAND);
    err = ASTinitNodeOperand(opr, opr3);

    err = ASTeditWhileNode(whileNode, opr);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "Operand added to while node condition successfully",
        "Failed to add operand to while node condition"
    );

    // test 125 test the while node condition value (it should be auto negated)
    testCase(
        testInstance,
        whileNode->data->whileLoop->condition->data->truthExpresion->operator == TOKEN_LESSOREQUAL,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 126 test adding variable b to the while node condition
    variableNode = ASTcreateNode(AST_NODE_VARIABLE);
    err = ASTinitNodeVariable(variableNode, &var2);

    err = ASTeditWhileNode(whileNode, variableNode);

    testCase(
        testInstance,
        err == SUCCESS,
        "ASTeditWhileNode",
        "Variable added to while node condition successfully",
        "Failed to add variable to while node condition"
    );

    // test 127 test the while node condition value
    outputNode = (ASTNodePtr)getDataAtIndex(whileNode->data->whileLoop->condition->data->truthExpresion->right->data->expresion->output, 0);

    testCase(
        testInstance,
        outputNode->type == AST_NODE_VARIABLE,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 128 test the while node condition value
    testCase(
        testInstance,
        strcmp(outputNode->data->variable->name, "b") == 0,
        "While node condition value check",
        "While node condition value is correct",
        "While node condition value is incorrect"
    );

    // test 129 test if the nod econdition value type (b)
    testCase(
        testInstance,
        outputNode->data->variable->type == dTypeI32,
        "While node condition value type check",
        "While node condition value type is correct",
        "While node condition value type is incorrect"
    );

    // test 120 free the while node
    testCase(
        testInstance,
        ASTfreeNode(&whileNode),
        "ASTfreeNode",
        "While node freed",
        "Failed to free while node"
    );

    finishTestInstance(testInstance);

}

int main(void) {
    test_case_1();
    test_case_2();
    return 0;
}  // end of tests
