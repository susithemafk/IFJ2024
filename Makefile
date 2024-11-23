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
TEST_DIR = tests

# Find all .c files in the tests directory
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Allow file argument for make command
file ?= $(word 1, $(TEST_FILES))

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

# Define the `printCmd` variable based on the platform
printCmd := $(shell if [ "$$(uname)" = "Darwin" ]; then echo "echo"; else echo "echo -e"; fi)

# Test names (adjust as needed)
TESTS = $(wildcard $(TEST_DIR)/*)

# Prepare the test environment
prepare:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "Preparing test environment ..."
	@chmod +x run_test.sh
	@$(printCmd) "\033[1;36mPermisions granted\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

# Targets
all: main

main: $(SRC_FILES) $(SRC_DIR)/main.c
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "Building main program ..."
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(SRC_DIR)/main.c -o $@
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;32mBuild completed successfully!   \033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

# Build test target (build only, no execution)
test:
	@if [ -f $(TEST_DIR)/$(file).c ]; then \
		./run_test.sh $(file) true false "-1" true; \
	elif [ "$(file)" = "integration" ]; then \
		./run_test.sh "$(file)" true false "$(testcase)" "$(print)"; \
	else \
		./run_test.sh; \
	fi

# Run specific test target
run:
	@if [ -f $(TEST_DIR)/$(file).c ]; then \
		./run_test.sh "$(file)" false false "-1" true; \
	elif [ "$(file)" = "integration" ]; then \
		./run_test.sh "$(file)" false false "$(testcase)" "$(print)"; \
	else \
		./run_test.sh; \
	fi

# Run specific test target with memory check
valgrind:
	@if [ -f $(TEST_DIR)/$(file).c ]; then \
		./run_test.sh "$(file)" false true "-1" true; \
	elif [ "$(file)" = "integration" ]; then \
		./run_test.sh "$(file)" false true "$(testcase)" "$(print)"; \
	else \
		./run_test.sh; \
	fi
	
clean:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;33mCleaning build files...         \033[0m"
	@rm -f main test_* ifj_to_go.zip
	@$(printCmd) "\033[1;32mAll build files cleaned\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

zip:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;33mZipping project...              \033[0m"
	@cd ../ && zip -rq ./IFJ2024/ifj_to_go.zip ./IFJ2024 -x "*/.git/*" "*/.gitignore" || \
		$(printCmd) "\033[1;31mZipping failed.\033[0m"
	@$(printCmd) "\033[1;32mProject zipped successfully: ../IFJ2024/ifj_to_go.zip\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

# help command
help:
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;33mCommand                        					\033[1;36m| \033[1;33mDescription                                           \033[1;36m| \033[1;33mExample      			        	\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m                           					\033[0m\033[1;36m| \033[0mCompile the main program                              \033[1;36m| \033[1;35mmake                     			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m run                    						\033[0m\033[1;36m| \033[0mRun a file (provide file=<test_name>)                 \033[1;36m| \033[1;35mmake run file=test1.c    			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m test                      					\033[0m\033[1;36m| \033[0mRun a file with debug macros enabled                  \033[1;36m| \033[1;35mmake test file=test1.c   			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m valgrind                  					\033[0m\033[1;36m| \033[0mRun a file under Valgrind (provide file=<test_name>)  \033[1;36m| \033[1;35mmake valgrind file=test1.c			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m run file=integration          					\033[0m\033[1;36m| \033[0mRun all integration tests                             \033[1;36m| \033[1;35mmake run file=integration     		\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m test file=integration          					\033[0m\033[1;36m| \033[0mRun all integration tests with debug macros           \033[1;36m| \033[1;35mmake test file=integration    		\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m valgrind file=integration      					\033[0m\033[1;36m| \033[0mRun all integration tests under Valgrind              \033[1;36m| \033[1;35mmake valgrind file=integration		\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m run file=integration testcase=<test_num> print=<bool>		\033[0m\033[1;36m| \033[0mRun specific integration test                         \033[1;36m| \033[1;35mmake run file=integration testcase=1	\033[1;36m	|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m test file=integration testcase=<test_num> print=<bool>		\033[0m\033[1;36m| \033[0mRun specific integration test with debug macros       \033[1;36m| \033[1;35mmake test file=integration testcase=1	   \033[1;36m	|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m valgrind file=integration testcase=<test_num> print=<bool>	\033[0m\033[1;36m| \033[0mRun specific integration test under Valgrind      	\033[1;36m| \033[1;35mmake valgrind file=integration testcase=1\033[1;36m  	|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m clean                		     				\033[0m\033[1;36m| \033[0mClean build files                                 	\033[1;36m| \033[1;35mmake clean             			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m zip          		             				\033[0m\033[1;36m| \033[0mZip the project                                   	\033[1;36m| \033[1;35mmake zip                 			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m help			                			\033[0m\033[1;36m| \033[0mShow this help message                                \033[1;36m| \033[1;35mmake help                			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	
	





