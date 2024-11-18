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
#include <string.h>
#include "semantical/sem_validator.h"
#include "semantical/sem_enums.h"
#include "utility/linked_list.h"
#include "semantical/abstract_syntax_tree.h"
#include "semantical/symtable.h"
#include "utility/binary_search_tree.h"

// ####################### Semantic validation #######################

// Function to validate the AST
enum ERR_CODES validateAST(ASTNodePtr ast, SymTable *table, ASTNodePtr *currentFunc) {

    // check if the call is valid
    if (ast == NULL || table == NULL) {
        #ifdef DEBUG
        DEBUG_MSG("validateAST - invalid call");
        #endif
        return E_INTERNAL;
    }

    SymFunctionPtr funDef;

    // check the type of the AST

    // functions definitions should not be saved to the stack, it should be symttable -> function definitions
    switch (ast->type) {
        case AST_NODE_DECLARE:
            return __validateDeclare(ast, table);
        case AST_NODE_ASSIGN:
            return __validateAssign(ast, table);
        case AST_NODE_IF_ELSE:
            return __validateIfElse(ast);
        case AST_NODE_WHILE:
            return __validateWhile(ast);
        case AST_NODE_RETURN:
            return __validateReturn(ast, *currentFunc);
        case AST_NODE_FUNCTION:
            // if we are entering a function, set the current function
            *currentFunc = ast;
            return SUCCESS;
        case AST_NODE_FUNC_CALL:
            // if a function call is here, it needs to have a runn type of None, so the definitons should have the return type None
            funDef = symTableFindFunction(table, ast->data->functionCall->functionName);
            if (funDef == NULL) return E_SEMANTIC_UND_FUNC_OR_VAR;
            if (funDef->returnType != dTypeVoid) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return __validateFuncCall(ast, funDef);
        default:
            return E_INTERNAL; // this should never happen, since that would mean bad syntax
    }
}

