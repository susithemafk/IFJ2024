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
#include "semantical/symtable.h"
#include "utility/enumerations.h"


/*
AST 1

pub fn bar(param : []u8) []u8 {
    const r = foo(param);
    return r;
}
*/
void return_asts_1(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {

    bool result;
    enum ERR_CODES err;

    // pub fn bar(param : []u8) []u8 { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 1 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    // create the function node
    ASTNodePtr function1 = ASTcreateNode(AST_NODE_FUNCTION);

    if (test != NULL) {
        testCase(
            test,
            function1 != NULL,
            "AST 1 - ASTcreateNode",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    // declare the params of the function
    SymVariable *param1 = symTableDeclareVariable(table, "param", dTypeU8, false, -1); // param : []u8

    if (test != NULL) {
        testCase(
            test,
            param1 != NULL,
            "AST 1 - symtbale declare param",
            "Symtable declared param",
            "Symtable failed to declare param"
        );
    }

    // edit the function node (name, return type, nullable, params)
    err = ASTeditFunctionNode(function1, "bar", dTypeU8, 0, param1); // pub fn bar(param : []u8) []u8

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    // save the AST to the buffer
    insertNodeAtIndex(returnASts, (void *)function1, -1);

    // const r = foo(param);

    ASTNodePtr declare1 = ASTcreateNode(AST_NODE_DECLARE);

    if (test != NULL) {
        testCase(
            test,
            declare1 != NULL,
            "AST 1 - ASTcreateNode declare",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *r1 = symTableDeclareVariable(table, "r", dTypeNone, false, -1); // const r    

    if (test != NULL) {
        testCase(
            test,
            r1 != NULL,
            "AST 1 - symtbale declare r",
            "Symtable declared r",
            "Symtable failed to declare r"
        );
    }

    err = ASTeditDeclareNode(declare1, r1, NULL); // const r = 

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr call1 = ASTcreateNode(AST_NODE_FUNC_CALL);

    if (test != NULL) {
        testCase(
            test,
            call1 != NULL,
            "AST 1 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    // get the variable
    SymVariable *var_param = symTableFindVariable(table, "param");

    if (test != NULL) {
        testCase(
            test,
            var_param != NULL,
            "AST 1 - symtbale find param",
            "Symtable found param",
            "Symtable failed to find param"
        );
    }

    err = ASTeditFunctionCallNode(call1, "foo", var_param, NULL); // foo(param)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditDeclareNode(declare1, NULL, call1); // const r = foo(param)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    // save the AST to the buffer
    insertNodeAtIndex(returnASts, (void *)declare1, -1);

    // return r;
    ASTNodePtr return1 = ASTcreateNode(AST_NODE_RETURN);

    if (test != NULL) {
        testCase(
            test,
            return1 != NULL,
            "AST 1 - ASTcreateNode return",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable * var_r = symTableFindVariable(table, "r");

    if (test != NULL) {
        testCase(
            test,
            var_r != NULL,
            "AST 1 - symtbale find r",
            "Symtable found r",
            "Symtable failed to find r"
        );
    }

    err = ASTeditReturnNode(return1, var_r, NULL, NULL); // return r

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }
    insertNodeAtIndex(returnASts, (void *)return1, -1);

    err = symTableExitScope(table); // exit function

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 1 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }
}


/*
AST 2

pub fn foo(par : []u8) []u8 {
    const ret = bar(par);
    return ret;
}
*/
void return_asts_2(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {

    bool result;
    enum ERR_CODES err;

    // pub fn foo(par : []u8) []u8 { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 2 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    SymVariable *par = symTableDeclareVariable(table, "par", dTypeU8, false, -1); // par : []u8

    if (test != NULL) {
        testCase(
            test,
            par != NULL,
            "AST 2 - symtbale declare par",
            "Symtable declared par",
            "Symtable failed to declare par"
        );
    }

    ASTNodePtr function2 = ASTcreateNode(AST_NODE_FUNCTION);

    if (test != NULL) {
        testCase(
            test,
            function2 != NULL,
            "AST 2 - ASTcreateNode",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    err = ASTeditFunctionNode(function2, "foo", dTypeU8, 0, par); // pub fn foo(par : []u8) []u8

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)function2, -1);

    // const ret = bar(par);
    ASTNodePtr declare = ASTcreateNode(AST_NODE_DECLARE);

    if (test != NULL) {
        testCase(
            test,
            declare != NULL,
            "AST 2 - ASTcreateNode declare",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *ret = symTableDeclareVariable(table, "ret", dTypeU8, false, -1); // const ret

    if (test != NULL) {
        testCase(
            test,
            ret != NULL,
            "AST 2 - symtbale declare ret",
            "Symtable declared ret",
            "Symtable failed to declare ret"
        );
    }

    err = ASTeditDeclareNode(declare, ret, NULL); // const ret =

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr call = ASTcreateNode(AST_NODE_FUNC_CALL);

    if (test != NULL) {
        testCase(
            test,
            call != NULL,
            "AST 2 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *var_par = symTableFindVariable(table, "par");

    if (test != NULL) {
        testCase(
            test,
            var_par != NULL,
            "AST 2 - symtbale find par",
            "Symtable found par",
            "Symtable failed to find par"
        );
    }

    err = ASTeditFunctionCallNode(call, "bar", var_par, NULL); // bar(par)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditDeclareNode(declare,  NULL, call); // const ret = bar(par)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    // save
    insertNodeAtIndex(returnASts, (void *)declare, -1);

    // return ret;
    ASTNodePtr return1 = ASTcreateNode(AST_NODE_RETURN);

    if (test != NULL) {
        testCase(
            test,
            return1 != NULL,
            "AST 2 - ASTcreateNode return",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *var_ret = symTableFindVariable(table, "ret");

    if (test != NULL) {
        testCase(
            test,
            var_ret != NULL,
            "AST 2 - symtbale find ret",
            "Symtable found ret",
            "Symtable failed to find ret"
        );
    }

    err = ASTeditReturnNode(return1, ret, NULL, NULL); // return ret

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)return1, -1);

    err = symTableExitScope(table); // exit function

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 2 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }
}


/*
AST 3

pub fn main() void {
    const par = ifj.string("ahoj");
    _ = bar(par);
}
*/
void return_asts_3(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {

    bool result;
    enum ERR_CODES err;

    // pub fn main() void { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 3 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr function3 = ASTcreateNode(AST_NODE_FUNCTION);

    if (test != NULL) {
        testCase(
            test,
            function3 != NULL,
            "AST 3 - ASTcreateNode",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    err = ASTeditFunctionNode(function3, "main", dTypeVoid, 0, NULL); // pub fn main() void

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    // save the AST to the buffer
    insertNodeAtIndex(returnASts, (void *)function3, -1);

    // const par = ifj.string("ahoj");
    ASTNodePtr declare3 = ASTcreateNode(AST_NODE_DECLARE);

    if (test != NULL) {
        testCase(
            test,
            declare3 != NULL,
            "AST 3 - ASTcreateNode declare",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *par3 = symTableDeclareVariable(table, "par", dTypeU8, false, -1); // const par

    if (test != NULL) {
        testCase(
            test,
            par3 != NULL,
            "AST 3 - symtbale declare par",
            "Symtable declared par",
            "Symtable failed to declare par"
        );
    }

    err = ASTeditDeclareNode(declare3, par3, NULL); // const par =

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr call3 = ASTcreateNode(AST_NODE_FUNC_CALL);

    if (test != NULL) {
        testCase(
            test,
            call3 != NULL,
            "AST 3 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    err = ASTeditFunctionCallNode(call3, "ifj.string", NULL, NULL); // ifj.string()

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    struct TOKEN valueToken3 = {.value = "ahoj", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(call3, NULL, NULL, &valueToken3); // ifj.string("ahoj")

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditDeclareNode(declare3, NULL, call3); // const par = ifj.string("ahoj")

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    // save the AST to the buffer
    insertNodeAtIndex(returnASts, (void *)declare3, -1);

    // _ = bar(par);
    ASTNodePtr assign3 = ASTcreateNode(AST_NODE_ASSIGN);

    if (test != NULL) {
        testCase(
            test,
            assign3 != NULL,
            "AST 3 - ASTcreateNode assign",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *var_ = symTableFindVariable(table, "_");

    if (test != NULL) {
        testCase(
            test,
            var_ != NULL,
            "AST 3 - symtbale find _",
            "Symtable found _",
            "Symtable failed to find _"
        );
    }

    err = ASTeditAssignNode(assign3, var_, NULL); // _ =

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }

    ASTNodePtr call3_2 = ASTcreateNode(AST_NODE_FUNC_CALL);

    if (test != NULL) {
        testCase(
            test,
            call3_2 != NULL,
            "AST 3 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    SymVariable *var_par = symTableFindVariable(table, "par");

    if (test != NULL) {
        testCase(
            test,
            var_par != NULL,
            "AST 3 - symtbale find par",
            "Symtable found par",
            "Symtable failed to find par"
        );
    }

    err = ASTeditFunctionCallNode(call3_2, "bar", var_par, NULL); // bar(par)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditAssignNode(assign3, NULL, call3_2); // _ = bar(par)

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)assign3, -1);

    err = symTableExitScope(table); // exit function

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 3 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

}

/*
AST 4

pub fn build(x : []u8, y : []u8) []u8 {
    const res = ifj. concat(x, y);
    return res;
}
*/
void return_asts_4(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {

}



/*
AST 5

pub fn main() void {
    const a = ifj.string("ahoj ");
    var ct : []u8= ifj.string("svete");
    ct = build(a, ct);
    ifj.write(ct);
}
*/
void return_asts_5(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {

}

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
void return_asts_6(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
}


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
void return_asts_7(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
}

/*
AST 8

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}
*/
void return_asts_8(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
}


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
void return_asts_9(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
}

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
void return_asts_10(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
}