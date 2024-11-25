/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  
 * @author <xsvobov00> Veronika Svobodová
 * @file precident.c
 * @date 17.11. 2024
 * @brief test file for the precedent analysis
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "syntaxical/precident.h"

// ####################### TEST CASES #######################

// First we try to define the variables, that will be used in the tokens

// variables, a, b, c, d, e, f, g
char *_a = "a";
char *_b = "b";
char *_c = "c";
char *_d = "d";
char *_g = "g";

// values for the tokens, 2, 3, 6, 3.14, "ahoj"
char *_two = "2";
char *_three = "3";
char *_six = "6";
char *_pi = "3.14";

// arythmetic tokens
char *_plus = "+";
char *_minus = "-";
char *_multiply = "*";
char *_divide = "/";
char *_lpar = "(";
char *_rpar = ")";

// comparison tokens
char *_equals = "==";
char *_lessthan = "<";
char *_assign = "=";
char *_semicolon = ";";

// TOKENS

void prepareTestEnviroment(LinkedList **buffer, unsigned int *startIdx) {
    *buffer = initLinkedList(false);
    *startIdx = 0;
}

void freeTestEnviroment(LinkedList **buffer, unsigned int *startIdx) {
    removeList(buffer); 
    *startIdx = 0;
}

int main(void) {
    struct TOKEN a = {.value=_a, .type=TOKEN_IDENTIFIER};
    struct TOKEN b = {.value=_b, .type=TOKEN_IDENTIFIER};
    struct TOKEN c = {.value=_c, .type=TOKEN_IDENTIFIER};
    struct TOKEN d = {.value=_d, .type=TOKEN_IDENTIFIER};
    struct TOKEN g = {.value=_g, .type=TOKEN_IDENTIFIER};

    struct TOKEN two = {.value=_two, .type=TOKEN_INTEGER_LITERAL};
    struct TOKEN three = {.value=_three, .type=TOKEN_INTEGER_LITERAL};
    struct TOKEN six = {.value=_six, .type=TOKEN_INTEGER_LITERAL};
    struct TOKEN pi = {.value=_pi, .type=TOKEN_FLOAT_LITERAL};

    struct TOKEN plus = {.value=_plus, .type=TOKEN_PLUS};
    struct TOKEN minus = {.value=_minus, .type=TOKEN_MINUS};
    struct TOKEN multiply = {.value=_multiply, .type=TOKEN_MULTIPLY};
    struct TOKEN divide = {.value=_divide, .type=TOKEN_DIVIDE};
    struct TOKEN lpar = {.value=_lpar, .type=TOKEN_LPAR};
    struct TOKEN rpar = {.value=_rpar, .type=TOKEN_RPAR};

    struct TOKEN equals = {.value=_equals, .type=TOKEN_EQUALS};
    struct TOKEN lessthan = {.value=_lessthan, .type=TOKEN_LESSTHAN};
    struct TOKEN assign = {.value=_assign, .type=TOKEN_ASSIGN};
    struct TOKEN semicolon = {.value=_semicolon, .type=TOKEN_SEMICOLON};

    LinkedList *buffer;
    enum ERR_CODES err;
    unsigned int startIdx = 0;
    TestInstancePtr test;

    test = initTestInstance("Precedent analysis test for expressions");

    // ####################### TEST CASES #######################

    // test case 1, a;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> a;",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 1,
        "Validating start index -> a;",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 1) printf("Start index: %d\n", startIdx);
    freeTestEnviroment(&buffer, &startIdx);

    // test case 2, a = 2; (shoudl fail)
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&assign, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> a = 2;",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );
    if (err != E_SYNTAX) printErrCode(err);

    freeTestEnviroment(&buffer, &startIdx);

    // test case 3, 2 + a / 3;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> 2 + a / 3;",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );

    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 5,
        "Validating start index -> 2 + a / 3;",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 5) printf("Start index: %d\n", startIdx);
    freeTestEnviroment(&buffer, &startIdx);

    // test case 4, 2 + a / 3 * 6;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&six, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> 2 + a / 3 * 6;",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 7,
        "Validating start index -> 2 + a / 3 * 6;",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 7) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);

    // test case 5, 2*(3+6);
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&six, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> 2*(3+6);",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 7,
        "Validating start index -> 2*(3+6);",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 7) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);

    // test case 6, a * b + (c - d) / 2 *(3 + 6*(pi - 2));
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&b, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&c, -1);
    insertNodeAtIndex(buffer, (void *)&minus, -1);
    insertNodeAtIndex(buffer, (void *)&d, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&six, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&pi, -1);
    insertNodeAtIndex(buffer, (void *)&minus, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> a * b + (c - d) / 2 *(3 + 6*(pi - 2));",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 23,
        "Validating start index -> a * b + (c - d) / 2 *(3 + 6*(pi - 2));",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 23) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);

    // bad expresion, complex, a * + (c -* d) / 2 *(3 + 6*(pi - 2);
    prepareTestEnviroment(&buffer, &startIdx);
    
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&c, -1);
    insertNodeAtIndex(buffer, (void *)&minus, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&d, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&six, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&pi, -1);
    insertNodeAtIndex(buffer, (void *)&minus, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> a * + (c -* d) / 2 *(3 + 6*(pi - 2);",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );

    freeTestEnviroment(&buffer, &startIdx);

    // testing a lot of () ((((((((a))))))));
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating expresion -> ((((((((a))))))));",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 17,
        "Validating start index -> ((((((((a))))))));",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 17) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);

    //testing expressions starting with an operator (multiple)
    //Test case: + + a;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> + + a;",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );
    if (err != E_SYNTAX) printErrCode(err);

    freeTestEnviroment(&buffer, &startIdx);

    //Test case: * * a;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> * * a;",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );
    if (err != E_SYNTAX) printErrCode(err);

    freeTestEnviroment(&buffer, &startIdx);

    //Test case: / / a;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&semicolon, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> / / a;",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );
    if (err != E_SYNTAX) printErrCode(err);

    freeTestEnviroment(&buffer, &startIdx);

    //Test case: < < a)
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&lessthan, -1);
    insertNodeAtIndex(buffer, (void *)&lessthan, -1);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, true, NULL);
    testCase(
        test,
        err == E_SYNTAX,
        "Validating expresion -> < < a)",
        "Precedent analysis failed (expected)",
        "Precedent analysis passed (unexpected)"
    );
    if (err != E_SYNTAX) printErrCode(err);

    freeTestEnviroment(&buffer, &startIdx);

    // ####################### TRUTH EXPRESIONS #######################
    // test case 1, a == 2) 
    // truth expressions will be only found in ifs and whiles, so the end of the epxression will be )
    // if (a == 2) -> a == 2)

    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&equals, -1);
    insertNodeAtIndex(buffer, (void *)&two, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, false, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating truth expresion -> a == 2)",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 3,
        "Validating start index -> a == 2)",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 3) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);

    // test case 2, a + 3 > (6 * g + (pi * b/(c - (d *c))));;
    prepareTestEnviroment(&buffer, &startIdx);
    insertNodeAtIndex(buffer, (void *)&a, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&three, -1);
    insertNodeAtIndex(buffer, (void *)&lessthan, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&six, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&g, -1);
    insertNodeAtIndex(buffer, (void *)&plus, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&pi, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&b, -1);
    insertNodeAtIndex(buffer, (void *)&divide, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&c, -1);
    insertNodeAtIndex(buffer, (void *)&minus, -1);
    insertNodeAtIndex(buffer, (void *)&lpar, -1);
    insertNodeAtIndex(buffer, (void *)&d, -1);
    insertNodeAtIndex(buffer, (void *)&multiply, -1);
    insertNodeAtIndex(buffer, (void *)&c, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);
    insertNodeAtIndex(buffer, (void *)&rpar, -1);

    err = startPrecedentAnalysis(buffer, &startIdx, false, NULL);
    testCase(
        test,
        err == SUCCESS,
        "Validating truth expresion -> a + 3 > (6 * g + (pi * b/(c - (d *c))))$",
        "Precedent analysis passed (expected)",
        "Precedent analysis failed (unexpected)"
    );
    if (err != SUCCESS) printErrCode(err);

    testCase(
        test,
        startIdx == 25,
        "Validating start index -> a + 3 > (6 * g + (pi * b/(c - (d *c))))$",
        "Start index is correct (expected)",
        "Start index is incorrect (unexpected)"
    );
    if (startIdx != 25) printf("Start index: %d\n", startIdx);

    freeTestEnviroment(&buffer, &startIdx);


    // ####################### END OF TEST CASES #######################
    removeList(&buffer);
    finishTestInstance(test);
}
