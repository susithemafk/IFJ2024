# Compiler settings
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -fcommon
DBFLAGS = -std=c99 -Wall -Wextra -pedantic -fcommon -g -fsanitize=address

# Include directory
INCLUDES = -Iinclude

# Source directories
SRC_DIR = src
CODE_GEN_DIR = $(SRC_DIR)/code_generation
LEXICAL_DIR = $(SRC_DIR)/lexical
SEMANTICAL_DIR = $(SRC_DIR)/semantical
SYNTAXICAL_DIR = $(SRC_DIR)/syntaxical
UTILITY_DIR = $(SRC_DIR)/utility

# Test directory
TEST_DIR = tests

# Collect all .c files from directories
SRC_FILES = $(wildcard $(CODE_GEN_DIR)/*.c) \
            $(wildcard $(LEXICAL_DIR)/*.c) \
            $(wildcard $(SEMANTICAL_DIR)/*.c) \
            $(wildcard $(SYNTAXICAL_DIR)/*.c) \
            $(wildcard $(UTILITY_DIR)/*.c) \
            $(wildcard $(SRC_DIR)/*.c)

# Linked list test source files
LINKED_LIST_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_linked_list.c

# Binary search tree test source files
BST_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_binary_search_tree.c

# Symbol table test source files
SYMTABLE_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(SEMANTICAL_DIR)/symtable.c $(SEMANTICAL_DIR)/abstract_syntax_tree.c $(TEST_DIR)/test_symtable.c

# Lexical analyzer test source files
LEX_TEST_SRC = $(wildcard $(LEXICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_lex.c

# Function calling validation test source files
FUNC_CALL_TEST_SRC = $(wildcard $(SEMANTICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_function_validation.c

# AST test source files
AST_TEST_SRC = $(wildcard $(SEMANTICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_ast.c


# Targets
all: main

main: $(SRC_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) -o main

clean:
	rm -f main test_list test_bst test_lex

run: main
	./main < src/input.txt

valgrind: main
	valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes ./main

# Compile and run the test for the linked list
test-list: $(LINKED_LIST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKED_LIST_TEST_SRC) -o test_list -std=c99
	./test_list
	rm -f test_list

test-bst: $(BST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(BST_TEST_SRC) -o test_bst -std=c99
	./test_bst
	rm -f test_bst

# Compile and run the test for the binary search tree (BST)
test-symtable: $(SYMTABLE_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SYMTABLE_TEST_SRC) -o test_symtable -std=c99
	./test_symtable
	rm -f test_symtable

# Compile and run the test for the abstract syntax tree (AST)
test-ast: $(AST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(AST_TEST_SRC) -o test_ast -std=c99
	./test_ast
	rm -f test_ast

# Compile and run the test for the lexical analyzer (LEX)
test-lex: $(LEX_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LEX_TEST_SRC) -o test_lex -std=c99
	./test_lex < ./src/input.txt
	rm -f test_lex

# Compile and run the test for the function calling validation
test-func-call: $(FUNC_CALL_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(FUNC_CALL_TEST_SRC) -o test_func_call -std=c99
	./test_func_call
	rm -f test_func_call
