CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c2x

.PHONY: all clean test

all: test

test: test/ch 

test/ch: ch_test.o ch.o utils.o
	$(CC) $(CFLAGS) ch_test.o ch.o utils.o -o ch_test
	./ch_test

utils.o: ../utils.c
	$(CC) $(CFLAGS) -c ../utils.c -o utils.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o *_test 
