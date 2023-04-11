#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define KEY_ESC 27
#define KEY_SUPPR 127
#define KEY_ESP 32
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
    char mot[MAX_WORD_LENGTH]="";

    initscr(); // Initialise ncurses
    cbreak(); // Désactive le buffering de ligne
    noecho(); // Ne pas afficher les caractères saisis
    keypad(stdscr, TRUE); // Active les touches spéciales (F1, flèches, etc.)

    printw("Entrez du texte. Appuyez sur ESC pour quitter.\n");

    do {
        ch = getch();
        if (ch != (char)KEY_ESC && ch != (char)KEY_SUPPR ) { // Si l'utilisateur décide de sortir de la fenêtre
            printw("%c", ch);
            char c[2] = {ch, '\0'}; // Crée un tableau de caractères pour stocker le caractère entré
            if (ch != (char)KEY_ESP) {
                strcat(mot, c);
            }
            else{
                memset(mot, 0, sizeof(mot));
            }
        }
        if (ch == (char)KEY_SUPPR){
            move(getcury(stdscr), getcurx(stdscr) - 1);
            delch();
            refresh();
            mot[strlen(mot) - 1] = '\0'; // Supprime le dernier caractère de la chaîne mot
        }
        
    } while (ch != (char)KEY_ESC);

    endwin(); // Ferme ncurses
}

void create_occ(){
    // Ouvrir le fichier d'entrée
    FILE *input_file = fopen("mots_courants.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée\n");
        exit(EXIT_FAILURE);
    }
    
    // Compter le nombre d'occurrences de chaque mot
    int num_words = 0;
    WordCount word_counts[MAX_WORDS];
    char current_word[MAX_WORD_LENGTH];
    while (fscanf(input_file, "%s", current_word) == 1) {
        int found = 0;
        for (int i = 0; i < num_words; i++) {
            if (strcmp(current_word, word_counts[i].word) == 0) {
                word_counts[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(word_counts[num_words].word, current_word);
            word_counts[num_words].count = 1;
            num_words++;
        }
    }
    
    // Fermer le fichier d'entrée
    fclose(input_file);
    
    // Ouvrir le fichier de sortie
    FILE *output_file = fopen("mots_courants_occurrence.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sortie\n");
        exit(EXIT_FAILURE);
    }
    
    // Écrire tous les mots avec leur occurence
    for (int i = 0; i < num_words; i++) {
            fprintf(output_file, "%s %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    // Fermer le fichier de sortie
    fclose(output_file);
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wc1 = (const WordCount *) a;
    const WordCount *wc2 = (const WordCount *) b;
    if (wc1->count > wc2->count) {
        return -1;
    } else if (wc1->count < wc2->count) {
        return 1;
    } else {
        return strcmp(wc1->word, wc2->word);
    }
}

void sort_word_counts(WordCount *word_counts, int num_words) {
    qsort(word_counts, num_words, sizeof(WordCount), compare_word_counts);
}

void tri_occ(){
    // Ouvrir le fichier d'entrée
    FILE *input_file = fopen("mots_courants_occurrence.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée\n");
        exit(EXIT_FAILURE);
    }
    
    // Compter le nombre d'occurrences de chaque mot
    int num_words = 0;
    WordCount word_counts[MAX_WORDS];
    char current_word[MAX_WORD_LENGTH];
    while (fscanf(input_file, "%s %d", current_word, &word_counts[num_words].count) == 2) {
        strcpy(word_counts[num_words].word, current_word);
        num_words++;
    }
    
    // Fermer le fichier d'entrée
    fclose(input_file);
    
    // Trier les mots par ordre décroissant de leur nombre d'occurrences
    sort_word_counts(word_counts, num_words);
    
    // Écrire les mots triés dans le nouveau fichier
    FILE *output_file = fopen("mots_courants_occurrence.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sortie\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_words; i++) {
        fprintf(output_file, "%s %d\n", word_counts[i].word, word_counts[i].count);
    }
    fclose(output_file);
}

void suggest_words(char *input_prefix) {
    // Ouvrir le fichier d'entrée
    FILE *input_file = fopen("mots_courants_occurrence.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée\n");
        exit(EXIT_FAILURE);
    }
    
    // Compter le nombre d'occurrences de chaque mot qui commence par l'entrée utilisateur
    int num_words = 0;
    WordCount word_counts[MAX_WORDS];
    char current_word[MAX_WORD_LENGTH];
    while (fscanf(input_file, "%s %d", current_word, &word_counts[num_words].count) == 2) {
        if (strncmp(current_word, input_prefix, strlen(input_prefix)) == 0) {
            strcpy(word_counts[num_words].word, current_word);
            num_words++;
        }
    }
    
    // Fermer le fichier d'entrée
    fclose(input_file);
    
    // Trier les mots par ordre décroissant de leur nombre d'occurrences
    sort_word_counts(word_counts, num_words);
    
    // Afficher les trois premiers mots les plus fréquents (ou moins si il y en a moins que 3)
    printf("Voici les trois mots les plus fréquents qui commencent par '%s' :\n", input_prefix);
    for (int i = 0; i < num_words && i < 3; i++) {
        printf("%s\n", word_counts[i].word);
    }
}
