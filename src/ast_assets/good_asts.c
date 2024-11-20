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
    SymVariable *param1 = symTableDeclareVariable(table, "param", dTypeU8, false, false); // param : []u8

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

    SymVariable *r1 = symTableDeclareVariable(table, "r", dTypeNone, false, false); // const r    

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

    SymVariable *par = symTableDeclareVariable(table, "par", dTypeU8, false, false); // par : []u8

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

    SymVariable *ret = symTableDeclareVariable(table, "ret", dTypeU8, false, false); // const ret

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

    SymVariable *par3 = symTableDeclareVariable(table, "par", dTypeU8, false, false); // const par

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

    bool result;
    enum ERR_CODES err;

    //pub fn build(x : []u8, y : []u8) []u8 { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 4 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr function4 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(function4, "build", dTypeU8, 0, NULL); // pub fn build() []u8
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    SymVariable* var1 = symTableDeclareVariable(table, "x", dTypeU8, false, false); //x : []u8
    
    if(test != NULL) {
        testCase(
            test,
            var1 != NULL,
            "AST 4 - symtbale declare x",
            "Symtable declared x",
            "Symtable failed to declare x"
        );
    }

    err = ASTeditFunctionNode(function4, NULL, dTypeUndefined, -1, var1); 
   
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    SymVariable* var2 = symTableDeclareVariable(table, "y", dTypeU8, false, false); //y : []u8
    
    if(test != NULL) {
        testCase(
            test,
            var2 != NULL,
            "AST 4 - symtbale declare y",
            "Symtable declared y",
            "Symtable failed to declare y"
        );
    }
    
    err = ASTeditFunctionNode(function4, NULL, dTypeUndefined, -1, var2);
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)function4, -1); //pub fn build(x : []u8, y : []u8) []u8

    //const res = ifj.concat(x, y);
    ASTNodePtr declare4 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *res = symTableDeclareVariable(table, "res", dTypeNone, false, false); // const res
    
    if(test != NULL) {
        testCase(
            test,
            res != NULL,
            "AST 4 - symtbale declare res",
            "Symtable declared res",
            "Symtable failed to declare res"
        );
    }
    
    err = ASTeditDeclareNode(declare4, res, NULL); // const res =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall, "ifj.concat", NULL, NULL); // ifj.concat()

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    SymVariable *var_x = symTableFindVariable(table, "x");
    
    if(test != NULL) {
        testCase(
            test,
            var_x != NULL,
            "AST 4 - symtbale find x",
            "Symtable found x",
            "Symtable failed to find x"
        );
    }
    
    SymVariable *var_y = symTableFindVariable(table, "y");
    
    if(test != NULL) {
        testCase(
            test,
            var_y != NULL,
            "AST 4 - symtbale find y",
            "Symtable found y",
            "Symtable failed to find y"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall, NULL, var_x, NULL); // ifj.concat(x)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall, NULL, var_y, NULL); // ifj.concat(x, y)

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditDeclareNode(declare4, NULL, fnCall); // const res = ifj.concat(x, y)

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)declare4, -1);

    // return res;
    ASTNodePtr return4 = ASTcreateNode(AST_NODE_RETURN);
    SymVariable *var_res = symTableFindVariable(table, "res");

    if(test != NULL) {
        testCase(
            test,
            var_res != NULL,
            "AST 4 - symtbale find res",
            "Symtable found res",
            "Symtable failed to find res"
        );
    }

    err = ASTeditReturnNode(return4, var_res, NULL, NULL); // return res

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)return4, -1);

    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 4 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

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

    bool result;
    enum ERR_CODES err;

    // pub fn main() void { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 5 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr func5 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func5, "main", dTypeVoid, 0, NULL); // pub fn main() void
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)func5, -1);

    // const a = ifj.string("ahoj ");
    ASTNodePtr declare5 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *a = symTableDeclareVariable(table, "a", dTypeNone, false, false); // const a
    
    if(test != NULL) {
        testCase(
            test,
            a != NULL,
            "AST 5 - symtbale declare a",
            "Symtable declared a",
            "Symtable failed to declare a"
        );
    }
    
    err = ASTeditDeclareNode(declare5, a, NULL); // const a =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr fnCall5 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall5, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken5 = {.value= "ahoj ", .type = TOKEN_U8};

    err = ASTeditFunctionCallNode(fnCall5, NULL, NULL, &valueToken5); // ifj.string("ahoj ")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    err = ASTeditDeclareNode(declare5, NULL, fnCall5); // const a = ifj.string("ahoj ")

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)declare5, -1);

    // var ct : []u8= ifj.string("svete");
    ASTNodePtr declare5_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *ct = symTableDeclareVariable(table, "ct", dTypeU8, true, false); // var ct : []u8
    
    if(test != NULL) {
        testCase(
            test,
            ct != NULL,
            "AST 5 - symtbale declare ct",
            "Symtable declared ct",
            "Symtable failed to declare ct"
        );
    }
    
    err = ASTeditDeclareNode(declare5_2, ct, NULL); // var ct : []u8 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall5_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall5_2, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken5_2 = {.value= "svete", .type= TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall5_2, NULL, NULL, &valueToken5_2); // ifj.string("svete")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare5_2, NULL, fnCall5_2); // var ct : []u8 = ifj.string("svete")

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }


    insertNodeAtIndex(returnASts, (void *)declare5_2, -1);

    // ct = build(a, ct);
    ASTNodePtr assign5 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var_ct = symTableFindVariable(table, "ct");

    if(test != NULL) {
        testCase(
            test,
            var_ct != NULL,
            "AST 5 - symtbale find ct",
            "Symtable found ct",
            "Symtable failed to find ct"
        );
    }

    err = ASTeditAssignNode(assign5, var_ct, NULL); // ct =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }

    ASTNodePtr fnCall5_3 = ASTcreateNode(AST_NODE_FUNC_CALL);
    SymVariable *var_a = symTableFindVariable(table, "a");
    
    if(test != NULL) {
        testCase(
            test,
            var_a != NULL,
            "AST 5 - symtbale find a",
            "Symtable found a",
            "Symtable failed to find a"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall5_3, "build", var_a, NULL); // build(a)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall5_3, NULL, var_ct, NULL); // build(a, ct)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditAssignNode(assign5, NULL, fnCall5_3); // ct = build(a, ct)

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)assign5, -1);

    // ifj.write(ct);
    ASTNodePtr fnCall5_4 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall5_4, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall5_4, NULL, var_ct, NULL); // ifj.write(ct)

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)fnCall5_4, -1);

    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 5 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

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
    
    bool result;
    enum ERR_CODES err;

    // pub fn main() void { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }
    
    ASTNodePtr func6 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func6, "main", dTypeVoid, 0, NULL); // pub fn main() void
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)func6, -1);

    // ifj.write("Zadejte cislo pro vypocet faktorialu\n");
    ASTNodePtr fnCall6 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6 = {.value = "Zadejte cislo pro vypocet faktorialu\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6, NULL, NULL, &valueToken6); // ifj.write("Zadejte cislo pro vypocet faktorialu\n")

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)fnCall6, -1);

    // const a = ifj.readi32();
    ASTNodePtr declare6 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *a = symTableDeclareVariable(table, "a", dTypeI32, false, false); // const a
    
    if(test != NULL) {
        testCase(
            test,
            a != NULL,
            "AST 6 - symtbale declare a",
            "Symtable declared a",
            "Symtable failed to declare a"
        );
    }
    
    err = ASTeditDeclareNode(declare6, a, NULL); // const a =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall6_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_2, "ifj.readi32", NULL, NULL); // ifj.readi32()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare6, NULL, fnCall6_2); // const a = ifj.readi32()

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)declare6, -1);

    // if (a) |val| { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter if",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }

    ASTNodePtr if6 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_a = symTableFindVariable(table, "a");

    if(test != NULL) {
        testCase(
            test,
            var_a != NULL,
            "AST 6 - symtbale find a",
            "Symtable found a",
            "Symtable failed to find a"
        );
    }

    SymVariable *val = symTableDeclareVariable(table, "val", var_a->type, var_a->mutable, false); // |val|
    
    if(test != NULL) {
        testCase(
            test,
            val != NULL,
            "AST 6 - symtbale declare val",
            "Symtable declared val",
            "Symtable failed to declare val"
        );
    }
    
    err = ASTeditIfNode(if6, NULL, NULL, NULL, var_a, val); // if (a) |val| {

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)if6, -1);

    // if (val < 0) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter if",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }

    ASTNodePtr if6_2 = ASTcreateNode(AST_NODE_IF_ELSE); //if
    SymVariable *var_val = symTableFindVariable(table, "val");

    if(test != NULL) {
        testCase(
            test,
            var_val != NULL,
            "AST 6 - symtbale find val",
            "Symtable found val",
            "Symtable failed to find val"
        );
    }

    err = ASTeditIfNode(if6_2, var_val, NULL, NULL, NULL, NULL); // if (val) {

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }

    struct TOKEN opr = {.type = TOKEN_LESSTHAN, .value = "<"};
    err = ASTeditIfNode(if6_2, NULL, NULL, &opr, NULL, NULL); // if (val <) {

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }

    struct TOKEN val1 = {.type = TOKEN_I32, .value = "0"};
    err = ASTeditIfNode(if6_2, NULL, &val1, NULL, NULL, NULL); // if (val < 0) {

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)if6_2, -1);

    // ifj.write("Faktorial ");
    ASTNodePtr fnCall6_3 = ASTcreateNode(AST_NODE_FUNC_CALL);
    
    if(test != NULL) {
        testCase(
            test,
            fnCall6_3 != NULL,
            "AST 6 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_3, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_3 = {.value = "Faktorial ", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_3, NULL, NULL, &valueToken6_3); // ifj.write("Faktorial ")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_3, -1);

    // ifj.write(val);
    ASTNodePtr fnCall6_4 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_4, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_val = symTableFindVariable(table, "val");
    
    if(test != NULL) {
        testCase(
            test,
            var2_val != NULL,
            "AST 6 - symtbale find val",
            "Symtable found val",
            "Symtable failed to find val"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_4, NULL, var2_val, NULL); // ifj.write(val)    
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_4, -1);

    // ifj.write(" nelze spocitat\n");
    ASTNodePtr fnCall6_5 = ASTcreateNode(AST_NODE_FUNC_CALL);
    
    if(test != NULL) {
        testCase(
            test,
            fnCall6_5 != NULL,
            "AST 6 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_5, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_5 = {.value = " nelze spocitat\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_5, NULL, NULL, &valueToken6_5); // ifj.write(" nelze spocitat\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_5, -1);

    err = symTableExitScope(table); // exit if

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited if",
            "Symtable failed to exit if"
        );
    }

    // else { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter else


    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter else",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }

    // var d: f64= ifj.i2f(val);
    ASTNodePtr declare6_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *d = symTableDeclareVariable(table, "d", dTypeF64, true, false); // var d: f64
    
    if(test != NULL) {
        testCase(
            test,
            d != NULL,
            "AST 6 - symtbale declare d",
            "Symtable declared d",
            "Symtable failed to declare d"
        );
    }
    
    err = ASTeditDeclareNode(declare6_2, d, NULL); // var d: f64 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall6_6 = ASTcreateNode(AST_NODE_FUNC_CALL);
    
    if(test != NULL) {
        testCase(
            test,
            fnCall6_6 != NULL,
            "AST 6 - ASTcreateNode call",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_6, "ifj.i2f", NULL, NULL); // ifj.i2f()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var3_val = symTableFindVariable(table, "val");
    
    if(test != NULL) {
        testCase(
            test,
            var3_val != NULL,
            "AST 6 - symtbale find val",
            "Symtable found val",
            "Symtable failed to find val"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_6, NULL, var3_val, NULL); // ifj.i2f(val)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare6_2, NULL, fnCall6_6); // var d: f64 = ifj.i2f(val)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare6_2, -1);

    // var vysl: f64 = 1.0;
    ASTNodePtr declare6_3 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *vysl = symTableDeclareVariable(table, "vysl", dTypeF64, true, false); // var vysl: f64
    
    if(test != NULL) {
        testCase(
            test,
            vysl != NULL,
            "AST 6 - symtbale declare vysl",
            "Symtable declared vysl",
            "Symtable failed to declare vysl"
        );
    }
    
    err = ASTeditDeclareNode(declare6_3, vysl, NULL); // var vysl: f64 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_6 = {.value = "1.0", .type = TOKEN_F64};
    declare6_3 = ASTcreateNode(AST_NODE_DECLARE);
    ASTinitNodeValue(declare6_3, &valueToken6_6);
    err = ASTeditDeclareNode(declare6_3, NULL, declare6_3); // var vysl: f64 = 1.0
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare6_3, -1);

    // while (d > 0) { ... }
    result = symTableMoveScopeDown(table, SYM_WHILE); // enter while
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter while",
            "Symtable entered while",
            "Symtable failed to enter while"
        );
    }
    
    ASTNodePtr while6 = ASTcreateNode(AST_NODE_WHILE);
    SymVariable *var_d = symTableFindVariable(table, "d");
    
    if(test != NULL) {
        testCase(
            test,
            var_d != NULL,
            "AST 6 - symtbale find d",
            "Symtable found d",
            "Symtable failed to find d"
        );
    }
    
    err = ASTeditWhileNode(while6, var_d, NULL, NULL, NULL, NULL); // while (d) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditWhileNode",
            "ASTeditWhileNode edited",
            "ASTeditWhileNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)while6, -1);

    // vysl = vysl * d;
    ASTNodePtr assign6 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var_vysl = symTableFindVariable(table, "vysl");
    
    if(test != NULL) {
        testCase(
            test,
            var_vysl != NULL,
            "AST 6 - symtbale find vysl",
            "Symtable found vysl",
            "Symtable failed to find vysl"
        );
    }
    
    err = ASTeditAssignNode(assign6, var_vysl, NULL); // vysl =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr expr1 = ASTcreateNode(AST_NODE_EXPRESION);
    SymVariable *var2_vysl = symTableFindVariable(table, "vysl");
    
    if(test != NULL) {
        testCase(
            test,
            var2_vysl != NULL,
            "AST 6 - symtbale find vysl",
            "Symtable found vysl",
            "Symtable failed to find vysl"
        );
    }
    
    err = ASTaddNodeToExpresion(expr1, var2_vysl, NULL, NULL); // vysl
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    struct TOKEN opr1 = {.type = TOKEN_MULTIPLY, .value = "*"};
    err = ASTaddNodeToExpresion(expr1, NULL, NULL, &opr1); // vysl *
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    SymVariable *var_d2 = symTableFindVariable(table, "d");
    
    if(test != NULL) {
        testCase(
            test,
            var_d2 != NULL,
            "AST 6 - symtbale find d",
            "Symtable found d",
            "Symtable failed to find d"
        );
    }
    
    err = ASTaddNodeToExpresion(expr1, var_d2, NULL, NULL); // vysl * d
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    err = ASTeditAssignNode(assign6, NULL, expr1); // vysl = vysl * d
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign6, -1);

    // d = d - 1.0;
    ASTNodePtr assign6_2 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var_d3 = symTableFindVariable(table, "d");
    
    if(test != NULL) {
        testCase(
            test,
            var_d3 != NULL,
            "AST 6 - symtbale find d",
            "Symtable found d",
            "Symtable failed to find d"
        );
    }
    
    err = ASTeditAssignNode(assign6_2, var_d3, NULL); // d =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr expr2 = ASTcreateNode(AST_NODE_EXPRESION);
    SymVariable *var_d4 = symTableFindVariable(table, "d");
    
    if(test != NULL) {
        testCase(
            test,
            var_d4 != NULL,
            "AST 6 - symtbale find d",
            "Symtable found d",
            "Symtable failed to find d"
        );
    }
    
    err = ASTaddNodeToExpresion(expr2, var_d4, NULL, NULL); // d
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    struct TOKEN opr2 = {.type = TOKEN_MINUS, .value = "-"};
    err = ASTaddNodeToExpresion(expr2, NULL, NULL, &opr2); // d -
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    struct TOKEN val2 = {.type = TOKEN_F64, .value = "1.0"};
    err = ASTaddNodeToExpresion(expr2, NULL, &val2, NULL); // d - 1.0
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add"
        );
    }
    
    err = ASTeditAssignNode(assign6_2, NULL, expr2); // d = d - 1.0
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign6_2, -1);

    err = symTableExitScope(table); // exit while

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited while",
            "Symtable failed to exit while"
        );
    }


    // ifj.write("Vysledek: ");
    ASTNodePtr fnCall6_7 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_7, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_7 = {.value = "Vysledek: ", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_7, NULL, NULL, &valueToken6_7); // ifj.write("Vysledek: ")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_7, -1);

    // ifj.write(vysl);
    ASTNodePtr fnCall6_8 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_8, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var4_vysl = symTableFindVariable(table, "vysl");
    
    if(test != NULL) {
        testCase(
            test,
            var4_vysl != NULL,
            "AST 6 - symtbale find vysl",
            "Symtable found vysl",
            "Symtable failed to find vysl"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_8, NULL, var4_vysl, NULL); // ifj.write(vysl)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_8, -1);

    // ifj.write(" = ");
    ASTNodePtr fnCall6_9 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_9, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_9 = {.value = " = ", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_9, NULL, NULL, &valueToken6_9); // ifj.write(" = ")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_9, -1);

    // const vysl_i32 = ifj.f2i(vysl);
    ASTNodePtr declare6_4 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *vysl_i32 = symTableDeclareVariable(table, "vysl_i32", dTypeI32, false, false); // const vysl_i32
    
    if(test != NULL) {
        testCase(
            test,
            vysl_i32 != NULL,
            "AST 6 - symtbale declare vysl_i32",
            "Symtable declared vysl_i32",
            "Symtable failed to declare vysl_i32"
        );
    }
    
    err = ASTeditDeclareNode(declare6_4, vysl_i32, NULL); // const vysl_i32 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall6_10 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_10, "ifj.f2i", NULL, NULL); // ifj.f2i()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var5_vysl = symTableFindVariable(table, "vysl");
    
    if(test != NULL) {
        testCase(
            test,
            var5_vysl != NULL,
            "AST 6 - symtbale find vysl",
            "Symtable found vysl",
            "Symtable failed to find vysl"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_10, NULL, var5_vysl, NULL); // ifj.f2i(vysl)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare6_4, NULL, fnCall6_10); // const vysl_i32 = ifj.f2i(vysl)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare6_4, -1);

    // ifj.write(vysl_i32);
    ASTNodePtr fnCall6_11 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_11, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var6_vysl_i32 = symTableFindVariable(table, "vysl_i32");
    
    if(test != NULL) {
        testCase(
            test,
            var6_vysl_i32 != NULL,
            "AST 6 - symtbale find vysl_i32",
            "Symtable found vysl_i32",
            "Symtable failed to find vysl_i32"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall6_11, NULL, var6_vysl_i32, NULL); // ifj.write(vysl_i32)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_11, -1);

    // ifj.write("\n");
    ASTNodePtr fnCall6_12 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_12, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_12 = {.value = "\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_12, NULL, NULL, &valueToken6_12); // ifj.write("\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_12, -1);

    err = symTableExitScope(table); // exit else
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }
    
    err = symTableExitScope(table); // exit if
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited if",
            "Symtable failed to exit if"
        );
    }
    
    result = symTableMoveScopeDown(table, SYM_IF); // enter else

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 6 - symtbale enter else",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }


    // ifj.write("Faktorial pro null nelze spocitat\n");
    ASTNodePtr fnCall6_13 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall6_13, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken6_13 = {.value = "Faktorial pro null nelze spocitat\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall6_13, NULL, NULL, &valueToken6_13); // ifj.write("Faktorial pro null nelze spocitat\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall6_13, -1);

    err = symTableExitScope(table); // exit else
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }
    
    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

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

    bool result;
    enum ERR_CODES err;

    // pub fn main() void { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 7 - symtbale enter function",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr func7 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func7, "main", dTypeVoid, 0, NULL); // pub fn main() void

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)func7, -1);

    // ifj.write("Zadejte cislo pro vypocet faktorialu: ");
    ASTNodePtr fnCall7 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7, "ifj.write", NULL, NULL); // ifj.write()
    struct TOKEN valueToken7 = {.value = "Zadejte cislo pro vypocet faktorialu:", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall7, NULL, NULL, &valueToken7); // ifj.write("Zadejte cislo pro vypocet faktorialu:")
    insertNodeAtIndex(returnASts, (void *)fnCall7, -1);

    // const inp = ifj.readi32();
    ASTNodePtr declare7 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *inp = symTableDeclareVariable(table, "inp", dTypeI32, false, false); // const inp
    err = ASTeditDeclareNode(declare7, inp, NULL); // const inp =

    ASTNodePtr fnCall7_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_2, "ifj.readi32", NULL, NULL); // ifj.readi32()

    err = ASTeditDeclareNode(declare7, NULL, fnCall7_2); // const inp = ifj.readi32()

    insertNodeAtIndex(returnASts, (void *)declare7, -1);

    // if (inp) |INP| { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    ASTNodePtr if7 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_inp = symTableFindVariable(table, "inp");
    SymVariable *INP = symTableDeclareVariable(table, "INP", var_inp->type, var_inp->mutable, false); // |INP|
    err = ASTeditIfNode(if7, NULL, NULL, NULL, var_inp, INP); // if (inp) |INP| {
    insertNodeAtIndex(returnASts, (void *)if7, -1);

    // if (INP < 0) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    ASTNodePtr if7_2 = ASTcreateNode(AST_NODE_IF_ELSE); //if
    SymVariable *var_INP = symTableFindVariable(table, "INP");
    err = ASTeditIfNode(if7_2, var_INP, NULL, NULL, NULL, NULL); // if (INP) {
    struct TOKEN opr = {.type = TOKEN_LESSTHAN, .value = "<"};
    err = ASTeditIfNode(if7_2, NULL, NULL, &opr, NULL, NULL); // if (INP <) {
    struct TOKEN val = {.type = TOKEN_I32, .value = "0"};
    err = ASTeditIfNode(if7_2, NULL, &val, NULL, NULL, NULL); // if (INP < 0) {
    insertNodeAtIndex(returnASts, (void *)if7_2, -1);

    // ifj.write("Faktorial nelze spocitat!\n");
    ASTNodePtr fnCall7_3 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_3, "ifj.write", NULL, NULL); // ifj.write()
    struct TOKEN valueToken7_3 = {.value = "Faktorial nelze spocitat!\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall7_3, NULL, NULL, &valueToken7_3); // ifj.write("Faktorial nelze spocitat!\n")
    insertNodeAtIndex(returnASts, (void *)fnCall7_3, -1);

    err = symTableExitScope(table); // exit if
    // else { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter else

    // const vysl = factorial(INP);
    ASTNodePtr declare7_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *vysl = symTableDeclareVariable(table, "vysl", dTypeI32, false, false); // const vysl
    err = ASTeditDeclareNode(declare7_2, vysl, NULL); // const vysl =
    ASTNodePtr fnCall7_4 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_4, "factorial", NULL, NULL); // factorial()
    SymVariable *var2_INP = symTableFindVariable(table, "INP");
    err = ASTeditFunctionCallNode(fnCall7_4, NULL, var2_INP, NULL); // factorial(INP)
    err = ASTeditDeclareNode(declare7_2, NULL, fnCall7_4); // const vysl = factorial(INP)
    insertNodeAtIndex(returnASts, (void *)declare7_2, -1);

    // ifj.write("Vysledek: ");
    ASTNodePtr fnCall7_5 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_5, "ifj.write", NULL, NULL); // ifj.write()
    struct TOKEN valueToken7_5 = {.value = "Vysledek: ", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall7_5, NULL, NULL, &valueToken7_5); // ifj.write("Vysledek: ")
    insertNodeAtIndex(returnASts, (void *)fnCall7_5, -1);

    // ifj.write(vysl);
    ASTNodePtr fnCall7_6 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_6, "ifj.write", NULL, NULL); // ifj.write()
    SymVariable *var3_vysl = symTableFindVariable(table, "vysl");   
    err = ASTeditFunctionCallNode(fnCall7_6, NULL, var3_vysl, NULL); // ifj.write(vysl)
    insertNodeAtIndex(returnASts, (void *)fnCall7_6, -1);

    err = symTableExitScope(table); // exit else
    err = symTableExitScope(table); // exit if
    result = symTableMoveScopeDown(table, SYM_IF); // enter else

    // ifj.write("Chyba pri nacitani celeho cisla!\n");
    ASTNodePtr fnCall7_7 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_7, "ifj.write", NULL, NULL); // ifj.write()
    struct TOKEN valueToken7_7 = {.value = "Chyba pri nacitani celeho cisla!\n", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall7_7, NULL, NULL, &valueToken7_7); // ifj.write("Chyba pri nacitani celeho cisla")
    insertNodeAtIndex(returnASts, (void *)fnCall7_7, -1);

    err = symTableExitScope(table); // exit else
    err = symTableExitScope(table); // exit function

}

