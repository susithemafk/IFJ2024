/** AUTHOR
 *
 * @author <253171> Vanesa Zimmermannová
 * @file code_generator.c
 * @date 20.10.2024
 * @brief Implementation of the code generator
 */

#include "code_generation/code_generator.h"
#include "code_generation/builtin_generator.h"
#include <stdio.h>
#include <stdlib.h>

#define PRINTLN(...)                                                                               \
    do {                                                                                           \
        printf(__VA_ARGS__);                                                                       \
        printf("\n");                                                                              \
    } while (0)

void generateCodeProgram(Program *program) {
    PRINTLN(".IFJcode24");

    PRINTLN("DEFVAR GF@_");
    PRINTLN("CREATEFRAME\n");

    PRINTLN("CALL function_main");
    PRINTLN("EXIT int@0\n");

    int size = getSize(program->functions);
    for (int i = 0; i < size; i++) {
        Function *function = (Function *)getDataAtIndex(program->functions, i);
        generateCodeFunction(function);
    }

    generateBuiltins();
}

void generateCodeFunction(Function *function) {
    PRINTLN("\nLABEL function_%s", function->id.name);
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    int size = getSize(function->params);
    for (int i = size - 1; i >= 0; i--) {
        Param *param = (Param *)getDataAtIndex(function->params, i);
        generateCodeParam(param);
    }

    generateCodeBody(&function->body);

    PRINTLN("POPFRAME");
    if (function->returnType.data_type == dTypeVoid)
        PRINTLN("RETURN");
    else
        PRINTLN("EXIT int@6");
}

void generateCodeBody(Body *body) {
    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        generateCodeStatement(statement);
    }
}

void generateCodeStatement(Statement *statement) {
    switch (statement->type) {
    case FunctionCallStatementType:
        generateCodeFunctionCall(&statement->data.function_call_statement);
        if (statement->data.function_call_statement.return_type.data_type != dTypeVoid)
            PRINTLN("POPS GF@_");
        break;
    case ReturnStatementType:
        generateCodeReturnStatement(&statement->data.return_statement);
        break;
    case WhileStatementType:
        generateCodeWhileStatement(&statement->data.while_statement);
        break;
    case IfStatementType:
        generateCodeIfStatement(&statement->data.if_statement);
        break;
    case AssigmentStatementType:
        generateCodeAssigmentStatement(&statement->data.assigment_statement);
        break;
    case VariableDefinitionStatementType:
        generateCodeVariableDefinitionStatement(&statement->data.variable_definition_statement);
        break;
    }
}

void generateCodeParam(Param *param) {
    PRINTLN("DEFVAR LF@%s_%d", param->id.name, param->id.var->id);
    PRINTLN("POPS LF@%s_%d", param->id.name, param->id.var->id);
}

void generateCodeVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    if (!statement->code_gen_defined) {
        PRINTLN("DEFVAR LF@%s_%d", statement->id.name, statement->id.var->id);
        statement->code_gen_defined = true;
    }

    generateCodeExpression(&statement->value);
    PRINTLN("POPS LF@%s_%d", statement->id.name, statement->id.var->id);
}

void generateCodeAssigmentStatement(AssigmentStatement *statement) {
    generateCodeExpression(&statement->value);

    if (statement->discard)
        PRINTLN("POPS GF@_");
    else
        PRINTLN("POPS LF@%s_%d", statement->id.name, statement->var->id);
}

void generateCodeIfStatement(IfStatement *statement) {
    static int if_counter = 0;
    int if_id = if_counter++;

    generateCodeExpression(&statement->condition);
    PRINTLN("POPS GF@_");

    PRINTLN("JUMPIFEQ $if_%d_else GF@_ %s", if_id,
            statement->non_nullable.name ? "nil@nil" : "bool@false");
    if (statement->non_nullable.name) {
        if (!statement->code_gen_defined) {
            PRINTLN("DEFVAR LF@%s_%d", statement->non_nullable.name,
                    statement->non_nullable.var->id);
            statement->code_gen_defined = true;
        }
        PRINTLN("MOVE LF@%s_%d GF@_", statement->non_nullable.name,
                statement->non_nullable.var->id);
    }
    generateCodeBody(&statement->if_body);
    PRINTLN("JUMP $if_%d_end", if_id);

    PRINTLN("LABEL $if_%d_else", if_id);
    generateCodeBody(&statement->else_body);
    PRINTLN("LABEL $if_%d_end", if_id);
}

void generateCodeWhileStatement(WhileStatement *statement) {
    static int while_counter = 0;
    int while_id = while_counter++;

    // TODO fix nested while loop redefinitions

    preGenerateBody(&statement->body);

    if (statement->non_nullable.name) {
        if (!statement->code_gen_defined) {
            PRINTLN("DEFVAR LF@%s_%d", statement->non_nullable.name,
                    statement->non_nullable.var->id);
            statement->code_gen_defined = true;
        }
    }
    PRINTLN("LABEL $while_%d_start", while_id);
    generateCodeExpression(&statement->condition);
    PRINTLN("POPS GF@_");

    PRINTLN("JUMPIFEQ $while_%d_end GF@_ %s", while_id,
            statement->non_nullable.name ? "nil@nil" : "bool@false");
    if (statement->non_nullable.name) {
        PRINTLN("MOVE LF@%s_%d GF@_", statement->non_nullable.name,
                statement->non_nullable.var->id);
    }

    generateCodeBody(&statement->body);
    PRINTLN("JUMP $while_%d_start", while_id);

    PRINTLN("LABEL $while_%d_end", while_id);
}

