/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file sem_validator.h
 * @date 25.10.2024
 * @brief src file for the semantic validator
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "semantical/sem_validator.h"
#include "semantical/sem_enums.h"
#include "utility/linked_list.h"
#include "semantical/abstract_syntax_tree.h"
#include "semantical/symtable.h"
#include "utility/binary_search_tree.h"

// ####################### Validator #######################

// Function to initialize the validator
ASTValidatorPtr initValidator(void) {

    // create the validator
    ASTValidatorPtr validator = (ASTValidatorPtr)malloc(sizeof(struct ASTValidator));
    if (validator == NULL) return NULL;

    // create the stack
    validator->stack = initLinkedList(false);
    if (validator->stack == NULL) {
        free(validator);
        return NULL;
    }

    return validator;
}

// Function to add an AST to the stack
enum ERR_CODES addASTToStack(ASTValidatorPtr validator, ASTNodePtr ast) {

    // check if the call is valid
    if (validator == NULL || ast == NULL) return E_INTERNAL;

    // insert the AST to the stack
    if (!insertNodeAtIndex(validator->stack, (void *)ast, -1)) return E_INTERNAL;

    return SUCCESS;
}

// Function to validate all the ASTs in the stack
enum ERR_CODES validateASTs(ASTValidatorPtr validator, BST *functionDefinitions) {

    // check if the call is valid
    if (validator == NULL || functionDefinitions == NULL) return E_INTERNAL;

    // get the size of the stack
    unsigned int size = getSize(validator->stack);

    // validate all the ASTs in the stack
    for (unsigned int i = 0; i < size; i++) {
        ASTNodePtr ast = (ASTNodePtr)getDataAtIndex(validator->stack, i);
        if (ast == NULL) return E_INTERNAL;

        // validate the AST
        enum ERR_CODES result = _validateAST(ast, functionDefinitions);
        if (result != SUCCESS) {
            freeValidator(&validator);
            return result;
        };

        // call code gen here?
    }

    // free the validator
    freeValidator(&validator);

    return SUCCESS;
}

// Function to free the validator
void freeValidator(ASTValidatorPtr *validator) {

    // check if the call is valid
    if (validator == NULL || *validator == NULL) return;

    // frre the stack
    unsigned int size = getSize((*validator)->stack);
    for (unsigned int i = 0; i < size; i++) {
        ASTNodePtr ast = (ASTNodePtr)getDataAtIndex((*validator)->stack, i);
        if (ast == NULL) continue;
        ASTfreeNode(&ast);
    }

    removeList(&(*validator)->stack);
    free(*validator);
    *validator = NULL;
}

// Function to validate the AST
enum ERR_CODES _validateAST(ASTNodePtr ast, BST *funcDefinitions) {

    // check if the call is valid
    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    // check the type of the AST
    switch (ast->type) {
        case AST_NODE_FUNCTION:
            return SUCCESS; // function is already validated, by the symtable
        case AST_NODE_DECLARE:
            return _validateDeclare(ast, funcDefinitions);
        case AST_NODE_ASSIGN:
            return _validateAssign(ast, funcDefinitions);
        case AST_NODE_IF_ELSE:
            return _validateIfElse(ast, funcDefinitions);
        case AST_NODE_WHILE:
            return _validateWhile(ast, funcDefinitions);
        default:
            return E_INTERNAL; // this should never happen, since that would mean bad syntax
    }
}

