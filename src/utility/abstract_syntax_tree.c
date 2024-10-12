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
#include "utility/abstract_syntax_tree.h"
#include "utility/linked_list.h"
#include "utility/enumerations.h"

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
            node->data->expresion->root = initLinkedList(true);
            if (node->data->expresion->root == NULL) {
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
            result = removeList(node->data->expresion->root);
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
    node->type = value->type;

    return SUCCESS;
}

// Function to add a node to an expresion
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, TOKEN_PTR oneToken) {

    if (expresionRoot == NULL) return E_INTERNAL;

    if (oneToken->type > TOKEN_MULTIPLY || oneToken->type < TOKEN_PLUS) {
        // we have something form here + - / *
        // which needs to be added to the operand, or to the next available node in the left/right thing
        // meaning, that in we have to set the operator, or add a new node, goint from left to right
    }

}