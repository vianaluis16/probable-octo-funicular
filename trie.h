#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode {
    struct TrieNode *children[256];
    int *positions;
    int pos_count;
    int is_end_of_word;
} TrieNode;

TrieNode *createTrieNode();
TrieNode *searchTrie(TrieNode *root, char *word);
void freeTrie(TrieNode *root);
void processTextTrie(char *text, char *keywords[], int keyword_count);
void insertTrie(TrieNode *root, char *word, int position);
void remissiveIndexTrie(char *keywords[], int keyword_count);
void printTrie(TrieNode *node, char *prefix, int depth) ;
TrieNode *getTrieRoot();



#endif
