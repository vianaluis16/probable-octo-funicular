#include "hash_table.h"
#include "utils.h"
#include <ctype.h>

HashEntry **hashTable = NULL;
int tableSize = INITIAL_TABLE_SIZE;  // Tamanho inicial da tabela
int currentSize = 0;  // Número de elementos atualmente na tabela

// Função de hash
int hashFunction(const char *word) {
    unsigned long hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower(c); // hash * 33 + c
    }
    return hash % tableSize;
}

// Inicializa a tabela hash
void initializeHashTable() {
    hashTable = (HashEntry **)malloc(sizeof(HashEntry *) * tableSize);
    for (int i = 0; i < tableSize; i++) {
        hashTable[i] = NULL;
    }
}

// Função para redimensionar a tabela hash
void resizeHashTable() {
    int newTableSize = tableSize * 2;  // Dobrar o tamanho da tabela
    HashEntry **newTable = (HashEntry **)malloc(sizeof(HashEntry *) * newTableSize);

    for (int i = 0; i < newTableSize; i++) {
        newTable[i] = NULL;
    }

    // Rehashing: mover os elementos da tabela antiga para a nova
    for (int i = 0; i < tableSize; i++) {
        if (hashTable[i] != NULL) {
            int index = hashFunction(hashTable[i]->word) % newTableSize;
            while (newTable[index] != NULL) {  // Resolvendo colisões na nova tabela
                index = (index + 1) % newTableSize;
            }
            newTable[index] = hashTable[i];  // Inserir diretamente na nova tabela
        }
    }

    free(hashTable);  // Liberar a tabela antiga
    hashTable = newTable;  // A nova tabela se torna a tabela principal
    tableSize = newTableSize;  // Atualizar o tamanho da tabela
}

// Insere uma palavra e sua posição na tabela hash
void insertHash(char *word, int position) {
    if (currentSize >= tableSize * LOAD_FACTOR_THRESHOLD) {  // Se a tabela estiver 75% cheia
        resizeHashTable();  // Redimensiona a tabela
    }

    int index = hashFunction(word);
    int originalIndex = index;

    while (hashTable[index] != NULL && strcmp(hashTable[index]->word, word) != 0) {
        index = (index + 1) % tableSize; // Hashing linear para resolver colisões
        if (index == originalIndex) {
          printf("Tabela cheia\n");
          return;  // Tabela cheia
        }
    }

    if (hashTable[index] == NULL) {
        hashTable[index] = (HashEntry *)malloc(sizeof(HashEntry));
        strcpy(hashTable[index]->word, word);
        hashTable[index]->positions = (int *)malloc(sizeof(int));
        hashTable[index]->positions[0] = position;
        hashTable[index]->pos_count = 1;
        hashTable[index]->is_active = 1;
        currentSize++;  // Incrementa o número de elementos na tabela
    } else {
        hashTable[index]->positions = realloc(hashTable[index]->positions,
                                             (hashTable[index]->pos_count + 1) * sizeof(int));
        hashTable[index]->positions[hashTable[index]->pos_count++] = position;
    }
}

// Busca uma palavra na tabela hash
HashEntry *searchHash(const char *word) {
    int index = hashFunction(word);
    int originalIndex = index;

    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->word, word) == 0) {
            return hashTable[index];
        }
        index = (index + 1) % tableSize;
        if (index == originalIndex) {
            break;
        }
    }
    return NULL;
}

// Função para imprimir a tabela hash (índice remissivo)
void printHashTable(char *keywords[], int keyword_count) {
    qsort(keywords, keyword_count, sizeof(char *), compareStrings);
    printf("\nÍndice Remissivo:\n");
    for (int i = 0; i < keyword_count; i++) {
        printf("%s: ", keywords[i]);

        HashEntry *entry = searchHash(keywords[i]);
        if (entry) {
            for (int j = 0; j < entry->pos_count; j++) {
                printf("%d ", entry->positions[j]);
            }
        } else {
            printf("(não encontrada)");
        }
        printf("\n");
    }
}

// Função para imprimir a tabela hash completa
void printFullHashTable() {
    printf("\nTabela Hash Completa:\n");
    for (int i = 0; i < tableSize; i++) {
        if (hashTable[i] != NULL) {
            printf("[%d] Palavra: %s | Posições: ", i, hashTable[i]->word);
            for (int j = 0; j < hashTable[i]->pos_count; j++) {
                printf("%d ", hashTable[i]->positions[j]);
            }
            printf("\n");
        } else {
            printf("[%d] (vazio)\n", i);
        }
    }
}

// Função para processar o texto e construir o índice remissivo
void processTextHash(char *text, char *keywords[], int keyword_count) {
    int position = 1;
    char *ptr = text;

    while (*ptr) {
        while (*ptr && !isAlphaNumericUTF8(ptr)) {
            ptr++;
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
                insertHash(word, word_position);
            }
        }
    }
}

// Função para liberar a memória da tabela hash
void freeHashTable() {
    for (int i = 0; i < tableSize; i++) {
        if (hashTable[i] != NULL) {
            free(hashTable[i]->positions);  // Liberar posições
            free(hashTable[i]);
        }
    }
    free(hashTable);  // Liberar a tabela hash
    hashTable = NULL;
    tableSize = INITIAL_TABLE_SIZE;
    currentSize = 0;
}
