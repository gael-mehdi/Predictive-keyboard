#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define KEY_ESC 27
#define KEY_SUPPR 127
#define MAX_WORD_LENGTH	40		/* Maximum word length */
#define dictionaryFileName "mots_courants.txt"
#define HASH_SIZE 37987	/* Prime number */
#define BASE 128
#define MAX_WORDS 10000

typedef struct _element{
	char word[MAX_WORD_LENGTH];
	struct _element* next;
} Element;

typedef struct _hashTable{
	unsigned int size;
	unsigned int nbOccupiedEntries;
	unsigned int nbElements;
	Element** Elements;
} HashTable;

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int min(int x, int y, int z);
int levenshtein_distance(char *s1, char *s2);
void fenetre();
void initializeHashTable(HashTable* hashTab);
unsigned long getHashValue(char* string);
void insertElementToHashTable(HashTable* hashTab, char* word);
void loadDictionaryFromFile(HashTable* hashTab);
bool checkExistenceWordInDictionary(HashTable* hashTab, char* word);
void creating_occ();