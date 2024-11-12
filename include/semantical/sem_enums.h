/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file sem_enums.h
 * @date 18.10.2024
 * @brief enumerations for semantic analysis
 */

#ifndef SEMANTICAL_ENUMS_H
#define SEMANTICAL_ENUMS_H

#include <stdbool.h>
#include "utility/binary_search_tree.h"
#include "utility/linked_list.h"
#include "utility/enumerations.h"

// missing -> return and continue

// types of nodes in the ASTtract syntax tree
enum ASTNodeTypes {
    AST_NODE_DECLARE, // node for declaration -> var a = 6
    AST_NODE_ASSIGN, // node for assignment -> a = 6
    AST_NODE_TRUTH_EXPRESION, // node for truth expresion -> a == 6
    AST_NODE_EXPRESION, // node for expresion -> a + 6
    AST_NODE_IF_ELSE, // node for if else statement -> if (a == 6) { ... } else { ... }
    AST_NODE_FUNC_CALL, // node for function call -> func(a, b, c)
    AST_NODE_FUNCTION, // node for function -> func(a, b, c) { ... }
    AST_NODE_WHILE, // node for while loop -> while (a == 6) { ... }
    AST_NODE_VALUE, // node for value -> 6, 3.14, 'a', "string"
    AST_NODE_VARIABLE, // node for variable -> a
    AST_NODE_OPERAND, // node for operand -> +, -, *, /, ...
    AST_NODE_RETURN, // node for return -> return a
    AST_NODE_ELSE_START, // node for else start
    AST_BLOCK_END // node for block end
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

/**
 * @brief Struct for the AST Node
 * @param type - type of the node
 * @param data - a union of the possible data types
 * @note this struct is used for all the nodes in the AST
 * @note the data is a union of all the possible data types
*/
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
    struct ASTNodeReturn *returnNode; // for return nodes
    struct SymVariable *variable; // for variables
    struct TOKEN *operand; // for operands
    bool elseStart; // for else start
    bool blockEnd; // for if, function and while end
} *ASTNodeDataPtr;

/**2
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
 * @brief Struct for the AST Node return
 * @param expression - the return expression
*/
typedef struct ASTNodeReturn {
    struct ASTNode *expression; // the return expression
} *ASTNodeReturnPtr;

/**
 * @brief Struct for the AST Node Declare
 * @param varName - name of the variable
 * @param variable - pointer to the variable in the symbol table
 * @param value - value of the variable (can be a value, expresion, function call, ....)
 * @note this stuct represents a variabel declaration, not the value
*/
typedef struct ASTNodeDeclare {
    struct SymVariable *variable; // pointer to the variable in the symbol table
    struct ASTNode *value; // value of the variable (can be a value, expresion, function call, ....)
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
    struct SymVariable *variable; // pointer to the variable in the symbol table, that we are assigning to
    struct ASTNode *value; // value of the variable (can be a value, expresion, function call, ....)
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
    LinkedList *output; // root of the expresion (adding terminals linearly)
    LinkedList *operators; // root of the expresion (adding operators linearly)
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
    ASTNodePtr condition; // condition for the if statement, should be a truth expresion
    struct SymVariable *truVar; // variable, that has to be nullabale, in this case it can be used instaid of the condition 
    struct SymVariable *nulVar; // new variable, only accasable in the if and else block, has to be not be nullable
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
    bool nullable; // if the function can return null
    LinkedList *arguments; // list of arguments
} *ASTNodeFunctionPtr;

/**
 * @brief Struct for the AST Node While
 * @param condition - condition of the while loop
*/
typedef struct ASTNodeWhile {
    ASTNodePtr condition; // condition of the while loop
    struct SymVariable *truVar; // variable, that has to be nullabale, in this case it can be used instaid of the condition
    struct SymVariable *nulVar; // new variable, only accasable in the while block, has to be not be nullable    
} *ASTNodeWhilePtr;

// >>>>>>>>>>>>>>>>>>> AST NODES END <<<<<<<<<<<<<<<<<<<<

#endif // SEMANTICAL_ENUMS_H
