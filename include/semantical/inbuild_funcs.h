/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file inbuild_funcs.h
 * @date 20.10. 2024
 * @brief header file for the inbuild functions in zig in AST form
*/

#ifndef INBUILD_FUNCS_H
#define INBUILD_FUNCS_H

#include <stdbool.h>
#include <stdlib.h>
#include "semantical/symtable.h"
#include "utility/enumerations.h"

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

*/

enum ERR_CODES err;

SymFunctionPtr readstr;
SymFunctionPtr readi32;
SymFunctionPtr readf64;
SymFunctionPtr write;
SymFunctionPtr i2f;
SymFunctionPtr f2i;
SymFunctionPtr string;
SymFunctionPtr length;
SymFunctionPtr concat;
SymFunctionPtr substring;
SymFunctionPtr Strcmp;
SymFunctionPtr ord;
SymFunctionPtr chr;
SymFunctionPtr var;

void fillInBuildInFuncions(SymTable *table);

#endif // INBUILD_FUNCS_H
