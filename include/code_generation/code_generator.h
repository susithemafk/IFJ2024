

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <stdbool.h>
#include <string.h>
#include "semantical/sem_enums.h"
#include "utility/linked_list.h"

/*

Frames

Global frame GF
Local frame LF
Temporary frame TF

*/

// ####################### Code generation #######################

// return adress tracker
typedef struct ReturnAdressTracker {
    LinkedList *returnAdresses;
    unsigned int returnAdressCount;
} *ReturnAdressTrackerPtr;

// ADT for the return code (can have two modes, printing, and storing)
typedef struct ResultCodeHandler {
    char *code;
    bool print;
    struct ReturnAdressTracker *returnAdressTracker;
} *ResultCodeHandlerPtr;

/**
 * Function to generate the prolog
 * 
 * @return The prolog
 */
char *generateProlog(void);

/**
 * Function to create a new result code handler
 * 
 * @param print If the code should be printed
 * @return The new result code handler, NULL if an error occurred
*/
ResultCodeHandlerPtr initResultCodehandler(bool print);

/**
 * Function to free the result code handler
 * 
 * @param resultCodehandler The result code handler
 * @return true if successful, false if an error occurred
*/
bool freeResultCodehandler(ResultCodeHandlerPtr *resultCodehandler);

/**
 * Function to fetch the code
 * 
 * @param resultCodehandler The result code handler
 * @param code The code to fetch
 * @return true if successful, false if an error occurred
*/
bool fetchCode(ResultCodeHandlerPtr resultCodehandler, char **code);   

/**
 * Function to handle new code being generated
 * 
 * @param resultCodehandler The result code handler
 * @param code The code to add
 * @return true if successful, false if an error occurred
*/
bool handleNewCode(ResultCodeHandlerPtr resultCodehandler, char *code);

/**
 * Function to prepare the return adress tracker
 * 
 * @return The new return adress tracker, NULL if an error occurred
*/
ReturnAdressTrackerPtr initReturnAdressTracker(void);

/**
 * Function to free the return adress tracker
 * 
 * @param returnAdressTracker The return adress tracker
 * @return true if successful, false if an error occurred
*/
bool freeReturnAdressTracker(ReturnAdressTrackerPtr *returnAdressTracker);

/**
 * Function to covert AST to code
 * 
 * @param codeGenhHandler The return adress tracker
 * @param currentFucn The current function
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
bool generateCode(ResultCodeHandlerPtr codeGenhHandler, ASTNodePtr *currentFucn, ASTNodePtr ast);

/**
 * Function to convert func definiton to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateFunction(ASTNodePtr ast);

/**
 * Function to convert return node to code
 * 
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateReturn(ASTNodePtr ast);

/**
 * Function to indicate the end of the function block
 * 
 * @return true if successful, false if an error occurred 
 * @note only used for void functions
*/
char *generateEndOfFunction(void);

/**
 * Function to convert declare node to code
 * 
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateDeclare(ASTNodePtr ast);

/**
 * Function to convert assign node to code
 * 
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateAssign(ASTNodePtr ast);

/**
 * Function to convert function call to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
*/
char *_generateFunctionCall(ASTNodePtr ast);

/**
 * Function to convert value node to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateValue(ASTNodePtr ast);

/**
 * Function to convert variable node to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateVariable(enum DATA_TYPES type, char *varName);

/**
 * Function to convert expresion root node to code
 * 
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateExpresionRoot(ASTNodePtr ast);

/**
 * Function to convert truth expresion node to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateTruthExpresion(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast);

/**
 * Function to convert if else node to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateIF(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast);

/**
 * Function to generate the start of the else block
 * 
 * @return true if successful, false if an error occurred
*/
char *_generateElseStart(void);

/**
 * Function to generate the end of the else block
 * 
 * @return true if successful, false if an error occurred
*/
char *_generateEndIfElse(void);

/**
 * Function to convert while node to code
 * 
 * @param returnAdressTracker The return adress tracker
 * @param ast The AST to convert
 * @return true if successful, false if an error occurred
*/
char *_generateWhile(ReturnAdressTrackerPtr returnAdressTracker, ASTNodePtr ast);

/**
 * Function to convert a string, to special asm string
 * 
 * @return The escaped string
*/
char *escapeString(const char *input);


#endif // CODE_GENERATOR_H
