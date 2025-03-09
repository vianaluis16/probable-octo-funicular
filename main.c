#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include "hash_table.h"
#include "trie.h"
#include "utils.h"


// Declaração explícita de strdup
#ifndef _POSIX_C_SOURCE
char *strdup(const char *s);
#endif

int main() {
    setlocale(LC_ALL, "");

    TrieNode *root = getTrieRoot();
    if (root != NULL) {
        char prefix[100] = "";
        printTrie(root, prefix, 0);
    }

    char text[2000];
    char textFilename[100];
    char keywordsFilename[100];
    FILE *textFile, *keywordsFile;
    char **keywords = malloc(sizeof(char *) * 100);
    int keyword_count = 0;
    TrieNode *trieRoot = createTrieNode();

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Carregar arquivo de texto\n");
        printf("2. Carregar arquivo de palavras-chave\n");
        printf("3. Criar índice remissivo (Tabela Hash)\n");
        printf("4. Criar índice remissivo (Árvore de Pesquisa Digital)\n");
        printf("5. Imprimir índice remissivo (Tabela Hash)\n");
        printf("6. Imprimir índice remissivo (Árvore de Pesquisa Digital)\n");
        printf("7. Imprimir Tabela Hash\n");
        printf("8. Imprimir Árvore de Pesquisa Digital\n");
        printf("9. Excluir índice remissivo\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &choice);
        getchar(); // Limpar o buffer do teclado

        switch (choice) {
            case 1:
                printf("Informe o nome do arquivo de texto (inclua a extensão .txt): ");
                scanf("%s", textFilename);
                textFile = fopen(textFilename, "r");
                if (textFile == NULL) {
                    printf("Arquivo '%s' não encontrado.\n", textFilename);
                    break;
                }
                fgets(text, sizeof(text), textFile);
                fclose(textFile);
                printf("Arquivo de texto carregado com sucesso.\n");
                break;
            case 2:
                printf("Informe o nome do arquivo de palavras-chave (inclua a extensão .txt): ");
                scanf("%s", keywordsFilename);
                keywordsFile = fopen(keywordsFilename, "r");
                if (keywordsFile == NULL) {
                    printf("Arquivo '%s' não encontrado.\n", keywordsFilename);
                    break;
                }

                char line[100];
                while (fgets(line, sizeof(line), keywordsFile)) {
                    line[strcspn(line, "\n")] = 0; // Remove a nova linha
                    keywords[keyword_count] = strdup(line);
                    keyword_count++;
                }
                fclose(keywordsFile);
                printf("Arquivo de palavras-chave carregado com sucesso.\n");
                break;
            case 3:
                initializeHashTable();
                processTextHash(text, keywords, keyword_count); // Passa trieRoot
                printf("Índice remissivo (Tabela Hash) criado.\n");
                break;
            case 4:
                processTextTrie(text, keywords,keyword_count);
                printf("Índice remissivo (Árvore de Pesquisa Digital) criado.\n");
                break;
            case 5:
                printHashTable(keywords, keyword_count);
                break;

            case 6:
                remissiveIndexTrie(keywords, keyword_count);
                break;

            case 7:
                printFullHashTable();
                break;
             case 8:{
                TrieNode *root = getTrieRoot();
                 if (root != NULL) {
                    char prefix[100] = "";
                     printTrie(root, prefix, 0);
                } else {
                    printf("A Trie está vazia.\n");
                    }
                }
                break;
            case 9:
                freeHashTable();
                freeTrie(trieRoot);
                trieRoot = createTrieNode();
                printf("Índice remissivo excluído.\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (choice != 0);

    freeTrie(trieRoot);
    for (int i = 0; i < keyword_count; i++) {
        free(keywords[i]);
    }
    free(keywords);
    freeHashTable();
    return 0;
}
