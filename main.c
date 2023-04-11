#include "fonctions.h"

int main()
{

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
    FILE *output_file = fopen("mots_courants_occurrence", "w");
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

    return 0;
}
