#include "trie.h"
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "utils.h"
static TrieNode *trieRoot = NULL;

TrieNode *createTrieNode() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->is_end_of_word = 0;
    node->pos_count = 0;
    node->positions = NULL;
    for (int i = 0; i < 256; i++) node->children[i] = NULL;
    return node;
}



void insertTrie(TrieNode *root, char *word, int position) {
    TrieNode *node = root;

    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }

    while (*word) {
        unsigned char c = (unsigned char)*word;
        if (!node->children[c]) node->children[c] = createTrieNode();
        node = node->children[c];
        word++;
    }
    node->is_end_of_word = 1;
    node->positions = realloc(node->positions, (node->pos_count + 1) * sizeof(int));
    node->positions[node->pos_count++] = position;
}

TrieNode *searchTrie(TrieNode *root, char *word) {
    TrieNode *node = root;

    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }

    while (*word) {
        unsigned char c = (unsigned char)*word;
        if (!node->children[c]) return NULL;
        node = node->children[c];
        word++;
    }
    return node->is_end_of_word ? node : NULL;
}

void processTextTrie(char *text, char *keywords[], int keyword_count) {
    trieRoot = createTrieNode(); // Corrigido: inicializando a Trie na variável global
    int position = 1;  // Contador de posição
    char *ptr = text;

    while (*ptr) {
        while (*ptr && (!isAlphaNumericUTF8(ptr))) {
            int len = mbtowc(NULL, ptr, MB_CUR_MAX);
            ptr += (len > 0) ? len : 1;
            position++;
        }
        if (!*ptr) break;

        char word[100];
        int len = 0;
        char *start_pos = ptr;

        while (*ptr && isAlphaNumericUTF8(ptr) && len < 99) {
            int char_len = mbtowc(NULL, ptr, MB_CUR_MAX);
            if (char_len <= 0) break;

            for (int i = 0; i < char_len; i++) {
                word[len++] = ptr[i];
            }
            ptr += char_len;
        }
        word[len] = '\0';

        if (ispunct(word[len - 1])) {
            word[--len] = '\0';
        }

        int word_position = 1;
        char *scan = text;
        while (scan < start_pos) {
            int char_len = mbtowc(NULL, scan, MB_CUR_MAX);
            if (char_len <= 0) char_len = 1;
            scan += char_len;
            word_position++;
        }

        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }

        for (int i = 0; i < keyword_count; i++) {
            char temp[100];
            strcpy(temp, keywords[i]);

            for (int j = 0; temp[j]; j++) {
                temp[j] = tolower(temp[j]);
            }

            if (strcmp(word, temp) == 0) {
                insertTrie(trieRoot, word, word_position);
            }
        }
    }
}

void remissiveIndexTrie(char *keywords[], int keyword_count) {
    printf("\nÍndice remissivo em ordem alfabética:\n");

    for (int i = 0; i < keyword_count; i++) {
        printf("%s: ", keywords[i]);

        char temp[100];
        strcpy(temp, keywords[i]);

        // Converte a palavra para minúscula
        for (int j = 0; temp[j]; j++) {
            temp[j] = tolower(temp[j]);
        }

        // Busca na Trie
        TrieNode *node = searchTrie(trieRoot, temp);
        if (node) {
            // Imprime as posições onde a palavra foi encontrada
            for (int j = 0; j < node->pos_count; j++) {
                printf("%d ", node->positions[j]);
            }
        } else {
            // Caso a palavra não seja encontrada
            printf("(não encontrada)");
        }
        printf("\n");
    }
}



void printTrie(TrieNode *node, char *prefix, int depth) {
    if (!node) return;

    for (int i = 0; i < 256; i++) {
        if (node->children[i]) {
            char new_prefix[100];
            snprintf(new_prefix, sizeof(new_prefix), "%s%c", prefix, i);

            for (int j = 0; j < depth; j++) printf("  ");  // Adiciona espaçamento para a profundidade
            printf("└── %c\n", i);

            printTrie(node->children[i], new_prefix, depth + 1);
        }
    }
}


void freeTrie(TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < 256; i++) {
        if (root->children[i]) {
            freeTrie(root->children[i]);
        }
    }
    if (root->positions) {
        free(root->positions);
    }
    free(root);
}

TrieNode *getTrieRoot() {
    return trieRoot;
}

