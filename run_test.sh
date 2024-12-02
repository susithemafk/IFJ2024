#!/bin/bash
# /** AUTHOR
#  * 
#  * @author <247581> Martin Mendl
#  * @file run_test.sh
#  * @date 25.11. 2024
#  * @brief This script runs unit tests, as well as integration tests for the given translator binary.
#  * @note this script is run by the Makefile
#  */

# ANSI escape codes for colored output
RESET="\033[0m"
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
CYAN="\033[1;36m"
MAGENTA="\033[1;35m"
WHITE_DARK="\033[2;37m"

# Decorations
SEPARATOR="${BLUE}========================================================${RESET}"

# Helper function to print a formatted message
print_section() {
    echo -e "\n$SEPARATOR"
    echo -e "${MAGENTA}$1${RESET}"
    echo -e "$SEPARATOR"
}

# Exit with error code if the program fails
handle_exit_code() {
    local exit_code=$1
    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}Error: Program exited with code $exit_code${RESET}"
        exit $exit_code
    else
        echo -e "${GREEN}Program exited successfully.${RESET}"
    fi
}

# Check if the ./tests directory exists
if [ ! -d "./tests" ]; then
    echo -e "${RED}Error: Tests directory './tests' not found.${RESET}"
    exit 1
fi

print_section "STARTING TEST"

# Fetch arguments
TEST_NAME=$1     # Test name (e.g., bst, list, integration ..)
DEBUG_FLAG=$2    # Debug mode (true/false)
USE_VALGRIND=$3  # Use valgrind (true/false)
TESTCASE=$4      # Test case (optional)
PRINT=$5         # Print output (optional)

# Set default values for optional parameters
if [ -z "$TESTCASE" ]; then
    echo "No test case provided. Running all test cases."
    TESTCASE="-1"
fi

if [ -z "$PRINT" ]; then
    PRINT=true
fi

# Check if the test is integration
if [ "$TEST_NAME" = "integration" ]; then
    echo "Doing integration tests"
else
    # Validate test name for other tests
    if [ -z "$TEST_NAME" ] || [ ! -f "./tests/$TEST_NAME.c" ]; then
        echo -e "${RED}Error: Invalid or missing test name.${RESET}"
        echo -e "${CYAN}Available tests are:${RESET}"
        # List all .c files in the ./tests directory with `-` and colors
        ls ./tests/*.c | sed 's|.*/||; s|\.c$||' | while read -r line; do
            echo -e "${WHITE_DARK}-${RESET} ${line}"
        done
        echo -e "${CYAN}Or run with file ${YELLOW}integration${CYAN} for integration tests.${RESET}"
        exit 0
    fi
fi

# Validate debug flag
if [ "$DEBUG_FLAG" != "true" ] && [ "$DEBUG_FLAG" != "false" ]; then
    echo -e "${RED}Error: Invalid debug flag. Use 'true' or 'false'.${RESET}"
    exit 1
fi

# Validate valgrind flag
if [ "$USE_VALGRIND" != "true" ] && [ "$USE_VALGRIND" != "false" ]; then
    echo -e "${RED}Error: Invalid valgrind flag. Use 'true' or 'false'.${RESET}"
    exit 1
fi

# Print received arguments
echo -e "${GREEN}Test Name: $TEST_NAME${RESET}"
echo -e "${GREEN}Debug Mode: $DEBUG_FLAG${RESET}"
echo -e "${GREEN}Use Valgrind: $USE_VALGRIND${RESET}"

# Compiler and flags
CC="gcc"
CFLAGS="-std=c99 -Wall -Wextra -pedantic -fcommon -DUSE_CUSTOM_STRUCTURE"
INCLUDES="-Iinclude"

# Directories
SRC_DIR="src"
TEST_DIR="tests"

# Source files
SRC_FILES=$(find $SRC_DIR -type f -name "*.c" | grep -v "/main.c$")

# Add the main.c file from the src directory for integration tests add the test.c file for other tests
if [ "$TEST_NAME" == "integration" ]; then
    SRC_FILES="$SRC_FILES $SRC_DIR/main.c"
else
    SRC_FILES="$SRC_FILES $TEST_DIR/$TEST_NAME.c"
fi

# Add debug flag
if [ "$DEBUG_FLAG" == "true" ]; then
    CFLAGS+=" -DDEBUG"
fi

# Compile the program
OUTPUT_BIN="test_program"
print_section "COMPILING TEST: @$TEST_NAME.c"
echo -e "${YELLOW}Compiling with flags: $CFLAGS${RESET}"
$CC $CFLAGS $INCLUDES $SRC_FILES -o $OUTPUT_BIN
handle_exit_code $?

# Run integration tests (passing testcase and print to integration_tests.sh)
if [ "$TEST_NAME" == "integration" ]; then
    echo "Running integration tests..."
    ./integration_tests.sh $OUTPUT_BIN $TESTCASE $PRINT $USE_VALGRIND

    handle_exit_code $?  # Check if the integration tests script ran successfully
else
    echo "Running unit tests..."
    # Run the program
    print_section "RUNNING TEST: @$TEST_NAME"
    if [ "$USE_VALGRIND" == "true" ]; then
        echo -e "${CYAN}Running with Valgrind...${RESET}"
        if [ "$(uname)" = "Darwin" ]; then
            echo -e "\033[1;32mUsing leaks on macOS...\033[0m"
            leaks --atExit --fullStacks -- ./$OUTPUT_BIN
        else
            echo -e "\033[1;32mUsing valgrind on non-macOS system...\033[0m"
            valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$OUTPUT_BIN
        fi
    else
        echo -e "${CYAN}Running normally...${RESET}"
        ./$OUTPUT_BIN
    fi
    handle_exit_code $? # Check exit code
fi

#Clean up
print_section "CLEANING UP"
rm -f ./$OUTPUT_BIN
echo -e "${GREEN}Cleanup complete.${RESET}"

# Done message
print_section "TEST COMPLETED"
echo -e "${GREEN}All tasks finished successfully.${RESET}"

exit 0
