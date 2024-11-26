#!/bin/bash

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

# Check if binary name is passed as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <binary-name>"
    exit 1
fi

# Binary name
BINARY_NAME=$1

# Path to the input Zig file and the expected output file
ZIG_FILE="./test_inputs/code_gen/code_gen.zig"
EXPECTED_OUTPUT="./test_inputs/code_gen/expected_out.out"

# Temporary files
PROGRAM_CODE="program.code"
PRG_OUT="prg.out"

# Run the compiled binary (assuming it's already built) and redirect its output to program.code
print_section "Running the compiled binary"
echo -e "${YELLOW}Running with: ./$BINARY_NAME < $ZIG_FILE > $PROGRAM_CODE${RESET}"
./$BINARY_NAME < $ZIG_FILE > $PROGRAM_CODE
if [ $? -ne 0 ]; then
    echo -e "${RED}Error running the program!${RESET}"
    rm -f ./$BINARY_NAME
    exit 1
fi

# Run the interpreter
print_section "Running the interpreter"
echo -e "${YELLOW}Running with: ic24int $PROGRAM_CODE > $PRG_OUT${RESET}"
ic24int $PROGRAM_CODE > $PRG_OUT
if [ $? -ne 0 ]; then
    echo -e "${RED}Error running the interpreter!${RESET}"
    rm -f ./$BINARY_NAME $PROGRAM_CODE
    exit 1
fi

# Compare the output with the expected output
print_section "Comparing output"
echo -e "${YELLOW}Comparing $PRG_OUT with $EXPECTED_OUTPUT${RESET}"
if diff $PRG_OUT $EXPECTED_OUTPUT > /dev/null; then
    echo -e "${GREEN}Test passed: Output matches expected.${RESET}"
else
    echo -e "${RED}Test failed: Output does not match expected.${RESET}"
    echo -e "${CYAN}Expected Output:${RESET}"
    cat $EXPECTED_OUTPUT
    echo -e "${CYAN}Actual Output:${RESET}"
    cat $PRG_OUT
    rm -f ./$BINARY_NAME $PROGRAM_CODE $PRG_OUT
    exit 1
fi

# Cleanup
print_section "Cleaning up"
rm -f ./$BINARY_NAME $PROGRAM_CODE $PRG_OUT
echo -e "${GREEN}Cleanup complete.${RESET}"

# Done message
print_section "TEST COMPLETED"
echo -e "${GREEN}All tasks finished successfully.${RESET}"

exit 0
