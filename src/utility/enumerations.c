/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file enumerations.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include "utility/enumerations.h"
#include <string.h>

char *_getKeyWordFromString(enum KEY_WORDS_SRC keyWord) {
    switch (keyWord) {
        case W_CONST:
            return "const";
        case W_ELSE:
            return "else";
        case W_FN:
            return "fn";
        case W_IF:
            return "if";
        case W_I32:
            return "i32";
        case W_F64:
            return "f64";
        case W_NULL:
            return "null";
        case W_PUB:
            return "pub";
        case W_RETURN:
            return "return";
        case W_U8:
            return "u8";
        case W_VAR:
            return "var";
        case W_VOID:
            return "void";
        case W_WHILE:
            return "while";
        default:
            return "unknown";
    }
}

enum KEY_WORDS_SRC _getEnumfromKeyWord(char *keyWord) {

    if (strcmp(keyWord, "const") == 0) {
        return W_CONST;
    } else if (strcmp(keyWord, "else") == 0) {
        return W_ELSE;
    } else if (strcmp(keyWord, "fn") == 0) {
        return W_FN;
    } else if (strcmp(keyWord, "if") == 0) {
        return W_IF;
    } else if (strcmp(keyWord, "i32") == 0) {
        return W_I32;
    } else if (strcmp(keyWord, "f64") == 0) {
        return W_F64;
    } else if (strcmp(keyWord, "null") == 0) {
        return W_NULL;
    } else if (strcmp(keyWord, "pub") == 0) {
        return W_PUB;
    } else if (strcmp(keyWord, "return") == 0) {
        return W_RETURN;
    } else if (strcmp(keyWord, "u8") == 0) {
        return W_U8;
    } else if (strcmp(keyWord, "var") == 0) {
        return W_VAR;
    } else if (strcmp(keyWord, "void") == 0) {
        return W_VOID;
    } else if (strcmp(keyWord, "while") == 0) {
        return W_WHILE;
    }

    return -1;
}

unsigned int hashString(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)hash;
}

