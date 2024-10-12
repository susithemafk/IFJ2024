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
#include "semantical/symtable.h"


// ####################### ASTTRACT SYNTAX TREE #######################

// types of nodes in the ASTtract syntax tree
enum ASTNodeTypes {
    AST_NODE_DECLARE, // node for declaration -> var a = 6
    AST_NODE_ASSIGN, // node for assignment -> a = 6
    AST_NODE_TRUTH_EXPRESION, // node for truth expresion -> a == 6
    ASB_NODE_EXPRESION, // node for expresion -> a + 6
    AST_NODE_IF_ELSE, // node for if else statement -> if (a == 6) { ... } else { ... }
    AST_NODE_FUNC_CALL, // node for function call -> func(a, b, c)
    AST_NODE_FUNCTION, // node for function -> func(a, b, c) { ... }
    AST_NODE_WHILE, // node for while loop -> while (a == 6) { ... }
    AST_NODE_VALUE, // node for value -> 6, 3.14, 'a', "string"
};

// >>>>>>>>>>>>>>>>>>> AST NODES START <<<<<<<<<<<<<<<<<<<<

/**
 * @brief Struct for the AST Tree
 * @param root - root of the tree
 * @param current - current node in the tree
 * @param finished - if the tree is finished
*/
typedef struct ASTTree {
    enum ASTNodeTypes type; // type of tree (command, expresion, ...)
    struct ASTNode *root; // root of the tree
    struct ASTNode *current; // current node in the tree
    bool finished; // if the tree is finished
} *ASTTreePtr;


typedef union ASTNodeData {
    struct ASTNodeDeclare *declare; // for declarations
    struct ASTNodeAssign *assign; // for assignments
    struct ASTNodeTruthExpresion *truthExpresion; // for truth expresions
    struct ASTNodeExpresionRoot *expresion; // for expresions
    struct ASTNodeIf *ifElse; // for if else statements
    struct ASTNodeFunctionCall *functionCall; // for function calls
    struct ASTNodeFunction *function; // for functions
    struct ASTNodeWhile *whileLoop; // for while loops
    struct ASTNodeValue *value; // for values
} *ASTNodeDataPtr;

/**
 * @brief Struct for the AST Node
 * @param type - type of the node
 * @param data - a union of the possible data types
*/
typedef struct ASTNode  {
    enum ASTNodeTypes type; // type of the node
    bool finished; // if the node is finished inicializing
    ASTNodeDataPtr data; // data of the node 
} *ASTNodePtr;

/**
 * @brief Struct for the AST Node Declare
 * @param varName - name of the variable
 * @param variable - pointer to the variable in the symbol table
 * @param value - value of the variable (can be a value, expresion, function call, ....)
 * @note this stuct represents a variabel declaration, not the value
*/
typedef struct ASTNodeDeclare {
    char *varName;
    SymVariable *variable; // pointer to the variable in the symbol table
    ASTNodePtr value; // value of the variable (can be a value, expresion, function call, ....)
} *ASTNodeDeclarePtr;

/**
 * @brief Struct for the AST Node Assign
 * @param varName - name of the variable
 * @param variable - pointer to the variable in the symbol table
 * @param value - value of the variable (can be a value, expresion, function call, ....)
 * @note this stuct represents a variabel assignment, not the value
*/
typedef struct ASTNodeValue {
    enum DATA_TYPES type; // type of the value
    char *value; // value of the node
} *ASTNodeValuePtr;

/**
 * @brief Struct for the AST Node Assign
 * @param variable - pointer to the variable in the symbol table, that we are assigning to
 * @param value - value of the variable (can be a value, expresion, function call, ....)
 * @note this stuct represents a variabel assignment, not the value
*/
typedef struct ASTNodeAssign {
    SymVariable *variable; // pointer to the variable in the symbol table, that we are assigning to
    ASTNodePtr value; // value of the variable (can be a value, expresion, function call, ....)
} *ASTNodeAssignPtr;

/**
 * @brief Struct for the AST Node Truth Expresion
 * @param left - left side of the expresion
 * @param right - right side of the expresion
 * @param operator - operator of the expresion
 * @note this stuct represents a truth expresion, not the value
*/
typedef struct ASTNodeTruthExpresion {
    ASTNodePtr left; // left side of the expresion
    ASTNodePtr right; // right side of the expresion
    enum TOKEN_TYPE operator; // operator of the expresion
} *ASTNodeTruthExpresionPtr;

/**
 * @brief Struct for the AST Node Expresion root (used for navigation)
 * @param root - root of the expresion
 * @param prepared - if the expresion is prepared for the code generator
*/
typedef struct ASTNodeExpresionRoot {
    LinkedList *root; // root of the expresion (adding terminals linearly)
    bool prepared; // if the expresion is prepared for the code generator (mby redundant)
} *ASTNodeExpresionRootPtr;

/**
 * @brief Struct for the AST Node Expresion
 * @param left - left side of the expresion
 * @param right - right side of the expresion
 * @param operator - operator of the expresion
*/
typedef struct ASTNodeExpresion {
    ASTNodePtr left; // left side of the expresion
    ASTNodePtr right; // right side of the expresion
    enum TOKEN_TYPE operator; // operator of the expresion
} *ASTNodeExpresionPtr;

