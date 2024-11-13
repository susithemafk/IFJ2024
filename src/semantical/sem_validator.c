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

    // init the func defs
    validator->funcDefs = initFunctionDefinitions();
    if (validator->funcDefs == NULL) {
        free(validator->stack);
        free(validator);
        return NULL;
    }

    return validator;
}

// Function to add an AST to the stack
enum ERR_CODES addASTToStack(ASTValidatorPtr validator, ASTNodePtr ast) {

    // check if the call is valid
    if (validator == NULL || ast == NULL) return E_INTERNAL;

    // save any function definitions
    if (ast->type == AST_NODE_FUNCTION) {
        if (!addFunctionDefinition(validator->funcDefs, ast)) return E_INTERNAL;
    }

    // insert the AST to the stack
    if (!insertNodeAtIndex(validator->stack, (void *)ast, -1)) return E_INTERNAL;

    return SUCCESS;
}

// Function to validate all the ASTs in the stack
enum ERR_CODES validateASTs(ASTValidatorPtr validator) {

    // check if the call is valid
    if (validator == NULL) return E_INTERNAL;

    ASTNodePtr currentFunc = NULL;

    // get the size of the stack
    unsigned int size = getSize(validator->stack);

    // validate all the ASTs in the stack
    for (unsigned int i = 0; i < size; i++) {
        ASTNodePtr ast = (ASTNodePtr)getDataAtIndex(validator->stack, i);
        if (ast == NULL) return E_INTERNAL;
        if (ast->type == AST_NODE_FUNCTION) currentFunc = ast;

        // validate the AST
        enum ERR_CODES result = _validateAST(ast, validator->funcDefs, currentFunc);
        if (result != SUCCESS) {
            freeValidator(&validator);
            return result;
        };

        // call code gen here?
        // what we have available -> the AST, and the current function definition

        // Free the ast, after the code gen finished, funcion definitions will be freed at the end of the code gen
        if (ast->type != AST_NODE_FUNCTION) {
            ASTfreeNode(&ast);
        }
    }

    // free the validator
    freeValidator(&validator);

    return SUCCESS;
}

// Function to free the validator
void freeValidator(ASTValidatorPtr *validator) {

    // check if the call is valid
    if (validator == NULL || *validator == NULL) return;

    // free the function definitions
    freeFunctionDefinitions(&(*validator)->funcDefs);

    // frre the stack
    unsigned int size = getSize((*validator)->stack);
    for (unsigned int i = 0; i < size; i++) {
        ASTNodePtr ast = (ASTNodePtr)getDataAtIndex((*validator)->stack, i);
        ASTfreeNode(&ast);
    }

    removeList(&(*validator)->stack);
    free(*validator);
    *validator = NULL;
}

// Function to validate the AST
enum ERR_CODES _validateAST(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions, ASTNodePtr currentFunc) {

    // check if the call is valid
    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    // check the type of the AST

    // functions definitions should not be saved to the stack, it should be symttable -> function definitions
    switch (ast->type) {
        case AST_NODE_DECLARE:
            return __validateDeclare(ast, funcDefinitions);
        case AST_NODE_ASSIGN:
            return __validateAssign(ast, funcDefinitions);
        case AST_NODE_IF_ELSE:
            return __validateIfElse(ast);
        case AST_NODE_WHILE:
            return __validateWhile(ast);
        case AST_NODE_RETURN:
            return __validateReturn(ast, currentFunc);
        case AST_NODE_FUNC_CALL:
            return __validateFuncCall(ast, funcDefinitions);
        case AST_NODE_FUNCTION:
            return SUCCESS; // function definitions shoudl be all fine, this should be handeled by te Func defs validator
        default:
            return E_INTERNAL; // this should never happen, since that would mean bad syntax
    }
}

// Function to get the return type of a node
enum DATA_TYPES getOneNodeType(ASTNodePtr node) {

    switch (node->type) {

        case AST_NODE_VALUE:
            return node->data->value->type;
        case AST_NODE_VARIABLE:
            return node->data->variable->type;
        default:
            return dTypeUndefined;
    }
}

// Function to check if a node is nullable
bool nodeIsNullable(ASTNodePtr node) {

    switch (node->type) {

        case AST_NODE_VALUE:
            return false;
        case AST_NODE_VARIABLE:
            return node->data->variable->nullable;
        default:
            return false;
    }
}

