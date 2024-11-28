/** AUTHOR
 *
 * @author  <247581> Martin Mendl  
 * @author <xsvobov00> Veronika Svobodová
 * @author <253171> Vanesa Zimmermannová
 * @file precident.h
 * @date 17.11. 2024
 * @brief header file for the precedent analysis
 */

#ifndef PRECIDENT_H
#define PRECIDENT_H

#include <stdbool.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "syntaxical/ast.h"
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#else 
#include "ast.h"
#include "enumerations.h"
#include "linked_list.h"
#endif

#ifdef DEBUG
#define DEBUG_TOKEN_RULE(stackItem, token, rule)                 \
    do {                                                        \
        printf("Rule checking result:\n");                      \
        printf("- stack item: ");                               \
        printTokenType(stackItem);                              \
        printf("- input item: ");                               \
        printTokenType(token->type);                            \
        printf("- rule: ");                                     \
        switch (rule) {                                         \
            case GREATER: printf(">\n"); break;                 \
            case LESS:    printf("<\n"); break;                 \
            case EQUAL:   printf("=\n"); break;                 \
            case END:     printf("END\n"); break;               \
            case ERROR:   printf("ERROR\n"); break;             \
            default:      printf("UNKNOWN RULE\n"); break;      \
        }                                                       \
    } while (0)
#define DEBUG_STACK(stack) printStack(stack)
#else
#define DEBUG_TOKEN_RULE(stackItem, token, rule) // No-op in release builds
#define DEBUG_STACK(stack) // No-op in release builds
#endif

// ####################### PRECIDENT #######################

extern const char precedentTable[7][7];

// rules like < > = " "
enum PRECEDENT_RULE {
    GREATER,
    LESS,
    EQUAL,
    ERROR,
    END,
};

// states for the expression
enum STATE_EXPRESSION {
    STATE_EX_E,
};

// states for the truth expression
enum STATE_TRUTH_EXPRESSION {
    STATE_TEX_E,
    STATE_TEX_R,
};

enum StackItemType {
    STACK_ITEM_TOKEN,
    STACK_ITEM_OPERATION, // for redux stuff
    STACK_ITEM_EXPRESSION,
    STACK_ITEM_TRUTH_EXPRESSION
};

union STACK_CONTENT {
    enum TOKEN_TYPE operation;                        // all the inputs thatn can be in the precident table
    enum PRECEDENT_RULE rule;                         // shifting and reducing rules
    enum STATE_EXPRESSION stateExpression;            // stats for all expressions
    enum STATE_TRUTH_EXPRESSION stateTruthExpression; // states for truth expressions
};

typedef struct StackItem {
    enum StackItemType type;
    union STACK_CONTENT content;
    Expression ast_node;
} *StackItemPtr;

void removeStack(LinkedList **stack);

/**
 * Function to get the precedent rule, based on the input, and what is the current thing
 *
 * @param left - the left token
 * @param right - the right token
 * @return enum PRECEDENT_RULE
 */
enum PRECEDENT_RULE _getPrecedentRule(enum TOKEN_TYPE stack, enum TOKEN_TYPE input);

/**
 * Function to get the conrsponding index for the precedent table
 *
 * @param token - the token type
 * @return int
 */
int _getPrecedentIndex(enum TOKEN_TYPE token);

/**
 * Functio to find the current operator in the stack
 *
 * @param stack - pointer to the stack
 * @param index - pointer to the index
 * @return enum TOKEN_TYPE
 */
enum TOKEN_TYPE _findActiveElement(LinkedList *stack, int *index);

/**
 * Function to create a stack item
 *
 * @param type - type of the stack item
 * @param enum_type - the enum type
 * @return StackItemPtr
 */
StackItemPtr __createStackItem(enum StackItemType type, int enum_type);

/**
 * Function to find the index of the < form the right of the stack
 *
 * @param stack - pointer to the stack
 * @return int
 */
int _findStartIndexForRedux(LinkedList *stack);

/**
 * Function to start the precedent analysis
 *
 * @param buffer - pointer to the buffer
 * @param startIdx - pointer to the start index
 * @param doExpresion - bool value, if true, we are handeling expresion, if false, we are handeling truth expresion
 * @return enum ERR_CODES
 */
enum ERR_CODES startPrecedentAnalysis(LinkedList *buffer, unsigned int *startIdx, bool doExpresion, Expression *expr);

/**
 * Function to apply the rules for the expresion
 *
 * @param stack - pointer to the stack
 * @param ruleStartIdx - the index of the rule start
 * @return enum ERR_CODES
 */
enum ERR_CODES _applyRulesExpresion(LinkedList *stack, unsigned int ruleStartIdx);

void printStack(LinkedList *stack);

/**
 * Function to apply the rules for the truth expression
 *
 * @param stack - pointer to the stack
 * @param ruleStartIdx - the index of the rule start
 * @return enum ERR_CODES
 */
enum ERR_CODES _applyRulesTruthExpression(LinkedList *stack, unsigned int ruleStartIdx);

#endif // PRECIDENT_H
