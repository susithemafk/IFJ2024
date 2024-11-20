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

    struct TOKEN valueToken3 = {.value = "ahoj", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken5 = {.value= "ahoj ", .type = TOKEN_STRING};

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
    
    struct TOKEN valueToken5_2 = {.value= "svete", .type= TOKEN_STRING};
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
    
    struct TOKEN valueToken6 = {.value = "Zadejte cislo pro vypocet faktorialu\n", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken6_3 = {.value = "Faktorial ", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken6_5 = {.value = " nelze spocitat\n", .type = TOKEN_STRING};
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
    //---------------------------------------------------------------------------------------------
    
    struct TOKEN valueToken6_6 = {.value = "1.0", .type = TOKEN_F64};
    ASTNodePtr valueNode6_1 = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(valueNode6_1, &valueToken6_6);

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 6 - ASTinitNodeValue",
            "ASTinitNodeValue edited",
            "ASTinitNodeValue failed to edit"
        );
    }

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
    
    struct TOKEN valueToken6_7 = {.value = "Vysledek: ", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken6_9 = {.value = " = ", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken6_12 = {.value = "\n", .type = TOKEN_STRING};
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
    
    struct TOKEN valueToken6_13 = {.value = "Faktorial pro null nelze spocitat\n", .type = TOKEN_STRING};
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
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken7 = {.value = "Zadejte cislo pro vypocet faktorialu:", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall7, NULL, NULL, &valueToken7); // ifj.write("Zadejte cislo pro vypocet faktorialu:")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall7, -1);

    // const inp = ifj.readi32();
    ASTNodePtr declare7 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *inp = symTableDeclareVariable(table, "inp", dTypeI32, false, false); // const inp
    
    if(test != NULL) {
        testCase(
            test,
            inp != NULL,
            "AST 7 - symtbale declare inp",
            "Symtable declared inp",
            "Symtable failed to declare inp"
        );
    }
    
    err = ASTeditDeclareNode(declare7, inp, NULL); // const inp =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }


    ASTNodePtr fnCall7_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_2, "ifj.readi32", NULL, NULL); // ifj.readi32()

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }


    err = ASTeditDeclareNode(declare7, NULL, fnCall7_2); // const inp = ifj.readi32()

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }


    insertNodeAtIndex(returnASts, (void *)declare7, -1);

    // if (inp) |INP| { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 7 - symtbale enter if",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }
    
    ASTNodePtr if7 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_inp = symTableFindVariable(table, "inp");
    
    if(test != NULL) {
        testCase(
            test,
            var_inp != NULL,
            "AST 7 - symtbale find inp",
            "Symtable found inp",
            "Symtable failed to find inp"
        );
    }
    
    SymVariable *INP = symTableDeclareVariable(table, "INP", var_inp->type, var_inp->mutable, false); // |INP|
    
    if(test != NULL) {
        testCase(
            test,
            INP != NULL,
            "AST 7 - symtbale declare INP",
            "Symtable declared INP",
            "Symtable failed to declare INP"
        );
    }
    
    err = ASTeditIfNode(if7, NULL, NULL, NULL, var_inp, INP); // if (inp) |INP| {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)if7, -1);

    // if (INP < 0) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 7 - symtbale enter if",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }
    
    ASTNodePtr if7_2 = ASTcreateNode(AST_NODE_IF_ELSE); //if
    SymVariable *var_INP = symTableFindVariable(table, "INP");
    
    if(test != NULL) {
        testCase(
            test,
            var_INP != NULL,
            "AST 7 - symtbale find INP",
            "Symtable found INP",
            "Symtable failed to find INP"
        );
    }
    
    err = ASTeditIfNode(if7_2, var_INP, NULL, NULL, NULL, NULL); // if (INP) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN opr = {.type = TOKEN_LESSTHAN, .value = "<"};
    err = ASTeditIfNode(if7_2, NULL, NULL, &opr, NULL, NULL); // if (INP <) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN val = {.type = TOKEN_I32, .value = "0"};
    err = ASTeditIfNode(if7_2, NULL, &val, NULL, NULL, NULL); // if (INP < 0) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)if7_2, -1);

    // ifj.write("Faktorial nelze spocitat!\n");
    ASTNodePtr fnCall7_3 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_3, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken7_3 = {.value = "Faktorial nelze spocitat!\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall7_3, NULL, NULL, &valueToken7_3); // ifj.write("Faktorial nelze spocitat!\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall7_3, -1);

    err = symTableExitScope(table); // exit if
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - symTableExitScope",
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
            "AST 7 - symtbale enter else",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }


    // const vysl = factorial(INP);
    ASTNodePtr declare7_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *vysl = symTableDeclareVariable(table, "vysl", dTypeI32, false, false); // const vysl
    
    if(test != NULL) {
        testCase(
            test,
            vysl != NULL,
            "AST 7 - symtbale declare vysl",
            "Symtable declared vysl",
            "Symtable failed to declare vysl"
        );
    }
    
    err = ASTeditDeclareNode(declare7_2, vysl, NULL); // const vysl =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall7_4 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_4, "factorial", NULL, NULL); // factorial()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_INP = symTableFindVariable(table, "INP");
    
    if(test != NULL) {
        testCase(
            test,
            var2_INP != NULL,
            "AST 7 - symtbale find INP",
            "Symtable found INP",
            "Symtable failed to find INP"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall7_4, NULL, var2_INP, NULL); // factorial(INP)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare7_2, NULL, fnCall7_4); // const vysl = factorial(INP)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare7_2, -1);

    // ifj.write("Vysledek: ");
    ASTNodePtr fnCall7_5 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_5, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken7_5 = {.value = "Vysledek: ", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall7_5, NULL, NULL, &valueToken7_5); // ifj.write("Vysledek: ")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall7_5, -1);

    // ifj.write(vysl);
    ASTNodePtr fnCall7_6 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_6, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var3_vysl = symTableFindVariable(table, "vysl");   
    
    if(test != NULL) {
        testCase(
            test,
            var3_vysl != NULL,
            "AST 7 - symtbale find vysl",
            "Symtable found vysl",
            "Symtable failed to find vysl"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall7_6, NULL, var3_vysl, NULL); // ifj.write(vysl)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall7_6, -1);

    err = symTableExitScope(table); // exit else
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }
    
    err = symTableExitScope(table); // exit if
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - symTableExitScope",
            "Symtable exited if",
            "Symtable failed to exit if"
        );
    }
    
    result = symTableMoveScopeDown(table, SYM_IF); // enter else

    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 7 - symtbale enter else",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }


    // ifj.write("Chyba pri nacitani celeho cisla!\n");
    ASTNodePtr fnCall7_7 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall7_7, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken7_7 = {.value = "Chyba pri nacitani celeho cisla!\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall7_7, NULL, NULL, &valueToken7_7); // ifj.write("Chyba pri nacitani celeho cisla")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall7_7, -1);

    err = symTableExitScope(table); // exit else
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }
    
    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 7 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }


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
    
    if(test != NULL) {
        testCase(
            test,
            n != NULL,
            "AST 8 - symtbale declare n",
            "Symtable declared n",
            "Symtable failed to declare n"
        );
    }
    
    err = ASTeditFunctionNode(func8, NULL, dTypeUndefined, -1, n);
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }
    
    SymVariable *m = symTableDeclareVariable(table, "m", dTypeI32, false, false); // m : i32
    
    if(test != NULL) {
        testCase(
            test,
            m != NULL,
            "AST 8 - symtbale declare m",
            "Symtable declared m",
            "Symtable failed to declare m"
        );
    }
    
    err = ASTeditFunctionNode(func8, NULL, dTypeUndefined, -1, m);
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)func8, -1); // pub fn decrement(n: i32, m: i32) i32

    // return n - m;
    ASTNodePtr return8 = ASTcreateNode(AST_NODE_RETURN);

    SymVariable *var_n = symTableFindVariable(table, "n");
    
    if(test != NULL) {
        testCase(
            test,
            var_n != NULL,
            "AST 8 - symtbale find n",
            "Symtable found n",
            "Symtable failed to find n"
        );
    }
    
    SymVariable *var_m = symTableFindVariable(table, "m");
    
    if(test != NULL) {
        testCase(
            test,
            var_m != NULL,
            "AST 8 - symtbale find m",
            "Symtable found m",
            "Symtable failed to find m"
        );
    }
    
    err = ASTeditReturnNode(return8, var_n, NULL, NULL); // return n
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }
    
    struct TOKEN opr = {.type = TOKEN_MINUS, .value = "-"};
    err = ASTeditReturnNode(return8, NULL, NULL, &opr); // return n -
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }

    err = ASTeditReturnNode(return8, var_m, NULL, NULL); // return n - m

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)return8, -1);

    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 8 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

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
    
    if(test != NULL) {
        testCase(
            test,
            n != NULL,
            "AST 9 - symtbale declare n",
            "Symtable declared n",
            "Symtable failed to declare n"
        );
    }
    
    err = ASTeditFunctionNode(func9, NULL, dTypeUndefined, -1, n);
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionNode",
            "ASTeditFunctionNode edited",
            "ASTeditFunctionNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)func9, -1); // pub fn factorial(n: i32) i32

    // var result: i32 = 0 - 1;
    ASTNodePtr declare9 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *result1 = symTableDeclareVariable(table, "result", dTypeI32, true, false); // var result : i32
    
    if(test != NULL) {
        testCase(
            test,
            result1 != NULL,
            "AST 9 - symtbale declare result",
            "Symtable declared result",
            "Symtable failed to declare result"
        );
    }
    
    err = ASTeditDeclareNode(declare9, result1, NULL); // var result : i32 =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    //if (n < 2) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 9 - symtbale enter if",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }
    
    ASTNodePtr if9 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_n = symTableFindVariable(table, "n");
    
    if(test != NULL) {
        testCase(
            test,
            var_n != NULL,
            "AST 9 - symtbale find n",
            "Symtable found n",
            "Symtable failed to find n"
        );
    }
    
    err = ASTeditIfNode(if9, var_n, NULL, NULL, NULL, NULL); // if (n) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN opr = {.type = TOKEN_LESSTHAN, .value = "<"};    
    err = ASTeditIfNode(if9, NULL, NULL, &opr, NULL, NULL); // if (n <) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN val = {.type = TOKEN_I32, .value = "2"};
    err = ASTeditIfNode(if9, NULL, &val, NULL, NULL, NULL); // if (n < 2) {

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }

    insertNodeAtIndex(returnASts, (void *)if9, -1);

    // result = 1;
    ASTNodePtr assign9 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var_result = symTableFindVariable(table, "result");
    
    if(test != NULL) {
        testCase(
            test,
            var_result != NULL,
            "AST 9 - symtbale find result",
            "Symtable found result",
            "Symtable failed to find result"
        );
    }
    
    err = ASTeditAssignNode(assign9, var_result, NULL); // result =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    struct TOKEN val1 = {.type = TOKEN_I32, .value = "1"};
    ASTNodePtr value = ASTcreateNode(AST_NODE_VALUE);
    err = ASTinitNodeValue(value, &val1); // 1

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTinitNodeValue",
            "ASTinitNodeValue edited",
            "ASTinitNodeValue failed to edit"
        );
    }

    err = ASTeditAssignNode(assign9, NULL, value); // result = 1
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign9, -1);

    err = symTableExitScope(table); // exit if
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - symTableExitScope",
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
            "AST 9 - symtbale enter else",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }


    // const decremented_n = decrement(n, 1);
    ASTNodePtr declare9_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *decremented_n = symTableDeclareVariable(table, "decremented_n", dTypeI32, false, false); // const decremented_n
    
    if(test != NULL) {
        testCase(
            test,
            decremented_n != NULL,
            "AST 9 - symtbale declare decremented_n",
            "Symtable declared decremented_n",
            "Symtable failed to declare decremented_n"
        );
    }
    
    err = ASTeditDeclareNode(declare9_2, decremented_n, NULL); // const decremented_n =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall9 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall9, "decrement", NULL, NULL); // decrement()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_n = symTableFindVariable(table, "n");
    
    if(test != NULL) {
        testCase(
            test,
            var2_n != NULL,
            "AST 9 - symtbale find n",
            "Symtable found n",
            "Symtable failed to find n"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall9, NULL, var2_n, NULL); // decrement(n)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN val2 = {.type = TOKEN_I32, .value = "1"};
    err = ASTeditFunctionCallNode(fnCall9, NULL, NULL, &val2); // decrement(n, 1)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare9_2, NULL, fnCall9); // const decremented_n = decrement(n, 1)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare9_2, -1);

    // const temp_result = factorial(decremented_n);
    ASTNodePtr declare9_3 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *temp_result = symTableDeclareVariable(table, "temp_result", dTypeI32, false, false); // const temp_result
    
    if(test != NULL) {
        testCase(
            test,
            temp_result != NULL,
            "AST 9 - symtbale declare temp_result",
            "Symtable declared temp_result",
            "Symtable failed to declare temp_result"
        );
    }
    
    err = ASTeditDeclareNode(declare9_3, temp_result, NULL); // const temp_result =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall9_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall9_2, "factorial", NULL, NULL); // factorial()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var3_decremented_n = symTableFindVariable(table, "decremented_n");
    
    if(test != NULL) {
        testCase(
            test,
            var3_decremented_n != NULL,
            "AST 9 - symtbale find decremented_n",
            "Symtable found decremented_n",
            "Symtable failed to find decremented_n"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall9_2, NULL, var3_decremented_n, NULL); // factorial(decremented_n)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare9_3, NULL, fnCall9_2); // const temp_result = factorial(decremented_n)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare9_3, -1);

    // result = n * temp_result;
    ASTNodePtr assign9_2 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var2_result = symTableFindVariable(table, "result");
    
    if(test != NULL) {
        testCase(
            test,
            var2_result != NULL,
            "AST 9 - symtbale find result",
            "Symtable found result",
            "Symtable failed to find result"
        );
    }
    
    err = ASTeditAssignNode(assign9_2, var2_result, NULL); // result =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr expr1 = ASTcreateNode(AST_NODE_EXPRESION);
    var2_n = symTableFindVariable(table, "n");
    
    if(test != NULL) {
        testCase(
            test,
            var2_n != NULL,
            "AST 9 - symtbale find n",
            "Symtable found n",
            "Symtable failed to find n"
        );
    }
    
    err = ASTaddNodeToExpresion(expr1, var2_n, NULL, NULL); // n
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion edited",
            "ASTaddNodeToExpresion failed to edit"
        );
    }
    
    struct TOKEN opr1 = {.type = TOKEN_MULTIPLY, .value = "*"};
    err = ASTaddNodeToExpresion(expr1, NULL, NULL, &opr1); // n *
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion edited",
            "ASTaddNodeToExpresion failed to edit"
        );
    }
    
    SymVariable *var_temp_result = symTableFindVariable(table, "temp_result");
    
    if(test != NULL) {
        testCase(
            test,
            var_temp_result != NULL,
            "AST 9 - symtbale find temp_result",
            "Symtable found temp_result",
            "Symtable failed to find temp_result"
        );
    }
    
    err = ASTaddNodeToExpresion(expr1, var_temp_result, NULL, NULL); // n * temp_result
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion edited",
            "ASTaddNodeToExpresion failed to edit"
        );
    }
    
    err = ASTeditAssignNode(assign9_2, NULL, expr1); // result = n * temp_result
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign9_2, -1);

    err = symTableExitScope(table); // exit else

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }


    // return result;
    ASTNodePtr return9 = ASTcreateNode(AST_NODE_RETURN);
    SymVariable *var3_result = symTableFindVariable(table, "result");
    
    if(test != NULL) {
        testCase(
            test,
            var3_result != NULL,
            "AST 9 - symtbale find result",
            "Symtable found result",
            "Symtable failed to find result"
        );
    }
    
    err = ASTeditReturnNode(return9, var3_result, NULL, NULL); // return result
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - ASTeditReturnNode",
            "ASTeditReturnNode edited",
            "ASTeditReturnNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)return9, -1);

    err = symTableExitScope(table); // exit function
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 9 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

}

