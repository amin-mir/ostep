CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

.PHONY: all clean test

all: test

test: test/sll test/cll test/sht test/mht test/sht-load test/mht-load

test/sll: sll_test.o sll.o utils.o
	$(CC) $(CFLAGS) sll_test.o sll.o utils.o -o sll_test
	./sll_test

test/cll: cll_test.o cll.o utils.o
	$(CC) $(CFLAGS) cll_test.o cll.o utils.o -o cll_test
	./cll_test

test/sht: sht_test.o sht.o sll.o utils.o
	$(CC) $(CFLAGS) sht_test.o sht.o sll.o utils.o -o sht_test
	./sht_test

test/mht: mht_test.o mht.o cll.o utils.o
	$(CC) $(CFLAGS) mht_test.o mht.o cll.o utils.o -o mht_test
	./mht_test

test/sht-load: ht_load_test.o sht.o sll.o utils.o
	$(CC) $(CFLAGS) -DSL ht_load_test.o sht.o sll.o utils.o -o sht_load_test
	./sht_load_test 2

test/mht-load: ht_load_test.o mht.o cll.o utils.o
	$(CC) $(CFLAGS) ht_load_test.o mht.o cll.o utils.o -o mht_load_test
	./mht_load_test 2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o *_test 
