/**
 * @file my_utils.c
 *
 * @author Marek Sucharda xsucha18
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef USE_CUSTOM_STRUCTURE
#include "utility/my_utils.h"
#else
#include "my_utils.h"
#endif

// Function to check, if a character is a digit
bool isDigit(char input)
{
	return (input >= '0' && input <= '9');
}

// Function to check, if a character is a letter
int hex_to_int(char hex)
{
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;
	if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	return -1;
}

// Function to copy a string
char *copyString(char *string) {
    char *newString = malloc(strlen(string) + 1);
    if (!newString) {
        return NULL;
    }
    strcpy(newString, string);

    return newString;
}
