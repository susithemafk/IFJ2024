/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  
 * @author <xsvobov00> Veronika Svobodová
 * @file precedent2.c
 * @date 17.11. 2024
 * @brief test file for the precedent analysis
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "syntaxical/precedent.h"
#include "semantical/symtable.h"
#include "syntaxical/parser.h"

// ####################### TEST CASES #######################


int main(void) {
    TestInstancePtr test;
    SymTable *table = symTableInit();

    FILE *input = fopen("./test_inputs/expressions/expressions.txt", "r");

    test = initTestInstance("Precedent analysis test for expressions");

    enum ERR_CODES err = firstPass(input, table->tokenBuffer);
    testCase(
        test,
        err == SUCCESS,
        "validating first pass",
        "First pass passed (expected)",
        "First pass failed (unexpected)"
    );

    fclose(input);

    LinkedList *buffer = table->tokenBuffer;

    /*
    here we need to go throu all the toens, first we should find 0|1 folowed by :, telling is the exp is valid or invalid
    than there will be an expression, that we need, after which we will have token identifier, which can be Expresion. or truthExpresion, after which the expresion or truth expresion will start
    */
    unsigned int size = getSize(buffer);
    for (unsigned int i = 0; i < size; i++) {

        // get the first token, shold be the number of the test
        TOKEN_PTR token = (TOKEN_PTR)getDataAtIndex(buffer, i++);

        if (token->type == TOKEN_EOF) break;
        if (token->type != TOKEN_INTEGER_LITERAL) {
            printf("token value: %s\n", token->value);
            printf("invalid test!\n");
            return 1;
        } // invalid token
        int expCount = atoi(token->value);

        token = (TOKEN_PTR)getDataAtIndex(buffer, i++); // get the :
        token = (TOKEN_PTR)getDataAtIndex(buffer, i++); // get the :

        bool result_valid;
        if (strcmp(token->value, "0") == 0) {
            printf("result should be invalid\n");
            result_valid = false;
        } else if (strcmp(token->value, "1") == 0) {
            printf("result should be valid\n");
            result_valid = true;
        } else return 1; // invalid token .. 

        // get the :
        token = (TOKEN_PTR)getDataAtIndex(buffer, i++);
        if (strcmp(token->value, ":") != 0) return 1; // invalid token

        // get the expresion type
        token = (TOKEN_PTR)getDataAtIndex(buffer, i++);
        bool doExpresion;
        if (strcmp(token->value, "Expresion") == 0) doExpresion = true;
        else doExpresion = false;

        // here the current i should be the start of the expresion
        enum ERR_CODES err = startPrecedentAnalysis(buffer, &i, doExpresion, NULL);
        char message[100];
        DEBUG_PRINT("error: %d", err);
        sprintf(message, "Validating %s %d", (doExpresion) ? "Expresion" : "Truth Expresion", expCount);

        if (result_valid) {
            testCase(
                test,
                err == SUCCESS,
                message,
                "Precedent analysis passed (expected)",
                "Precedent analysis failed (unexpected)"
            );
        } else {
            testCase(
                test,
                (err == E_SYNTAX) || (err == E_SEMANTIC_INCOMPATABLE_TYPES),
                message,
                "Precedent analysis failed (expected)",
                "Precedent analysis passed (unexpected)"
            );
        }

        // check the token at the end, if it is ; or )
        if (result_valid) {
            token = (TOKEN_PTR)getDataAtIndex(buffer, i++);
            if (doExpresion) {
                testCase(
                    test,
                    token->type == TOKEN_SEMICOLON,
                    "Validating end of expresion",
                    "End of expresion is correct (expected)",
                    "End of expresion is incorrect (unexpected)"
                );

            } else {
                testCase(
                    test,
                    token->type == TOKEN_RPAR,
                    "Validating end of truth expresion",
                    "End of truth expresion is correct (expected)",
                    "End of truth expresion is incorrect (unexpected)"
                );
            }
        } 
      
        // skip the test
        while (i < size) {
            token = (TOKEN_PTR)getDataAtIndex(buffer, i);
            if (token->type != TOKEN_IDENTIFIER) {
                i++;
                continue;
            }
            if (strcmp(token->value, "END") == 0) {
                break;
            }
            i++;
        }
    }
    symTableFree(&table);
    finishTestInstance(test);
}
