CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon -g -fsanitize=address
INCLUDES=include/

all: main

main: src/*.c
	$(CC) $(CFLAGS) src/*.c -o main

clean:
	rm -f main test_list

run: main
	./main

valgrind: main
	valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes ./main

# Compile and run the test for the linked list
test-list: $(INCLUDES)linked_list.c $(INCLUDES)test_linked_list.c
	$(CC) $(CFLAGS) $(INCLUDES)linked_list.c $(INCLUDES)test_linked_list.c -o test_list
	./test_list
	rm -f test_list
