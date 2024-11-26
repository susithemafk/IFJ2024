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
    symEditFuncDef(readstr, "ifj_readstr", dTypeU8, 1);
    symTableAddFunction(table, readstr);

    //pub fn ifj.readi32() ?i32;
    readi32 = symInitFuncDefinition();
    symEditFuncDef(readi32, "ifj_readi32", dTypeI32, 1);
    symTableAddFunction(table, readi32);
   
    //pub fn ifj.readf64() ?f64;
    readf64 = symInitFuncDefinition();
    symEditFuncDef(readf64, "ifj_readf64", dTypeF64, 1);
    symTableAddFunction(table, readf64);

    //pub fn ifj.write(term) void;
    write = symInitFuncDefinition();
    symEditFuncDef(write, "ifj_write", dTypeVoid, 0);
    symAddParamToFunc(write, dTypeNone, false);
    symTableAddFunction(table, write);

    //pub fn ifj.i2f(term: i32) f64;
    i2f = symInitFuncDefinition();
    symEditFuncDef(i2f, "ifj_i2f", dTypeF64, 0);
    symAddParamToFunc(i2f, dTypeI32, false);
    symTableAddFunction(table, i2f);

    //pub fn ifj.f2i(term: f64) i32;
    f2i = symInitFuncDefinition();
    symEditFuncDef(f2i, "ifj_f2i", dTypeI32, 0);
    symAddParamToFunc(f2i, dTypeF64, false);
    symTableAddFunction(table, f2i);
   
    //pub fn ifj.string(term) []u8;
    string = symInitFuncDefinition();
    symEditFuncDef(string, "ifj_string", dTypeU8, 0);
    symAddParamToFunc(string, dTypeNone, false);
    symTableAddFunction(table, string);

    //pub fn ifj.length(s: []u8) i32;
    length = symInitFuncDefinition();
    symEditFuncDef(length, "ifj_length", dTypeI32, 0);
    symAddParamToFunc(length, dTypeU8, false);
    symTableAddFunction(table, length);

    //pub fn ifj.concat(s1: []u8, s2: []u8) []u8;
    concat = symInitFuncDefinition();
    symEditFuncDef(concat, "ifj_concat", dTypeU8, 0);
    symAddParamToFunc(concat, dTypeU8, false);
    symAddParamToFunc(concat, dTypeU8, false);
    symTableAddFunction(table, concat);
   
    //pub fn ifj.substring(s: []u8, i: i32, j: i32) ?[]u8;
    substring = symInitFuncDefinition();
    symEditFuncDef(substring, "ifj_substring", dTypeU8, 1);
    symAddParamToFunc(substring, dTypeU8, false);
    symAddParamToFunc(substring, dTypeI32, false);
    symAddParamToFunc(substring, dTypeI32, false);
    symTableAddFunction(table, substring);
  
    //pub fn ifj.strcmp(s1: []u8, s2: []u8) i32;
    Strcmp = symInitFuncDefinition();
    symEditFuncDef(Strcmp, "ifj_strcmp", dTypeI32, 0);
    symAddParamToFunc(Strcmp, dTypeU8, false);
    symAddParamToFunc(Strcmp, dTypeU8, false);
    symTableAddFunction(table, Strcmp);

    //pub fn ifj.ord(s: []u8, i: i32) i32;
    ord = symInitFuncDefinition();
    symEditFuncDef(ord, "ifj_ord", dTypeI32, 0);
    symAddParamToFunc(ord, dTypeU8, false);
    symAddParamToFunc(ord, dTypeI32, false);
    symTableAddFunction(table, ord);
   
    //pub fn ifj.chr(i: i32) []u8;
    chr = symInitFuncDefinition();
    symEditFuncDef(chr, "ifj_chr", dTypeU8, 0);
    symAddParamToFunc(chr, dTypeI32, false);
    symTableAddFunction(table, chr);
   
}
