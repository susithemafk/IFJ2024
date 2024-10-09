/**
 * IFJ projeKt 2024
 *
 * @author <xsucha18> Marek Sucharda
 * @author <xsvobov00> Veronika Svobodová
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility/enumerations.h"

#include "lexical/scanner.h"

static enum ERR_CODES status;
static struct TOKEN token;

int main(void)
{
	FILE *input = stdin;
	scanner_init(input);

	status = scanner_get_token(&token);

	while (status == SUCCESS && token.type != TOKEN_EOF)
	{
		// printf("Token type: %d\n", token.type);
		printf("Token value: %s\n", token.value);
		status = scanner_get_token(&token);
	}

	return 0;
}