// Function to change the type of a node
void changeNodeToType(ASTNodePtr node, enum DATA_TYPES type) {

    if (node == NULL) return;

    switch (node->type) {

        case AST_NODE_VALUE:
            node->data->value->type = type;
            // in case changing float to int or int to float, change the stored value??
            break;
        case AST_NODE_VARIABLE:
            node->data->variable->type = type;
            break;
        default:
            return;
    }
}

// Function to check if a node is a literal
enum DATA_TYPES getReturnTypeOnOperation(ASTNodePtr one, ASTNodePtr two, ASTNodePtr operand) {
    if (one == NULL || two == NULL || operand == NULL) return dTypeUndefined;

    enum DATA_TYPES oneType = getOneNodeType(one);
    enum DATA_TYPES twoType = getOneNodeType(two);

    // Early rejection for undefined or unsupported types
    if (oneType == dTypeUndefined || twoType == dTypeUndefined) return dTypeUndefined;
    if (oneType == dTypeU8 || twoType == dTypeU8) return dTypeUndefined; // u8 is unsupported

    // Handle cases where one node inherits the type from the other
    if (oneType == dTypeNone) {
        changeNodeToType(one, twoType);
        return twoType;
    }
    if (twoType == dTypeNone) {
        changeNodeToType(two, oneType);
        return oneType;
    }

    // Special handling for division (`/`)
    if (operand->data->operand->type == TOKEN_DIVIDE) {
        if (oneType == dTypeI32 && twoType == dTypeI32) {
            return dTypeI32; // Integer division
        }
        if (oneType == dTypeF64 || twoType == dTypeF64) {
            // Allow implicit conversion of `i32` literals to `f64`
            if (one->type == AST_NODE_VALUE && oneType == dTypeI32) {
                changeNodeToType(one, dTypeF64);
            }
            if (two->type == AST_NODE_VALUE && twoType == dTypeI32) {
                changeNodeToType(two, dTypeF64);
            }
            return dTypeF64; // Floating-point division
        }
        return dTypeUndefined; // Mixed or unsupported types
    }

    if (oneType == twoType) {
        return oneType; // Same types
    }

    // Handling for `+`, `-`, and `*`
    // Handle mixed types
    if ((oneType == dTypeI32 && twoType == dTypeF64) || (oneType == dTypeF64 && twoType == dTypeI32)) {
        // Allow implicit conversion of `i32` literals to `f64`
        if (one->type == AST_NODE_VALUE && oneType == dTypeI32) {
            changeNodeToType(one, dTypeF64);
        }
        if (two->type == AST_NODE_VALUE && twoType == dTypeI32) {
            changeNodeToType(two, dTypeF64);
        }
        return dTypeF64; // Mixed operations result in `f64`
    }
    return dTypeUndefined; // Unsupported mixed types
}


// Function to find the return type of some AST (variable, value, expresion, func call)
enum ERR_CODES __findASTreturnType(ASTNodePtr ast, fnDefinitionsPtr defs, enum DATA_TYPES *returnType, bool *nullable) {

    if (ast == NULL) return E_INTERNAL;

    // value
    if (ast->type == AST_NODE_VALUE) {
        *returnType = ast->data->value->type;
        *nullable = false;
        return SUCCESS;
    }

    // variable
    if (ast->type == AST_NODE_VARIABLE) {
        *returnType = ast->data->variable->type;
        *nullable = ast->data->variable->nullable;
        return SUCCESS;
    }

    // function call
    if (ast->type == AST_NODE_FUNC_CALL) {
        ASTNodePtr funcDef = findFunctionDefinition(defs, ast->data->functionCall->functionName);
        if (funcDef == NULL) return E_SEMANTIC_UND_FUNC_OR_VAR;
        *returnType = funcDef->data->function->returnType;
        *nullable = funcDef->data->function->nullable;
        return SUCCESS;
    }

