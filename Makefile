# Compiler settings
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -fcommon
DBFLAGS = $(CFLAGS) -g -fsanitize=address
INCLUDES = -Iinclude

# Directories
SRC_DIR = src
CODE_GEN_DIR = $(SRC_DIR)/code_generation
LEXICAL_DIR = $(SRC_DIR)/lexical
SEMANTICAL_DIR = $(SRC_DIR)/semantical
SYNTAXICAL_DIR = $(SRC_DIR)/syntaxical
UTILITY_DIR = $(SRC_DIR)/utility
AST_ASSETS_DIR = $(SRC_DIR)/ast_assets
TEST_DIR = tests
BUILD_DIR = build

# Debug flag
DEBUG_FLAG = -DDEBUG

# main src files without the main.c
SRC_FILES = $(filter-out $(SRC_DIR)/main.c, \
            $(wildcard $(CODE_GEN_DIR)/*.c) \
            $(wildcard $(LEXICAL_DIR)/*.c) \
            $(wildcard $(SEMANTICAL_DIR)/*.c) \
            $(wildcard $(SYNTAXICAL_DIR)/*.c) \
            $(wildcard $(UTILITY_DIR)/*.c) \
            $(wildcard $(SRC_DIR)/*.c) \
            $(wildcard $(AST_ASSETS_DIR)/*.c))

# Targets
all: main

main: $(SRC_FILES) $(SRC_DIR)/main.c
	@echo "Building main program..."
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(SRC_DIR)/main.c -o $@
	@echo "Done."

run: main
	@./main < src/input.txt

# Build test target (build only, no execution)
test_%: $(TEST_DIR)/test_%.c
	@echo "Building $@..."
	@rm -f $@
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(TEST_DIR)/test_$*.c -o $@ -std=c99
	@echo "Done."

# Run specific test target
run_%:
	@echo "Building $@..."
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUG_FLAG) $(SRC_FILES) $(TEST_DIR)/test_$*.c -o $@ -std=c99
	@echo "running $@..."
	@if [ "$*" = "lex" ]; then \
		./$@ < ./tests/lexical/input.txt; \
	else \
		./$@; \
	fi
	@rm -f $@
	@echo "Cleaning $@..."
	@echo "Done."

# Run specific test target with memory check
valgrind_%:
	@echo "Building $@..."
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(TEST_DIR)/test_$*.c -o $@ -std=c99
	@echo "Running $@ with memory checker..."
	@if [ "$*" = "lex" ]; then \
		if [ "$$(uname)" = "Darwin" ]; then \
			echo "Using leaks on macOS for lex test..."; \
			leaks --atExit --fullStacks -- ./test_$* < ./tests/lexical/input.txt; \
		else \
			echo "Using valgrind on non-macOS system for lex test..."; \
			valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes --show-leak-kinds=all ./test_$* < ./tests/lexical/input.txt; \
		fi; \
	else \
		if [ "$$(uname)" = "Darwin" ]; then \
			echo "Using leaks on macOS..."; \
			leaks --atExit --fullStacks -- ./$@; \
		else \
			echo "Using valgrind on non-macOS system..."; \
			valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes --show-leak-kinds=all ./$@; \
		fi; \
	fi
	@rm -f $@
	@echo "Cleaning $@..."
	@echo "Done."


clean:
	@echo "Cleaning build files..."
	@rm -f main test_* ifj_to_go.zip
	@rm -f 

zip:
	@echo "Zipping project..."
	@cd ../ && zip -rq ./IFJ2024/ifj_to_go.zip ./IFJ2024 -x "*/.git/*" "*/.gitignore" || echo "Zipping failed."
	@echo "Project zipped successfully: ../IFJ2024/ifj_to_go.zip"

# Define the `printCmd` variable based on the platform
printCmd := $(shell if [ "$$(uname)" = "Darwin" ]; then echo "echo"; else echo "echo -e"; fi)

# help command
help:
	@$(printCmd) "\033[1;36m+--------------------------------+-----------------------------------------+--------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;33mCommand                        \033[1;36m| \033[1;33mDescription                             \033[1;36m| \033[1;33mExample                  \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+--------------------------------+-----------------------------------------+--------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m                           \033[0m\033[1;36m| \033[0mCompile the main program                \033[1;36m| \033[1;35mmake                     \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m run                       \033[0m\033[1;36m| \033[0mCompile and run the main program        \033[1;36m| \033[1;35mmake run                 \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m test_<test_name>          \033[0m\033[1;36m| \033[0mCompile the test program                \033[1;36m| \033[1;35mmake test_ast            \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m run_<test_name>      	 \033[0m\033[1;36m| \033[0mCompile and run the test program        \033[1;36m| \033[1;35mmake run_ast             \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m valgrind_<test_name> 	 \033[0m\033[1;36m| \033[0mCompile and run the test with Valgrind  \033[1;36m| \033[1;35mmake valgrind_ast        \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m clean                     \033[0m\033[1;36m| \033[0mClean build files                       \033[1;36m| \033[1;35mmake clean               \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m zip                       \033[0m\033[1;36m| \033[0mZip the project                         \033[1;36m| \033[1;35mmake zip                 \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m help                      \033[0m\033[1;36m| \033[0mShow this help message                  \033[1;36m| \033[1;35mmake help                \033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+--------------------------------+-----------------------------------------+--------------------------+\033[0m"





