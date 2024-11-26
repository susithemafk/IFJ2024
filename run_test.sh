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

# Check if binary name is passed as an argument
if [ -z "$1" ]; then
    echo -e "${RED}Usage: $0 <binary-name>${RESET}"
    exit 1
fi

# Binary name
BINARY_NAME=$1

# Path to the input Zig file and the expected output file
ZIG_FILE="./test_inputs/code_gen/code_gen.zig"
EXPECTED_OUTPUT="./test_inputs/code_gen/expected_out.out"

# Compile the Zig file (assuming zig build-exe is the appropriate command)
echo -e "${YELLOW}Compiling $ZIG_FILE...${RESET}"
zig build-exe $ZIG_FILE -o $BINARY_NAME

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed.${RESET}"
    exit 1
fi

# Run the compiled binary and redirect its output to a temporary file
echo -e "${CYAN}Running the compiled binary...${RESET}"
./$BINARY_NAME > prg.out

# Compare the output with the expected output
echo -e "${BLUE}Comparing output with expected output...${RESET}"
if diff prg.out $EXPECTED_OUTPUT > /dev/null; then
    echo -e "${GREEN}Test passed: Output matches expected.${RESET}"
    rm -f ./$BINARY_NAME  # Remove the binary after success
    exit 0
else
    echo -e "${RED}Test failed: Output does not match expected.${RESET}"
    echo -e "${MAGENTA}Expected Output:${RESET}"
    cat $EXPECTED_OUTPUT
    echo -e "${MAGENTA}Actual Output:${RESET}"
    cat prg.out
    rm -f ./$BINARY_NAME  # Remove the binary after failure
    exit 1
fi