/*
AST 10

pub fn main() void {
    const str1 = ifj.string("Toto je text v programu jazyka IFJ24");
    var str2 = ifj.string(", ktery jeste trochu obohatime");
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
    
    if(test != NULL) {
        testCase(
            test,
            str1 != NULL,
            "AST 10 - symtbale declare str1",
            "Symtable declared str1",
            "Symtable failed to declare str1"
        );
    }
    
    err = ASTeditDeclareNode(declare10, str1, NULL); // const str1 =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr fnCall10 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10 = {.value = "Toto je text v programu jazyka IFJ24", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10, NULL, NULL, &valueToken10); // ifj.string("Toto je text v programu jazyka IFJ24")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10, NULL, fnCall10); // const str1 = ifj.string("Toto je text v programu jazyka IFJ24")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10, -1);

    // var str2 = ifj.string(", ktery jeste trochu obohatime");
    ASTNodePtr declare10_2 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *str2 = symTableDeclareVariable(table, "str2", dTypeNone, true, false); // var str2
    
    if(test != NULL) {
        testCase(
            test,
            str2 != NULL,
            "AST 10 - symtbale declare str2",
            "Symtable declared str2",
            "Symtable failed to declare str2"
        );
    }
    
    err = ASTeditDeclareNode(declare10_2, str2, NULL); // var str2 =

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }

    ASTNodePtr fnCall10_2 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_2, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_2 = {.value = ", ktery jeste trochu obohatime", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_2, NULL, NULL, &valueToken10_2); // ifj.string(", ktery jeste trochu obohatime")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10_2, NULL, fnCall10_2); // var str2 = ifj.string(", ktery jeste trochu obohatime")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10_2, -1);

    // str2 = ifj.concat(str1, str2);
    ASTNodePtr assign10 = ASTcreateNode(AST_NODE_ASSIGN); 
    SymVariable *var_str2 = symTableFindVariable(table, "str2");
    
    if(test != NULL) {
        testCase(
            test,
            var_str2 != NULL,
            "AST 10 - symtbale find str2",
            "Symtable found str2",
            "Symtable failed to find str2"
        );
    }
    
    err = ASTeditAssignNode(assign10, var_str2, NULL); // str2 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_3 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_3, "ifj.concat", NULL, NULL); // ifj.concat()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var_str1 = symTableFindVariable(table, "str1");
    
    if(test != NULL) {
        testCase(
            test,
            var_str1 != NULL,
            "AST 10 - symtbale find str1",
            "Symtable found str1",
            "Symtable failed to find str1"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_3, NULL, var_str1, NULL); // ifj.concat(str1)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_str2 = symTableFindVariable(table, "str2");
    
    if(test != NULL) {
        testCase(
            test,
            var2_str2 != NULL,
            "AST 10 - symtbale find str2",
            "Symtable found str2",
            "Symtable failed to find str2"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_3, NULL, var_str2, NULL); // ifj.concat(str1, str2)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditAssignNode(assign10, NULL, fnCall10_3); // str2 = ifj.concat(str1, str2)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign10, -1);

    // ifj.write(str1);
    ASTNodePtr fnCall10_4 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_4, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_str1 = symTableFindVariable(table, "str1");
    
    if(test != NULL) {
        testCase(
            test,
            var2_str1 != NULL,
            "AST 10 - symtbale find str1",
            "Symtable found str1",
            "Symtable failed to find str1"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_4, NULL, var_str1, NULL); // ifj.write(str1)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_4, -1);

    // ifj.write("\n");
    ASTNodePtr fnCall10_5 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_5, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_5 = {.value = "\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_5, NULL, NULL, &valueToken10_5); // ifj.write("\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_5, -1);

    // ifj.write(str2);
    ASTNodePtr fnCall10_6 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_6, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var3_str2 = symTableFindVariable(table, "str2");
    
    if(test != NULL) {
        testCase(
            test,
            var3_str2 != NULL,
            "AST 10 - symtbale find str2",
            "Symtable found str2",
            "Symtable failed to find str2"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_6, NULL, var3_str2, NULL); // ifj.write(str2)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_6, -1);

    // ifj.write("\n");
    ASTNodePtr fnCall10_7 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_7, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_7 = {.value = "\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_7, NULL, NULL, &valueToken10_7); // ifj.write("\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_7, -1);

    // ifj.write("Zadejte serazenou posloupnost malych pismen a-h:\n");
    ASTNodePtr fnCall10_8 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_8, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_8 = {.value = "Zadejte serazenou posloupnost malych pismen a-h:\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_8, NULL, NULL, &valueToken10_8); // ifj.write("Zadejte serazenou posloupnost malych pismen a-h:\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_8, -1);

    // var newInput = ifj.readstr();
    ASTNodePtr declare10_3 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *newInput = symTableDeclareVariable(table, "newInput", dTypeNone, true, false); // var newInput
    
    if(test != NULL) {
        testCase(
            test,
            newInput != NULL,
            "AST 10 - symtbale declare newInput",
            "Symtable declared newInput",
            "Symtable failed to declare newInput"
        );
    }
    
    err = ASTeditDeclareNode(declare10_3, newInput, NULL); // var newInput =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_9 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_9, "ifj.readstr", NULL, NULL); // ifj.readstr()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10_3, NULL, fnCall10_9); // var newInput = ifj.readstr()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10_3, -1);

    // var all: []u8= ifj.string("");
    ASTNodePtr declare10_4 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *all = symTableDeclareVariable(table, "all", dTypeU8, true, false); // var all: []u8
    
    if(test != NULL) {
        testCase(
            test,
            all != NULL,
            "AST 10 - symtbale declare all",
            "Symtable declared all",
            "Symtable failed to declare all"
        );
    }
    
    err = ASTeditDeclareNode(declare10_4, all, NULL); // var all: []u8 =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_10 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_10, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_10 = {.value= "", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_10, NULL, NULL, &valueToken10_10); // ifj.string("")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10_4, NULL, fnCall10_10); // var all: []u8 = ifj.string("")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10_4, -1);

    // while (newInput) |inpOK| { ... }
    result = symTableMoveScopeDown(table, SYM_WHILE); // enter while
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 10 - symTableMoveScopeDown",
            "Symtable entered while",
            "Symtable failed to enter while"
        );
    }
    
    ASTNodePtr while10 = ASTcreateNode(AST_NODE_WHILE);
    SymVariable *var_newInput = symTableFindVariable(table, "newInput");
    
    if(test != NULL) {
        testCase(
            test,
            var_newInput != NULL,
            "AST 10 - symtbale find newInput",
            "Symtable found newInput",
            "Symtable failed to find newInput"
        );
    }
    
    SymVariable *inpOK = symTableDeclareVariable(table, "inpOK", var_newInput->type, var_newInput->mutable, false); // |inpOK|
    
    if(test != NULL) {
        testCase(
            test,
            inpOK != NULL,
            "AST 10 - symtbale declare inpOK",
            "Symtable declared inpOK",
            "Symtable failed to declare inpOK"
        );
    }
    
    err = ASTeditWhileNode(while10, NULL, NULL, NULL, var_newInput, inpOK); // while (newInput) |inpOK| {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditWhileNode",
            "ASTeditWhileNode edited",
            "ASTeditWhileNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)while10, -1);

    // const abcdefgh = ifj.string("abcdefgh");
    ASTNodePtr declare10_5 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *abcdefgh = symTableDeclareVariable(table, "abcdefgh", dTypeNone, false, false); // const abcdefgh
    
    if(test != NULL) {
        testCase(
            test,
            abcdefgh != NULL,
            "AST 10 - symtbale declare abcdefgh",
            "Symtable declared abcdefgh",
            "Symtable failed to declare abcdefgh"
        );
    }
    
    err = ASTeditDeclareNode(declare10_5, abcdefgh, NULL); // const abcdefgh =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_11 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_11, "ifj.string", NULL, NULL); // ifj.string()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_11 = {.value= "abcdefgh", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_11, NULL, NULL, &valueToken10_11); // ifj.string("abcdefgh")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10_5, NULL, fnCall10_11); // const abcdefgh = ifj.string("abcdefgh")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10_5, -1);

    // const strcmpResult = ifj.strcmp(inpOK, abcdefgh);
    ASTNodePtr declare10_6 = ASTcreateNode(AST_NODE_DECLARE);
    SymVariable *strcmpResult = symTableDeclareVariable(table, "strcmpResult", dTypeNone, false, false); // const strcmpResult
    
    if(test != NULL) {
        testCase(
            test,
            strcmpResult != NULL,
            "AST 10 - symtbale declare strcmpResult",
            "Symtable declared strcmpResult",
            "Symtable failed to declare strcmpResult"
        );
    }
    
    err = ASTeditDeclareNode(declare10_6, strcmpResult, NULL); // const strcmpResult =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_12 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_12, "ifj.strcmp", NULL, NULL); // ifj.strcmp()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var_inpOK = symTableFindVariable(table, "inpOK");
    
    if(test != NULL) {
        testCase(
            test,
            var_inpOK != NULL,
            "AST 10 - symtbale find inpOK",
            "Symtable found inpOK",
            "Symtable failed to find inpOK"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_12, NULL, var_inpOK, NULL); // ifj.strcmp(inpOK)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var_abcdefgh = symTableFindVariable(table, "abcdefgh");
    
    if(test != NULL) {
        testCase(
            test,
            var_abcdefgh != NULL,
            "AST 10 - symtbale find abcdefgh",
            "Symtable found abcdefgh",
            "Symtable failed to find abcdefgh"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_12, NULL, var_abcdefgh, NULL); // ifj.strcmp(inpOK, abcdefgh)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditDeclareNode(declare10_6, NULL, fnCall10_12); // const strcmpResult = ifj.strcmp(inpOK, abcdefgh)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditDeclareNode",
            "ASTeditDeclareNode edited",
            "ASTeditDeclareNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)declare10_6, -1);

    // if (strcmpResult == 0) { ... }
    result = symTableMoveScopeDown(table, SYM_IF); // enter if
    
    if(test != NULL) {
        testCase(
            test,
            result,
            "AST 10 - symTableMoveScopeDown",
            "Symtable entered if",
            "Symtable failed to enter if"
        );
    }
    
    ASTNodePtr if10 = ASTcreateNode(AST_NODE_IF_ELSE);
    SymVariable *var_strcmpResult = symTableFindVariable(table, "strcmpResult");
    
    if(test != NULL) {
        testCase(
            test,
            var_strcmpResult != NULL,
            "AST 10 - symtbale find strcmpResult",
            "Symtable found strcmpResult",
            "Symtable failed to find strcmpResult"
        );
    }
    
    err = ASTeditIfNode(if10, var_strcmpResult, NULL, NULL, NULL, NULL); // if (strcmpResult) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN opr10 = {.type = TOKEN_EQUALS, .value = "=="};
    err = ASTeditIfNode(if10, NULL, NULL, &opr10, NULL, NULL); // if (strcmpResult ==) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    struct TOKEN val10 = {.type = TOKEN_I32, .value = "0"};
    err = ASTeditIfNode(if10, NULL, &val10, NULL, NULL, NULL); // if (strcmpResult == 0) {
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditIfNode",
            "ASTeditIfNode edited",
            "ASTeditIfNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)if10, -1);

    // ifj.write("Spravne zadano!\n");
    ASTNodePtr fnCall10_13 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_13, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_13 = {.value= "Spravne zadano!\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_13, NULL, NULL, &valueToken10_13); // ifj.write("Spravne zadano!\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_13, -1);

    // ifj.write(all);
    ASTNodePtr fnCall10_14 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_14, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var_all = symTableFindVariable(table, "all");
    
    if(test != NULL) {
        testCase(
            test,
            var_all != NULL,
            "AST 10 - symtbale find all",
            "Symtable found all",
            "Symtable failed to find all"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_14, NULL, var_all, NULL); // ifj.write(all)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_14, -1);

    // newInput = null;
    ASTNodePtr assign10_2 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var2_newInput = symTableFindVariable(table, "newInput");
    
    if(test != NULL) {
        testCase(
            test,
            var2_newInput != NULL,
            "AST 10 - symtbale find newInput",
            "Symtable found newInput",
            "Symtable failed to find newInput"
        );
    }
    
    err = ASTeditAssignNode(assign10_2, var2_newInput, NULL); // newInput =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    struct TOKEN val10_2 = {.type = TOKEN_NULL, .value = "null"};
    ASTNodePtr expr10_2 = ASTcreateNode(AST_NODE_EXPRESION);

    if(test != NULL) {
        testCase(
            test,
            expr10_2 != NULL,
            "AST 10 - ASTcreateNode",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }

    err = ASTaddNodeToExpresion(expr10_2, NULL, &val10_2, NULL); // null

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTaddNodeToExpresion",
            "ASTaddNodeToExpresion added",
            "ASTaddNodeToExpresion failed to add null node"
        );
    }
    
    err = ASTeditAssignNode(assign10_2, NULL, expr10_2); // newInput = null
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign10_2, -1);

    err = symTableExitScope(table); // exit if

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - symTableExitScope",
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
            "AST 10 - symTableMoveScopeDown",
            "Symtable entered else",
            "Symtable failed to enter else"
        );
    }

    // ifj.write("Spatne zadana posloupnost, zkuste znovu:\n");
    ASTNodePtr fnCall10_15 = ASTcreateNode(AST_NODE_FUNC_CALL);
    if(test != NULL) {
        testCase(
            test,
            fnCall10_15 != NULL,
            "AST 10 - ASTcreateNode",
            "ASTcreateNode created",
            "ASTcreateNode failed to create"
        );
    }
    err = ASTeditFunctionCallNode(fnCall10_15, "ifj.write", NULL, NULL); // ifj.write()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    struct TOKEN valueToken10_15 = {.value="Spatne zadana posloupnost, zkuste znovu:\n", .type = TOKEN_STRING};
    err = ASTeditFunctionCallNode(fnCall10_15, NULL, NULL, &valueToken10_15); // ifj.write("Spatne zadana posloupnost, zkuste znovu:\n")
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)fnCall10_15, -1);

    // all = ifj.concat(all, inpOK);
    ASTNodePtr assign10_3 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var3_all = symTableFindVariable(table, "all");
    
    if(test != NULL) {
        testCase(
            test,
            var3_all != NULL,
            "AST 10 - symtbale find all",
            "Symtable found all",
            "Symtable failed to find all"
        );
    }
    
    err = ASTeditAssignNode(assign10_3, var3_all, NULL); // all =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_16 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_16, "ifj.concat", NULL, NULL); // ifj.concat()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var4_all = symTableFindVariable(table, "all");
    
    if(test != NULL) {
        testCase(
            test,
            var4_all != NULL,
            "AST 10 - symtbale find all",
            "Symtable found all",
            "Symtable failed to find all"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_16, NULL, var4_all, NULL); // ifj.concat(all)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    SymVariable *var2_inpOK = symTableFindVariable(table, "inpOK");
    
    if(test != NULL) {
        testCase(
            test,
            var2_inpOK != NULL,
            "AST 10 - symtbale find inpOK",
            "Symtable found inpOK",
            "Symtable failed to find inpOK"
        );
    }
    
    err = ASTeditFunctionCallNode(fnCall10_16, NULL, var2_inpOK, NULL); // ifj.concat(all, inpOK)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditAssignNode(assign10_3, NULL, fnCall10_16); // all = ifj.concat(all, inpOK)
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign10_3, -1);

    // newInput = ifj.readstr();
    ASTNodePtr assign10_4 = ASTcreateNode(AST_NODE_ASSIGN);
    SymVariable *var5_newInput = symTableFindVariable(table, "newInput");
    
    if(test != NULL) {
        testCase(
            test,
            var5_newInput != NULL,
            "AST 10 - symtbale find newInput",
            "Symtable found newInput",
            "Symtable failed to find newInput"
        );
    }
    
    err = ASTeditAssignNode(assign10_4, var5_newInput, NULL); // newInput =
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    ASTNodePtr fnCall10_17 = ASTcreateNode(AST_NODE_FUNC_CALL);
    err = ASTeditFunctionCallNode(fnCall10_17, "ifj.readstr", NULL, NULL); // ifj.readstr()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditFunctionCallNode",
            "ASTeditFunctionCallNode edited",
            "ASTeditFunctionCallNode failed to edit"
        );
    }
    
    err = ASTeditAssignNode(assign10_4, NULL, fnCall10_17); // newInput = ifj.readstr()
    
    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - ASTeditAssignNode",
            "ASTeditAssignNode edited",
            "ASTeditAssignNode failed to edit"
        );
    }
    
    insertNodeAtIndex(returnASts, (void *)assign10_4, -1);

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }


    err = symTableExitScope(table); // exit else

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - symTableExitScope",
            "Symtable exited else",
            "Symtable failed to exit else"
        );
    }

    err = symTableExitScope(table); // exit while

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - symTableExitScope",
            "Symtable exited while",
            "Symtable failed to exit while"
        );
    }

    err = symTableExitScope(table); // exit function

    if(test != NULL) {
        testCase(
            test,
            err == SUCCESS,
            "AST 10 - symTableExitScope",
            "Symtable exited function",
            "Symtable failed to exit function"
        );
    }

}
