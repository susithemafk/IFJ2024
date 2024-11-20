#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"
#include "semantical/symtable.h"
#include "syntaxical/parser.h"
#include "syntaxical/parser_pass1.h"

int main(void) {
	parser_init(stdin);
	return 0;
}
