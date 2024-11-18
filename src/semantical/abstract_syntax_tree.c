/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file abstract_syntax_tree.c
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
            node->data->declare->variable = NULL;
            node->data->declare->value = NULL;
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
            node->data->assign->variable = NULL;
            node->data->assign->value = NULL;
            break;

        case AST_NODE_TRUTH_EXPRESION:
            node->data->truthExpresion = (ASTNodeTruthExpresionPtr)malloc(sizeof(struct ASTNodeTruthExpresion));
            if (node->data->truthExpresion == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->truthExpresion->left = NULL;
            node->data->truthExpresion->right = NULL;
            node->data->truthExpresion->operator = TOKEN_NONE;
            break;

        case AST_NODE_EXPRESION:
            node->data->expresion = (ASTNodeExpresionRootPtr)malloc(sizeof(struct ASTNodeExpresionRoot));
            if (node->data->expresion == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the expresion (stored in a linked list)
            node->data->expresion->output = initLinkedList(false);
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
            node->data->ifElse->condition = NULL;
            node->data->ifElse->truVar = NULL;
            node->data->ifElse->nulVar = NULL;
            break;

        case AST_NODE_FUNC_CALL:
            node->data->functionCall = (ASTNodeFunctionCallPtr)malloc(sizeof(struct ASTNodeFunctionCall));
            if (node->data->functionCall == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the arguments (stored in a linked list)
            node->data->functionCall->arguments = initLinkedList(false);
            if (node->data->functionCall->arguments == NULL) {
                free(node->data->functionCall);
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->functionCall->functionName = NULL;
            break;

        case AST_NODE_FUNCTION:
            node->data->function = (ASTNodeFunctionPtr)malloc(sizeof(struct ASTNodeFunction));
            if (node->data->function == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            // space for the arguments (stored in a linked list)
            node->data->function->arguments = initLinkedList(false);
            if (node->data->function->arguments == NULL) {
                free(node->data->function);
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->function->functionName = NULL;
            node->data->function->returnType = dTypeUndefined;
            break;

        case AST_NODE_WHILE:
            node->data->whileLoop = (ASTNodeWhilePtr)malloc(sizeof(struct ASTNodeWhile));
            if (node->data->whileLoop == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->whileLoop->condition = NULL;
            node->data->whileLoop->nulVar = NULL;
            node->data->whileLoop->truVar = NULL;
            break;

        case AST_NODE_VALUE:
            node->data->value = (ASTNodeValuePtr)malloc(sizeof(struct ASTNodeValue));
            if (node->data->value == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->value->value = NULL;
            break;

        case AST_NODE_VARIABLE:
            node->data->variable = NULL;
            break;

        case AST_NODE_OPERAND:
            node->data->operand = NULL;
            break;

        case AST_NODE_RETURN:
            node->data->returnNode = (ASTNodeReturnPtr)malloc(sizeof(struct ASTNodeReturn));
            if (node->data->returnNode == NULL) {
                free(node->data);
                free(node);
                return NULL;
            }
            node->data->returnNode->expression = NULL;
            break;
        
        case AST_NODE_ELSE_START:
            node->data->elseStart = true;
            break;

        case AST_BLOCK_END:
            node->data->blockEnd = true;
            break;

        default:
            free(node->data);
            free(node);  // Free the node and data if the type is not recognized
            return NULL;
    }

    return node;  // Return the fully initialized node
}

// Function to Free an ABS node
bool ASTfreeNode(ASTNodePtr *nodePtr) {

    if (nodePtr == NULL || *nodePtr == NULL) return false;

    ASTNodePtr node = *nodePtr;

    // here i can add some other swithc for recusive stuff of all the nodes, perhaps not needed doe

    bool result = true;
    // make sure, in case we have a linked list, that we free it
    switch(node->type) {
        case AST_NODE_DECLARE:
            if (node->data->declare->value != NULL) {
                result = ASTfreeNode(&node->data->declare->value);
            }
            free(node->data->declare);
            break;
        case AST_NODE_ASSIGN:
            if (node->data->assign->value != NULL) {
                result = ASTfreeNode(&node->data->assign->value);
            }
            free(node->data->assign);
            break;
        case AST_NODE_TRUTH_EXPRESION:
            if (node->data->truthExpresion->left != NULL) {
                result = ASTfreeNode(&node->data->truthExpresion->left);
            }
            if (node->data->truthExpresion->right != NULL) {
                result = ASTfreeNode(&node->data->truthExpresion->right);
            }
            free(node->data->truthExpresion);
            break;
        case AST_NODE_EXPRESION:
            if (node->data->expresion->output != NULL) {
                for (unsigned int i = 0; i < getSize(node->data->expresion->output); i++) {
                    ASTNodePtr output = (ASTNodePtr)getDataAtIndex(node->data->expresion->output, i);
                    result = ASTfreeNode(&output);
                }
                result = removeList(&node->data->expresion->output);
            }

            if (node->data->expresion->operators != NULL) {
                for (unsigned int i = 0; i < getSize(node->data->expresion->operators); i++) {
                    ASTNodePtr operator = (ASTNodePtr)getDataAtIndex(node->data->expresion->operators, i);
                    result = ASTfreeNode(&operator);
                }
                result = removeList(&node->data->expresion->operators);
            }
            free(node->data->expresion);    
            break;
        case AST_NODE_IF_ELSE:
            if (node->data->ifElse->condition != NULL) {
                result = ASTfreeNode(&node->data->ifElse->condition);
            }
            free(node->data->ifElse);
            break;
        case AST_NODE_FUNC_CALL:
            free(node->data->functionCall->functionName);
            for (unsigned int i = 0; i < getSize(node->data->functionCall->arguments); i++) {
                ASTNodePtr argument = (ASTNodePtr)getDataAtIndex(node->data->functionCall->arguments, i);
                result = ASTfreeNode(&argument);
            }
            result = removeList(&node->data->functionCall->arguments);
            free(node->data->functionCall);
            break;
        case AST_NODE_FUNCTION:
            free(node->data->function->functionName);
            for (unsigned int i = 0; i < getSize(node->data->function->arguments); i++) {
                ASTNodePtr argument = (ASTNodePtr)getDataAtIndex(node->data->function->arguments, i);
                result = ASTfreeNode(&argument);
            }
            result = removeList(&node->data->function->arguments);
            free(node->data->function);
            break;
        case AST_NODE_WHILE:
            if (node->data->whileLoop->condition != NULL) {
                result = ASTfreeNode(&node->data->whileLoop->condition);
            }
            free(node->data->whileLoop);
            break;
        case AST_NODE_VALUE:
            if (node->data->value->value != NULL) {
                free(node->data->value->value); // free the string value
            }
            free(node->data->value);
            break;
        case AST_NODE_OPERAND:
            if (node->data->operand != NULL) {
                free(node->data->operand->value);
                free(node->data->operand);
            }
            break;
        case AST_NODE_RETURN:
            if (node->data->returnNode->expression != NULL) {
                result = ASTfreeNode(&node->data->returnNode->expression);
            }
            free(node->data->returnNode);
            break;
        case AST_NODE_ELSE_START:
            break;
        default:
            break;
    }
    free(node->data);
    free(node);

    *nodePtr = NULL;

    return result;
}

// Function to init the operand node
enum ERR_CODES ASTinitNodeOperand(ASTNodePtr operandNode, TOKEN_PTR operand) {

    // internal error handeling
    if (operandNode == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (operandNode->type != AST_NODE_OPERAND) return E_INTERNAL;

    // malloc the operand
    operandNode->data->operand = (TOKEN_PTR)malloc(sizeof(struct TOKEN));

    // check for internal errors
    if (operandNode->data->operand == NULL) return E_INTERNAL;

    // make space for the value in the token
    operandNode->data->operand->value = malloc(sizeof(char) * (strlen(operand->value) + 1));
    if (operandNode->data->operand->value == NULL) return E_INTERNAL;

    strcpy(operandNode->data->operand->value, operand->value);
    operandNode->data->operand->type = operand->type;

    // copy the operand
    return SUCCESS;
}


// Function to init the value node
enum ERR_CODES ASTinitNodeValue(ASTNodePtr valueNode, TOKEN_PTR value) {

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

// Function to init the variable node
enum ERR_CODES ASTinitNodeVariable(ASTNodePtr variableNode, struct SymVariable *variable) {

    // internal error handeling
    if (variableNode == NULL || variable == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (variableNode->type != AST_NODE_VARIABLE) return E_INTERNAL;

    // save the variable ptr
    variableNode->data->variable = variable;
  
    return SUCCESS;
}


// Function to add a node to an expression
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand) {

    // check for internal errors
    if (expresionRoot == NULL || (variable == NULL && value == NULL && operand == NULL)) return E_INTERNAL;
    if ((variable != NULL) + (value != NULL) + (operand != NULL) != 1) return E_INTERNAL; // only one can be not null

    // adding variable
    if (variable != NULL) {

        // create a new variable node
        ASTNodePtr variableNode = ASTcreateNode(AST_NODE_VARIABLE);
        if (variableNode == NULL) return E_INTERNAL;

        // init the variable node
        if (ASTinitNodeVariable(variableNode, variable) != SUCCESS) {
            ASTfreeNode(&variableNode);
            return E_INTERNAL;
        }

        if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)variableNode, -1)) return E_INTERNAL;
            return SUCCESS;
    }

    // adding value
    if (value != NULL) {

        // create a new value node
        ASTNodePtr valueNode = ASTcreateNode(AST_NODE_VALUE);
        if (valueNode == NULL) return E_INTERNAL;

        // init the value node
        if (ASTinitNodeValue(valueNode, value) != SUCCESS) {
            ASTfreeNode(&valueNode);
            return E_INTERNAL;
        }

        if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void*)valueNode, -1)) return E_INTERNAL;
            return SUCCESS;
    }

    // handle right parenthesis (pop to output until left parenthesis is found)
    if (operand->type == TOKEN_RPAR) {
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

    // create a new operand node
    ASTNodePtr expresionPart = ASTcreateNode(AST_NODE_OPERAND);
    if (expresionPart == NULL) return E_INTERNAL;

    // init the operand node
    if (ASTinitNodeOperand(expresionPart, operand) != SUCCESS) {
        ASTfreeNode(&expresionPart);
        return E_INTERNAL;
    }
    
    // handle left parenthesis (push onto stack)
    if (operand->type == TOKEN_LPAR) {
        if (!insertNodeAtIndex(expresionRoot->data->expresion->operators, (void*)expresionPart, 0)) return E_INTERNAL;
        return SUCCESS;
    }

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

// Function to finish the expresion
enum ERR_CODES ASTfinishExpresion(ASTNodePtr expresionRoot) {
    // check for internal errors
    if (expresionRoot == NULL) return E_INTERNAL;

    /*
    what we need to do here, is to pop all the operators from the stack and add them to the output
    than remove the stack, and set the poitner to it to NULL
    */

    ASTNodePtr popedVariable = NULL;
    while (getSize(expresionRoot->data->expresion->operators) > 0) {
        if (!popNodeAtIndex(expresionRoot->data->expresion->operators, 0, (void *)&popedVariable)) return E_INTERNAL;
        if (!insertNodeAtIndex(expresionRoot->data->expresion->output, (void *)popedVariable, -1)) return E_INTERNAL;
    }

    removeList(&expresionRoot->data->expresion->operators);
    expresionRoot->data->expresion->operators = NULL;
    expresionRoot->finished = true;
    
    return SUCCESS;
}

// Function to edit a function node
enum ERR_CODES ASTeditFunctionNode(ASTNodePtr functionNode, char *functionName, enum DATA_TYPES returnType, int nullable, struct SymVariable *argument) {

    // check for internal errors
    if (functionNode == NULL || (functionName == NULL && returnType == dTypeUndefined && argument == NULL && nullable == -1)) return E_INTERNAL;

    // check if the node is of the correct type
    if (functionNode->type != AST_NODE_FUNCTION) return E_INTERNAL;

    if (functionNode->finished) return E_INTERNAL; // mby different return type

    ASTNodeFunctionPtr node = functionNode->data->function;

    // if we are givven an argument, we add it to the list
    if (argument != NULL) {

        // create the variable node
        ASTNodePtr arg = ASTcreateNode(AST_NODE_VARIABLE);
        if (arg == NULL) return E_INTERNAL;

        // init the variable node
        enum ERR_CODES err = ASTinitNodeVariable(arg, argument);

        if (err != SUCCESS) {
            ASTfreeNode(&arg);
            return err;
        }

        // copy the argument
        if (!insertNodeAtIndex(node->arguments, (void*)arg, -1)) return E_INTERNAL;
    }

    // save func name
    if (functionName != NULL) {
        node->functionName = malloc(sizeof(char) * (strlen(functionName) + 1));
        if (node->functionName == NULL) return E_INTERNAL;

        strcpy(node->functionName, functionName);
    }

    // save the return type
    if (returnType != dTypeUndefined) {
        node->returnType = returnType;
    }

    // save the nullable
    if (nullable != -1) {
        node->nullable = (nullable == 1) ? true : false;
    }

    return SUCCESS;
}

// Function to edit the function call node
enum ERR_CODES ASTeditFunctionCallNode(ASTNodePtr functionCallNode, char *functionName, struct SymVariable *varArg, TOKEN_PTR valueArg) {

    //check call validity
    if (functionCallNode == NULL) return E_INTERNAL;
    bool editArg = (varArg != NULL) + (valueArg != NULL) == 1;
    bool editName = functionName != NULL;
    if (!editArg && !editName) return E_INTERNAL;

    if (functionCallNode->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    // handle the function call name
    if (functionName != NULL) {
        ASTNodeFunctionCallPtr curFunc = functionCallNode->data->functionCall;
        if (curFunc->functionName != NULL) return E_INTERNAL;

        // copy the function name
        curFunc->functionName = malloc(sizeof(char) * (strlen(functionName) + 1));
        if (curFunc->functionName == NULL) return E_INTERNAL;

        strcpy(curFunc->functionName, functionName);
    }

    if (!editArg) return SUCCESS; // skip this part, if we are not editing the argument

    // handeling var arguments
    if (varArg != NULL) {
        // create the variable node
        ASTNodePtr arg = ASTcreateNode(AST_NODE_VARIABLE);
        if (arg == NULL) return E_INTERNAL;

        // init the variable node
        enum ERR_CODES err = ASTinitNodeVariable(arg, varArg);

        if (err != SUCCESS) {
            ASTfreeNode(&arg);
            return err;
        }

        // save the arg
        if (!insertNodeAtIndex(functionCallNode->data->functionCall->arguments, (void*)arg, -1)) return E_INTERNAL;
        return SUCCESS;
    }


    // handeling value arguments
    // create the value node
    ASTNodePtr arg = ASTcreateNode(AST_NODE_VALUE);
    if (arg == NULL) return E_INTERNAL;


    // init the value node
    enum ERR_CODES err = ASTinitNodeValue(arg, valueArg);

    if (err != SUCCESS) {
        ASTfreeNode(&arg);
        return err;
    }

    // save the arg
    if (!insertNodeAtIndex(functionCallNode->data->functionCall->arguments, (void*)arg, -1)) return E_INTERNAL;
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
        declareNode->data->declare->variable = variable;
        if (declareNode->data->declare->variable == NULL) return E_INTERNAL;
    }

    if (value != NULL) {
        if (
            value->type != AST_NODE_VALUE &&
            value->type != AST_NODE_VARIABLE &&
            value->type != AST_NODE_FUNC_CALL &&
            value->type != AST_NODE_EXPRESION
            ) return E_SYNTAX;

        if (declareNode->data->declare->variable == NULL) return E_INTERNAL;

        declareNode->data->declare->value = value;
    }

    return SUCCESS;
}

// Function to edit the assing node
enum ERR_CODES ASTeditAssignNode(ASTNodePtr assignNode, struct SymVariable *variable, ASTNodePtr value) {

    // check for internal errors
    if (assignNode == NULL) return E_INTERNAL;
    if (variable == NULL && value == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (assignNode->type != AST_NODE_ASSIGN) return E_INTERNAL;

    // save the variable
    if (variable != NULL) {
        // check for mutable error 
        if (variable->mutable == false && variable->id != 0) return E_SEMANTIC_REDIFINITION; // check for redefinition
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
enum ERR_CODES ASTeditTruthExpresion(ASTNodePtr truthExpresion, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand) {

    // check for internal errors
    if (truthExpresion == NULL) return E_INTERNAL;

    // check if the node is of correct type
    if (truthExpresion->type != AST_NODE_TRUTH_EXPRESION) return E_INTERNAL;
    if ((variable != NULL) + (value != NULL) + (operand != NULL) != 1) return E_INTERNAL;

    // handle the operand
    if (operand != NULL) {
        // handle expresion saving (adding to the ==, !=, >=, <=, >, <)
        if (
            operand->type == TOKEN_EQUALS || 
            operand->type == TOKEN_NOTEQUAL || 
            operand->type == TOKEN_GREATEROREQUAL || 
            operand->type == TOKEN_LESSOREQUAL ||
            operand->type == TOKEN_GREATERTHAN ||
            operand->type == TOKEN_LESSTHAN
            ) {
                
                // cant have two operands
                if (truthExpresion->data->truthExpresion->operator != TOKEN_NONE) return E_INTERNAL;

                // negate the operand, for later ez code gen
                enum TOKEN_TYPE opr = negateOperand(operand->type);

                truthExpresion->data->truthExpresion->operator = opr;
                return SUCCESS;
            }   
    }

    // handle exppresion part
    // need to find the expresion to add to the truth expresion
    ASTNodePtr expresion = (truthExpresion->data->truthExpresion->operator == TOKEN_NONE) ? 
        truthExpresion->data->truthExpresion->left : // we dont have the operand yet, choose the left side 
        truthExpresion->data->truthExpresion->right; // we have the operand, choose right side

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

    enum ERR_CODES result = ASTaddNodeToExpresion(expresion, variable, value, operand);
    if (result != SUCCESS) return result;
    return SUCCESS;
}

// Function to edit the if node
enum ERR_CODES ASTeditIfNode(
    ASTNodePtr ifNode, 
    struct SymVariable *variable, 
    TOKEN_PTR value, 
    TOKEN_PTR operand, 
    struct SymVariable *truVar, 
    struct SymVariable *nulVar
) {

    // check for internal errors
    if (ifNode == NULL) return E_INTERNAL;
    if (ifNode->type != AST_NODE_IF_ELSE) return E_INTERNAL;
    int normalCondition = (variable != NULL) + (value != NULL) + (operand != NULL);
    bool doingWierdCondition = (truVar != NULL || nulVar != NULL);

    if (normalCondition != 1 && !doingWierdCondition) return E_INTERNAL; // we are not setting condtion and not seting wierd condition
    if (normalCondition == 1 && doingWierdCondition) return E_INTERNAL; // we are setting condtion and setting wierd condition
    
    if (truVar != NULL) ifNode->data->ifElse->truVar = truVar; // set the truVar
    if (nulVar != NULL) ifNode->data->ifElse->nulVar = nulVar; // set the nulVar

    if (normalCondition != 1) return SUCCESS; // we are not setting normal condtion, we can leave

    // setting the normla condition
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

    // add to the expresion
    return ASTeditTruthExpresion(truthExpresion, variable, value, operand);
}


// Function to edit the while node
enum ERR_CODES ASTeditWhileNode(
    ASTNodePtr whileNode, 
    struct SymVariable *variable, 
    TOKEN_PTR value, 
    TOKEN_PTR operand, 
    struct SymVariable *truVar, 
    struct SymVariable *nulVar
) {

    // ckeck for internal errors
    if (whileNode == NULL) return E_INTERNAL;
    if (whileNode->type != AST_NODE_WHILE) return E_INTERNAL;

    int normalCondition = (variable != NULL) + (value != NULL) + (operand != NULL);
    bool doingWierdCondition = (truVar != NULL || nulVar != NULL);

    if (normalCondition != 1 && !doingWierdCondition) return E_INTERNAL; // we are not setting condtion and not seting wierd condition
    if (normalCondition == 1 && doingWierdCondition) return E_INTERNAL; // we are setting condtion and setting wierd condition

    if (truVar != NULL) whileNode->data->whileLoop->truVar = truVar; // set the truVar
    if (nulVar != NULL) whileNode->data->whileLoop->nulVar = nulVar; // set the nulVar

    if (normalCondition != 1) return SUCCESS; // we are not setting normal condtion, we can leave

    // setting the normla condition
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
    return ASTeditTruthExpresion(truthExpresion, variable, value, operand);
}

// Function to edit the return node
enum ERR_CODES ASTeditReturnNode(ASTNodePtr returnNode, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand) {

    // check for internal errors
    if (returnNode == NULL) return E_INTERNAL;

    // check if the node is of the correct type
    if (returnNode->type != AST_NODE_RETURN) return E_INTERNAL;

    if (returnNode->data->returnNode->expression == NULL) {
        // create a new expresion
        ASTNodePtr newExpresion = ASTcreateNode(AST_NODE_EXPRESION);
        if (newExpresion == NULL) return E_INTERNAL;

        // add the expresion to the return node
        returnNode->data->returnNode->expression = newExpresion;
    }

    // add the expresion part to the return node
    enum ERR_CODES err = ASTaddNodeToExpresion(returnNode->data->returnNode->expression, variable, value, operand);
    if (err != SUCCESS) return err;

    return SUCCESS;
}


