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
#include "syntaxical/parser.h"

static enum ERR_CODES status;

int main(void)
{
	FILE *input = stdin;
	puts("_______Initiating scanner_______");
	scanner_init(input);
	puts("_______Initiating parser_______");

	status = parser_init();

	if (status != SUCCESS)
	{
		printf("Chyba pri inicializaci parseru!\n");
		return status;
	}

	status = parser_parse();

	if (status == SUCCESS)
	{
		printf("Syntaxe je spravna!\n");
		// AST is ready to work with now
	}
	else
	{
		printf("Chyba v syntaxi!\n");
	}

	return 0;
}
