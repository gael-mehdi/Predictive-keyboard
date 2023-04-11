#include "fonctions.h"

int main()
{
    create_occ();
    tri_occ();
    
    // Demander l'entrée utilisateur
    char input_word[MAX_WORD_LENGTH];
    printf("Entrez un mot : ");
    scanf("%s", input_word);
    
    // Suggérer des mots
    suggest_words(input_word);
    
    return 0;
}
