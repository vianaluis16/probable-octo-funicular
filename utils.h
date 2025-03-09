#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>
#include "trie.h"

int isAlphaNumericUTF8(const char *ptr);
void processText(char *text, char *keywords[], int keyword_count, TrieNode *trieRoot);

#endif