/**
 * @brief Struct for the AST Node If 
 * @param condition - condition of the if statement
*/
typedef struct ASTNodeIf {
    ASTNodePtr condition; // condition of the if statement
} *ASTNodeIfPtr;

/**
 * @brief Struct for the AST Node Function Call
 * @param functionName - name of the function
 * @param arguments - list of arguments
*/
typedef struct ASTNodeFunctionCall {
    char *functionName; // name of the function
    LinkedList *arguments; // list of arguments
} *ASTNodeFunctionCallPtr;


/**
 * @brief Struct for the AST Node Function
 * @param functionName - name of the function
 * @param arguments - list of arguments
*/
typedef struct ASTNodeFunction {
    char *functionName; // name of the function
    enum DATA_TYPES returnType; // return type of the function
    LinkedList *arguments; // list of arguments
} *ASTNodeFunctionPtr;

/**
 * @brief Struct for the AST Node While
 * @param condition - condition of the while loop
*/
typedef struct ASTNodeWhile {
    ASTNodePtr condition; // condition of the while loop
} *ASTNodeWhilePtr;

// >>>>>>>>>>>>>>>>>>> AST NODES END <<<<<<<<<<<<<<<<<<<<

// >>>>>>>>>>>>>>>>>>> AST FUNCTIONS START <<<<<<<<<<<<<<<<<<<<

/*
Stuff we need to be able to do
create a new node
append a node at the current active node (choose left or right in expression tree - separate function)
Add a param to a function
Add a param to a function call
Add a condition to an if statement
Add a condition to a while loop
Create a new function call
Create a new function
Create a new while loop
Create a new if statement
Create a new expression
Create a new truth expresion
*/


/**
 * Function to create a new AST Tree
 * 
 * @param type The type of the tree
 * @return The new AST Tree, NULL if an error occurred
*/
ASTNodePtr ASTcreateNode(enum ASTNodeTypes type);

/**
 * Functio to edit Node Value
 * 
 * @param node The node to edit
 * @param value The new value
 * @return erro code
*/
enum ERR_CODES ASTeditNodeValue(ASTNodePtr valueNode, TOKEN_PTR value);

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
enum ERR_CODES ASTaddNodeToExpresion(ASTNodePtr expresionRoot, TOKEN_PTR oneToken);

/**
 * Function to order the expresion for code gen
 * 
 * @param expresionRoot The root of the expresion
 * @return true if successful, false if an error occurred
*/
bool ASTprepareExpresion(ASTNodePtr expresionRoot);

/**
 * Function to add to a truth expresion
 * 
 * @param truthExpresion The truth expresion
 * @param left The left side of the expresion
 * @param right The right side of the expresion
 * @param operator The operator of the expresion
 * @return true if successful, false if an error occurred
 * @note this function can be called multiple times
*/
bool ASTaddNodeToTruthExpresion(ASTNodePtr truthExpresion, ASTNodePtr left, ASTNodePtr right, enum TOKEN_TYPE operator);

/**
 * Function to edit the declaration node
 * 
 * @param declareNode The declare node
 * @param varName The new name of the variable
 * @param variable The new variable in the symbol table
*/
bool ASTeditDeclareNode(ASTNodePtr declareNode, char *varName, ASTNodePtr variable);

/**
 * Function to edit the assign node
 * 
 * @param assignNode The assign node
 * @param variable The variable in the symbol table
 * @param value The value to assign
 * @return true if successful, false if an error occurred
*/
bool ASTeditAssignNode(ASTNodePtr assignNode, SymVariable *variable, ASTNodePtr value);

/**
 * Function to edit the if node
 * 
 * @param ifNode The if node
 * @param condition The condition of the if statement
*/
bool ASTeditIfNode(ASTNodePtr ifNode, ASTNodePtr condition);

/**
 * Function to edit a new AST Node If
 * 
 * @param condition The condition of the if statement
 * @return The new AST Node If, NULL if an error occurred
*/
bool ASTeditIfNode(ASTNodePtr ifNode, ASTNodePtr condition);

/**
 * Function to edit the function call node
 * 
 * @param functionCallNode The function call node
 * @param functionName The name of the function
 * @param arguments The list of arguments
 * @return true if successful, false if an error occurred
 * @note the arguments will be added one by one
*/
bool ASTeditFunctionCallNode(ASTNodePtr functionCallNode, char *functionName, ASTNodePtr argument);

/**
 * Function to edit the function node
 * 
 * @param functionNode The function node
 * @param functionName The name of the function
 * @param returnType The return type of the function
 * @param arguments The list of arguments
 * @return true if successful, false if an error occurred
 * @note the arguments will be added one by one
*/
bool ASTeditFunctionNode(ASTNodePtr functionNode, char *functionName, enum DATA_TYPES returnType, ASTNodePtr argument);

/**
 * Function to edit the while node
 * 
 * @param whileNode The while node
 * @param condition The condition of the while loop
*/
bool ASTeditWhileNode(ASTNodePtr whileNode, ASTNodePtr condition);


/**
 * Function to destroy a node
 * 
 * @param node The node to destroy
*/
bool ASTfreeNode(ASTNodePtr node);


#endif // ASTTRACT_SYNTAX_TREE_H
