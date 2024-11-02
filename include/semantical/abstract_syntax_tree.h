/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file symbol_tree.h
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
enum ERR_CODES ASTinitNodeOperand(ASTNodePtr operandNode, struct TOKEN operand);

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
 * @param node The node to add
 * @param operator The operator of the expresion
 * @return err code
 * @note The function will try to fill in the expresion from left to right, in case 
 * it is supposed add to operand to the left/right side of the epxresion, it will add a new nodee
*/
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, ASTNodePtr expresionPart);

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
 * @param left The left side of the expresion
 * @param right The right side of the expresion
 * @param opr The operator of the expresion
 * @return err code
*/
enum ERR_CODES ASTeditTruthExpresion(ASTNodePtr truthExpresion, ASTNodePtr expresionPart);

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
 * @param condition The condition of the if statement
*/
enum ERR_CODES ASTeditIfNode(ASTNodePtr ifNode, ASTNodePtr conditionPart);

/**
 * Function to edit the function call node
 * 
 * @param functionCallNode The function call node
 * @param functionName The name of the function
 * @param arguments The list of arguments
 * @return err codes
 * @note the arguments will be added one by one
*/
enum ERR_CODES ASTeditFunctionCallNode(ASTNodePtr functionCallNode, char *functionName, ASTNodePtr argument);

/**
 * Function to edit the function node
 * 
 * @param functionNode The function node
 * @param functionName The name of the function
 * @param returnType The return type of the function
 * @param arguments The list of arguments
 * @return err codes
 * @note the arguments will be added one by one
*/
enum ERR_CODES ASTeditFunctionNode(ASTNodePtr functionNode, char *functionName, enum DATA_TYPES returnType, int nullable, ASTNodePtr argument);

/**
 * Function to edit the while node
 * 
 * @param whileNode The while node
 * @param condition The condition of the while loop
*/
enum ERR_CODES ASTeditWhileNode(ASTNodePtr whileNode, ASTNodePtr condition);

/**
 * Function to edit the return node
 * 
 * @param returnNode The return node
 * @param expresionPart The return expresion
 * @return err codes
*/
enum ERR_CODES ASTeditReturnNode(ASTNodePtr returnNode, ASTNodePtr expresionPart);

/**
 * Function to destroy a node
 * 
 * @param node The node to destroy
*/
bool ASTfreeNode(ASTNodePtr *nodePtr);



#endif // ASTTRACT_SYNTAX_TREE_H