/*
AST 8

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}
*/
void return_asts_8(SymTable *table, LinkedList *returnASts, TestInstancePtr test) {
    
    bool result;
    enum ERR_CODES err;

    // pub fn decrement(n: i32, m: i32) i32 { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 8 - symTableMoveScopeDown",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr func8 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func8, "decrement", dTypeI32, 0, NULL); // pub fn decrement() i32

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    SymVariable *n = symTableDeclareVariable(table, "n", dTypeI32, false, false); // n : i32
    err = ASTeditFunctionNode(func8, NULL, dTypeUndefined, -1, n);
    SymVariable *m = symTableDeclareVariable(table, "m", dTypeI32, false, false); // m : i32
    err = ASTeditFunctionNode(func8, NULL, dTypeUndefined, -1, m);
    insertNodeAtIndex(returnASts, (void *)func8, -1); // pub fn decrement(n: i32, m: i32) i32

    // return n - m;
    ASTNodePtr return8 = ASTcreateNode(AST_NODE_RETURN);

    SymVariable *var_n = symTableFindVariable(table, "n");
    SymVariable *var_m = symTableFindVariable(table, "m");
    err = ASTeditReturnNode(return8, var_n, NULL, NULL); // return n
    struct TOKEN opr = {.type = TOKEN_MINUS, .value = "-"};
    err = ASTeditReturnNode(return8, NULL, NULL, &opr); // return n -
    err = ASTeditReturnNode(return8, var_m, NULL, NULL); // return n - m

    insertNodeAtIndex(returnASts, (void *)return8, -1);

    err = symTableExitScope(table); // exit function

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
    
    bool result;
    enum ERR_CODES err;

    // pub fn factorial(n: i32) i32 { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 9 - symTableMoveScopeDown",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr func9 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func9, "factorial", dTypeI32, 0, NULL); // pub fn factorial() i32

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    SymVariable *n = symTableDeclareVariable(table, "n", dTypeI32, false, false); // n : i32
    err = ASTeditFunctionNode(func9, NULL, dTypeUndefined, -1, n);
    insertNodeAtIndex(returnASts, (void *)func9, -1); // pub fn factorial(n: i32) i32

    // var result: i32 = 0 - 1;
    ASTNodePtr declare9 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *result1 = symTableDeclareVariable(table, "result", dTypeI32, true, false); // var result : i32
    err = ASTeditDeclareNode(declare9, result1, NULL); // var result : i32 =

    //if (n < 2) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    ASTNodePtr if9 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_n = symTableFindVariable(table, "n");
    err = ASTeditIfNode(if9, var_n, NULL, NULL, NULL, NULL); // if (n) {
    struct TOKEN opr = {.type = TOKEN_LESSTHAN, .value = "<"};
    err = ASTeditIfNode(if9, NULL, NULL, &opr, NULL, NULL); // if (n <) {
    struct TOKEN val = {.type = TOKEN_I32, .value = "2"};
    err = ASTeditIfNode(if9, NULL, &val, NULL, NULL, NULL); // if (n < 2) {
    insertNodeAtIndex(returnASts, (void *)if9, -1);

    // result = 1;
    ASTNodePtr assign9 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var_result = symTableFindVariable(table, "result");
    err = ASTeditAssignNode(assign9, var_result, NULL); // result =
    struct TOKEN val1 = {.type = TOKEN_I32, .value = "1"};
    ASTNodePtr value = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(value, &val1); // 1

    err = ASTeditAssignNode(assign9, NULL, value); // result = 1
    insertNodeAtIndex(returnASts, (void *)assign9, -1);

    err = symTableExitScope(table); // exit if
    // else { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter else

    // const decremented_n = decrement(n, 1);
    ASTNodePtr declare9_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *decremented_n = symTableDeclareVariable(table, "decremented_n", dTypeI32, false, false); // const decremented_n
    err = ASTeditDeclareNode(declare9_2, decremented_n, NULL); // const decremented_n =
    ASTNodePtr fnCall9 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall9, "decrement", NULL, NULL); // decrement()
    SymVariable *var2_n = symTableFindVariable(table, "n");
    err = ASTeditFunctionCallNode(fnCall9, NULL, var2_n, NULL); // decrement(n)
    struct TOKEN val2 = {.type = TOKEN_I32, .value = "1"};
    err = ASTeditFunctionCallNode(fnCall9, NULL, NULL, &val2); // decrement(n, 1)
    err = ASTeditDeclareNode(declare9_2, NULL, fnCall9); // const decremented_n = decrement(n, 1)
    insertNodeAtIndex(returnASts, (void *)declare9_2, -1);

    // const temp_result = factorial(decremented_n);
    ASTNodePtr declare9_3 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *temp_result = symTableDeclareVariable(table, "temp_result", dTypeI32, false, false); // const temp_result
    err = ASTeditDeclareNode(declare9_3, temp_result, NULL); // const temp_result =
    ASTNodePtr fnCall9_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall9_2, "factorial", NULL, NULL); // factorial()
    SymVariable *var3_decremented_n = symTableFindVariable(table, "decremented_n");
    err = ASTeditFunctionCallNode(fnCall9_2, NULL, var3_decremented_n, NULL); // factorial(decremented_n)
    err = ASTeditDeclareNode(declare9_3, NULL, fnCall9_2); // const temp_result = factorial(decremented_n)
    insertNodeAtIndex(returnASts, (void *)declare9_3, -1);

    // result = n * temp_result;
    ASTNodePtr assign9_2 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var2_result = symTableFindVariable(table, "result");
    err = ASTeditAssignNode(assign9_2, var2_result, NULL); // result =
    ASTNodePtr expr1 = ASTcreateNode(AST_NODE_EXPRESION);
    var2_n = symTableFindVariable(table, "n");
    err = ASTaddNodeToExpresion(expr1, var2_n, NULL, NULL); // n
    struct TOKEN opr1 = {.type = TOKEN_MULTIPLY, .value = "*"};
    err = ASTaddNodeToExpresion(expr1, NULL, NULL, &opr1); // n *
    SymVariable *var_temp_result = symTableFindVariable(table, "temp_result");
    err = ASTaddNodeToExpresion(expr1, var_temp_result, NULL, NULL); // n * temp_result
    err = ASTeditAssignNode(assign9_2, NULL, expr1); // result = n * temp_result
    insertNodeAtIndex(returnASts, (void *)assign9_2, -1);

    err = symTableExitScope(table); // exit else

    // return result;
    ASTNodePtr return9 = ASTcreateNode(AST_NODE_RETURN);
    SymVariable *var3_result = symTableFindVariable(table, "result");
    err = ASTeditReturnNode(return9, var3_result, NULL, NULL); // return result
    insertNodeAtIndex(returnASts, (void *)return9, -1);

    err = symTableExitScope(table); // exit function
    
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
    bool result;
    enum ERR_CODES err;

    // pub fn main() void { ... }
    result = symTableMoveScopeDown(table, SYM_FUNCTION); // enter function

    if (test != NULL) {
        testCase(
            test,
            result,
            "AST 10 - symTableMoveScopeDown",
            "Symtable entered function",
            "Symtable failed to enter function"
        );
    }

    ASTNodePtr func10 = ASTcreateNode(AST_NODE_FUNCTION);
    err = ASTeditFunctionNode(func10, "main", dTypeVoid, 0, NULL); // pub fn main() void    

    if (test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)func10, -1);

    // const str1 = ifj.string("Toto je text v programu jazyka IFJ24");
    ASTNodePtr declare10 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *str1 = symTableDeclareVariable(table, "str1", dTypeNone, false, false); // const str1
    err = ASTeditDeclareNode(declare10, str1, NULL); // const str1 =

    ASTNodePtr fnCall10 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10, "ifj.string", NULL, NULL); // ifj.string()
    struct TOKEN valueToken10 = {.value = "Toto je text v programu jazyka IFJ24", .type = TOKEN_U8};
    err = ASTeditFunctionCallNode(fnCall10, NULL, NULL, &valueToken10); // ifj.string("Toto je text v programu jazyka IFJ24")
}
