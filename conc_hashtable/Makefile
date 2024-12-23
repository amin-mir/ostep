CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

.PHONY: all clean test

all: test

test: test/seq-ll test/hasht-sl test/conc-hasht-sl

test/seq-ll: ll_test.o ll_seq.o utils.o
	$(CC) $(CFLAGS) ll_test.o ll_seq.o utils.o -o ll_test
	./ll_test

test/hasht-sl: hasht_test.o hasht_sl.o ll_seq.o utils.o
	$(CC) $(CFLAGS) hasht_test.o hasht_sl.o ll_seq.o utils.o -o ht_test
	./ht_test

test/conc-hasht-sl: conc_test.o hasht_sl.o ll_seq.o utils.o
	$(CC) $(CFLAGS) conc_test.o hasht_sl.o ll_seq.o utils.o -o conc_ht_sl
	./conc_ht_sl 4

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o ll_test ht_test conc_ht_sl
