/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the builtin functions generator
 */

#ifndef BUILTIN_GENERATOR_H
#define BUILTIN_GENERATOR_H

/**
 * Generates the built-in function for writing output.
 */
void generateFuncWrite(void);

/**
 * Generates the built-in function for reading a string input.
 */
void generateFuncReadStr(void);

/**
 * Generates the built-in function for reading an integer input.
 */
void generateFuncReadInt(void);

/**
 * Generates the built-in function for reading a float input.
 */
void generateFuncReadFloat(void);

/**
 * Generates the built-in function for converting an integer to a float.
 */
void generateFuncInt2Float(void);

/**
 * Generates the built-in function for converting a float to an integer.
 */
void generateFuncFloat2Int(void);

/**
 * Generates the built-in function for handling string operations.
 */
void generateFuncString(void);

/**
 * Generates the built-in function for calculating the length of a string.
 */
void generateFuncLength(void);

/**
 * Generates the built-in function for concatenating two strings.
 */
void generateFuncConcat(void);

/**
 * Generates the built-in function for extracting a substring from a string.
 */
void generateFuncSubStr(void);

/**
 * Generates the built-in function for comparing two strings.
 */
void generateFuncStrCompare(void);

/**
 * Generates the built-in function for getting the ordinal value of a character in a string.
 */
void generateFuncStrOrd(void);

/**
 * Generates the built-in function for getting the character from an ordinal value.
 */
void generateFuncStrChr(void);

/**
 * Generates all the built-in functions.
 */
void generateBuiltins(void);

#endif // BUILTIN_GENERATOR_H
