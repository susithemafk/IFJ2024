/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symbol_tree.c
 * @date 28.9. 2024
 * @brief Implementation of the symbol tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "semantical/abstract_syntax_tree.h"
#include "utility/linked_list.h"
#include "utility/enumerations.h"
#include "semantical/function_call_validation.h"

// Function to create a new AST Node
ASTNodePtr ASTcreateNode(enum ASTNodeTypes type) {

    // 1st malloc: Allocate memory for the AST node itself
    ASTNodePtr node = (ASTNodePtr)malloc(sizeof(struct ASTNode));

    // Check if memory allocation was successful for the node
    if (node == NULL) return NULL;

    // Initialize the node type and the finished flag
    node->type = type;
    node->finished = false;

    // 2nd malloc: Allocate memory for the union (ASTNodeData)
    node->data = (ASTNodeDataPtr)malloc(sizeof(union ASTNodeData));
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    // 3rd malloc: Allocate memory for the specific member inside the union based on node type
    switch (type) {
        case AST_NODE_DECLARE:
            node->data->declare = (ASTNodeDeclarePtr)malloc(sizeof(struct ASTNodeDeclare));
            if (node->data->declare == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_ASSIGN:
            node->data->assign = (ASTNodeAssignPtr)malloc(sizeof(struct ASTNodeAssign));
            if (node->data->assign == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_TRUTH_EXPRESION:
            node->data->truthExpresion = (ASTNodeTruthExpresionPtr)malloc(sizeof(struct ASTNodeTruthExpresion));
            if (node->data->truthExpresion == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case ASB_NODE_EXPRESION:
            node->data->expresion = (ASTNodeExpresionRootPtr)malloc(sizeof(struct ASTNodeExpresion));
            if (node->data->expresion == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the expresion (stored in a linked list)
            node->data->expresion->output = initLinkedList(true);
            node->data->expresion->operators = initLinkedList(false);
            if (node->data->expresion->output == NULL || node->data->expresion->operators == NULL) {
                if (node->data->expresion->output != NULL) removeList(node->data->expresion->output);
                if (node->data->expresion->operators != NULL) removeList(node->data->expresion->operators);
                free(node->data->expresion);
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_IF_ELSE:
            node->data->ifElse = (ASTNodeIfPtr)malloc(sizeof(struct ASTNodeIf));
            if (node->data->ifElse == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_FUNC_CALL:
            node->data->functionCall = (ASTNodeFunctionCallPtr)malloc(sizeof(struct ASTNodeFunctionCall));
            if (node->data->functionCall == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the arguments (stored in a linked list)
            node->data->functionCall->arguments = initLinkedList(true);
            if (node->data->functionCall->arguments == NULL) {
                free(node->data->functionCall);
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_FUNCTION:
            node->data->function = (ASTNodeFunctionPtr)malloc(sizeof(struct ASTNodeFunction));
            if (node->data->function == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the arguments (stored in a linked list)
            node->data->function->arguments = initLinkedList(true);
            if (node->data->function->arguments == NULL) {
                free(node->data->function);
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_WHILE:
            node->data->whileLoop = (ASTNodeWhilePtr)malloc(sizeof(struct ASTNodeWhile));
            if (node->data->whileLoop == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_VALUE:
            node->data->value = (ASTNodeValuePtr)malloc(sizeof(struct ASTNodeValue));
            if (node->data->value == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        default:
            free(node->data);
            free(node);  // Free the node and data if the type is not recognized
            return NULL;
    }

    return node;  // Return the fully initialized node
}

// Function to Free an ABS node
bool ASTfreeNode(ASTNodePtr node) {

    if (node == NULL) {
        return false;
    }

    // here i can add some other swithc for recusive stuff of all the nodes, perhaps not needed doe

    bool result = true;
    // make sure, in case we have a linked list, that we free it
    switch(node->type) {
        case AST_NODE_DECLARE:
            free(node->data->declare);
            break;
        case AST_NODE_ASSIGN:
            free(node->data->assign);
            break;
        case AST_NODE_TRUTH_EXPRESION:
            free(node->data->truthExpresion);
            break;
        case ASB_NODE_EXPRESION:
            result = removeList(node->data->expresion->output);
            result = removeList(node->data->expresion->operators);
            free(node->data->expresion);    
            break;
        case AST_NODE_IF_ELSE:
            free(node->data->ifElse);
            break;
        case AST_NODE_FUNC_CALL:
            result = removeList(node->data->functionCall->arguments);
            free(node->data->functionCall);
            break;
        case AST_NODE_FUNCTION:
            result = removeList(node->data->function->arguments);
            free(node->data->function);
            break;
        case AST_NODE_WHILE:
            free(node->data->whileLoop);
            break;
        case AST_NODE_VALUE:
            free(node->data->value->value); // free the string value
            free(node->data->value);
            break;
        default:
            break;
    }
    free(node->data);
    free(node);

    return result;
}

// Function to edit the value node
enum ERR_CODES ASTeditNodeValue(ASTNodePtr valueNode, TOKEN_PTR value) {

    // internal error handeling
    if (valueNode == NULL || value == NULL)  return E_INTERNAL;

    // check if the node is of the correct type
    if (valueNode->type != AST_NODE_VALUE) return E_INTERNAL;

    ASTNodeValuePtr node = valueNode->data->value;

    // copy the values
    node->value = malloc(sizeof(char) * (strlen(value->value) + 1));
    if (node->value == NULL) return E_INTERNAL;
    
    strcpy(node->value, value->value);
    node->type = covertTokneDataType(value->type);
    if (node->type == dTypeNone) return E_INTERNAL;

    return SUCCESS;
}

// Function to add a node to an expression
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, ASTNodePtr expresionPart) {

    // check for internal errors
    if (expresionRoot == NULL || expresionPart == NULL) return E_INTERNAL;

    // in case the expresionPart is a function call, value, or variable, we add it to the output
    if (
        expresionPart->type == AST_NODE_FUNC_CALL ||
        expresionPart->type == AST_NODE_VALUE ||
        expresionPart->type == AST_NODE_VARIABLE
        ) {
            insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)expresionPart, -1);
            return SUCCESS;
        }

    // handle left parenthesis (push onto stack)
    if (expresionPart->data->operand->type == TOKEN_LPAR) {
        insertNodeAtIndex(expresionRoot->data->expresion->operators, (void*)expresionPart, 0);
        return SUCCESS;
    }

    // handle right parenthesis (pop to output until left parenthesis is found)
    if (expresionPart->data->operand->type == TOKEN_RPAR) {
        ASTNodePtr topOfStack = (ASTNodePtr)getDataAtIndex(expresionRoot->data->expresion->operators, 0);
        ASTNodePtr poppedVariable = NULL;

        while (topOfStack != NULL && topOfStack->data->operand->type != TOKEN_LPAR) {
            // pop the operator from the stack and add it to the output
            if (!popNodeAtIndex(expresionRoot->data->expresion->operators, 0, (void*)&poppedVariable)) return E_INTERNAL;
            if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)poppedVariable, -1)) return E_INTERNAL;
            
            topOfStack = (ASTNodePtr)getDataAtIndex(expresionRoot->data->expresion->operators, 0);
        }

        // check for error (no matching left parenthesis)
        if (topOfStack == NULL || topOfStack->data->operand->type != TOKEN_LPAR) return E_SYNTAX;

        // pop and discard the left parenthesis
        if (!removeNodeAtIndex(expresionRoot->data->expresion->operators, 0)) return E_INTERNAL;

        return SUCCESS;
    }

    // check if we're adding an operator, if not return error
    if (expresionPart->type != AST_NODE_OPERAND) return E_SYNTAX;

    // add the operator while respecting precedence
    ASTNodePtr popedVariable = NULL;
    ASTNodePtr topOfStack = (ASTNodePtr)getDataAtIndex(expresionRoot->data->expresion->operators, 0);
    enum TOKEN_TYPE operator = expresionPart->data->operand->type;

    while (
        topOfStack != NULL && 
        topOfStack->type == AST_NODE_OPERAND &&
        topOfStack->data->operand->type >= operator
        ) {

        // pop the top of the stack and add it to the output
        if (!popNodeAtIndex(expresionRoot->data->expresion->operators, 0, (void *)&popedVariable)) return E_INTERNAL;
        if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void *)popedVariable, -1)) return E_INTERNAL;
        topOfStack = (ASTNodePtr)getDataAtIndex(expresionRoot->data->expresion->operators, 0);
    }

    // check for internal error
    if (topOfStack != NULL && topOfStack->type != AST_NODE_OPERAND) return E_INTERNAL;

    // add the operator to the stack
    if (!insertNodeAtIndex(expresionRoot->data->expresion->operators, (void *)expresionPart, 0)) return E_INTERNAL;

    return SUCCESS;
}

// Function to get the data type of a node
enum DATA_TYPES ASTgetNodyReturnType(ASTNodePtr node) {

    if (node == NULL) return dTypeNone;

    switch (node->type) {
        case AST_NODE_VALUE:
            return node->data->value->type;
        case AST_NODE_VARIABLE:
            return node->data->variable->type;
    }
}

enum ERR_CODES ASTcompareTypes(ASTNodePtr node1, ASTNodePtr node2) {

    if (node1 == NULL || node2 == NULL) return E_SEMANTIC_INCOMPATABLE_TYPES;

    enum DATA_TYPES type1 = ASTgetNodyReturnType(node1);
    enum DATA_TYPES type2 = ASTgetNodyReturnType(node2);

    if (type1 == dTypeNone || type2 == dTypeNone) return E_SEMANTIC_UNKNOWN_TYPE;

    if (type1 != type2) return E_SEMANTIC_INCOMPATABLE_TYPES;

    return SUCCESS;
}

// Functio to prepare the eppression for code gen, need to check for semantical erros
enum ERR_CODES ASTprepareExpresion(ASTNodePtr expresionRoot, FunctionCallValidatorPtr validator, enum DATA_TYPES expresionType) {

    LinkedList *stack = expresionRoot->data->expresion->operators;
    if (stack == NULL) return E_INTERNAL;
    
    // first we need to empty the stack, in case there are any operands left, ignore perentheses
    ASTNodePtr popedVariable = NULL;
    while (stack->size > 0) {
        if (!popNodeAtIndex(stack, 0, (void*)&popedVariable)) return E_INTERNAL;
        // ignoring left and right parenthesis
        if (popedVariable->data->operand->type == TOKEN_LPAR || popedVariable->data->operand->type == TOKEN_RPAR) {
            continue;
        }
        insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)popedVariable, -1);
    }

    // now, since the stack is empty, we can simulate the handeling of this expresion, checking for types etc.
    LinkedList *output = expresionRoot->data->expresion->output;
    ASTNodePtr oneNode = NULL;
    ASTNodePtr terminalOne = NULL;
    ASTNodePtr terminalTwo = NULL;

    for (unsigned int i = 0; i < getSize(output); i++) {
        oneNode = (ASTNodePtr)getDataAtIndex(output, i);
        // handle a value (like : 2, 3.5, ...)
        if (
            oneNode->type == AST_NODE_VALUE || 
            oneNode->type == AST_NODE_VARIABLE ||
            oneNode->type == AST_NODE_FUNC_CALL
            ) {
            if (!insertNodeAtIndex(stack, (void *)oneNode, 0)) return E_INTERNAL;
            continue;
        }

        if (oneNode->type != AST_NODE_OPERAND) return E_SYNTAX; // check for syntax error

        // handle an operator

        terminalOne = (ASTNodePtr)getDataAtIndex(stack, 0);
        terminalTwo = (ASTNodePtr)getDataAtIndex(stack, 1);

        if (terminalOne == NULL || terminalTwo == NULL) return E_INTERNAL; // check for syntax error

        // check for the types of the operands, calling a function for this will be the best

        // for variables and values
        if (ASTcompareTypes(terminalOne, terminalTwo) != SUCCESS) return E_SEMANTIC_INCOMPATABLE_TYPES;

    }
        
}


