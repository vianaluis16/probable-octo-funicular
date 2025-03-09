#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 100
#define LOAD_FACTOR_THRESHOLD 0.75  // Tabela hash será redimensionada quando atingir 75% de capacidade

typedef struct HashEntry {
    char word[100];
    int *positions;
    int pos_count;
    int is_active;
} HashEntry;

extern HashEntry **hashTable;  // Alterado para ponteiro duplo, já que a tabela pode ser redimensionada
extern int tableSize;  // Tamanho da tabela atual
extern int currentSize;  // Número de entradas na tabela

void initializeHashTable();
void insertHash(char *word, int position);
HashEntry *searchHash(const char *word);
void printHashTable(char *keywords[], int keyword_count);
void printFullHashTable();
void processTextHash(char *text, char *keywords[], int keyword_count);
void freeHashTable();
void resizeHashTable();  // Função para redimensionar a tabela

#endif
