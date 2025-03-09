CC = gcc
CFLAGS = -Wall -Wextra
OBJS = main.o hash_table.o trie.o utils.o

main: $(OBJS)
	$(CC) $(OBJS) -o main

main.o: main.c hash_table.h trie.h utils.h
	$(CC) $(CFLAGS) -c main.c

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -c trie.c

utils.o: utils.c utils.h trie.h hash_table.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f main $(OBJS)
