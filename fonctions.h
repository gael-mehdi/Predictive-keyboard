#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define KEY_ESC 27
#define KEY_ESP 32
#define KEY_SUPPR 8
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define MAX_WORD_LENGTH	40		/* Maximum word length */
#define dictionaryFileName "mots_courants.txt"
#define MAX_WORDS 10000

typedef struct _element{
	char word[MAX_WORD_LENGTH];
	struct _element* next;
} Element;

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

void append_word_to_file(const char *word);
void create_occ();
int compare_word_counts(const void *a, const void *b);
void sort_word_counts(WordCount *word_counts, int num_words);
void tri_occ();
void suggest_words(char *input_prefix,WordCount word_counts[]);
void effacer_jusqu_au_premier_crochet_de_la_droite();
void effacer_jusqu_au_premier_espace();
void fenetre();
void supprimer_mot_au_fichier(char *nom_fichier, char *mot);
void choix_menu_supprimer();
void ajouter_mot_au_fichier(char *nom_fichier, char *mot);
void choix_menu_ajouter();
void menu();