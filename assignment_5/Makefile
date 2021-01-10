CC     = gcc
CFLAGS = -g -std=c99 -pedantic -Wall

graph: graphCreate.c graphMain.c graphSearch.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f tree
