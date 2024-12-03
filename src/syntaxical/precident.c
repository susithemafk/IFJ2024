/** AUTHOR
 *
 * @author <247581> Martin Mendl   
 * @author <xsvobov00> Veronika Svobodová
 * @author <253171> Vanesa Zimmermannová
 * @file precident.c
 * @date 17.11. 2024
 * @brief main src file for the precedent analysis
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/precident.h"
#include "utility/my_utils.h"
#else
#include "precident.h"
#include "my_utils.h"
#endif

// Precedent table
const char precedentTable[7][7] = {
    "><<><>>", 
    ">><><>>", 
    "<<<=<< ", 
    ">> > >>",
    ">> > >>", 
    "<<<><>>", 
    "<<< <<$"
};

// function to remove stack
void removeStack(LinkedList **stack) {
    int size = getSize(*stack);

    for (int i = 0; i < size; i++) {
        StackItemPtr item = (StackItemPtr)getDataAtIndex(*stack, i);
        freeExpression(&item->ast_node);
    }
    removeList(stack);
}

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
    case TOKEN_INTEGER_LITERAL:
    case TOKEN_FLOAT_LITERAL:
    case TOKEN_NULL:
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

    if (stackIdx == -1 || inputIdx == -1)
        return ERROR;

    char rule = precedentTable[stackIdx][inputIdx];

    switch (rule) {
    case '>':
        return GREATER;
    case '<':
        return LESS;
    case '=':
        return EQUAL;
    case '$':
        return END;
    default:
        return ERROR;
    }
}

// Function to find the curretn token type
enum TOKEN_TYPE _findActiveElement(LinkedList *stack, int *index) {

    unsigned int size = getSize(stack);
    while (size > 0) {
        StackItemPtr rule = (StackItemPtr)getDataAtIndex(stack, size - 1);
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
        StackItemPtr rule = (StackItemPtr)getDataAtIndex(stack, size - 1);
        size--;
        if (rule->type != STACK_ITEM_OPERATION)
            continue;
        if (rule->content.rule == LESS)
            return (int)size + 1;
    }

    return -1;
}

// Function to create a stack item
StackItemPtr __createStackItem(enum StackItemType type, int enumType) {

    StackItemPtr item = (StackItemPtr)malloc(sizeof(struct StackItem));
    if (item == NULL)
        return NULL;

    memset(item, 0, sizeof(struct StackItem));

    item->type = type;
    switch (type) {
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
        DEBUG_PRINT("Invalid stack item type\ntype: %d\n", type);
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
        switch (item->type) {
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
            case END:
                printf("%sEND%s\n", COLOR_FUNC, COLOR_RESET);
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
enum ERR_CODES startPrecedentAnalysis(LinkedList *buffer, unsigned int *startIdx, bool doExpresion, Expression *expr) {

    // internal err check
    if (buffer == NULL)
        return E_INTERNAL;

    // create the stack
    LinkedList *stack = initLinkedList(true);
    if (stack == NULL)
        return E_INTERNAL;

    // insert token NONE as the first element
    StackItemPtr none = __createStackItem(STACK_ITEM_TOKEN, (int)TOKEN_NONE);
    struct TOKEN endOfStack = {.value = "", .type = TOKEN_NONE};

    if (!insertNodeAtIndex(stack, (void *)none, -1)) {
        DEBUG_PRINT("Failed to insert the first element to the stack\n");
        free(none);
        removeStack(&stack);
        return E_INTERNAL;
    }

    DEBUG_PRINT("Starting the precedent analysis");

    int count = 1;
    bool end = false;
    bool skipEndCheck = false;
    TOKEN_PTR token;

    // go through the buffer
    while (1) {

        if (!end) token = (TOKEN_PTR)getDataAtIndex(buffer, *startIdx);
        if (token == NULL) break;

        DEBUG_PRINT("Cycle start\n");
        DEBUG_STACK(stack);

        // end detection check
        if (doExpresion) {
            // for epxressions
            if (token->type == TOKEN_SEMICOLON) {
                token = &endOfStack;
                end = true;
            }

        // for truth expresions
        } else if (!skipEndCheck) {
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

        DEBUG_TOKEN_RULE(stackItem, token, rule);

        if (rule == ERROR ||
            index == -1) { // thie -1 problebly does not need to be here, but just to make sure
            removeStack(&stack);
            return E_SYNTAX;
        }

        // if we have a <, we need to reduce, move the < after the active element
        if (rule == LESS || rule == EQUAL) {

            // shifting the < after the active element
            if (rule == LESS) {

                StackItemPtr less = __createStackItem(STACK_ITEM_OPERATION, (int)LESS);
                if (!insertNodeAtIndex(stack, (void *)less, (getSize(stack) - 1 == (unsigned int)index) ? -1: index + 1)) {
                    free(less);
                    removeStack(&stack);
                    return E_INTERNAL;
                }

                DEBUG_PRINT("Shifting the < after the active element");
                DEBUG_STACK(stack);

            }

            if (!end) (*startIdx)++;
            if (token->type == TOKEN_NONE) continue;

            // adding the token to the stack
            StackItemPtr newToken = __createStackItem(STACK_ITEM_TOKEN, (int)token->type);

            if (newToken == NULL) {
                removeStack(&stack);
                return E_INTERNAL;
            }

            if (!insertNodeAtIndex(stack, (void *)newToken, -1)) {
                free(newToken);
                removeStack(&stack);
                return E_INTERNAL;
            }

            if (token->type == TOKEN_IDENTIFIER) {
                newToken->ast_node.expr_type = IdentifierExpressionType;
                newToken->ast_node.data.identifier.name = token->value;
                if (newToken->ast_node.data.identifier.name == NULL) {
                    removeStack(&stack);
                    return E_INTERNAL;
                }
            }

            if (isLiteral(token->type)) {
                newToken->ast_node.expr_type = LiteralExpressionType;
                newToken->ast_node.data.literal.value = token->value;
                if (newToken->ast_node.data.literal.value == NULL) {
                    removeStack(&stack);
                    return E_INTERNAL;
                }
                
                newToken->ast_node.data_type.is_nullable = token->type == TOKEN_NULL;
                newToken->ast_node.data_type.data_type = covertTokneDataType(token->type);
                newToken->ast_node.data.literal.data_type = newToken->ast_node.data_type;
            }

            DEBUG_PRINT("Inserted the token to the stack");
            DEBUG_STACK(stack);
            continue;
        }

        // if we have a >, we need to do the redux
        if (rule == GREATER) {

            int ruleStartIdx = _findStartIndexForRedux(stack);

            DEBUG_PRINT("Doing the redux");

            if (ruleStartIdx == -1) {
                removeStack(&stack);
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
            if (!removeNodeAtIndex(stack, ruleStartIdx - 1)) {
                removeStack(&stack);
                return E_INTERNAL;
            }

            if (err != SUCCESS) {
                removeStack(&stack);
                return err;
            }

            DEBUG_PRINT("Redux done");
            DEBUG_STACK(stack);

            skipEndCheck = true; // we need to preform the end cycle again, with the same token,
                                 // after we did the redux
            continue;
        }

        if (rule == END) {
            if (getSize(stack) != 2) {
                removeStack(&stack);
                return E_SYNTAX;
            }

            StackItemPtr last = (StackItemPtr)getDataAtIndex(stack, 1);

            // if we ar solving expression, and the last item is not expression, we have a syntax error
            if (doExpresion && last->type != STACK_ITEM_EXPRESSION) {
                removeStack(&stack);
                return E_SYNTAX;
            }

            // if we are solving truth expression, and the last item is not truth expression (R), we have a syntax error
            if (!doExpresion) {
                DEBUG_PRINT("Last item: %d\n", last->type);
                if (last->type == STACK_ITEM_TRUTH_EXPRESSION) {
                    DEBUG_PRINT("Last inner type: %d\n", last->content.stateTruthExpression);
                    if (last->content.stateTruthExpression == STATE_TEX_E) {
                        DEBUG_PRINT("error\n");
                        removeStack(&stack);
                        return E_SEMANTIC_INCOMPATABLE_TYPES; // handle truth expression, with no R
                    }
                }

                if (last->type != STACK_ITEM_TRUTH_EXPRESSION) {
                    removeStack(&stack);
                    return E_SYNTAX;
                }
            }

            // correct check for expresion syntax
            if (expr) *expr = last->ast_node;
            removeList(&stack);
            return SUCCESS;
        }
    }

    // should never happen, hopefully :D
    removeStack(&stack);
    return E_SYNTAX;
}

// Function ho apply the rules for the expression
enum ERR_CODES _applyRulesExpresion(LinkedList *stack, unsigned int ruleStartIdx) {

    // internal err check
    if (stack == NULL)
        return E_INTERNAL;

    StackItemPtr item1 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx);
    StackItemPtr item2 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx + 1);
    StackItemPtr item3 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx + 2);

    // E -> E operand E
    if (item1 != NULL && item1->type == STACK_ITEM_EXPRESSION &&
        item1->content.stateExpression == STATE_EX_E && item2 != NULL &&
        item2->type == STACK_ITEM_TOKEN && item3 != NULL && item3->type == STACK_ITEM_EXPRESSION &&
        item3->content.stateExpression == STATE_EX_E) {

        DEBUG_PRINT("E -> E operand E");

        // we have to check if the operand is + - * /
        if (item2->content.operation != TOKEN_PLUS && item2->content.operation != TOKEN_MINUS &&
            item2->content.operation != TOKEN_MULTIPLY && item2->content.operation != TOKEN_DIVIDE)
            return E_SYNTAX; // invalid operand

        Expression *left = malloc(sizeof(Expression));
        if (left == NULL)
            return E_INTERNAL;

        Expression *right = malloc(sizeof(Expression));
        if (right == NULL)
            return E_INTERNAL;

        *left = item1->ast_node;
        *right = item3->ast_node;

        item1->ast_node.data.binary_expr.left = left;
        item1->ast_node.data.binary_expr.right = right;
        item1->ast_node.data.binary_expr.operation = item2->content.operation;
        item1->ast_node.expr_type = BinaryExpressionType;

        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        return SUCCESS;
    }

    // E -> (E)
    if (item1 != NULL && item1->type == STACK_ITEM_TOKEN &&
        item1->content.operation == TOKEN_LPAR && item2 != NULL &&
        item2->type == STACK_ITEM_EXPRESSION && item2->content.stateExpression == STATE_EX_E &&
        item3 != NULL && item3->type == STACK_ITEM_TOKEN &&
        item3->content.operation == TOKEN_RPAR) {
        
        DEBUG_PRINT("E -> (E)");

        item1->content.stateExpression = STATE_EX_E;
        item1->type = STACK_ITEM_EXPRESSION;

        item1->ast_node = item2->ast_node;

        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        return SUCCESS;
    }

    // E -> id
    if (item1 != NULL && item1->type == STACK_ITEM_TOKEN &&
        (item1->content.operation == TOKEN_IDENTIFIER || isLiteral(item1->content.operation)) &&
        item2 == NULL && item3 == NULL) {

        DEBUG_PRINT("E -> id");

        item1->content.stateExpression = STATE_EX_E;
        item1->type = STACK_ITEM_EXPRESSION;

        return SUCCESS;
    }

    return E_SYNTAX;
}

// Function to apply the rules for the truth expression
enum ERR_CODES _applyRulesTruthExpression(LinkedList *stack, unsigned int ruleStartIdx) {

    // internal err check
    if (stack == NULL)
        return E_INTERNAL;

    StackItemPtr item1 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx);
    StackItemPtr item2 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx + 1);
    StackItemPtr item3 = (StackItemPtr)getDataAtIndex(stack, ruleStartIdx + 2);

    // E -> id
    if (item1 != NULL && item1->type == STACK_ITEM_TOKEN &&
        (item1->content.operation == TOKEN_IDENTIFIER || isLiteral(item1->content.operation)) &&
        item2 == NULL && item3 == NULL) {

        DEBUG_PRINT("E -> id");

        item1->type = STACK_ITEM_TRUTH_EXPRESSION;
        item1->content.stateTruthExpression = STATE_TEX_E;
        return SUCCESS;
    }

    // E -> (E)
    if (item1 != NULL && item1->type == STACK_ITEM_TOKEN &&
        item1->content.operation == TOKEN_LPAR && item2 != NULL &&
        item2->type == STACK_ITEM_TRUTH_EXPRESSION &&
        item2->content.stateTruthExpression == STATE_TEX_E && item3 != NULL &&
        item3->type == STACK_ITEM_TOKEN && item3->content.operation == TOKEN_RPAR) {

        DEBUG_PRINT("E -> (E)");

        item3->type = STACK_ITEM_TRUTH_EXPRESSION;
        item3->content.stateTruthExpression = STATE_TEX_E;

        item3->ast_node = item2->ast_node;

        if (!removeNodeAtIndex(stack, ruleStartIdx))
            return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx))
            return E_INTERNAL;

        return SUCCESS;
    }

    // E -> E operand E
    if (item1 != NULL && item1->type == STACK_ITEM_TRUTH_EXPRESSION &&
        item1->content.stateTruthExpression == STATE_TEX_E && item2 != NULL &&
        item2->type == STACK_ITEM_TOKEN &&
        (item2->content.operation == TOKEN_PLUS || item2->content.operation == TOKEN_MINUS ||
         item2->content.operation == TOKEN_MULTIPLY || item2->content.operation == TOKEN_DIVIDE) &&
        item3 != NULL && item3->type == STACK_ITEM_TRUTH_EXPRESSION &&
        item3->content.stateTruthExpression == STATE_TEX_E) {

        DEBUG_PRINT("E -> E operand E");
        // we have to check if the operand is + - * /

        Expression *left = malloc(sizeof(Expression));
        if (left == NULL)
            return E_INTERNAL;

        Expression *right = malloc(sizeof(Expression));
        if (right == NULL)
            return E_INTERNAL;

        *left = item1->ast_node;
        *right = item3->ast_node;

        item1->ast_node.data.binary_expr.left = left;
        item1->ast_node.data.binary_expr.right = right;
        item1->ast_node.data.binary_expr.operation = item2->content.operation;
        item1->ast_node.expr_type = BinaryExpressionType;

        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx + 1))
            return E_INTERNAL;
        return SUCCESS;
    }

    // R -> E truth operand E
    if (item1 != NULL && item1->type == STACK_ITEM_TRUTH_EXPRESSION &&
        item1->content.stateTruthExpression == STATE_TEX_E && item2 != NULL &&
        item2->type == STACK_ITEM_TOKEN &&
        (item2->content.operation == TOKEN_LESSTHAN ||
         item2->content.operation == TOKEN_GREATERTHAN ||
         item2->content.operation == TOKEN_LESSOREQUAL ||
         item2->content.operation == TOKEN_GREATEROREQUAL ||
         item2->content.operation == TOKEN_NOTEQUAL || item2->content.operation == TOKEN_EQUALS) &&
        item3 != NULL && item3->type == STACK_ITEM_TRUTH_EXPRESSION &&
        item3->content.stateTruthExpression == STATE_TEX_E) {
        
        DEBUG_PRINT("R -> E truth operand E");

        item3->content.stateTruthExpression = STATE_TEX_R;

        Expression *left = malloc(sizeof(Expression));
        if (left == NULL)
            return E_INTERNAL;

        Expression *right = malloc(sizeof(Expression));
        if (right == NULL)
            return E_INTERNAL;

        *left = item1->ast_node;
        *right = item3->ast_node;

        item3->ast_node.data.binary_expr.left = left;
        item3->ast_node.data.binary_expr.right = right;
        item3->ast_node.data.binary_expr.operation = item2->content.operation;
        item3->ast_node.expr_type = BinaryExpressionType;

        if (!removeNodeAtIndex(stack, ruleStartIdx))
            return E_INTERNAL;
        if (!removeNodeAtIndex(stack, ruleStartIdx))
            return E_INTERNAL;
        return SUCCESS;
    }

    DEBUG_PRINT("Invalid rule");

    return E_SYNTAX;
}
