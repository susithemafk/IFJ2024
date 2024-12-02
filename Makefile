# /** AUTHOR
#  * 
#  * @author <247581> Martin Mendl
#  * @file Makefile
#  * @date 25.11. 2024
#  * @brief Makefile for IFJ project
#  */

# Compiler settings
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -fcommon -DUSE_CUSTOM_STRUCTURE
INCLUDES = -Iinclude

# Directories
SRC_DIR = src
CODE_GEN_DIR = $(SRC_DIR)/code_generation
LEXICAL_DIR = $(SRC_DIR)/lexical
SEMANTICAL_DIR = $(SRC_DIR)/semantical
SYNTAXICAL_DIR = $(SRC_DIR)/syntaxical
UTILITY_DIR = $(SRC_DIR)/utility
TEST_DIR = tests

#

# Find all .c files in the tests directory
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Allow file argument for make command
file ?= $(word 1, $(TEST_FILES))

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

# Prepare the test environment
prepare:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "Preparing test environment ..."
	@chmod +x run_test.sh
	@chmod +x code_gen_test.sh
	@chmod +x integration_tests.sh
	@chmod +x submit_make.sh
	@chmod u+x is_it_ok.sh
	@$(printCmd) "\033[1;36mPermisions granted\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

.PHONY: test_code_gen test run valgrind clean zip submit help

# test the code generation
test_code_gen:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "Testing code generation ..."
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(SRC_DIR)/main.c -o main
	@./code_gen_test.sh main 
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;32mCode generation tests done!   \033[0m"
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
	@rm -f main ifj_to_go.zip
	@rm -f xsucha18.zip Makefile.tmp
	@$(printCmd) "\033[1;32mAll build files cleaned\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

zip:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;33mZipping project...              \033[0m"
	@cd ../ && zip -rq ./IFJ2024/ifj_to_go.zip ./IFJ2024 -x "*/.git/*" "*/.gitignore" "*/snake/.env/*" || \
		$(printCmd) "\033[1;31mZipping failed.\033[0m"
	@$(printCmd) "\033[1;32mProject zipped successfully: ../IFJ2024/ifj_to_go.zip\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

submit:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;33mPreparing submission zip...       \033[0m"
	@./submit_make.sh
	@$(printCmd) "\033[1;32mSubmission zip created: ./xsucha18.zip\033[0m"
	@$(printCmd) "\033[1;36m==================================\033[0m"

control_submit:
	@$(printCmd) "\033[1;36m==================================\033[0m"
	@$(printCmd) "\033[1;33mPreparing submission zip...       \033[0m"
	@./submit_make.sh
	@$(printCmd) "\033[1;32mSubmission zip created: ./xsucha18.zip\033[0m"
	@rm -rf testdir
	@mkdir testdir
	./is_it_ok.sh ./xsucha18.zip testdir
	@rm -rf testdir
	@$(printCmd) "\033[1;36m==================================\033[0m"

.PHONY: cake

cake:
	@$(printCmd) "wait a minute, doing something cool ..."
	@cd ./snake && \
	if ! command -v python3 &> /dev/null; then \
		echo "Python3 not found. Installing..."; \
		if [ -f /etc/debian_version ]; then \
			sudo apt-get update && sudo apt-get install -y python3 python3-venv python3-pip; \
		elif [ -f /etc/fedora-release ]; then \
			sudo dnf install -y python3 python3-venv python3-pip; \
		else \
			echo "Unsupported Linux distribution. Please install Python3 manually."; \
			exit 1; \
		fi; \
	fi && \
	if [ ! -d ".env" ]; then \
		python3 -m venv .env; \
	fi && \
	source .env/bin/activate && \
	python3 -m pip install --quiet --upgrade pip pynput && \
	python3 snake.py && \
	stty sane && \
	read -t 1 -n 10000 discard || true && \
	clear


# help command
help:
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;33mCommand                        					\033[1;36m| \033[1;33mDescription                                           \033[1;36m| \033[1;33mExample      			        	\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m                           					\033[0m\033[1;36m| \033[0mCompile the main program                              \033[1;36m| \033[1;35mmake                     			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m prepare                     					\033[0m\033[1;36m| \033[0mPrepare the test environment                          \033[1;36m| \033[1;35mmake prepare             			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m test_code_gen               					\033[0m\033[1;36m| \033[0mRun code generation tests                             \033[1;36m| \033[1;35mmake test_code_gen         			\033[1;36m|\033[0m"
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
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m submit          		             				\033[0m\033[1;36m| \033[0mCreate submission zip                            	\033[1;36m| \033[1;35mmake submit                 			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m control_submit          		             			\033[0m\033[1;36m| \033[0mCreate submission zip and check it with is_it_ok.sh	\033[1;36m| \033[1;35mmake control_submit               	  	\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m help			                			\033[0m\033[1;36m| \033[0mShow this help message                                \033[1;36m| \033[1;35mmake help                			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m| \033[1;32mmake\033[2;37m cake 			                			\033[0m\033[1;36m| \033[0mWhat coud this do? ;P                             	\033[1;36m| \033[1;35mmake cake                 			\033[1;36m|\033[0m"
	@$(printCmd) "\033[1;36m+-----------------------------------------------------------------------+-------------------------------------------------------+-----------------------------------------------+\033[0m"
	
	