    // Expresion
    if (ast->type == AST_NODE_EXPRESION) {

        *nullable = false;

        if (!ast->finished) {
            enum ERR_CODES err = ASTfinishExpresion(ast);
            if (err != SUCCESS) return E_INTERNAL;
        }

        LinkedList *output = initLinkedList(false);
        if (output == NULL) return E_INTERNAL;

        // go throught the expresion, and find the return type
        ASTNodePtr oneNode;
        for (unsigned int i = 0; i < getSize(ast->data->expresion->output); i++) {
            oneNode = (ASTNodePtr)getDataAtIndex(ast->data->expresion->output, i);
            if (oneNode == NULL) {
                removeList(&output);
                return E_INTERNAL;
            }

            // if it is value or var, add it to the output stack
            if (oneNode->type == AST_NODE_VALUE || oneNode->type == AST_NODE_VARIABLE) {
                if (!insertNodeAtIndex(output, (void *)oneNode, -1)) {
                    removeList(&output);
                    return E_INTERNAL;
                }
            }

            // in case we have an operand, we pop the first two values, and do some coversion
            if (oneNode->type == AST_NODE_OPERAND) {
                ASTNodePtr one, two;
                if (!popNodeAtIndex(output, 0, (void *)&one)) {
                    removeList(&output);
                    return E_SYNTAX;
                }   
                if (!popNodeAtIndex(output, 0, (void *)&two)) {
                    removeList(&output);
                    return E_SYNTAX;
                }

                enum DATA_TYPES retType = getReturnTypeOnOperation(one, two, oneNode);

                if (nodeIsNullable(one) != nodeIsNullable(two)) {
                    removeList(&output);
                    return E_SEMANTIC_INCOMPATABLE_TYPES;
                }
                   
                if (retType == dTypeUndefined) {
                    removeList(&output);
                    *returnType = dTypeUndefined;
                    return SUCCESS;
                }

                // add the correct type back
                if (retType == getOneNodeType(one)) {
                    if (!insertNodeAtIndex(output, (void *)one, -1)) {
                        removeList(&output);
                        return E_INTERNAL;
                    }
                } else {
                    if (!insertNodeAtIndex(output, (void *)two, -1)) {
                        removeList(&output);
                        return E_INTERNAL;
                    }
                }
            }
        }

        // now the stack should have only one element, which should have the correc ret type
        if (getSize(output) != 1) {
            removeList(&output);
            return E_SYNTAX;
        }

        oneNode = (ASTNodePtr)getDataAtIndex(output, 0);
        if (oneNode == NULL) {
            removeList(&output);
            return E_INTERNAL;
        }

        *returnType = getOneNodeType(oneNode);
        removeList(&output);
        return SUCCESS;
    }

    return E_INTERNAL;
}

// function to validate the declare node
enum ERR_CODES __validateDeclare(ASTNodePtr ast, fnDefinitionsPtr defs) {

    /*
    what can happen here? If i am just declaring, it should be fine, so just return success
    if I am trying to assing some value to the declaration, we should either determin the type of the value, or check if the value is of the correc type
    */

    // check if the value is null
    if (ast->type != AST_NODE_DECLARE) return E_INTERNAL;

    // if value is NULL, return SUCCESS
    if (ast->data->declare->value == NULL) return SUCCESS;

    // now we need to find the return type of the value
    enum DATA_TYPES valueType;
    bool nullable;

    enum ERR_CODES result = __findASTreturnType(ast->data->declare->value, defs, &valueType, &nullable);
    if (result != SUCCESS) return result;

    // now, in case expresion can bu null, but we are tryitn to assign to not nullable value, we error out
    if (nullable && !ast->data->declare->variable->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // now we check type, if the type is dTypeNone, we can just assign the type
    if (ast->data->declare->variable->type == dTypeNone) {
        ast->data->declare->variable->type = valueType;
        return SUCCESS;
    }

    // if the types are not the same, we error out
    if (ast->data->declare->variable->type != valueType) return E_SEMANTIC_INCOMPATABLE_TYPES;

    return SUCCESS;
}

// function to validate the assign node
enum ERR_CODES __validateAssign(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions) {

    // check if the call is valid
    if (ast == NULL || funcDefinitions == NULL) return E_INTERNAL;

    // check for correct node type
    if (ast->type != AST_NODE_ASSIGN) return E_INTERNAL;

    enum DATA_TYPES valueType;
    bool nullable;

    // get the type of the value
    enum ERR_CODES result = __findASTreturnType(ast->data->assign->value, funcDefinitions, &valueType, &nullable);
    if (result != SUCCESS) return result;   

