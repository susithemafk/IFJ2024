/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file good_asts.c
 * @date 20.10. 2024
 * @brief main src file for the good ASTs
 */

#include <stdio.h>  
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ast_assets/good_asts.h"
#include "semantical/abstract_syntax_tree.h"
#include "semantical/sem_enums.h"
#include "utility/enumerations.h"

/*
AST 1

pub fn bar(param : []u8) []u8 {
    const r = foo(param);
    return r;
}
*/

/*
AST 2

pub fn foo(par : []u8) []u8 {
    const ret = bar(par);
    return ret;
}
*/

/*
AST 3

pub fn main() void {
    const par = ifj.string("ahoj");
    _ = bar(par);
}
*/

/*
AST 4

pub fn build(x : []u8, y : []u8) []u8 {
    const res = ifj. concat(x, y);
    return res;
}
*/

/*
AST 5

pub fn main() void {
    const a = ifj.string("ahoj ");
    var ct : []u8= ifj.string("svete");
    ct = build(a, ct);
    ifj.write(ct);
}
*/

/*
AST 6

pub fn main() void {
    ifj.write("Zadejte cislo pro vypocet faktorialu\n");
    const a = ifj.readi32();
    if (a) |val| {
        if (val < 0) {
            ifj.write("Faktorial "); 
            ifj.write(val);
            ifj.write(" nelze spocitat\n");
        } else {
            var d: f64= ifj.i2f(val);
            var vysl: f64 = 1.0;
            while (d > 0) {
                vysl = vysl * d;
                d = d - 1.0;
            }
            ifj.write("Vysledek: "); ifj.write(vysl); ifj.write(" = ");
            const vysl_i32 = ifj.f2i(vysl);
            ifj.write(vysl_i32); ifj.write("\n");
        }
    } else { // a == null
        ifj.write("Faktorial pro null nelze spocitat\n");
    }
}
*/

/*
AST 7

pub fn main() void {
    ifj.write("Zadejte cislo pro vypocet faktorialu: ");
    const inp = ifj.readi32();
    if (inp) |INP| {
        if (INP < 0) {
            ifj.write("Faktorial nelze spocitat!\n");
        } else {
            const vysl = factorial(INP);
        ifj.write("Vysledek: "); ifj.write(vysl);
        }
    } else {
        ifj.write("Chyba pri nacitani celeho cisla!\n");
    }
}
*/

/*
AST 8

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}
*/

/*
AST 9

pub fn factorial(n: i32) i32 {
    var result: i32 = 0 - 1;
    if (n < 2) {
        result = 1;
    } else {
        const decremented_n = decrement(n, 1);
        const temp_result = factorial(decremented_n);
        result = n * temp_result;
    }
    return result;
}
*/

/*
AST 10

pub fn main() void {
    const str1 = ifj.string("Toto je text v programu jazyka IFJ24");
    var str2 = ifj.string("
    , ktery jeste trochu obohatime");
    str2 = ifj.concat(str1, str2);
    ifj. write(str1); 
    ifj . write("\n");
    ifj .write(str2); ifj.write("\n");
    ifj.write("Zadejte serazenou posloupnost malych pismen a-h:\n");
    var newInput = ifj.readstr();
    var all: []u8= ifj.string("");
    while (newInput) |inpOK| {
        const abcdefgh = ifj.string("abcdefgh");
        const strcmpResult = ifj.strcmp(inpOK, abcdefgh);
        if (strcmpResult == 0) {
            ifj.write("Spravne zadano!\n");
            ifj.write(all); // vypsat spojene nepodarene vstupy
            newInput = null; // misto break;
        } else {
            ifj.write("Spatne zadana posloupnost, zkuste znovu:\n");
            all = ifj.concat(all, inpOK); // spojuji neplatne vstupy
            newInput = ifj.readstr();
        }
    }
}
*/