/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  
 * @author <xsvobov00> Veronika Svobodová
 * @file precident2.c
 * @date 17.11. 2024
 * @brief test file for the precedent analysis
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "syntaxical/precident.h"
#include "semantical/symtable.h"

// ####################### TEST CASES #######################


int main(void) {
    LinkedList *buffer = initLinkedList(false);
    TestInstancePtr test;
    SymTable *table = symTableInit();

    FILE *input = fopen("./test_inputs/expressions/expressions.txt", "r");

    test = initTestInstance("Precedent analysis test for expressions");

    enum ERR_CODES err = firstPass(table, input, buffer);
    testCase(
        test,
        err == E_SEMANTIC_UND_FUNC_OR_VAR,
        "Validating main function",
        "Main function not found (expected)",
        "Main function found (unexpected)"
    );

    fclose(input);

    /*
    here we need to go throu all the toens, first we should find 0|1 folowed by :, telling is the exp is valid or invalid
    than there will be an expression, that we need, after which we will have token identifier, which can be Expresion. or truthExpresion, after which the expresion or truth expresion will start
    */
    unsigned int size = getSize(buffer);
    for (unsigned int i = 0; i < size; i++) {

        // get the first token, shold be the number of the test
        TOKEN_PTR token = (TOKEN_PTR)getDataAtIndex(buffer, i++);

        if (token->type == TOKEN_EOF) break;
        if (token->type != TOKEN_I32) {
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
                err == E_SYNTAX,
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
    freeBuffer(&buffer);
    symTableFree(&table);
    finishTestInstance(test);
}
