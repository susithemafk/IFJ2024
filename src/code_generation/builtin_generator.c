/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 21.10.2024
 * @brief Implementation of the builtin functions generator
 */
#include <stdio.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "code_generation/builtin_generator.h"
#else
#include "builtin_generator.h"
#endif

#define PRINTLN(...)                                                                                                                                                                                   \
    do {                                                                                                                                                                                               \
        printf(__VA_ARGS__);                                                                                                                                                                           \
        printf("\n");                                                                                                                                                                                  \
    } while (0)

void generateFuncWrite(void) {
    PRINTLN("LABEL function_$ifj_write");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_print");
    PRINTLN("POPS TF@to_print");
    PRINTLN("WRITE TF@to_print");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}

void generateFuncReadStr(void) {
    PRINTLN("LABEL function_$ifj_readstr");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read string");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncReadInt(void) {
    PRINTLN("LABEL function_$ifj_readi32");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read int");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncReadFloat(void) {
    PRINTLN("LABEL function_$ifj_readf64");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read float");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}

void generateFuncInt2Float(void) {
    PRINTLN("LABEL function_$ifj_i2f");
    PRINTLN("INT2FLOATS");
    PRINTLN("RETURN");
}
void generateFuncFloat2Int(void) {
    PRINTLN("LABEL function_$ifj_f2i");
    PRINTLN("FLOAT2INTS");
    PRINTLN("RETURN");
}
void generateFuncString(void) {
    PRINTLN("LABEL function_$ifj_string");
    PRINTLN("RETURN");
}
void generateFuncLength(void) {
    PRINTLN("LABEL function_$ifj_length");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@str");
    PRINTLN("POPS TF@str");
    PRINTLN("DEFVAR TF@len");
    PRINTLN("STRLEN TF@len TF@str");
    PRINTLN("PUSHS TF@len");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncConcat(void) {
    PRINTLN("LABEL function_$ifj_concat");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@str1");
    PRINTLN("DEFVAR TF@str2");
    PRINTLN("DEFVAR TF@result");

    PRINTLN("POPS TF@str2");
    PRINTLN("POPS TF@str1");

    PRINTLN("CONCAT TF@result TF@str1 TF@str2");
    PRINTLN("PUSHS TF@result");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncSubStr(void) {
    PRINTLN("LABEL function_$ifj_substring");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@str");
    PRINTLN("DEFVAR TF@start");
    PRINTLN("DEFVAR TF@end");

    PRINTLN("POPS TF@end");
    PRINTLN("POPS TF@start");
    PRINTLN("POPS TF@str");

    PRINTLN("DEFVAR TF@help");

    PRINTLN("LT TF@help TF@start int@0");
    PRINTLN("JUMPIFEQ function_$ifj_substring_end TF@help bool@true");

    PRINTLN("LT TF@help TF@end int@0");
    PRINTLN("JUMPIFEQ function_$ifj_substring_end TF@help bool@true");

    PRINTLN("GT TF@help TF@start TF@end");
    PRINTLN("JUMPIFEQ function_$ifj_substring_end TF@help bool@true");

    PRINTLN("DEFVAR TF@len");
    PRINTLN("STRLEN TF@len TF@str");

    PRINTLN("LT TF@help TF@start TF@len");
    PRINTLN("JUMPIFEQ function_$ifj_substring_end TF@help bool@false");

    PRINTLN("GT TF@help TF@end TF@len");
    PRINTLN("JUMPIFEQ function_$ifj_substring_end TF@help bool@true");

    PRINTLN("DEFVAR TF@substr");
    PRINTLN("MOVE TF@substr string@");

    PRINTLN("LABEL function_$ifj_substring_cycle");
    PRINTLN("LT TF@help TF@start TF@end");
    PRINTLN("JUMPIFNEQ function_$ifj_substring_return TF@help bool@true");

    PRINTLN("GETCHAR TF@help TF@str TF@start");
    PRINTLN("CONCAT TF@substr TF@substr TF@help");
    PRINTLN("ADD TF@start TF@start int@1");
    PRINTLN("JUMP function_$ifj_substring_cycle");

    PRINTLN("LABEL function_$ifj_substring_return");
    PRINTLN("PUSHS TF@substr");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");

    PRINTLN("LABEL function_$ifj_substring_end");
    PRINTLN("PUSHS nil@nil");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncStrCompare(void) {
    PRINTLN("LABEL function_$ifj_strcmp");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@str1");
    PRINTLN("DEFVAR TF@str2");

    PRINTLN("POPS TF@str2");
    PRINTLN("POPS TF@str1");

    PRINTLN("DEFVAR TF@index");
    PRINTLN("MOVE TF@index int@0");
    PRINTLN("DEFVAR TF@len1");
    PRINTLN("DEFVAR TF@len2");

    PRINTLN("STRLEN TF@len1 TF@str1");
    PRINTLN("STRLEN TF@len2 TF@str2");

    PRINTLN("DEFVAR TF@help");
    PRINTLN("DEFVAR TF@char1");
    PRINTLN("DEFVAR TF@char2");

    PRINTLN("LABEL function_$ifj_strcmp_cycle");
    PRINTLN("LT TF@help TF@index TF@len1");
    PRINTLN("JUMPIFEQ function_$ifj_strcmp_str1_end TF@help bool@false");

    PRINTLN("LT TF@help TF@index TF@len2");
    PRINTLN("JUMPIFEQ function_$ifj_strcmp_str2_less TF@help bool@false");

    PRINTLN("GETCHAR TF@char1 TF@str1 TF@index");
    PRINTLN("GETCHAR TF@char2 TF@str2 TF@index");

    PRINTLN("LT TF@help TF@char1 TF@char2");
    PRINTLN("JUMPIFEQ function_$ifj_strcmp_str1_less TF@help bool@true");

    PRINTLN("GT TF@help TF@char1 TF@char2");
    PRINTLN("JUMPIFEQ function_$ifj_strcmp_str2_less TF@help bool@true");

    PRINTLN("ADD TF@index TF@index int@1");
    PRINTLN("JUMP function_$ifj_strcmp_cycle");

    PRINTLN("LABEL function_$ifj_strcmp_str1_end");
    PRINTLN("LT TF@help TF@len1 TF@len2");
    PRINTLN("JUMPIFEQ function_$ifj_strcmp_str1_less TF@help bool@true");
    PRINTLN("JUMP function_$ifj_strcmp_equal");

    PRINTLN("LABEL function_$ifj_strcmp_str1_less");
    PRINTLN("PUSHS int@-1");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");

    PRINTLN("LABEL function_$ifj_strcmp_str2_less");
    PRINTLN("PUSHS int@1");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");

    PRINTLN("LABEL function_$ifj_strcmp_equal");
    PRINTLN("PUSHS int@0");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncStrOrd(void) {
    PRINTLN("LABEL function_$ifj_ord");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@str");
    PRINTLN("DEFVAR TF@index");

    PRINTLN("POPS TF@index");
    PRINTLN("POPS TF@str");

    PRINTLN("DEFVAR TF@len");
    PRINTLN("STRLEN TF@len TF@str");

    PRINTLN("DEFVAR TF@help");
    PRINTLN("LT TF@help TF@index int@0");
    PRINTLN("JUMPIFEQ function_$ifj_ord_end TF@help bool@true");

    PRINTLN("LT TF@help TF@index TF@len");
    PRINTLN("JUMPIFEQ function_$ifj_ord_end TF@help bool@false");

    PRINTLN("STRI2INT TF@help TF@str TF@index");
    PRINTLN("PUSHS TF@help");

    PRINTLN("POPFRAME");
    PRINTLN("RETURN");

    PRINTLN("LABEL function_$ifj_ord_end");
    PRINTLN("PUSHS int@0");
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}
void generateFuncStrChr(void) {
    PRINTLN("LABEL function_$ifj_chr");
    PRINTLN("INT2CHARS");
    PRINTLN("RETURN");
}

void generateBuiltins(void) {
    PRINTLN("\n# Builtin functions\n");

    generateFuncWrite();
    generateFuncReadStr();
    generateFuncReadInt();
    generateFuncReadFloat();
    generateFuncInt2Float();
    generateFuncFloat2Int();
    generateFuncString();
    generateFuncLength();
    generateFuncConcat();
    generateFuncSubStr();
    generateFuncStrCompare();
    generateFuncStrOrd();
    generateFuncStrChr();
}
