#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "lexical/scanner.h"
#include "utility/enumerations.h"
#include "syntaxical/parser_pass1.h"

void parser_init()
{
	puts("parser_init");

	SymTable *table = symTableInit();
	puts("table initialized");
	LinkedList *buffer = initLinkedList(false);
	puts("buffer initialized");

	enum ERR_CODES err = firstPass(table, stdin, buffer);
	puts("first pass done");


	TOKEN_PTR token = (TOKEN_PTR)getDataAtIndex(buffer, 0);
	printf("token value: %s\n", token->value);

	freeBuffer(&buffer);
	symTableFree(&table);
}