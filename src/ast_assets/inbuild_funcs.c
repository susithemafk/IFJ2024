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
#include "ast_assets/inbuild_funcs.h"
#include "semantical/abstract_syntax_tree.h"
#include "semantical/sem_enums.h"
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

SymVariable write_var = {4, "write", dTypeNone, false, false, true};
SymVariable term1 = {1, "term", dTypeI32, false, false, true};
SymVariable term2 = {2, "term", dTypeF64, false, false, true};
SymVariable term3 = {3, "term", dTypeU8, false, false, true};
SymVariable s = {4, "s", dTypeU8, false, false, true};
SymVariable s1 = {5, "s1", dTypeU8, false, false, true};
SymVariable s2 = {6, "s2", dTypeU8, false, false, true};
SymVariable i = {7, "i", dTypeI32, false, false, true};
SymVariable j = {8, "j", dTypeI32, false, false, true};


void fillInBuildInFuncions(fnDefinitionsPtr validator) {

    //pub fn ifj.readstr() ?[]u8;
    readstr = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(readstr, "ifj.readstr", dTypeU8, 1, NULL);
    addFunctionDefinition(validator, readstr);

    //pub fn ifj.readi32() ?i32;
    readi32 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(readi32, "ifj.readi32", dTypeI32, 1, NULL);
    addFunctionDefinition(validator, readi32);

    //pub fn ifj.readf64() ?f64;
    readf64 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(readf64, "ifj.readf64", dTypeF64, 1, NULL);
    addFunctionDefinition(validator, readf64);

    //pub fn ifj.write(term) void;
    write = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(write, "ifj.write", dTypeVoid, 0, &write_var);


    //pub fn ifj.i2f(term: i32) f64;
    i2f = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(i2f, "ifj.i2f", dTypeF64, 0, &term1);
    addFunctionDefinition(validator, i2f);

    //pub fn ifj.f2i(term: f64) i32;
    f2i = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(f2i, "ifj.f2i", dTypeI32, 0, &term2);
    addFunctionDefinition(validator, f2i);

    //pub fn ifj.string(term) []u8;
    string = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(string, "ifj.string", dTypeU8, 0, &term3);
    addFunctionDefinition(validator, string);

    //pub fn ifj.length(s: []u8) i32;
    length = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(length, "ifj.length", dTypeI32, 0, &s);
    addFunctionDefinition(validator, length);

    //pub fn ifj.concat(s1: []u8, s2: []u8) []u8;
    concat = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(concat, "ifj.concat", dTypeU8, 0, &s1);
    err = ASTeditFunctionNode(concat, NULL, dTypeUndefined, -1, &s2);
    addFunctionDefinition(validator, concat);

    //pub fn ifj.substring(s: []u8, i: i32, j: i32) ?[]u8;
    substring = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(substring, "ifj.substring", dTypeU8, 1, &s);
    err = ASTeditFunctionNode(substring, NULL, dTypeUndefined, -1, &i);
    err = ASTeditFunctionNode(substring, NULL, dTypeUndefined, -1, &j);
    addFunctionDefinition(validator, substring);

    //pub fn ifj.strcmp(s1: []u8, s2: []u8) i32;
    Strcmp = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(Strcmp, "ifj.strcmp", dTypeI32, 0, &s1);
    err = ASTeditFunctionNode(Strcmp, NULL, dTypeUndefined, -1, &s2);
    addFunctionDefinition(validator, Strcmp);

    //pub fn ifj.ord(s: []u8, i: i32) i32;
    ord = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(ord, "ifj.ord", dTypeI32, 0, &s);
    err = ASTeditFunctionNode(ord, NULL, dTypeUndefined, -1, &i);
    addFunctionDefinition(validator, ord);

    //pub fn ifj.chr(i: i32) []u8;
    chr = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(chr, "ifj.chr", dTypeU8, 0, &i);
    addFunctionDefinition(validator, chr);
}