    // now if we are trying to assign to a not nullable value, but the value can be null, we error out
    if (nullable && !ast->data->assign->variable->nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // if the variable type is dTypeNone, we can just assign the type
    if (ast->data->assign->variable->type == dTypeNone) {
        ast->data->assign->variable->type = valueType;
        return SUCCESS;
    }

    // if the types are not the same, we error out
    if (ast->data->assign->variable->type != valueType) return E_SEMANTIC_INCOMPATABLE_TYPES;

    return SUCCESS;
}

// function to validate the return node
enum ERR_CODES __validateReturn(ASTNodePtr ast, ASTNodePtr currentFunc) {

    // check if the call is valid
    if (ast == NULL || currentFunc == NULL) return E_INTERNAL;

    // check for correct node type
    if (ast->type != AST_NODE_RETURN) return E_INTERNAL;

    // if the function is void, we should not return anything
    if (currentFunc->data->function->returnType == dTypeVoid) {

        // check if the return value is NULL
        if (ast->data->returnNode->expression == NULL) return SUCCESS;

        // check for return null?
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // if the function is not void, we should return something
    enum DATA_TYPES valueType;
    bool nullable;
    enum ERR_CODES err = __findASTreturnType(ast->data->returnNode->expression, NULL, &valueType, &nullable);

    if (err != SUCCESS) return err;

    // if the return type can be null, and we dont have null return, we error out
    if (currentFunc->data->function->nullable && !nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // if the return type is not the same as the function return type, we error out
    if (currentFunc->data->function->returnType != valueType) return E_SEMANTIC_INCOMPATABLE_TYPES;

    return SUCCESS;
}

// function to validate the if else node
enum ERR_CODES __validateIfElse(ASTNodePtr ast) {


    // internal err check
    if (ast == NULL) return E_INTERNAL;
    if (ast->type != AST_NODE_IF_ELSE) return E_INTERNAL;   

    // here, we can either have a truth expresion, or two variables
    if ((ast->data->ifElse->condition == NULL) + (ast->data->ifElse->truVar != NULL || ast->data->ifElse->nulVar != NULL) != 1) return E_SYNTAX;

    // handeling condition
    if (ast->data->ifElse->condition != NULL) {

        enum ERR_CODES err1, err2;
        enum DATA_TYPES valueType1, valueType2;
        bool nullable1, nullable2;

        // get the return type of the condition
        err1 = __findASTreturnType(ast->data->ifElse->condition, NULL, &valueType1, &nullable1);
        if (err1 != SUCCESS) return err1;
        err2 = __findASTreturnType(ast->data->ifElse->condition, NULL, &valueType2, &nullable2);
        if (err2 != SUCCESS) return err2;

        // now we have to go about and go along these rules -> 
        /*
        Equality Operators (==, !=):
        Operands of the same type (non-nullable):
        Valid for comparing i32 with i32, or f64 with f64.
        Mixed numeric types (e.g., i32 and f64):
        Valid if at least one operand is a constant/literal and can be implicitly converted without loss.
        Semantic error: If neither operand is a constant/literal, or the conversion results in precision loss.
        Nullable types:
        Nullable types can be compared with other nullable or non-nullable types of the same base type (e.g., ?i32 with i32 or ?i32).
        Semantic error:
        Comparison between completely incompatible types (e.g., ?i32 and f64).
        Nullable types compared to non-nullable types that are not the same base type.
        Relational Operators (<, >, <=, >=):
        Operands of the same type (non-nullable):
        Valid for numeric types (i32 or f64).
        Mixed numeric types:
        Valid if implicit conversion of a literal is possible without precision loss (e.g., f64 > 2 is valid because 2 converts to 2.0).
        Semantic error:
        If implicit conversion results in precision loss (e.g., i32 > 1.5).
        If neither operand is a literal or known at compile time.
        Nullable types:
        Semantic error: Any relational operation with a nullable operand
        */

        // if the types are not the same, we error out

        return SUCCESS; // remove 

    }

}

// function to validate the while node
enum ERR_CODES __validateWhile(ASTNodePtr ast) {

}

// function to validate the function call node
enum ERR_CODES __validateFuncCall(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions) {

}

