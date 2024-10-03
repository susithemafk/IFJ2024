/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file enumerations.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include "utility/enumerations.h"
#include <string.h>

char *_getKeyWordFromString(enum KEY_WORDS_TYPES keyWord) {
    switch (keyWord) {
        case WT_CONST:
            return "const";
        case WT_ELSE:
            return "else";
        case WT_FN:
            return "fn";
        case WT_IF:
            return "if";
        case WT_I32:
            return "i32";
        case WT_F64:
            return "f64";
        case WT_NULL:
            return "null";
        case WT_PUB:
            return "pub";
        case WT_RETURN:
            return "return";
        case WT_U8:
            return "u8";
        case WT_VAR:
            return "var";
        case WT_VOID:
            return "void";
        case WT_WHILE:
            return "while";
        default:
            return "unknown";
    }
}

enum KEY_WORDS_TYPES _getEnumfromKeyWord(char *keyWord) {

    if (strcmp(keyWord, "const") == 0) {
        return WT_CONST;
    } else if (strcmp(keyWord, "else") == 0) {
        return WT_ELSE;
    } else if (strcmp(keyWord, "fn") == 0) {
        return WT_FN;
    } else if (strcmp(keyWord, "if") == 0) {
        return WT_IF;
    } else if (strcmp(keyWord, "i32") == 0) {
        return WT_I32;
    } else if (strcmp(keyWord, "f64") == 0) {
        return WT_F64;
    } else if (strcmp(keyWord, "null") == 0) {
        return WT_NULL;
    } else if (strcmp(keyWord, "pub") == 0) {
        return WT_PUB;
    } else if (strcmp(keyWord, "return") == 0) {
        return WT_RETURN;
    } else if (strcmp(keyWord, "u8") == 0) {
        return WT_U8;
    } else if (strcmp(keyWord, "var") == 0) {
        return WT_VAR;
    } else if (strcmp(keyWord, "void") == 0) {
        return WT_VOID;
    } else if (strcmp(keyWord, "while") == 0) {
        return WT_WHILE;
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