// Function to get the return type of a node
enum DATA_TYPES ___getOneNodeType(ASTNodePtr node) {

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
bool ___nodeIsNullable(ASTNodePtr node) {

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
enum ERR_CODES ___changeNodeToType(ASTNodePtr node, enum DATA_TYPES type) {
    if (node == NULL) return E_INTERNAL;
    if (type == dTypeUndefined || type == dTypeNone || type == dTypeU8) return E_INTERNAL;

    switch (node->type) {
        case AST_NODE_VALUE:
            // incompatable types
            if (node->data->value->type == dTypeU8) {
                #ifdef DEBUG
                DEBUG_MSG("changing type of u8 value");
                #endif
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            };

            // No change needed
            if (node->data->value->type == type) return SUCCESS;

            // Convert int literal to float
            if (node->data->value->type == dTypeI32 && type == dTypeF64) {
                char *oldValue = node->data->value->value;
                size_t oldLen = strlen(oldValue);
                char *newValue = malloc(oldLen + 3); // Enough for ".0" and null terminator
                if (newValue == NULL) return E_INTERNAL;

                snprintf(newValue, oldLen + 3, "%s.0", oldValue);
                free(oldValue); // Free old value
                node->data->value->value = newValue;
                node->data->value->type = dTypeF64;
                return SUCCESS;
            } 

            // Convert float literal to int
            if (node->data->value->type == dTypeF64 && type == dTypeI32) {
                char *oldValue = node->data->value->value;
                char *dotPosition = strchr(oldValue, '.');

                // No decimal point, invalid float format
                if (dotPosition == NULL)  return E_SEMANTIC_INCOMPATABLE_TYPES;

                // Check if all characters after the decimal point are '0'
                char *p = dotPosition + 1;
                while (*p == '0') p++;

                // Cannot safely convert
                if (*p != '\0')   return E_SEMANTIC_INCOMPATABLE_TYPES;

                // If we only found '0's or nothing after '.'
                size_t newLen = dotPosition - oldValue;
                char *newValue = malloc(newLen + 1); // Enough for truncated value and null terminator
                if (newValue == NULL) return E_INTERNAL;

                strncpy(newValue, oldValue, newLen);
                newValue[newLen] = '\0'; // Null-terminate the string

                free(oldValue); // Free old value
                node->data->value->value = newValue;
                node->data->value->type = dTypeI32;
                return SUCCESS;
            } 
            #ifdef DEBUG
            DEBUG_MSG("unsupported conversion");
            #endif
            // Unsupported conversion
            return E_SEMANTIC_INCOMPATABLE_TYPES;

        case AST_NODE_VARIABLE:
            if (node->data->variable->type == dTypeU8) {
                #ifdef DEBUG
                DEBUG_MSG("trying to change type of u8 variable");
                #endif
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }

            // if the type has not been set yet, we can covert it.
            if (node->data->variable->type == dTypeNone) {
                node->data->variable->type = type;  
                return SUCCESS;
            }   
            // if the type is allready set, we cannot change it
            if (node->data->variable->type != type) {
                #ifdef DEBUG
                DEBUG_MSG("trying to change type of a variable that has allready been set");
                #endif
                return E_SEMANTIC_INCOMPATABLE_TYPES;
            }
            // pointless call in this case, where type before and after as the same, but whatever
            return SUCCESS;

        // conversion of other types is not supported
        default:
            #ifdef DEBUG
            DEBUG_MSG("this should never be seen. Unsupported type conversion");
            #endif
            return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    return E_INTERNAL;
}

// Function to check if a node is a literal
bool ___isLiteral(ASTNodePtr node) {
    return node && node->type == AST_NODE_VALUE; // Adjust for your AST structure
}

// Function to check if a node is a literal
enum DATA_TYPES __getReturnTypeOnOperation(ASTNodePtr one, ASTNodePtr two, ASTNodePtr operand) {
    // early rejection
    if (one == NULL || two == NULL || operand == NULL) return dTypeUndefined;

    enum DATA_TYPES oneType = ___getOneNodeType(one);
    enum DATA_TYPES twoType = ___getOneNodeType(two);
    enum ERR_CODES err;

    // Early rejection for unsupported or undefined types
    if (oneType == dTypeUndefined || twoType == dTypeUndefined) return dTypeUndefined;
    if (oneType == dTypeU8 || twoType == dTypeU8) return dTypeUndefined; // u8 is unsupported
    if (oneType == dTypeNone || twoType == dTypeNone) return dTypeNone; // Both are undefined

    // If types are the same, operation is valid
    if (oneType == twoType) return oneType;

    // Handle mixed `i32` and `f64` types
    if ((oneType == dTypeI32 && twoType == dTypeF64) || (oneType == dTypeF64 && twoType == dTypeI32)) {
        // Convert the literal operand to match the variable's type
        if (___isLiteral(one) && oneType == dTypeI32) {
            err = ___changeNodeToType(one, dTypeF64);
            return (err == SUCCESS) ? dTypeF64 : dTypeUndefined;
        }
        if (___isLiteral(two) && twoType == dTypeI32) {
            err = ___changeNodeToType(two, dTypeF64);
            return (err == SUCCESS) ? dTypeF64 : dTypeUndefined;
        }
        if (___isLiteral(one) && oneType == dTypeF64) {
            err = ___changeNodeToType(one, dTypeI32);
            return (err == SUCCESS) ? dTypeI32 : dTypeUndefined;
        }
        if (___isLiteral(two) && twoType == dTypeF64) {
            err = ___changeNodeToType(two, dTypeI32);
            return (err == SUCCESS) ? dTypeI32 : dTypeUndefined;
        }

        // If neither operand is a literal, operation fails
        return dTypeUndefined;
    }

    // If types are incompatible
    return dTypeUndefined;
}


// Function to find out what is the return type of any epxresion
enum ERR_CODES ___calculateExpresionType(ASTNodePtr expresion, enum DATA_TYPES *retType, bool *nullable) {

    // internal err check
    if (expresion == NULL) return E_INTERNAL;

    // finish the expresion
    if (!expresion->finished) {
        enum ERR_CODES err = ASTfinishExpresion(expresion);
        if (err != SUCCESS) return err;
    }

    LinkedList *postFixExpresion = expresion->data->expresion->output;

    LinkedList *stack = initLinkedList(false);
    if (stack == NULL) return E_INTERNAL;

    // if we have just one value in the epxresion, it can also be null, and the expresion is auto valid
    if (getSize(postFixExpresion) == 1) {
        ASTNodePtr node = (ASTNodePtr)getDataAtIndex(postFixExpresion, 0);
        *nullable = ___nodeIsNullable(node);
        *retType = ___getOneNodeType(node);
    }

    // go throught the expresion, 
    ASTNodePtr node, val1, val2;
    
    for (unsigned int i = 0; i < getSize(postFixExpresion); i++) {
        // read the node form left to right
        node = (ASTNodePtr)getDataAtIndex(postFixExpresion, i);

        switch (node->type) {
            // if it is a value or a variable, we just push it to the stack
            case AST_NODE_VARIABLE:
            case AST_NODE_VALUE:
                if (!insertNodeAtIndex(stack, (void *)node, 0)) {
                    removeList(&stack);
                    return E_INTERNAL;
                }
                break;

            // if it is an operand, we do stack[1] operand stack[0]
            case AST_NODE_OPERAND:
                // get the val1
                if (!popNodeAtIndex(stack, 0, (void *)&val2)) {
                    removeList(&stack);
                    return E_SYNTAX;
                }

                // get the val2
                if (!popNodeAtIndex(stack, 0, (void *)&val1)) {
                    removeList(&stack);
                    return E_SYNTAX;
                }

                // handle literal conversions
                enum DATA_TYPES retType = __getReturnTypeOnOperation(val1, val2, node);
                if (retType == dTypeUndefined) {
                    removeList(&stack);
                    return E_SEMANTIC_INCOMPATABLE_TYPES;
                }

                // oeprating on nullable types
                if (___nodeIsNullable(val1) || ___nodeIsNullable(val2)) {
                    removeList(&stack);
                    return E_SEMANTIC_INCOMPATABLE_TYPES;
                }

                // now we need to save the result back to the stack, choose the val, which has the same type as the result
                if (retType == ___getOneNodeType(val1)) {
                    if (!insertNodeAtIndex(stack, (void *)val1, 0)) {
                        removeList(&stack);
                        return E_INTERNAL;
                    }
                } else {
                    if (!insertNodeAtIndex(stack, (void *)val2, 0)) {
                        removeList(&stack);
                        return E_INTERNAL;
                    }
                }
                break;
            default:
                removeList(&stack);
                return E_INTERNAL;

        }
    }

    // now the stack should have only one element, which should have the correc ret type
    if (getSize(stack) != 1) {
        removeList(&stack);
        return E_SYNTAX;
    }

    node = (ASTNodePtr)getDataAtIndex(stack, 0);
    if (node == NULL) {
        removeList(&stack);
        return E_INTERNAL;
    }

    *retType = ___getOneNodeType(node);
    *nullable = false; // exprsion with more than one value cannot be null
    removeList(&stack);
    return SUCCESS;
}

// Function to find the return type of some AST (variable, value, expresion, func call)
enum ERR_CODES __findASTreturnType(ASTNodePtr ast, SymTable *table, enum DATA_TYPES *returnType, bool *nullable) {

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
        SymFunctionPtr funDef = symTableFindFunction(table, ast->data->functionCall->functionName);
        if (funDef == NULL) return E_SEMANTIC_UND_FUNC_OR_VAR;
        enum ERR_CODES err = __validateFuncCall(ast, funDef);
        if (err != SUCCESS) {
            #ifdef DEBUG
            DEBUG_MSG("failed to validate function call paramaters");
            #endif
            return err;
        }

        *returnType = funDef->returnType;
        *nullable = funDef->nullableReturn;
        return SUCCESS;
    }

    // Expresion
    if (ast->type == AST_NODE_EXPRESION) return ___calculateExpresionType(ast, returnType, nullable);
    return E_INTERNAL;
}

// function to validate the declare node
enum ERR_CODES __validateDeclare(ASTNodePtr ast, SymTable *table) {

    // check if the value is null
    if (ast->type != AST_NODE_DECLARE) return E_INTERNAL;

    // if value is NULL, return SUCCESS
    if (ast->data->declare->value == NULL) return SUCCESS;

    // now we need to find the return type of the value
    enum DATA_TYPES valueType;
    bool nullable;
    enum ERR_CODES result = __findASTreturnType(ast->data->declare->value, table, &valueType, &nullable);
    if (result != SUCCESS) {
        #ifdef DEBUG
        DEBUG_MSG("failed to find return type of the value");
        #endif
        return result;
    }

    // now, in case expresion can bu null, but we are tryitn to assign to not nullable value, we error out
    if (nullable && !ast->data->declare->variable->nullable) {
        #ifdef DEBUG
        DEBUG_MSG("nullable value assigned to not nullable variable");
        #endif
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // now we check type, if the type is dTypeNone, we can just assign the type
    if (ast->data->declare->variable->type == dTypeNone) {
        if (ast->data->declare->variable->id) ast->data->declare->variable->type = valueType; // ignore the _ variable
        return SUCCESS;
    }

    // if the types are not the same, we error out
    if (ast->data->declare->variable->type != valueType) {
        #ifdef DEBUG
        DEBUG_MSG("types are not the same");
        #endif
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }
    return SUCCESS;
}

// function to validate the assign node
enum ERR_CODES __validateAssign(ASTNodePtr ast, SymTable *table) {

    // check if the call is valid
    if (ast == NULL || table == NULL) return E_INTERNAL;

    // check for correct node type
    if (ast->type != AST_NODE_ASSIGN) return E_INTERNAL;

    enum DATA_TYPES valueType;
    bool nullable;

    // get the type of the value
    enum ERR_CODES result = __findASTreturnType(ast->data->assign->value, table, &valueType, &nullable);
    if (result != SUCCESS) {
        #ifdef DEBUG
        DEBUG_MSG("failed to find return type of the value");
        #endif
        return result;
    }

    // now if we are trying to assign to a not nullable value, but the value can be null, we error out
    if (nullable && !ast->data->assign->variable->nullable) {
        #ifdef DEBUG
        DEBUG_MSG("nullable value assigned to not nullable variable");
        #endif
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // if the variable type is dTypeNone, we can just assign the type
    if (ast->data->assign->variable->type == dTypeNone) {
        if (ast->data->assign->variable->id) ast->data->assign->variable->type = valueType; // ingnore the _ variable
        return SUCCESS;
    }

    // if the types are not the same, we error out
    if (ast->data->assign->variable->type != valueType) {
        #ifdef DEBUG
        DEBUG_MSG("types are not the same");
        #endif
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }
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
        return E_SEMANTIC_INCOMPATABLE_TYPES;
    }

    // if the function is not void, we should return something
    enum DATA_TYPES valueType;
    bool nullable;
    enum ERR_CODES err = __findASTreturnType(ast->data->returnNode->expression, NULL, &valueType, &nullable);

    if (err != SUCCESS) return err;

    // if the return type cant be null, and we have nullable return, we error out
    if (!currentFunc->data->function->nullable && nullable) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // if the return type is not the same as the function return type, we error out
    if (currentFunc->data->function->returnType != valueType) return E_SEMANTIC_INCOMPATABLE_TYPES;

    return SUCCESS;
}

// funciton to validate if a truth expreison is valid or not
enum ERR_CODES __validateTruthExpresion(ASTNodePtr truthExpression) {

    bool leftNullable, rightNullable;
    enum DATA_TYPES leftType, rightType;
    enum ERR_CODES err;

    // get the return type and nullable of the left side
    err = __findASTreturnType(truthExpression->data->truthExpresion->left, NULL, &leftType, &leftNullable);
    if (err != SUCCESS) return err;
    
    // get the return type and nullable of the right side
    err = __findASTreturnType(truthExpression->data->truthExpresion->right, NULL, &rightType, &rightNullable);
    if (err != SUCCESS) return err;

    unsigned int sizeLeft, sizeRight;
    sizeLeft = getSize(truthExpression->data->truthExpresion->left->data->expresion->output);
    sizeRight = getSize(truthExpression->data->truthExpresion->right->data->expresion->output);
    ASTNodePtr leftExp, rightExp;

    // handle nullability
    switch (truthExpression->data->truthExpresion->operator) {
        case TOKEN_GREATERTHAN:
        case TOKEN_GREATEROREQUAL:
        case TOKEN_LESSOREQUAL:
        case TOKEN_LESSTHAN:
            // if any of the types is nullable, operation is invalid
            if (leftNullable || rightNullable) return E_SEMANTIC_INCOMPATABLE_TYPES;
            break;
            // for equality operators, we can have nullable types
        default:
            break;
    }

    // type checking is the same for all operators
    // expresions on both sidex, types need to match
    if (sizeLeft != 1 && sizeRight != 1) {
        if (leftType != rightType) return E_SEMANTIC_INCOMPATABLE_TYPES;
        return SUCCESS;
    }
    // if any one of the types is not i32 and f64, we error out
    if (leftType != dTypeI32 && leftType != dTypeF64) return E_SEMANTIC_INCOMPATABLE_TYPES;
    if (rightType != dTypeI32 && rightType != dTypeF64) return E_SEMANTIC_INCOMPATABLE_TYPES;

    // if both sidex are of size 1
    if (sizeLeft == 1 && sizeRight == 1) {
        leftExp = (ASTNodePtr)getDataAtIndex(truthExpression->data->truthExpresion->left->data->expresion->output, 0);
        rightExp = (ASTNodePtr)getDataAtIndex(truthExpression->data->truthExpresion->right->data->expresion->output, 0);

        // both are values
        if (leftExp->type == AST_NODE_VALUE && rightExp->type == AST_NODE_VALUE) {
            // try to convert to higher type
            if (leftType == rightType)  return SUCCESS;
            if (leftType == dTypeI32) return ___changeNodeToType(leftExp, dTypeF64);
            return ___changeNodeToType(rightExp, dTypeF64);
        }

        // both are variables
        if (leftExp->type == AST_NODE_VARIABLE && rightExp->type == AST_NODE_VARIABLE) {
            if (leftType != rightType) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;
        }

        // left is value, right is variable
        if (leftExp->type == AST_NODE_VALUE) return ___changeNodeToType(leftExp, rightType);

        // right is value, left is variable
        if (rightExp->type == AST_NODE_VALUE) return ___changeNodeToType(rightExp, leftType);

        // to make the compiler happy
        return E_INTERNAL;
    }

    // one side is expresion, the other is not
    if (sizeLeft == 1) {
        leftExp = (ASTNodePtr)getDataAtIndex(truthExpression->data->truthExpresion->left->data->expresion->output, 0);
        if (leftExp->type == AST_NODE_VARIABLE) {
            if (leftType != rightType) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;
        } 
        // left side is value
        if (leftType != rightType) {
            // left side is i32, try to convert to f64
            if (leftType == dTypeI32) return ___changeNodeToType(leftExp, dTypeF64);
            // left side is f64, try to convert to i32
            return ___changeNodeToType(leftExp, dTypeI32);
        }
    }

    // right side is expresion, the other is not
    if (sizeRight == 1) {
        rightExp = (ASTNodePtr)getDataAtIndex(truthExpression->data->truthExpresion->right->data->expresion->output, 0);
        if (rightExp->type == AST_NODE_VARIABLE) {
            if (leftType != rightType) return E_SEMANTIC_INCOMPATABLE_TYPES;
            return SUCCESS;
        } 
        // right side is value
        if (leftType != rightType) {
            // right side is i32, try to convert to f64
            if (rightType == dTypeI32) return ___changeNodeToType(rightExp, dTypeF64);
            // right side is f64, try to convert to i32
            return ___changeNodeToType(rightExp, dTypeI32);
        }
    }

    // to make the compiler happy
    return E_INTERNAL;
}

// function to validate the if else node
enum ERR_CODES __validateIfElse(ASTNodePtr ast) {

    // internal err check
    if (ast == NULL) return E_INTERNAL;
    if (ast->type != AST_NODE_IF_ELSE) return E_INTERNAL;   

    // here, we can either have a truth expresion, or two variables
    if ((ast->data->ifElse->condition == NULL) + (ast->data->ifElse->truVar != NULL || ast->data->ifElse->nulVar != NULL) != 1) return E_SYNTAX;

    // handeling condition
    if (ast->data->ifElse->condition != NULL) return __validateTruthExpresion(ast->data->ifElse->condition);

    // handeling variables
    if (ast->data->ifElse->truVar == NULL || ast->data->ifElse->nulVar == NULL) return E_SYNTAX;

    return SUCCESS;
}

// function to validate the while node
enum ERR_CODES __validateWhile(ASTNodePtr ast) {

    // internal err check
    if (ast == NULL) return E_INTERNAL;
    if (ast->type != AST_NODE_WHILE) return E_INTERNAL;

    // here, we can either have a truth expresion, or two variables
    if ((ast->data->whileLoop->condition == NULL) + (ast->data->whileLoop->truVar != NULL || ast->data->whileLoop->nulVar != NULL) != 1) return E_SYNTAX;

    // handeling condition
    if (ast->data->whileLoop->condition != NULL) return __validateTruthExpresion(ast->data->whileLoop->condition);

    // handeling variables
    if (ast->data->whileLoop->truVar == NULL || ast->data->whileLoop->nulVar == NULL) return E_SYNTAX;

    return SUCCESS;
}

// function to validate the function call node
enum ERR_CODES __validateFuncCall(ASTNodePtr call, SymFunctionPtr funcDef) {

    // check if the call is valid
    if (call == NULL || funcDef == NULL) return E_INTERNAL;

    // check for correct node type
    if (call->type != AST_NODE_FUNC_CALL) return E_INTERNAL;

    // validate the arguments types
    unsigned int defSize = getSize(funcDef->paramaters);

    if (defSize != getSize(call->data->functionCall->arguments)) {
        #ifdef DEBUG
        DEBUG_MSG("invalid number of arguments");
        #endif
        return E_SEMANTIC_INVALID_FUN_PARAM;
    }
    SymFunctionParamPtr defArg;
    ASTNodePtr callArg;
    enum DATA_TYPES defArgType, callArgType;
    bool defArgNullable, callArgNullable;

    // check if the types of the args are the same
    for (unsigned int i = 0; i < defSize; i++) {
        defArg = (SymFunctionParamPtr)getDataAtIndex(funcDef->paramaters, i);
        callArg = (ASTNodePtr)getDataAtIndex(call->data->functionCall->arguments, i);

        // get the types and the nullability of the args
        defArgType = defArg->type;
        callArgType = ___getOneNodeType(callArg);

        defArgNullable = defArg->nullable;
        callArgNullable = ___nodeIsNullable(callArg);

        // if the argument is not nullable, and the call arg is nullable, we error out
        if (!defArgNullable && callArgNullable) {
            #ifdef DEBUG
            DEBUG_MSG("nullable value assigned to not nullable variable");
            printf("call arg nullable: %d\n", callArgNullable);
            printf("def arg nullable: %d\n", defArgNullable);
            #endif
            return E_SEMANTIC_INVALID_FUN_PARAM;
        }

        // if the argument is not defined int he function (for inbuild functions) we return success
        if (defArgType == dTypeNone) continue;

        // if both types are the same, we have a match
        if (defArgType == callArgType) continue;;

        // if both args are variables, we error out, since cannot convert variables
        if (callArg->type == AST_NODE_VARIABLE) {
            #ifdef DEBUG
            DEBUG_MSG("cannot convert variables");
            #endif
            return E_SEMANTIC_INVALID_FUN_PARAM;
        }

        // in this case, if both args are neither i32 nor f64 we have a probelm
        if (defArgType != dTypeI32 && defArgType != dTypeF64) {
            #ifdef DEBUG
            DEBUG_MSG("invalid function argument type in definition, one is not i32 or f64");
            #endif
            return E_SEMANTIC_INVALID_FUN_PARAM;
        }
        if (callArgType != dTypeI32 && callArgType != dTypeF64) {
            #ifdef DEBUG
            DEBUG_MSG("invalid function argument type in call, one is not i32 or f64");
            #endif
            return E_SEMANTIC_INVALID_FUN_PARAM;
        }
        
        // in case we have a variable, that is not inited, or that does not have a type, we know, we have an unknown type, se we err out
        if (callArgType != dTypeI32 && callArgType != dTypeF64) {
            #ifdef DEBUG
            DEBUG_MSG("unknown type");
            #endif
            return E_SEMANTIC_UNKNOWN_TYPE;
        }

        // now, if one of the args is a value, we can try to conver it to the other type
        if (callArgType == dTypeI32 && defArgType == dTypeF64) return ___changeNodeToType(callArg, dTypeF64);

        // same goes if call arg is f64 and def arg is i32
        if (callArgType == dTypeF64 && defArgType == dTypeI32) return ___changeNodeToType(callArg, dTypeI32);

        #ifdef DEBUG
        DEBUG_MSG("should never be here?");
        #endif
        // if we are here, we have a problem
        return E_SEMANTIC_INVALID_FUN_PARAM;
    }

    return SUCCESS;
}

