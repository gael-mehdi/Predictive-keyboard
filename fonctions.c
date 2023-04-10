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

// Cette fonction prend trois entiers en entrée et retourne le minimum de ces trois valeurs
int min(int x, int y, int z) {
    if (x <= y && x <= z) {
        return x;
    } else if (y <= x && y <= z) {
        return y;
    } else {
        return z;
    }
}

// Cette fonction calcule la distance de Levenshtein entre deux chaînes de caractères
int levenshtein_distance(char *s1, char *s2) {
    int len1 = strlen(s1); // Longueur de la première chaîne de caractères
    int len2 = strlen(s2); // Longueur de la deuxième chaîne de caractères
    int matrix[len1 + 1][len2 + 1]; // Initialisation d'une matrice pour stocker les distances entre les sous-chaînes

    // Initialisation de la première colonne de la matrice
    for (int i = 0; i <= len1; i++) {
        matrix[i][0] = i;
    }

    // Initialisation de la première ligne de la matrice
    for (int j = 0; j <= len2; j++) {
        matrix[0][j] = j;
    }

    // Calcul des distances entre les sous-chaînes
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1; // Coût de la substitution
            matrix[i][j] = min(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + cost); // Calcul de la distance de Levenshtein
        }
    }

    return matrix[len1][len2]; // Retourne la distance de Levenshtein entre les deux chaînes
}

// Cette fonction permet d'ouvrir une fenêtre avec la bibliothèque ncurses et de taper du texte dedans
void fenetre(){
    char ch;
    char mot[MAX_WORD_LENGHT];

    initscr(); // Initialise ncurses
    cbreak(); // Désactive le buffering de ligne
    noecho(); // Ne pas afficher les caractères saisis
    keypad(stdscr, TRUE); // Active les touches spéciales (F1, flèches, etc.)

    printw("Entrez du texte. Appuyez sur ESC pour quitter.\n");

    do {
        ch = getch();
        if (ch != (char)KEY_ESC && ch != (char)KEY_SUPPR ) { // Si l'utilisateur décide de sortir de la fenêtre
            printw("%c", ch); // Affiche le caractère saisi
        }
        if (ch == (char)KEY_SUPPR){
            move(getcury(stdscr), getcurx(stdscr) - 1);
            delch();
            refresh();
            mot[strlen(mot) - 1] = '\0'; // Supprime le dernier caractère de la chaîne mot
        }
        printw("%s", mot);
    } while (ch != (char)KEY_ESC);

    endwin(); // Ferme ncurses
}

void initializeHashTable(HashTable* hashTab){
	hashTab->size = HASH_SIZE;
	hashTab->nbOccupiedEntries = 0;
	hashTab->nbElements = 0;
	hashTab->Elements = (Element**)malloc(hashTab->size * sizeof(Element*));

	for (unsigned int i = 0; i < hashTab->size; i++){
        hashTab->Elements[i] = NULL;
    }
}



void loadDictionaryFromFile(HashTable* hashTab){
	
    FILE* fp;
	fp = fopen(dictionaryFileName, "r");

	if (fp != NULL){
		char word[MAX_WORD_LENGHT];
		while (fscanf(fp, "%s", word) == 1){
            insertElementToHashTable(hashTab, word);
        }
        fclose(fp);
	}
	else{
		printf("File not found.");
    }    
}

unsigned long getHashValue(char* string){
	unsigned long	hashValue = 0;
	int				i = 0;

	while ((*string) != '\0')
	{
		hashValue += hashValue % HASH_SIZE + ((*string) * (int)pow(BASE, i)) % HASH_SIZE;
		i++;
		string++;
	}
	return hashValue % HASH_SIZE;
}

void insertElementToHashTable(HashTable* hashTab, char* word){
	hashTab->nbElements++ ;
	unsigned long i = getHashValue(word) ;
	Element* elem = (Element*)malloc(sizeof(Element)) ;
	strcpy(elem->word, word);

	if (hashTab->Elements[i] == NULL){
		hashTab->nbOccupiedEntries++;
    }
	elem->next = hashTab->Elements[i] ;
	hashTab->Elements[i] = elem ;
}

bool checkExistenceWordInDictionary(HashTable* hashTab, char* word){
	unsigned long hashValue = getHashValue(word);
	Element* elem = hashTab->Elements[hashValue];
	bool found = 0;

	while (found == 0 && elem != NULL)
	{
		found = (strcmp(word, elem->word) == 0);
		elem = elem->next;
	}
	return found;
}