// helper function to validate the expresion
ASTNodePtr _getOperationResultType(
    ASTNodePtr left, 
    ASTNodePtr right, 
    ASTNodePtr operator, 
    BST *functionDefinitions,
    enum ERR_CODES *result
    ){

    // Validate input arguments
    if (left == NULL || right == NULL || operator == NULL || functionDefinitions == NULL) {
        *result = E_INTERNAL;
        return NULL;
    }

    // Verify the operator is a supported operand (+, -, *, /)
    if (operator->type != AST_NODE_OPERAND ||
        (operator->data->operand->type != TOKEN_PLUS &&
         operator->data->operand->type != TOKEN_MINUS &&
         operator->data->operand->type != TOKEN_MULTIPLY &&
         operator->data->operand->type != TOKEN_DIVIDE)) {
        *result = E_INTERNAL;
        return NULL;
    }

    // Ensure left and right nodes are valid types
    if (left->type != AST_NODE_VALUE && left->type != AST_NODE_VARIABLE) {
        *result = E_INTERNAL;
        return NULL;
    }
    if (right->type != AST_NODE_VALUE && right->type != AST_NODE_VARIABLE) {
        *result = E_INTERNAL;
        return NULL;
    }

    // Retrieve data types of left and right nodes
    enum DATA_TYPES leftType, rightType;

    if (left->type == AST_NODE_VALUE) {
        leftType = left->data->value->type;
    } else {
        leftType = left->data->variable->type;
    }

    if (right->type == AST_NODE_VALUE) {
        rightType = right->data->value->type;
    } else {
        rightType = right->data->variable->type;
    }

    // Create result node
    ASTNodePtr resultNode = ASTcreateNode(AST_NODE_VALUE);
    if (resultNode == NULL) {
        *result = E_INTERNAL;
        return NULL;
    }
    resultNode->data->value->value = NULL;

    if (leftType == dTypeNone || rightType == dTypeNone) {
        *result = E_SEMANTIC_INCOMPATABLE_TYPES;
        free(resultNode);  // Clean up on error
        return NULL;
    }

    // Check both types are compatible (i32 or f64)
    if ((leftType != dTypeI32 && leftType != dTypeF64) || 
        (rightType != dTypeI32 && rightType != dTypeF64)) {
        *result = E_SEMANTIC_INCOMPATABLE_TYPES;
        free(resultNode);  // Clean up on error
        return NULL;
    }

    // Determine result type based on operand types and implicit conversion rules
    if (leftType == rightType) {
        resultNode->data->value->type = leftType;
    } else if (leftType == dTypeI32 && left->type == AST_NODE_VALUE) {
        resultNode->data->value->type = dTypeF64;  // Convert literal i32 to f64
    } else if (rightType == dTypeI32 && right->type == AST_NODE_VALUE) {
        resultNode->data->value->type = dTypeF64;  // Convert literal i32 to f64
    } else {
        *result = E_SEMANTIC_INCOMPATABLE_TYPES;
        free(resultNode);  // Clean up on error
        return NULL;
    }

    *result = SUCCESS;
    return resultNode;
}

void _freeStack(LinkedList *stack) {

    // check if the call is valid
    if (stack == NULL) return;

    for (unsigned int i = 0; i < getSize(stack); i++) {
        ASTNodePtr node = (ASTNodePtr)getDataAtIndex(stack, i);
        if (node == NULL) continue;
        // only free the new extra valuses, that ware created during the validation
        if (node->type == AST_NODE_VALUE && node->data->value == NULL) {
            ASTfreeNode(&node);
        }
    }

    removeList(&stack);
}

// Function to validate and return the expresion type
enum ERR_CODES _checkExpresionType(ASTNodePtr expresion, BST *funcDefinitions, enum DATA_TYPES *result) {

    if (expresion == NULL || funcDefinitions == NULL) return E_INTERNAL;

    if (expresion->type != AST_NODE_EXPRESION) return E_INTERNAL;

    // finish the expresion (converting to postfix)
    enum ERR_CODES errCode = ASTfinishExpresion(expresion);

    if (errCode != SUCCESS) return errCode;

    LinkedList *postFixExpresion = expresion->data->expresion->output;

    LinkedList *stack = initLinkedList(false);
    if (stack == NULL) return E_INTERNAL;

