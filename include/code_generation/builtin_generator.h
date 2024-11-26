/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Declarations of the builtin functions generator
 */

#ifndef BUILTIN_GENERATOR_H
#define BUILTIN_GENERATOR_H


void generateFuncWrite(void);
void generateFuncReadStr(void);
void generateFuncReadInt(void);
void generateFuncReadFloat(void);
void generateFuncInt2Float(void);
void generateFuncFloat2Int(void);
void generateFuncString(void);
void generateFuncLength(void);
void generateFuncConcat(void);
void generateFuncSubStr(void);
void generateFuncStrCompare(void);
void generateFuncStrOrd(void);
void generateFuncStrChr(void);

void generateBuiltins(void);

#endif // BUILTIN_GENERATOR_H
