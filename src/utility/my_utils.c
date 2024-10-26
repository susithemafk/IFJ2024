#include "utility/my_utils.h"

bool isDigit(char input)
{
	return (input >= '0' && input <= '9');
}

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