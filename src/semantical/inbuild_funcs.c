/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file inbuild_funcs.c
 * @date 20.10. 2024
 * @brief main src file for the inbuild functions in zig in AST form
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "semantical/inbuild_funcs.h"
#include "utility/enumerations.h"
#include "semantical/symtable.h"

/*
// Functions for reading values:
pub fn ifj.readstr() ?[]u8;
pub fn ifj.readi32() ?i32;
pub fn ifj.readf64() ?f64;

// Functions for outputting values:
pub fn ifj.write(term) void;

// Built-in functions for number type conversion:
pub fn ifj.i2f(term: i32) f64;
pub fn ifj.f2i(term: f64) i32;

// Built-in functions for working with slices (string manipulation):
pub fn ifj.string(term) []u8;
pub fn ifj.length(s: []u8) i32;
pub fn ifj.concat(s1: []u8, s2: []u8) []u8;
pub fn ifj.substring(s: []u8, i: i32, j: i32) ?[]u8;
pub fn ifj.strcmp(s1: []u8, s2: []u8) i32;
pub fn ifj.ord(s: []u8, i: i32) i32;
pub fn ifj.chr(i: i32) []u8;
SymVariable write_var = {4, "write", DATA_TYPE_VOID, false, true, false};

*/

SymVariable write_var = {4, "write", dTypeNone, false, false, true, true};
SymVariable term1 = {1, "term", dTypeI32, false, false, true, true};
SymVariable term2 = {2, "term", dTypeF64, false, false, true, true};
SymVariable term3 = {3, "term", dTypeU8, false, false, true, true};
SymVariable s = {4, "s", dTypeU8, false, false, true, true};
SymVariable s1 = {5, "s1", dTypeU8, false, false, true, true};
SymVariable s2 = {6, "s2", dTypeU8, false, false, true, true};
SymVariable i = {7, "i", dTypeI32, false, false, true, true};
SymVariable j = {8, "j", dTypeI32, false, false, true, true};

void fillInBuildInFuncions(SymTable* table) {

    //pub fn ifj.readstr() ?[]u8;
    readstr = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(readstr, "ifj.readstr", dTypeU8, true);
    symTableAddFunction(table, readstr);

    //pub fn ifj.readi32() ?i32;
    readi32 = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(readi32, "ifj.readi32", dTypeI32, true);
    symTableAddFunction(table, readi32);
   
    //pub fn ifj.readf64() ?f64;
    readf64 = symInitFuncDefinition();
    symEditFuncDef(readf64, "ifj.readf64", dTypeF64, true);
    symTableAddFunction(table, readf64);

    //pub fn ifj.write(term) void;
    write = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(write, "ifj.write", dTypeVoid, false);
=======
    symEditFuncDef(write, "ifj_write", dTypeVoid, 0);
>>>>>>> code_gen
    symAddParamToFunc(write, dTypeNone, false);
    symTableAddFunction(table, write);

    //pub fn ifj.i2f(term: i32) f64;
    i2f = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(i2f, "ifj.i2f", dTypeF64, false);
=======
    symEditFuncDef(i2f, "ifj_i2f", dTypeF64, 0);
>>>>>>> code_gen
    symAddParamToFunc(i2f, dTypeI32, false);
    symTableAddFunction(table, i2f);

    //pub fn ifj.f2i(term: f64) i32;
    f2i = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(f2i, "ifj.f2i", dTypeI32, false);
=======
    symEditFuncDef(f2i, "ifj_f2i", dTypeI32, 0);
>>>>>>> code_gen
    symAddParamToFunc(f2i, dTypeF64, false);
    symTableAddFunction(table, f2i);
   
    //pub fn ifj.string(term) []u8;
    string = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(string, "ifj.string", dTypeU8, false);
=======
    symEditFuncDef(string, "ifj_string", dTypeU8, 0);
>>>>>>> code_gen
    symAddParamToFunc(string, dTypeNone, false);
    symTableAddFunction(table, string);

    //pub fn ifj.length(s: []u8) i32;
    length = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(length, "ifj.length", dTypeI32, false);
=======
    symEditFuncDef(length, "ifj_length", dTypeI32, 0);
>>>>>>> code_gen
    symAddParamToFunc(length, dTypeU8, false);
    symTableAddFunction(table, length);

    //pub fn ifj.concat(s1: []u8, s2: []u8) []u8;
    concat = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(concat, "ifj.concat", dTypeU8, false);
=======
    symEditFuncDef(concat, "ifj_concat", dTypeU8, 0);
>>>>>>> code_gen
    symAddParamToFunc(concat, dTypeU8, false);
    symAddParamToFunc(concat, dTypeU8, false);
    symTableAddFunction(table, concat);
   
    //pub fn ifj.substring(s: []u8, i: i32, j: i32) ?[]u8;
    substring = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(substring, "ifj.substring", dTypeU8, false);
=======
    symEditFuncDef(substring, "ifj_substring", dTypeU8, 1);
>>>>>>> code_gen
    symAddParamToFunc(substring, dTypeU8, false);
    symAddParamToFunc(substring, dTypeI32, false);
    symAddParamToFunc(substring, dTypeI32, false);
    symTableAddFunction(table, substring);
  
    //pub fn ifj.strcmp(s1: []u8, s2: []u8) i32;
    Strcmp = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(Strcmp, "ifj.strcmp", dTypeI32, false);
=======
    symEditFuncDef(Strcmp, "ifj_strcmp", dTypeI32, 0);
>>>>>>> code_gen
    symAddParamToFunc(Strcmp, dTypeU8, false);
    symAddParamToFunc(Strcmp, dTypeU8, false);
    symTableAddFunction(table, Strcmp);

    //pub fn ifj.ord(s: []u8, i: i32) i32;
    ord = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(ord, "ifj.ord", dTypeI32, false);
=======
    symEditFuncDef(ord, "ifj_ord", dTypeI32, 0);
>>>>>>> code_gen
    symAddParamToFunc(ord, dTypeU8, false);
    symAddParamToFunc(ord, dTypeI32, false);
    symTableAddFunction(table, ord);
   
    //pub fn ifj.chr(i: i32) []u8;
    chr = symInitFuncDefinition();
<<<<<<< HEAD
    symEditFuncDef(chr, "ifj.chr", dTypeU8, false);
=======
    symEditFuncDef(chr, "ifj_chr", dTypeU8, 0);
>>>>>>> code_gen
    symAddParamToFunc(chr, dTypeI32, false);
    symTableAddFunction(table, chr);
   
}