void generateCodeReturnStatement(ReturnStatement *statement) {
    generateCodeExpression(&statement->value);
    PRINTLN("POPFRAME");
    PRINTLN("RETURN");
}

void generateCodeExpression(Expression *expression) {
    switch (expression->expr_type) {
    case IdentifierExpressionType:
        generateCodeIdentifier(&expression->data.identifier);
        break;
    case FunctionCallExpressionType:
        generateCodeFunctionCall(&expression->data.function_call);
        break;
    case LiteralExpressionType:
        generateCodeLiteral(&expression->data.literal);
        break;
    case BinaryExpressionType:
        generateCodeBinaryExpression(&expression->data.binary_expr);
        break;
    }

    switch (expression->conversion) {
    case NoConversion:
        break;
    case IntToFloat:
        PRINTLN("INT2FLOATS");
        break;
    case FloatToInt:
        PRINTLN("FLOAT2INTS");
        break;
    default:
        break;
    }
}

void generateCodeFunctionCall(FunctionCall *function_call) {
    int size = getSize(function_call->arguments);
    for (int i = 0; i < size; i++) {
        Expression *expr = (Expression *)getDataAtIndex(function_call->arguments, i);
        generateCodeExpression(expr);
    }

    PRINTLN("CALL function_%s", function_call->func_id.name);
}

void generateCodeIdentifier(Identifier *identifier) {
    PRINTLN("PUSHS LF@%s_%d", identifier->name, identifier->var->id);
}

void generateCodeLiteral(Literal *literal) {
    switch (literal->data_type.data_type) {
    case dTypeI32:
        PRINTLN("PUSHS int@%s", literal->value);
        break;
    case dTypeF64: {
        double d = strtod(literal->value, NULL);
        PRINTLN("PUSHS float@%a", d);
    }

    break;
    case dTypeU8:
        printf("PUSHS string@");
        for (char *c = literal->value; *c; c++) {
            if (*c <= 32 || *c == 35 || *c == 92) {
                printf("\\%03d", *c);
            } else {
                printf("%c", *c);
            }
        }
        printf("\n");
        break;
    case dTypeBool:
        PRINTLN("PUSHS bool@%s", literal->value);
        break;
    case dTypeVoid:
        PRINTLN("PUSHS nil@nil");
    case dTypeUndefined:
    case dTypeNone:
        break;
    }
}

void generateCodeBinaryExpression(BinaryExpression *binary_expression) {
    generateCodeExpression(binary_expression->left);
    generateCodeExpression(binary_expression->right);

    switch (binary_expression->operation) {
    case TOKEN_PLUS:
        PRINTLN("ADDS");
        break;
    case TOKEN_MINUS:
        PRINTLN("SUBS");
        break;
    case TOKEN_MULTIPLY:
        PRINTLN("MULS");
        break;
    case TOKEN_DIVIDE:
        if (binary_expression->left->data_type.data_type == dTypeI32 &&
            binary_expression->right->data_type.data_type == dTypeI32)
            PRINTLN("IDIVS");
        else
            PRINTLN("DIVS");
        break;
    case TOKEN_EQUALS:
        PRINTLN("EQS");
        break;
    case TOKEN_NOTEQUAL:
        PRINTLN("EQS");
        PRINTLN("NOTS");
        break;
    case TOKEN_LESSTHAN:
        PRINTLN("LTS");
        break;
    case TOKEN_LESSOREQUAL:
        PRINTLN("GTS");
        PRINTLN("NOTS");
        break;
    case TOKEN_GREATERTHAN:
        PRINTLN("GTS");
        break;
    case TOKEN_GREATEROREQUAL:
        PRINTLN("LTS");
        PRINTLN("NOTS");
        break;
    default:
        break;
    }
}

void preGenerateBody(Body *body) {
    int size = getSize(body->statements);
    for (int i = 0; i < size; i++) {
        Statement *statement = (Statement *)getDataAtIndex(body->statements, i);
        preGenerateStatement(statement);
    }
}

void preGenerateStatement(Statement *statement) {
    switch (statement->type) {
    case IfStatementType:
        preGenerateIfStatement(&statement->data.if_statement);
        break;
    case WhileStatementType:
        preGenerateWhileStatement(&statement->data.while_statement);
        break;
    case VariableDefinitionStatementType:
        preGenerateVariableDefinitionStatement(&statement->data.variable_definition_statement);
        break;
    default:
        // no need to pregenerate anything
        break;
    }
}

void preGenerateVariableDefinitionStatement(VariableDefinitionStatement *statement) {
    PRINTLN("DEFVAR LF@%s_%d", statement->id.name, statement->id.var->id);
    statement->code_gen_defined = true;
}

void preGenerateIfStatement(IfStatement *statement) {
    if (statement->non_nullable.name) {
        PRINTLN("DEFVAR LF@%s_%d", statement->non_nullable.name, statement->non_nullable.var->id);
        statement->code_gen_defined = true;
    }
    preGenerateBody(&statement->if_body);
    preGenerateBody(&statement->else_body);
}

void preGenerateWhileStatement(WhileStatement *statement) {
    if (statement->non_nullable.name) {
        PRINTLN("DEFVAR LF@%s_%d", statement->non_nullable.name, statement->non_nullable.var->id);
        statement->code_gen_defined = true;
    }
    preGenerateBody(&statement->body);
}
