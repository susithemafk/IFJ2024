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
    fnDefinitionsPtr funcDefs; // all the function definitions are here
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
 * @param defs - pointer to the function definitions
 * @return enum ERR_CODES
 */
enum ERR_CODES validateASTs(ASTValidatorPtr validator);
/**
 * Function to free the validator
 * 
 * @param validator - pointer to the validator
 * @return none
 */
void freeValidator(ASTValidatorPtr *validator);

/**
 * Function to validate one AST
 * 
 * @param ast - pointer to the AST
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
 */
enum ERR_CODES _validateAST(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions, ASTNodePtr currentFunc);


/**
 * Function to validate the declare AST
 * 
 * @param ast - pointer to the AST
 * @param funcDefinitions - pointer to the function definitions
 * @return enum ERR_CODES
 */
enum ERR_CODES __validateDeclare(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions);

enum ERR_CODES __validateAssign(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions);

enum ERR_CODES __validateIfElse(ASTNodePtr ast);

enum ERR_CODES __validateWhile(ASTNodePtr ast);

enum ERR_CODES __validateReturn(ASTNodePtr ast, ASTNodePtr currentFunc);

enum ERR_CODES __validateFuncCall(ASTNodePtr ast, fnDefinitionsPtr funcDefinitions);

#endif // SEM_VALIDATOR_H