    // iterate over the postfix expresion
    for (unsigned int i = 0; i < getSize(postFixExpresion); i++) {
        ASTNodePtr node = (ASTNodePtr)getDataAtIndex(postFixExpresion, i);
        if (node == NULL) {
            _freeStack(stack);
            return E_INTERNAL;
        }

        // check if the node is an operand
        if (node->type == AST_NODE_OPERAND) {
            // pop the first two nodes from the stack and preform the validation call
            ASTNodePtr right = NULL;
            ASTNodePtr left = NULL;
            if (!popNodeAtIndex(stack, 0, (void *)&right)) {
                _freeStack(stack);
                return E_INTERNAL;
            }

            if (!popNodeAtIndex(stack, 0, (void *)&left)) {
                ASTfreeNode(&right);
                _freeStack(stack);
                return E_INTERNAL;
            }

            // get the result type of the operation
            ASTNodePtr resultNode = _getOperationResultType(left, right, node, funcDefinitions, &errCode);

            // if error accured, free the nodes and stack
            if (errCode != SUCCESS) {
                ASTfreeNode(&expresion);
                _freeStack(stack);
                return errCode;
            }

            // push the result to the stack
            if (!insertNodeAtIndex(stack, (void *)resultNode, 0)) {
                ASTfreeNode(&resultNode);
                _freeStack(stack);
                return E_INTERNAL;
            }
        }

        // if the node is a value, variable or function call, push it to the stack
        if (node->type == AST_NODE_VALUE || node->type == AST_NODE_VARIABLE) {
            if (!insertNodeAtIndex(stack, (void *)node, 0)) {
                _freeStack(stack);
                return E_INTERNAL;
            }
            continue;
        }

        // if the node is not recognized, free the stack and return error
        _freeStack(stack);
        ASTfreeNode(&expresion);
        return E_INTERNAL;
    }

    // if the stack size here is not 1, we have an error in syntax 
    if (getSize(stack) != 1) {
        _freeStack(stack);
        ASTfreeNode(&expresion);
        return E_SYNTAX;
    }

    // if the stack size is 1, we have the result in the stack
    ASTNodePtr resultNode = (ASTNodePtr)getDataAtIndex(stack, 0);

    // the resutlNode could be variable, value or function call

    if (resultNode->type == AST_NODE_VALUE) {
        *result = resultNode->data->value->type;
    } else if (resultNode->type == AST_NODE_VARIABLE) {
        *result = resultNode->data->variable->type;
    } else {
        _freeStack(stack);
        ASTfreeNode(&expresion);
        return E_INTERNAL;
    }

    // free the stack
    _freeStack(stack);

    return SUCCESS;
}

// Helper function to validate the value node's type compatibility with the variable's type
enum ERR_CODES __validateValueType(ASTNodePtr value, struct SymVariable *variable, BST *funcDefinitions) {
    if (value == NULL) return SUCCESS;
    enum DATA_TYPES result;
    enum ERR_CODES errCode;

    switch (value->type) {
        case AST_NODE_VALUE:
            if (value->data->value->value == NULL) return E_INTERNAL;
            if (value->data->value->type != variable->type) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;

        case AST_NODE_VARIABLE:
            if (value->data->variable->type != variable->type) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;

        case AST_NODE_FUNC_CALL: // handled by the symtable
            return SUCCESS;

        case AST_NODE_EXPRESION: 
            errCode = _checkExpresionType(value, funcDefinitions, &result);
            if (errCode != SUCCESS) return errCode;
            if (result != variable->type) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;

        default:
            return E_INTERNAL;
    }
}

// Function to validate the declare node
enum ERR_CODES _validateDeclare(ASTNodePtr ast, BST *funcDefinitions) {
    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    struct SymVariable *variable = ast->data->declare->variable;
    ASTNodePtr value = ast->data->declare->value;

    if (value == NULL) return SUCCESS;
    return __validateValueType(value, variable, funcDefinitions);
}

