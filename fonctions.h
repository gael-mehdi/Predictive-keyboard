#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_SUPPR 127

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
void menu(HashTable* hashTab);
void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName);
void insertElementToHashTable(HashTable* hashTab, char* word);
void initializeHashTable(HashTable* hashTab);
bool checkExistenceWordInDictionary(HashTable* hashTab, char* word);
void printHashTableCharacteristics(HashTable* hashTab);