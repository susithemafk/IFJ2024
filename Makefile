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
LINKED_LIST_TEST_SRC = $(UTILITY_DIR)/linked_list.c $(TEST_DIR)/test_linked_list.c

# Targets
all: main

main: $(SRC_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) -o main

clean:
	rm -f main test_list

run: main
	./main

valgrind: main
	valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes ./main

# Compile and run the test for the linked list
test-list: $(LINKED_LIST_TEST_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKED_LIST_TEST_SRC) -o test_list
	./test_list
	rm -f test_list
