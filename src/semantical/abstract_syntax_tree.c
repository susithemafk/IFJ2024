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
#include "semantical/symtable.h"
#include "semantical/sem_enums.h"
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

        case AST_NODE_EXPRESION:
            node->data->expresion = (ASTNodeExpresionRootPtr)malloc(sizeof(struct ASTNodeExpresionRoot));
            if (node->data->expresion == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the expresion (stored in a linked list)
            node->data->expresion->output = initLinkedList(true);
            node->data->expresion->operators = initLinkedList(false);
            if (node->data->expresion->output == NULL || node->data->expresion->operators == NULL) {
                if (node->data->expresion->output != NULL) removeList(&node->data->expresion->output);
                if (node->data->expresion->operators != NULL) removeList(&node->data->expresion->operators);
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

        case AST_NODE_VARIABLE:
            node->data->variable = (SymVariable *)malloc(sizeof(SymVariable));
            if (node->data->variable == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            break;

        case AST_NODE_OPERAND:
            node->data->operand = (TOKEN_PTR)malloc(sizeof(struct TOKEN));
            if (node->data->operand == NULL) {
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
        case AST_NODE_EXPRESION:
            result = removeList(&node->data->expresion->output);
            result = removeList(&node->data->expresion->operators);
            free(node->data->expresion);    
            break;
        case AST_NODE_IF_ELSE:
            free(node->data->ifElse);
            break;
        case AST_NODE_FUNC_CALL: // need to fix free of symvariables
            result = removeList(&node->data->functionCall->arguments);
            free(node->data->functionCall);
            break;
        case AST_NODE_FUNCTION: // need to fix free of symvariables
            result = removeList(&node->data->function->arguments);
            free(node->data->function);
            break;
        case AST_NODE_WHILE:
            free(node->data->whileLoop);
            break;
        case AST_NODE_VALUE:
            free(node->data->value->value); // free the string value
            free(node->data->value);
            break;
        case AST_NODE_VARIABLE:
            free(node->data->variable->name);
            free(node->data->variable);
            break;
        case AST_NODE_OPERAND:
            free(node->data->operand->value);
            free(node->data->operand);
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
            if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)expresionPart, -1)) return E_INTERNAL;
            return SUCCESS;
        }

    // handle left parenthesis (push onto stack)
    if (expresionPart->data->operand->type == TOKEN_LPAR) {
        if (!insertNodeAtIndex(expresionRoot->data->expresion->operators, (void*)expresionPart, 0)) return E_INTERNAL;
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

// Function to prepare the expresion node

// Function to edit a function node
enum ERR_CODES ASTeditFunctionNode(ASTNodePtr functionNode, char *functionName, enum DATA_TYPES returnType, ASTNodePtr argument) {

    // check for internal errors
    if (functionNode == NULL && functionName == NULL && returnType == dTypeUndefined && argument == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (functionNode->type != AST_NODE_FUNCTION) return E_INTERNAL;

    if (functionNode->finished) return E_INTERNAL; // mby different return type

    ASTNodeFunctionPtr node = functionNode->data->function;

    // if we are givven an argument, we add it to the list
    if (argument != NULL) {
        if (argument->type != AST_NODE_VARIABLE) return E_INTERNAL;

        // copy the argument
        ASTNodePtr newArgument = ASTcreateNode(AST_NODE_VARIABLE);
        if (newArgument == NULL) return E_INTERNAL;

        newArgument->data->variable->name = malloc(sizeof(char) * (strlen(argument->data->variable->name) + 1));
        if (newArgument->data->variable->name == NULL) {
            ASTfreeNode(newArgument);
            return E_INTERNAL;
        }

        strcpy(newArgument->data->variable->name, argument->data->variable->name);
        newArgument->data->variable->type = argument->data->variable->type;

        if (!insertNodeAtIndex(node->arguments, (void*)newArgument, -1)) {
            ASTfreeNode(newArgument);
            return E_INTERNAL;
        }
    }

    if (functionName != NULL) {
        node->functionName = malloc(sizeof(char) * (strlen(functionName) + 1));
        if (node->functionName == NULL) return E_INTERNAL;

        strcpy(node->functionName, functionName);
    }

    if (returnType != dTypeUndefined) {
        node->returnType = returnType;
    }

    return SUCCESS;
}

// Function to finalize the function node
enum ERR_CODES ASTFinishFuncNode(ASTNodePtr functionCallNode) {

    if (functionCallNode == NULL) return E_INTERNAL;

    if (functionCallNode->type != AST_NODE_FUNCTION) return E_INTERNAL;

    if (functionCallNode->finished) return E_INTERNAL;

    functionCallNode->finished = true;

    return SUCCESS;
}

// Function to edit the function call node
enum ERR_CODES ASTeditFunctionCallNode(ASTNodePtr functionCallNode, char *functionName, ASTNodePtr argument) {
    //check call validity
    if (functionCallNode == NULL && functionName == NULL && argument == NULL) return E_INTERNAL;
    if (functionCallNode->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    if (functionCallNode->data->functionCall->current == NULL) {
        functionCallNode->data->functionCall->current = functionCallNode;
    }

    if (functionName != NULL) {
        ASTNodeFunctionCallPtr curFunc = functionCallNode->data->functionCall->current->data->functionCall;
        // redifining of the function name is not allowed
        if (curFunc->functionName != NULL) return E_INTERNAL;

        // copy the function name
        curFunc->functionName = malloc(sizeof(char) * (strlen(functionName) + 1));
        if (curFunc->functionName == NULL) return E_INTERNAL;

        strcpy(curFunc->functionName, functionName);
    }

    // if we are given a function call as an argument, we need to switch to it
    if (argument != NULL) {
        // handle function calls
        if (argument->type == AST_NODE_FUNC_CALL) {
            argument->data->functionCall->parent = functionCallNode->data->functionCall->current;
            functionCallNode->data->functionCall->current = argument;
            if (!insertNodeAtIndex(functionCallNode->data->functionCall->arguments, (void *)argument, -1)) return E_INTERNAL;
            return SUCCESS;
        }

        // only allow variables, values
        if (
            argument->type != AST_NODE_VARIABLE &&
            argument->type != AST_NODE_VALUE &&
            argument->type != AST_NODE_EXPRESION
            ) return E_SYNTAX;

        // save the argument
        if (!insertNodeAtIndex(functionCallNode->data->functionCall->current->data->functionCall->arguments, (void *)argument, -1)) return E_INTERNAL;
    }

    return SUCCESS;
}
// Function to switch to outer function call, in a function call
enum ERR_CODES ASTswitchToOuterFunctionCall(ASTNodePtr functionCallNode) {

    // check for internal errors
    if (functionCallNode == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (functionCallNode->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    // check if the current function call has a parent
    if (functionCallNode->data->functionCall->current->data->functionCall->parent == NULL) return E_INTERNAL;

    // switch to the parent function call
    functionCallNode->data->functionCall->current = functionCallNode->data->functionCall->current->data->functionCall->parent;

    return SUCCESS;
}

// Function to edit the declared node
enum ERR_CODES ASTeditDeclareNode(ASTNodePtr declareNode, struct SymVariable *variable, ASTNodePtr value) {

    // check for internal errors
    if (declareNode == NULL) return E_INTERNAL;
    if (variable == NULL && value == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (declareNode->type != AST_NODE_DECLARE) return E_INTERNAL;

    if (variable != NULL) {
        // copy the variable
        declareNode->data->declare->variable = copyVariable(variable);
        if (declareNode->data->declare->variable == NULL) return E_INTERNAL;
    }

    if (value != NULL) {
        if (
            value->type != AST_NODE_VALUE &&
            value->type != AST_NODE_VARIABLE &&
            value->type != AST_NODE_FUNC_CALL &&
            value->type != AST_NODE_EXPRESION
            ) return E_SYNTAX;

        declareNode->data->declare->value = value;
    }

    return SUCCESS;
}

// Function to edit the assing node
enum ERR_CODES ASTeditAssignNode(ASTNodePtr assignNode, ASTNodePtr declarNode, ASTNodePtr value) {

    // check for internal errors
    if (assignNode == NULL) return E_INTERNAL;
    if (declarNode == NULL && value == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (assignNode->type != AST_NODE_ASSIGN) return E_INTERNAL;

    // save the variable
    if (declarNode != NULL) {
        if (declarNode->type != AST_NODE_DECLARE) return E_INTERNAL;
        SymVariable *variable = declarNode->data->declare->variable;
        if (variable == NULL) return E_INTERNAL;
        if (variable->mutable == false) return E_SEMANTIC_REDIFINITION; // check for redefinition
        assignNode->data->assign->variable = variable;
    }

    // save the value
    if (value != NULL) {
        if (
            value->type != AST_NODE_VALUE &&
            value->type != AST_NODE_VARIABLE &&
            value->type != AST_NODE_FUNC_CALL &&
            value->type != AST_NODE_EXPRESION
            ) return E_SYNTAX;

        assignNode->data->assign->value = value;
    }

    return SUCCESS;
}

// Function to edit the truth expresion
enum ERR_CODES ASTeditTruthExpresion(ASTNodePtr truthExpresion, ASTNodePtr expresionPart, enum TOKEN_TYPE opr) {

    // check for internal errors
    if (truthExpresion == NULL && expresionPart == NULL && opr == TOKEN_NONE) return E_INTERNAL;

    // check if we want to add both parts
    if (expresionPart != NULL && opr != TOKEN_NONE) return E_INTERNAL;

    // check if the node is of correct type
    if (truthExpresion->type != AST_NODE_TRUTH_EXPRESION) return E_INTERNAL;

    // handle expresion saving
    if (expresionPart != NULL) {

        // need to find the expresion to add to the truth expresion
        ASTNodePtr expresion = (truthExpresion->data->truthExpresion->operator == TOKEN_NONE) ? 
            truthExpresion->data->truthExpresion->left : 
            truthExpresion->data->truthExpresion->right;

        if (expresion == NULL) {
            // need to initialize the expresion
            ASTNodePtr newExpresion = ASTcreateNode(AST_NODE_EXPRESION);
            if (newExpresion == NULL) return E_INTERNAL;

            if (truthExpresion->data->truthExpresion->operator == TOKEN_NONE) {
                // add the expresion to the truth expresion
                truthExpresion->data->truthExpresion->left = newExpresion;
            } else  {
                // add the expresion to the truth expresion
                truthExpresion->data->truthExpresion->right = newExpresion;
            }
            expresion = newExpresion;
        }

        enum ERR_CODES result = ASTaddNodeToExpresion(expresion, expresionPart);
        if (result != SUCCESS) return result;
    }

    // handle the operand
    if (opr != TOKEN_NONE) {
        if (truthExpresion->data->truthExpresion->operator != TOKEN_NONE) return E_INTERNAL;
        truthExpresion->data->truthExpresion->operator = opr;
    }

    return SUCCESS;
}

// Function to edit the if node
enum ERR_CODES ASTeditIfNode(ASTNodePtr ifNode, ASTNodePtr conditionPart) {

    // check for internal errors
    if (ifNode == NULL || conditionPart == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (ifNode->type != AST_NODE_IF_ELSE) return E_INTERNAL;

    /*
    in here, we have to either init the truth expresion or add to it
    */

   ASTNodePtr truthExpresion = ifNode->data->ifElse->condition;

    // we have an empty if statement
    if (truthExpresion == NULL) {

        // create a new truth expresion
        ASTNodePtr newTruthExpresion = ASTcreateNode(AST_NODE_TRUTH_EXPRESION);
        if (newTruthExpresion == NULL) return E_INTERNAL;

        // add the condition to the truth expresion
        truthExpresion = newTruthExpresion;
        ifNode->data->ifElse->condition = truthExpresion;
    }

    // add to the truth expresion

    /*
    in here, we have two options, either it will be an operad, or not the tokes enum values are form 7 to 12 (inclusive)
    */

    if (conditionPart->type == AST_NODE_OPERAND) {
        if (conditionPart->data->operand->type < 7 || conditionPart->data->operand->type > 12) return E_SYNTAX;
        enum ERR_CODES result = ASTeditTruthExpresion(truthExpresion, NULL, conditionPart->data->operand->type);
        if (result != SUCCESS) return result;
        return SUCCESS;
    }

    // add to the expresion
    return ASTeditTruthExpresion(truthExpresion, conditionPart, TOKEN_NONE);
}


// Function to edit the while node
enum ERR_CODES ASTeditWhileNode(ASTNodePtr whileNode, ASTNodePtr condition) {

    // ckeck for internal errors
    if (whileNode == NULL || condition == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (whileNode->type != AST_NODE_WHILE) return E_INTERNAL;

    ASTNodePtr truthExpresion = whileNode->data->whileLoop->condition;

    if (truthExpresion == NULL) {

        // create a new truth expresion
        ASTNodePtr newTruthExpresion = ASTcreateNode(AST_NODE_TRUTH_EXPRESION);
        if (newTruthExpresion == NULL) return E_INTERNAL;

        // add the condition to the truth expresion
        truthExpresion = newTruthExpresion;
        whileNode->data->whileLoop->condition = truthExpresion;
    }

    // add to the truth expresion

    if (condition->type == AST_NODE_OPERAND) {
        if (condition->data->operand->type < 7 || condition->data->operand->type > 12) return E_SYNTAX;
        enum ERR_CODES result = ASTeditTruthExpresion(truthExpresion, NULL, condition->data->operand->type);
        if (result != SUCCESS) return result;
        return SUCCESS;
    }

    // add to the expresion
    return ASTeditTruthExpresion(truthExpresion, condition, TOKEN_NONE);
}


