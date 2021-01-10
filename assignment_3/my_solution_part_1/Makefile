CC = gcc

CFLAGS = -g -O2 -std=c99 -pedantic -Wall -lm

speller: dictionary.h dictionary.c speller.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: example check-example debug-example

example: speller
	./speller < exampleInput.txt

check-example: speller
	cat exampleInput.txt | ./speller | diff exampleOutput.txt -

debug-example: speller
	valgrind ./speller < exampleInput.txt

time-largeExample: speller
	time ./speller < large_exampleInput.txt
