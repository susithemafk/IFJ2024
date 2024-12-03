/**
 * Random utility functions
 * @file my_utils.h
 * @author Marek Sucharda xsucha18
 */
#include <stdbool.h>

/**
 * Function to check, if a character is a digin
 * 
 * @param input character to check
 * @return true if character is a digit, false otherwise
 */
bool isDigit(char input);

/**
 * Function to check, if a character is a letter
 * 
 * @param input character to check
 * @return true if character is a letter, false otherwise
 */
int hex_to_int(char hex);

/**
 * Function to copy a string
 * 
 * @param string to copy
 * @return copied string
*/
char *copyString(char *string);
