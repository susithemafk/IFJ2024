#!/bin/bash

# Define the output ZIP file name for testing
ZIP_FILE="xsucha18.zip"

# Define the working directory for submission files
SUBMIT_DIR="submit"

# Ensure clean start: remove existing submit directory if it exists
if [ -d "$SUBMIT_DIR" ]; then
    rm -rf "$SUBMIT_DIR"
fi

# remove the zip file, if it exists
if [ -f "$ZIP_FILE" ]; then
    rm "$ZIP_FILE"
fi

# Create the submit directory
mkdir -p "$SUBMIT_DIR/test_inputs"

# Collect all .c and .h files, flattening the structure
FILES=$(find src include -type f \( -name "*.c" -o -name "*.h" \) -exec basename {} \;)

# Copy files into the submit directory
for FILE in $FILES; do
    find src include -type f -name "$FILE" -exec cp {} "$SUBMIT_DIR" \;
done

# Add additional required files
cp ./docs/rozdeleni "$SUBMIT_DIR/rozdeleni"
cp ./docs/dokumentace.pdf "$SUBMIT_DIR/dokumentace.pdf"

# Generate the new Makefile in the submit directory
cat << 'EOF' > "$SUBMIT_DIR/Makefile"
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -fcommon

# Collect all .h and .c files in the current directory
INCLUDES = $(wildcard ./*.h)
SOURCES = $(wildcard ./*.c)
TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
EOF

# Change into the submit directory
cd "$SUBMIT_DIR"

# Create the ZIP file
zip "../$ZIP_FILE" ./*.c ./*.h ./rozdeleni ./dokumentace.pdf ./Makefile > /dev/null 2>&1

# Move back to the original directory
cd ..

# Clean up the submit directory
rm -rf "$SUBMIT_DIR"

echo "Test ZIP file created: $ZIP_FILE"
