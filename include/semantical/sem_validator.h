/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file sem_validator.h
 * @date 25.10.2024
 * @brief Header file for the semantic validator
 */

#ifndef SEM_VALIDATOR_H
#define SEM_VALIDATOR_H

#include <stdbool.h>
#include "utility/linked_list.h" // for creating a stack
#include "semantical/abstract_syntax_tree.h" // for the abstract syntax tree
#include "semantical/symtable.h" // for the function defintions

/**
 * Function to validate one AST
 * 
 * @param ast - pointer to the AST
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
 */
enum ERR_CODES validateAST(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions, ASTNodePtr *currentFunc);

/**
 * Function to get the variable type of a node
 * 
 * @param node - pointer to the AST node
 * @return enum DATA_TYPES
 */
enum DATA_TYPES getOneNodeType(ASTNodePtr node);

/**
 * Function to get the nullable value of a node
 * 
 * @param node - pointer to the AST node
 * @return bool
 */
bool ___nodeIsNullable(ASTNodePtr node);

/**
 * Function to find the return type of some AST (variable, value, expresion, func call)
 * 
 * @param ast - pointer to the AST
 * @param defs - pointer to the function definitions
 * @param returnType - pointer to the return type
*/
enum ERR_CODES ___changeNodeToType(ASTNodePtr node, enum DATA_TYPES type);

/**
 * Function to find out, if a node is a literal
 * 
 * @param ast - pointer to the AST
 * @return bool
*/
bool ___isLiteral(ASTNodePtr node);

/**
 * Function to get the return type of an operation
 * 
 * @param one - pointer to the first node
 * @param two - pointer to the second node
 * @param operand - pointer to the operand
 * @return enum DATA_TYPES
*/
enum DATA_TYPES __getReturnTypeOnOperation(ASTNodePtr one, ASTNodePtr two, ASTNodePtr operand);

/**
 * Function to find out what is the return type of any epxresion
 * 
 * @param expresion - pointer to the expresion
 * @param retType - pointer to the return type
 * @param nullable - pointer to the nullable value
 * @return enum ERR_CODES
*/
enum ERR_CODES ___calculateExpresionType(ASTNodePtr expresion, enum DATA_TYPES *retType, bool *nullable);

/**
 * Function to find out the return type of an AST node
 * 
 * @param ast - pointer to the AST
 * @param defs - pointer to the function definitions
 * @param returnType - pointer to the return type
 * @param nullable - pointer to the nullable value
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __findASTreturnType(ASTNodePtr ast, fnDefinitionsPtr defs, enum DATA_TYPES *returnType, bool *nullable);

/**
 * Function to validate the declare node
 * 
 * @param ast - pointer to the AST
 * @param defs - pointer to the function definitions
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateDeclare(ASTNodePtr ast, fnDefinitionsPtr defs);


/**
 * Function to validate the assign node
 * 
 * @param ast - pointer to the AST
 * @param defs - pointer to the function definitions
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateAssign(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions);

/**
 * Function to validate the if else node
 * 
 * @param ast - pointer to the AST
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateReturn(ASTNodePtr ast, ASTNodePtr currentFunc);

/**
 * Function to validate truth expresion node
 * 
 * @param truthExpression - pointer to the AST
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateTruthExpresion(ASTNodePtr truthExpression);

/**
 * Function to validate while node
 * 
 * @param ast - pointer to the AST
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateIfElse(ASTNodePtr ast);

/**
 * Function to validate while node
 * 
 * @param ast - pointer to the AST
 * @return enum ERR_CODES
*/
enum ERR_CODES __validateWhile(ASTNodePtr ast);

/**
 * Function to validate function call
 * 
 * @param call - pointer to the function call
 * @param funcDef - pointer to the function definition  
 * @return enum ERR_CODES
 * @note this function is internal
*/
enum ERR_CODES __validateFuncCall(ASTNodePtr call, ASTNodePtr funcDef);

#endif // SEM_VALIDATOR_H
