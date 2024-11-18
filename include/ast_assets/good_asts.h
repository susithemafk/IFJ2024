/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file good_asts.h
 * @date 20.10. 2024
 * @brief Header file for know ASTs that are used in the tests
 */

#ifndef GOOD_ASTS_H
#define GOOD_ASTS_H

#include <stdbool.h>
#include "semantical/abstract_syntax_tree.h"
#include "semantical/sem_enums.h"
#include "semantical/symtable.h"
#include "utility/enumerations.h"

// ####################### GOOD ASTS #######################
void return_asts_1(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_2(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_3(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_4(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_5(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_6(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_7(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_8(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_9(SymTable *table, LinkedList * returnASts, TestInstancePtr test);
void return_asts_10(SymTable *table, LinkedList * returnASts, TestInstancePtr test);

#endif // GOOD_ASTS_H
