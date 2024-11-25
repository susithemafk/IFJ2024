/** AUTHOR
 * 
 * @author  <247581> Martin Mendl  
 * @author <xsvobov00> Veronika Svobodová
 * @file parser_pass1.h
 * @date 17.11. 2024
 * @brief header file for the first pass of the syntaxical parser
 */

#ifndef PARSER_PASS1_H
#define PARSER_PASS1_H

#include <stdbool.h>
#include <string.h>
#include "semantical/symtable.h"
#include "utility/linked_list.h"
#include "lexical/scanner.h"


// ####################### PARSER #######################

/** 
 * Function to do the first pass over the program
 * @param symtable - the symbol table
 * @return the buffer of tokens
 */
enum ERR_CODES firstPass(SymTable *table, FILE *input, LinkedList *buffer);

/**
 * Function to save a new token to the buffer
 * 
 * @param token - the token to save
 * @param buffer - the buffer to save the token to
 * @return bool - if the token was saved correctly
 */
bool saveNewToken(struct TOKEN token, LinkedList *buffer);

/**
 * Funcion to parse a function definition
 * 
 * @param buffer - the buffer of tokens
 * @param table - the symbol table
 * @return enum ERR_CODES
*/
enum ERR_CODES pubfn(LinkedList *buffer, SymTable *table);

/**
 * Function to parse a function call
 * 
 * @param buffer - the buffer of tokens
 * @param table - the symbol table
 * @return enum ERR_CODES
*/
void freeBuffer(LinkedList **buffer);


#endif // PARSER_H
