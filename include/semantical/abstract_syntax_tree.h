/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file abstract_syntax_tree.h
 * @date 28.9. 2024
 * @brief Header file for the symbol tree
 */

#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include <stdbool.h>
#include "utility/enumerations.h"
#include "utility/linked_list.h"
#include "semantical/sem_enums.h"

// ####################### ASTTRACT SYNTAX TREE #######################

/**
 * Function to create a new AST Tree
 * 
 * @param type The type of the tree
 * @return The new AST Tree, NULL if an error occurred
*/
ASTNodePtr ASTcreateNode(enum ASTNodeTypes type);

/**
 * Function to init Node operand
 * 
 * @param node The node to init
 * @param operand The operand
 * @return erro code
*/
enum ERR_CODES ASTinitNodeOperand(ASTNodePtr operandNode, TOKEN_PTR operand);

/**
 * Function to init Node Value
 * 
 * @param node The node to init
 * @param value The new value
 * @return erro code
*/
enum ERR_CODES ASTinitNodeValue(ASTNodePtr valueNode, TOKEN_PTR value);

/**
 * Function to init Node Variable
 * 
 * @param node the node to init
 * @param declaration pointer to the declaration node in the symbol table
 * @return err code
*/
enum ERR_CODES ASTinitNodeVariable(ASTNodePtr variableNode, struct SymVariable *variable);

/**
 * Function to add to an expresion
 * 
 * @param expresionRoot The root of the expresion
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @param operand The operand
 * @return err code
 * @note this function expects, that only one of the arguments is not NULL, the other two are NULL
*/
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand);

/**
 * Function to finish the expresion
 * 
 * @param expresionRoot The root of the expresion
 * @return err code
*/
enum ERR_CODES ASTfinishExpresion(ASTNodePtr expresionRoot);

/**
 * Function to add to a truth expresion
 * 
 * @param truthExpresion The truth expresion
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @param operand The operand
 * @return err code
 * @note the function expect only one of the 3 args to not be NULL
 * @note if the operand is ==, !=, >=, <=, >, <, the operand will be negated and saved, and switch from adding to the left of expresion and adding to the right
*/
enum ERR_CODES ASTeditTruthExpresion(ASTNodePtr truthExpresion, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand);

/**
 * Function to edit the declaration node
 * @param declareNode The declare node
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @return err codes
*/
enum ERR_CODES ASTeditDeclareNode(ASTNodePtr declareNode, struct SymVariable *variable, ASTNodePtr value);

/**
 * Function to edit the assign node
 * 
 * @param assignNode The assign node
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @return true if successful, false if an error occurred
*/
enum ERR_CODES ASTeditAssignNode(ASTNodePtr assignNode, struct SymVariable *variable, ASTNodePtr value);

/**
 * Function to edit the if node
 * 
 * @param ifNode The if node
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @param operand The operand
 * @param truVar The variable to assign if the condition is true
 * @param nulVar The variable to assign if the condition is false
 * @return err codes
 * @note the function expect, that you will either set one of these 3 args to not be NULl -> (varibale, value, operand)
 * @note or you will set one or both of the truVar and nulVar to not be NULL
 * @note if you are setting one of the 3 args, it works just like in editing truth expresion
 * 
*/
enum ERR_CODES ASTeditIfNode(ASTNodePtr ifNode, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand, struct SymVariable *truVar, struct SymVariable *nulVar);

/**
 * Function to edit the function call node
 * 
 * @param functionCallNode The function call node
 * @param functionName The name of the function
 * @param varArg The variable argument
 * @param valueArg The value argument
 * @return err codes
 * @note the arguments will be added one by one, the function expects, that only one of the varArg and valueArg is not NULL
*/
enum ERR_CODES ASTeditFunctionCallNode(ASTNodePtr functionCallNode, char *functionName, struct SymVariable *varArg, TOKEN_PTR valueArg);

/**
 * Function to edit the function node
 * 
 * @param functionNode The function node
 * @param functionName The name of the function
 * @param returnType The return type of the function
 * @param nullable The nullable value of the function
 * @param argument one argument of the function
 * @return err codes
 * @note the arguments will be added one by one
*/
enum ERR_CODES ASTeditFunctionNode(ASTNodePtr functionNode, char *functionName, enum DATA_TYPES returnType, int nullable, struct SymVariable *argument);

/**
 * Function to edit the while node
 * 
 * @param whileNode The while node
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @param operand The operand
 * @param truVar The variable to assign if the condition is true
 * @param nulVar The variable to assign if the condition is false
 * @return err codes
 * @note the function expect, that you will either set one of these 3 args to not be NULl -> (varibale, value, operand)
 * @note or you will set one or both of the truVar and nulVar to not be NULL
 * @note if you are setting one of the 3 args, it works just like in editing truth expresion
*/
enum ERR_CODES ASTeditWhileNode(ASTNodePtr whileNode, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand, struct SymVariable *truVar, struct SymVariable *nulVar);

/**
 * Function to edit the return node
 * 
 * @param returnNode The return node
 * @param expresionPart The return expresion
 * @return err codes
*/
enum ERR_CODES ASTeditReturnNode(ASTNodePtr returnNode, struct SymVariable *variable, TOKEN_PTR value, TOKEN_PTR operand);

/**
 * Function to destroy a node
 * 
 * @param node The node to destroy
*/
bool ASTfreeNode(ASTNodePtr *nodePtr);



#endif // ASTTRACT_SYNTAX_TREE_H