// Function to validate the assign node
enum ERR_CODES _validateAssign(ASTNodePtr ast, BST *funcDefinitions) {
    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    struct SymVariable *variable = ast->data->assign->variable;
    ASTNodePtr value = ast->data->assign->value;

    if (variable->mutable == false) return E_SEMANTIC_REDIFINITION;
    if (value == NULL) return E_SYNTAX;

    return __validateValueType(value, variable, funcDefinitions);
}

enum ERR_CODES _validateTruthExpresion(ASTNodePtr ast, BST *funcDefinitions) {

    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    // Check if the node is a truth expression
    if (ast->type != AST_NODE_TRUTH_EXPRESION) return E_INTERNAL;

    // Get the left and right side of the expression
    ASTNodePtr left = ast->data->truthExpresion->left;
    ASTNodePtr right = ast->data->truthExpresion->right;
    enum TOKEN_TYPE operator = ast->data->truthExpresion->operator;

    // Get the types from the left and right expressions
    enum DATA_TYPES leftType, rightType;
    enum ERR_CODES errCode;

    errCode = _checkExpresionType(left, funcDefinitions, &leftType);
    if (errCode != SUCCESS) return errCode;

    errCode = _checkExpresionType(right, funcDefinitions, &rightType);
    if (errCode != SUCCESS) return errCode;

    // Comparisons with strings, non-types, or undefined types are not allowed
    if (leftType == dTypeU8 || rightType == dTypeU8) return E_SEMANTIC_INCOMPATABLE_TYPES;
    if (leftType == dTypeNone || rightType == dTypeNone) return E_INTERNAL;
    if (leftType == dTypeUndefined || rightType == dTypeUndefined) return E_SEMANTIC_UNKNOWN_TYPE;

    switch (operator) {

        // Check for the == and != operators
        case TOKEN_EQUALS:
        case TOKEN_NOTEQUAL:
            if (leftType == rightType) return SUCCESS;

            // Allow implicit conversion for literals/constants
            if ((left->type == AST_NODE_VALUE && rightType == dTypeF64) ||
                (right->type == AST_NODE_VALUE && leftType == dTypeF64)) {
                return SUCCESS;
            }

            // Allow nullable comparisons if one operand is null-compatible
            if ((leftType == dTypeNone && rightType != dTypeNone) ||
                (rightType == dTypeNone && leftType != dTypeNone)) {
                return SUCCESS;
            }

            return E_SEMANTIC_INCOMPATABLE_TYPES;

        // Check for the <, >, <=, >= operators
        case TOKEN_LESSTHAN:
        case TOKEN_GREATERTHAN:
        case TOKEN_LESSOREQUAL:
        case TOKEN_GREATEROREQUAL:
            // Only numeric types are allowed, and neither should be nullable
            if (
                (leftType == dTypeI32 || leftType == dTypeF64) &&
                (rightType == dTypeI32 || rightType == dTypeF64)
            ) {

                // Allow implicit conversion if one side is a compatible literal
                if (
                    (left->type == AST_NODE_VALUE && leftType == dTypeI32 && rightType == dTypeF64) ||
                    (right->type == AST_NODE_VALUE && rightType == dTypeI32 && leftType == dTypeF64)
                ) return SUCCESS;

                if (leftType == rightType) return SUCCESS;
            }
            return E_SEMANTIC_INCOMPATABLE_TYPES;
        
        default:
            return E_INTERNAL;
    }
}

// Function to validate the if else node
enum ERR_CODES _validateIfElse(ASTNodePtr ast, BST *funcDefinitions) {
    // just validate the truth expresion
    return _validateTruthExpresion(ast->data->ifElse->condition, funcDefinitions);

}

// Function to validate the while node
enum ERR_CODES _validateWhile(ASTNodePtr ast, BST *funcDefinitions) {
    // just validate the truth expresion
    return _validateTruthExpresion(ast->data->whileLoop->condition, funcDefinitions);
}





