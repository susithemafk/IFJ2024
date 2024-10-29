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

/*
what we need to do:

- have a function to save an AST in the stack
- have a function to validate an AST using the function call definitions

*/

// ####################### Validator #######################

typedef struct ASTValidator {
    LinkedList *stack; // stak of the ASTs
} *ASTValidatorPtr;

/**
 * Function to initialize the validator
 * 
 * @param funcDefintions - pointer to the finished function definitions BST
 * @return pointer to the initialized validator
 */
ASTValidatorPtr initValidator(void);

/**
 * Function to add an AST to the stack
 * 
 * @param validator - pointer to the validator
 * @param ast - pointer to the AST
 * @return enum ERR_CODES
 */
enum ERR_CODES addASTToStack(ASTValidatorPtr validator, ASTNodePtr ast);

/**
 * Function to validate all the ASTs in the stack
 * 
 * @param validator - pointer to the validator
 * @param functionDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
 */
enum ERR_CODES validateASTs(ASTValidatorPtr validator, BST *functionDefinitions);

/**
 * Function to free the validator
 * 
 * @param validator - pointer to the validator
 * @return none
 */
void freeValidator(ASTValidatorPtr *validator);

/**
 * Function to validate AST for truth Expresion node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES _validateTruthExpresion(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to validate, if value and variable are of the same type
 * 
 * @param value - pointer to the value node
 * @param variable - pointer to the variable node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES __validateValueType(ASTNodePtr value, struct SymVariable *variable, BST *funcDefinitions);

/**
 * Function to validate AST for declare node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES _validateDeclare(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to validate AST for assign node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES _validateAssign(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to validate AST for if node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES _validateIfElse(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to validate AST for while node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
*/
enum ERR_CODES _validateWhile(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to validate any AST node
 * 
 * @param ast - pointer to the root AST node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
 * @note a large switch case for the AST node types
*/
enum ERR_CODES _validateAST(ASTNodePtr ast, BST *funcDefinitions);

/**
 * Function to check if expression is valid and return the resulting type
 * 
 * @param expression - pointer to the expression node
 * @param funcDefinitions - pointer to the function definitions
 * @return enum DATA_TYPES
*/
enum ERR_CODES _checkExpresionType(ASTNodePtr expression, BST *funcDefinitions, enum DATA_TYPES *result);

/**
 * Function to return the resulting type of an operation
 * 
 * @param left - left operand
 * @param right - right operand
 * @param operator - operator
 * @param result - pointer to the result err code
 * @return pointer to the resulting node, with the correct type
*/
ASTNodePtr _getOperationResultType(ASTNodePtr left, ASTNodePtr right, ASTNodePtr operator, BST *functionDefinitions, enum ERR_CODES *result);


/**
 * Function to free the stack, and its contects
 * 
 * @param stack - pointer to the stack
 * @return none
*/
void _freeStack(LinkedList *stack);

#endif // SEM_VALIDATOR_H
