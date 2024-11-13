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
AST_ASSETS_DIR = $(SRC_DIR)/ast_assets

# Test directory
TEST_DIR = tests

# Collect all .c files from directories
SRC_FILES = $(wildcard $(CODE_GEN_DIR)/*.c) \
            $(wildcard $(LEXICAL_DIR)/*.c) \
            $(wildcard $(SEMANTICAL_DIR)/*.c) \
            $(wildcard $(SYNTAXICAL_DIR)/*.c) \
            $(wildcard $(UTILITY_DIR)/*.c) \
            $(wildcard $(SRC_DIR)/*.c) \
			$(wildcard $(AST_ASSETS_DIR)/*.c)

# Linked list test source files
LINKED_LIST_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_linked_list.c

# Binary search tree test source files
BST_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_binary_search_tree.c

# Symbol table test source files
SYMTABLE_TEST_SRC = $(wildcard $(UTILITY_DIR)/*.c) $(AST_ASSETS_DIR)/inbuild_funcs.c $(SEMANTICAL_DIR)/symtable.c $(SEMANTICAL_DIR)/abstract_syntax_tree.c $(TEST_DIR)/test_symtable.c

# Lexical analyzer test source files
LEX_TEST_SRC = $(wildcard $(LEXICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(TEST_DIR)/test_lex.c

# AST test source files
AST_TEST_SRC = $(wildcard $(SEMANTICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(AST_ASSETS_DIR)/inbuild_funcs.c $(TEST_DIR)/test_ast.c

# AST integration test source files
AST_INTEGRATION_TEST_SRC = $(wildcard $(SEMANTICAL_DIR)/*.c) $(wildcard $(UTILITY_DIR)/*.c) $(wildcard $(AST_ASSETS_DIR)/*.c) $(TEST_DIR)/test_ast_integration.c

# Targets
all: main

main: $(SRC_FILES)
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) -o main

# Clean target to remove all executables
clean:
	@rm -f $(wildcard test_*)
	@rm -f main  # Optional, if you want to explicitly remove the main program
	@rm -f ifj_to_go.zip

run: main
	@./main < src/input.txt

valgrind: main
	@valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes ./main

# Compile and run the test for the linked list
test-list: $(LINKED_LIST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKED_LIST_TEST_SRC) -o test_list -std=c99
	@./test_list
	@rm -f test_list

test-bst: $(BST_TEST_SRC)
	@$(CC) $(CFLAGS) $(INCLUDES) $(BST_TEST_SRC) -o test_bst -std=c99
	@./test_bst
	rm -f test_bst

# Compile and run the test for the binary search tree (BST)
test-symtable: $(SYMTABLE_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SYMTABLE_TEST_SRC) -o test_symtable -std=c99
	@./test_symtable
	@rm -f test_symtable

# Compile and run the test for the abstract syntax tree (AST)
test-ast: $(AST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(AST_TEST_SRC) -o test_ast -std=c99
	@./test_ast
	@rm -f test_ast

# Compile and run the test for the lexical analyzer (LEX)
test-lex: $(LEX_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LEX_TEST_SRC) -o test_lex -std=c99
	@./test_lex < ./tests/lexical/input.txt
	@rm -f test_lex

# Compile and run the integration test for creating the ASTs
test-ast-integration: $(AST_INTEGRATION_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(AST_INTEGRATION_TEST_SRC) -o test_ast_integration -std=c99
	@./test_ast_integration
	@rm -f test_ast_integration

# command to zip the whole project
zip:
	@cd ../ && zip -r ./IFJ2024/ifj_to_go.zip IFJ2024 > /dev/null 2>&1

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all:            Build the main program"
	@echo "  main:           Build the main program"
	@echo "  clean:          Remove all executables"
	@echo "  run:            Run the main program"
	@echo "  valgrind:       Run the main program with valgrind"
	@echo "  test-list:      Run the test for the linked list"
	@echo "  test-bst:       Run the test for the binary search tree"
	@echo "  test-symtable:  Run the test for the symbol table"
	@echo "  test-ast:       Run the test for the abstract syntax tree"
	@echo "  test-lex:       Run the test for the lexical analyzer"
	@echo "  test-ast-integration: Run the integration test for creating the ASTs"
	@echo "  zip:            Zip the whole project"
	@echo "  help:           Display this help message"

