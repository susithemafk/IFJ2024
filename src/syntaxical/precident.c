/** AUTHOR
 * 
 * @author  <247581> Martin Mendl   <xsvobov00> Veronika Svobodová
 * @file precident.c
 * @date 17.11. 2024
 * @brief main src file for the precedent analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "syntaxical/precident.h"


// Precedent table
const char precedentTable[7][8] = {
    "><<><>>",   
    ">><><>>", 
    "<<<=<< ", 
    ">> > >>", 
    ">> > >>", 
    "<<<><>>", 
    "<<< << "
};

// Function to get the conrsponding index for the precedent table
int _getPrecedentIndex(enum TOKEN_TYPE token) {

    switch (token) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 0;
        case TOKEN_DIVIDE:
        case TOKEN_MULTIPLY:
            return 1;
        case TOKEN_LPAR:
            return 2;
        case TOKEN_RPAR:
            return 3;
        case TOKEN_IDENTIFIER:
        case TOKEN_I32:
        case TOKEN_F64:
            return 4;
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:
        case TOKEN_LESSTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
            return 5;
        case TOKEN_NONE:
            return 6;
        default:
            return -1;
    }

}

// Function to get the result of the precednetal analysis
enum PRECEDENT_RULE _getPrecedentRule(enum TOKEN_TYPE stack, enum TOKEN_TYPE input) {

    int stackIdx = _getPrecedentIndex(stack);
    int inputIdx = _getPrecedentIndex(input);

    if (stackIdx == -1 || inputIdx == -1) return ERROR;

    char rule = precedentTable[stackIdx][inputIdx];

    switch (rule) {
        case '>':
            return GREATER;
        case '<':
            return LESS;
        case '=':
            return EQUAL;
        default:
            return ERROR;
    }
}

// Function to find the curretn token type
enum TOKEN_TYPE _findActiveElement(LinkedList *stack, int *index) {

    unsigned int size = getSize(stack);
    while (size > 0) {
        StackItemPtr rule = (StackItemPtr)getDataAtIndex(stack, size-1);
        size--;
        if (rule->type != STACK_ITEM_TOKEN) continue;
        *index = size;
        return rule->content.operation;
    }

    return -1;
}

// Function to find the index of the < form the right of the stack, returns the index of element right after <
int _findStartIndexForRedux(LinkedList *stack) {

    unsigned int size = getSize(stack);

    while (size > 0) {
        StackItemPtr rule = (StackItemPtr)getDataAtIndex(stack, size-1);
        size--;
        if (rule->type != STACK_ITEM_OPERATION) continue;
        if (rule->content.rule == LESS) return (int)size+1;
    }

    return -1;
}

// Function to create a stack item
StackItemPtr __createStackItem(enum StackItemType type, int enumType) {

    StackItemPtr item = (StackItemPtr)malloc(sizeof(struct StackItem));
    if (item == NULL) return NULL;

    item->type = type;
    switch(type) {
        case STACK_ITEM_TOKEN:
            item->content.operation = (enum TOKEN_TYPE)enumType;
            break;
        case STACK_ITEM_OPERATION:
            item->content.rule = (enum PRECEDENT_RULE)enumType;
            break;
        case STACK_ITEM_EXPRESSION:
            item->content.stateExpression = (enum STATE_EXPRESSION)enumType;
            break;
        case STACK_ITEM_TRUTH_EXPRESSION:
            item->content.stateTruthExpression = (enum STATE_TRUTH_EXPRESSION)enumType;
            break;
        default:
            #ifdef DEBUG
            DEBUG_MSG("Invalid stack item type");
            printf("type: %d\n", type);
            #endif
            free(item);
            return NULL;
    }
    return item;
}

// Function to print the stack
void printStack(LinkedList *stack) {
    printf("Stack: [\n");
    unsigned int size = getSize(stack);
    for (unsigned int i = 0; i < size; i++) {
        StackItemPtr item = (StackItemPtr)getDataAtIndex(stack, i);
        switch(item->type) {
            case STACK_ITEM_TOKEN:
                if (item->content.operation == TOKEN_NONE) {
                    printf(" %u: %s$%s\n", i, COLOR_WARN, COLOR_RESET);
                    break;  
                }
                printf(" %u: ", i);
                printTokenType(item->content.operation);
                break;
            case STACK_ITEM_OPERATION:
                printf(" %u: ", i);
                switch (item->content.rule) {
                    case GREATER:
                        printf("%s>%s\n", COLOR_FUNC, COLOR_RESET);
                        break;
                    case LESS:
                        printf("%s<%s\n", COLOR_FUNC, COLOR_RESET);
                        break;
                    case EQUAL:
                        printf("%s=%s\n", COLOR_FUNC, COLOR_RESET);
                        break;
                    case ERROR:
                        printf("%sERROR%s\n", COLOR_FUNC, COLOR_RESET);
                        break;
                }
                break;
            case STACK_ITEM_EXPRESSION:
                printf(" %u: ", i);
                switch (item->content.stateExpression) {
                    case STATE_EX_E:
                        printf("%sE%s\n", COLOR_PASS, COLOR_RESET);
                        break;
                }
                break;
            case STACK_ITEM_TRUTH_EXPRESSION:
                printf(" %u: ", i);
                switch (item->content.stateTruthExpression) {
                    case STATE_TEX_E:
                        printf("%sE%s\n", COLOR_PASS, COLOR_RESET);
                        break;
                    case STATE_TEX_R:
                        printf("%sR%s\n", COLOR_PASS, COLOR_RESET);
                        break;
                }
                break;
        }
    }
    printf("]\n");
}

// Function to start the precedent analysis (doExpresion is true, handeling expression, false -> truth expression)
enum ERR_CODES startPrecedentAnalysis(LinkedList *buffer, unsigned int *startIdx, bool doExpresion) {

    // internal err check
    if (buffer == NULL) return E_INTERNAL;

    // create the stack
    LinkedList *stack = initLinkedList(true);
    if (stack == NULL) return E_INTERNAL;

    // insert token NONE as the first element
    StackItemPtr none = __createStackItem(STACK_ITEM_TOKEN, (int)TOKEN_NONE);
    struct TOKEN endOfStack = {.value="", .type=TOKEN_NONE};

    if (!insertNodeAtIndex(stack, (void *)none, -1)) {
        #ifdef DEBUG
        DEBUG_MSG("Failed to insert the first element to the stack");
        #endif
        free(none);
        removeList(&stack);
        return E_INTERNAL;
    }

    #ifdef DEBUG
    DEBUG_MSG("Starting the precedent analysis");
    #endif

    int count = 1;
    bool end = false;
    bool skipEndCheck = false;
    TOKEN_PTR token;

    // go through the buffer
    while (1) {

        if (!end) token = (TOKEN_PTR)getDataAtIndex(buffer, *startIdx);
        if (token == NULL) break;

        #ifdef DEBUG
        DEBUG_MSG("Cycle start\n");
        printStack(stack);
        #endif

        // end detection check
        if (doExpresion) {
            // for epxressions
            if (token->type == TOKEN_SEMICOLON) {
                token = &endOfStack;
                end = true;
            } 
        } else if (!skipEndCheck) {// for truth expresions
            if (token->type == TOKEN_LPAR) count++;
            if (token->type == TOKEN_RPAR) count--;
            if (count == 0) {
                end = true;
                token = &endOfStack;
            } 
        } else {
            skipEndCheck = false;
        }

        // get active element, and rule
        int index = -1;
        enum TOKEN_TYPE stackItem = _findActiveElement(stack, &index);
        enum PRECEDENT_RULE rule = _getPrecedentRule(stackItem, token->type);

        #ifdef DEBUG
        DEBUG_MSG("Rule checking reuslt:");
        printf("- stack item: ");
        printTokenType(stackItem);
        printf("- input item: ");
        printTokenType(token->type);
        switch(rule) {
            case GREATER:
                printf("- rule: >\n");
                break;
            case LESS:
                printf("- rule: <\n");
                break;
            case EQUAL:
                printf("- rule: =\n");
                break;
            case ERROR:
                printf("- rule: ERROR\n");
                break;
        }
        #endif

        if (rule == ERROR || index == -1) { // thie -1 problebly does not need to be here, but just to make sure
            removeList(&stack);
            return E_SYNTAX;
        }

        // if we have a <, we need to reduce, move the < after the active element
        if (rule == LESS || rule == EQUAL) {

            // shifting the < after the active element
            if (rule == LESS) {
               
                StackItemPtr less = __createStackItem(STACK_ITEM_OPERATION, (int)LESS);

                if (!insertNodeAtIndex(stack, (void *)less, (getSize(stack)-1 == (unsigned int)index) ? -1 : index+1))  {
                    free(less);
                    removeList(&stack);
                    return E_INTERNAL;
                }

                #ifdef DEBUG
                DEBUG_MSG("Shifting the < after the active element");
                printStack(stack);
                #endif

            }

            if (!end) (*startIdx)++;
            if (token->type == TOKEN_NONE) continue;
            
            // adding the token to the stack
            StackItemPtr newToken = __createStackItem(STACK_ITEM_TOKEN, (int)token->type);

            if (newToken == NULL) {
                removeList(&stack);
                return E_INTERNAL;
            }

            if (!insertNodeAtIndex(stack, (void *)newToken, -1)) {
                free(newToken);
                removeList(&stack);
                return E_INTERNAL;
            }
            #ifdef DEBUG
            DEBUG_MSG("Inserted the token to the stack");
            printStack(stack);
            #endif
            continue;   
        }

        // if we have a >, we need to do the redux
        if (rule == GREATER) {

            int ruleStartIdx = _findStartIndexForRedux(stack);

            #ifdef DEBUG
            DEBUG_MSG("Doing the redux");
            #endif

            if (ruleStartIdx == -1) {
                removeList(&stack);
                return E_SYNTAX;
            }

            // apply the rules
            enum ERR_CODES err; 
            if (doExpresion) {
                err = _applyRulesExpresion(stack, (unsigned int)ruleStartIdx);
            } else {
                err = _applyRulesTruthExpression(stack, (unsigned int)ruleStartIdx);
            }

            // remove the coresponding < 
            if (!removeNodeAtIndex(stack, ruleStartIdx-1)) {
                removeList(&stack);
                return E_INTERNAL;
            }

            if (err != SUCCESS) {
                removeList(&stack);
                return err;
            }

            #ifdef DEBUG
            DEBUG_MSG("Redux done");
            printStack(stack);
            #endif

            skipEndCheck = true;
            // end condiion check;
            if (!end) continue;
            if (getSize(stack) != 2) continue;

            StackItemPtr last = (StackItemPtr)getDataAtIndex(stack, 1);
            // correct check for expresion syntax
            if (doExpresion) {
                if (last->content.stateExpression == STATE_EX_E) {
                    removeList(&stack);
                    return SUCCESS;
                }
                continue;
            }

            // correct check for truth expresion syntax
            if (last->content.stateTruthExpression == STATE_TEX_R) {
                removeList(&stack);
                return SUCCESS;
            }
            continue;
        }

        if (rule == ERROR) {
            removeList(&stack);
            return E_INTERNAL;
        }
    }

    // should nnver happen, hopefully :D
    removeList(&stack);
    return E_SYNTAX;
}

// Function ho apply the rules for the expression
enum ERR_CODES _applyRulesExpresion(LinkedList *stack, unsigned int ruleStartIdx) {

    // internal err check
    if (stack == NULL) return E_INTERNAL;

    StackItemPtr item1 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx);
    StackItemPtr item2 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx+1);
    StackItemPtr item3 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx+2);

    // E -> E operand E
    if (
        item1 != NULL && item1->type == STACK_ITEM_EXPRESSION && item1->content.stateExpression == STATE_EX_E &&
        item2 != NULL && item2->type == STACK_ITEM_TOKEN &&
        item3 != NULL && item3->type == STACK_ITEM_EXPRESSION && item3->content.stateExpression == STATE_EX_E
    ) { 
        #if DEBUG
        DEBUG_MSG("E -> E operand E");
        #endif
        // we have to check if the operand is + - * /
        if (
            item2->content.operation != TOKEN_PLUS &&
            item2->content.operation != TOKEN_MINUS &&
            item2->content.operation != TOKEN_MULTIPLY &&
            item2->content.operation != TOKEN_DIVIDE
        ) return E_SYNTAX; // invalid operand
        
        if (!removeNodeAtIndex(stack, ruleStartIdx+1)) return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx+1)) return E_INTERNAL;
        return SUCCESS;
    }

    // E -> (E)
    if (
        item1 != NULL && item1->type == STACK_ITEM_TOKEN && item1->content.operation == TOKEN_LPAR &&
        item2 != NULL && item2->type == STACK_ITEM_EXPRESSION && item2->content.stateExpression == STATE_EX_E &&
        item3 != NULL && item3->type == STACK_ITEM_TOKEN && item3->content.operation == TOKEN_RPAR
    ) {
        #if DEBUG
        DEBUG_MSG("E -> (E)");
        #endif
        item1->content.stateExpression = STATE_EX_E;
        item1->type = STACK_ITEM_EXPRESSION;
        if (!removeNodeAtIndex(stack, ruleStartIdx+1)) return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx+1)) return E_INTERNAL;
        return SUCCESS;
    }

    // E -> id
    if (
        item1 != NULL && item1->type == STACK_ITEM_TOKEN && (
            item1->content.operation == TOKEN_IDENTIFIER ||
            item1->content.operation == TOKEN_I32 ||
            item1->content.operation == TOKEN_F64
        ) && item2 == NULL && item3 == NULL 
    ) {
        #if DEBUG
        DEBUG_MSG("E -> id");
        #endif
        item1->content.stateExpression = STATE_EX_E;
        item1->type = STACK_ITEM_EXPRESSION;
        return SUCCESS;
    }

   return E_SYNTAX;
}

// Function to apply the rules for the truth expression
enum ERR_CODES _applyRulesTruthExpression(LinkedList *stack, unsigned int ruleStartIdx) {

    // internal err check
    if (stack == NULL) return E_INTERNAL;

    StackItemPtr item1 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx);
    StackItemPtr item2 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx+1);   
    StackItemPtr item3 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx+2);

    // E -> id
    if (
        item1 != NULL && item1->type == STACK_ITEM_TOKEN && (
            item1->content.operation == TOKEN_IDENTIFIER ||
            item1->content.operation == TOKEN_I32 ||
            item1->content.operation == TOKEN_F64
        ) && item2 == NULL && item3 == NULL 
    ) {
        #if DEBUG
        DEBUG_MSG("E -> id");
        #endif
        item1->type = STACK_ITEM_TRUTH_EXPRESSION;
        item1->content.stateTruthExpression = STATE_TEX_E;
        return SUCCESS;
    }

    // E -> (E)
    if (
        item1 != NULL && item1->type == STACK_ITEM_TOKEN && item1->content.operation == TOKEN_LPAR &&
        item2 != NULL && item2->type == STACK_ITEM_TRUTH_EXPRESSION && item2->content.stateTruthExpression == STATE_TEX_E &&
        item3 != NULL && item3->type == STACK_ITEM_TOKEN && item3->content.operation == TOKEN_RPAR
    ) {
        #if DEBUG
        DEBUG_MSG("E -> (E)");
        #endif
        item1->content.stateTruthExpression = STATE_TEX_E;
        item1->type = STACK_ITEM_TRUTH_EXPRESSION;
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        item3->type = STACK_ITEM_TRUTH_EXPRESSION;
        item3->content.stateTruthExpression = STATE_TEX_E;
        return SUCCESS;
    }

    // E -> E operand E
    if (
        item1 != NULL && item1->type == STACK_ITEM_TRUTH_EXPRESSION && item1->content.stateTruthExpression == STATE_TEX_E &&
        item2 != NULL && item2->type == STACK_ITEM_TOKEN && (
            item2->content.operation == TOKEN_PLUS ||
            item2->content.operation == TOKEN_MINUS ||
            item2->content.operation == TOKEN_MULTIPLY ||
            item2->content.operation == TOKEN_DIVIDE
        ) &&
        item3 != NULL && item3->type == STACK_ITEM_TRUTH_EXPRESSION && item3->content.stateTruthExpression == STATE_TEX_E
    ) {
        #if DEBUG
        DEBUG_MSG("E -> E operand E");
        #endif
        // we have to check if the operand is + - * /
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        return SUCCESS;
    }

    // R -> E truth operand E
    if (
        item1 != NULL && item1->type == STACK_ITEM_TRUTH_EXPRESSION && item1->content.stateTruthExpression == STATE_TEX_E &&
        item2 != NULL && item2->type == STACK_ITEM_TOKEN && (
            item2->content.operation == TOKEN_LESSTHAN ||
            item2->content.operation == TOKEN_GREATERTHAN ||
            item2->content.operation == TOKEN_LESSOREQUAL ||
            item2->content.operation == TOKEN_GREATEROREQUAL ||
            item2->content.operation == TOKEN_NOTEQUAL ||
            item2->content.operation == TOKEN_EQUALS
        ) &&
        item3 != NULL && item3->type == STACK_ITEM_TRUTH_EXPRESSION && item3->content.stateTruthExpression == STATE_TEX_E
    ) {
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx)) return E_INTERNAL;
        item3->content.stateTruthExpression = STATE_TEX_R;
        return SUCCESS;
    }

    #ifdef DEBUG
    DEBUG_MSG("Invalid rule");
    #endif

    return E_SYNTAX;
}
