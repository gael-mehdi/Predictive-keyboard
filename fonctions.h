#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define KEY_ESC 27
#define KEY_SUPPR 127
#define MAX_WORD_LENGHT	40		/* Maximum word length */
#define dictionaryFileName "mots_courants.txt"
#define HASH_SIZE 37987	/* Prime number */
#define BASE 128

typedef struct _element{
	char word[MAX_WORD_LENGHT];
	struct _element* next;
} Element;

typedef struct _hashTable{
	unsigned int size;
	unsigned int nbOccupiedEntries;
	unsigned int nbElements;
	Element** Elements;
} HashTable;

int min(int x, int y, int z);
int levenshtein_distance(char *s1, char *s2);
void fenetre();
void initializeHashTable(HashTable* hashTab);
void loadDictionaryFromFile(HashTable* hashTab);
unsigned long getHashValue(char* string);
void insertElementToHashTable(HashTable* hashTab, char* word);
bool checkExistenceWordInDictionary(HashTable* hashTab, char* word);