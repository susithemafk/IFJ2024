CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon
# CFLAGS=-g
DBFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon -g -fsanitize=address

all: main

main: src/*.c
	$(CC) $(CFLAGS) src/*.c -o main

clean:
	rm -f main

run: main
	./main

valgrind: main
	valgrind -s --leak-check=full --track-origins=yes --dsymutil=yes ./main