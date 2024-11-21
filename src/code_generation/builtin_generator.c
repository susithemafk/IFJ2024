#include "code_generation/builtin_generator.h"
#include <stdio.h>

#define PRINTLN(...)                                                                                                                                                                                   \
    do {                                                                                                                                                                                               \
        printf(__VA_ARGS__);                                                                                                                                                                           \
        printf("\n");                                                                                                                                                                                  \
    } while (0)


void generateFuncWrite() {
    PRINTLN("LABEL function_write");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_print");
    PRINTLN("POPS TF@to_print");
    PRINTLN("WRITE TF@to_print");
    
    PRINTLN("POPFRAME");
}

void generateFuncReadStr(){
    PRINTLN("LABEL function_readString");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read string");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
}
void generateFuncReadInt(){
    PRINTLN("LABEL function_readInt");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read int");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
}
void generateFuncReadFloat(){
    PRINTLN("LABEL function_readFloat");
    PRINTLN("PUSHFRAME");
    PRINTLN("CREATEFRAME");

    PRINTLN("DEFVAR TF@to_read");
    PRINTLN("READ TF@to_read float");
    PRINTLN("PUSHS TF@to_read");

    PRINTLN("POPFRAME");
}

void generateBuiltins() {
    PRINTLN("\n# Builtin functions\n");

    generateFuncWrite();
    generateFuncReadStr();
    generateFuncReadInt();
    generateFuncReadFloat();
}
