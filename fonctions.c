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

void append_word_to_file(const char *word){
    FILE *file = fopen("mots_courants.txt", "a"); // Ouverture du fichier en mode "ajout" (append)
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", "mots_courants.txt");
        return;
    }

    fprintf(file, "%s\n", word); // Écriture du mot suivi d'un retour à la ligne

    fclose(file); // Fermeture du fichier
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

void suggest_words(char *input_prefix,WordCount word_counts[]) {
    // Ouvrir le fichier d'entrée
    FILE *input_file = fopen("mots_courants_occurrence.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée\n");
        exit(EXIT_FAILURE);
    }
    
    // Compter le nombre d'occurrences de chaque mot qui commence par l'entrée utilisateur
    int num_words = 0;
    char current_word[MAX_WORD_LENGTH];
    while (fscanf(input_file, "%s %d", current_word, &word_counts[num_words].count) == 2) {
        if (strncmp(current_word, input_prefix, strlen(input_prefix)) == 0) {
            strcpy(word_counts[num_words].word, current_word);
            num_words++;
        }
    }

    // Si moins de 3 mots ont été trouvés, compléter les suggestions avec les premiers mots du fichier "gutenberg.txt"
    if (num_words < 3) {
        // Ouvrir le fichier "gutenberg.txt"
        FILE *gutenberg_file = fopen("gutenberg.txt", "r");
        if (gutenberg_file == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier gutenberg.txt\n");
            exit(EXIT_FAILURE);
        }

        // Ajouter les premiers mots du fichier "gutenberg.txt" à la liste des suggestions
        while (fscanf(gutenberg_file, "%s", current_word)==1) {
        if (strncmp(current_word, input_prefix, strlen(input_prefix)) == 0) {
            strcpy(word_counts[num_words].word, current_word);
            num_words++;
        }
    }

        // Fermer le fichier "gutenberg.txt"
        fclose(gutenberg_file);
    }
    
   
    
    // Trier les mots par ordre décroissant de leur nombre d'occurrences
    sort_word_counts(word_counts, num_words);
    
    // Afficher les trois premiers mots les plus fréquents (ou moins si il y en a moins que 3)
    printw("[%s,%s,%s]", word_counts[0].word, word_counts[1].word, word_counts[2].word);
    
    // Fermer le fichier d'entrée
    fclose(input_file);

}

void effacer_jusqu_au_premier_crochet_de_la_droite() {
    int y, x;
    getyx(stdscr, y, x); // Récupère la position actuelle du curseur

    // Boucle jusqu'à ce qu'on trouve le premier crochet "[" ou qu'on arrive au début de la ligne
    while (x > 0 && mvwinch(stdscr, y, x-1) != '[') {
        move(y, x-1);
        delch();
        x--;
    }
}

void effacer_jusqu_au_premier_espace() {
    int y, x;
    getyx(stdscr, y, x); // Récupère la position actuelle du curseur

    // Boucle jusqu'à ce qu'on trouve le premier espace " " ou qu'on arrive au début de la ligne
    while (x > 0 && mvwinch(stdscr, y, x-2) != ' ') {
        move(y, x-1);
        delch();
        x--;
    }
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
        effacer_jusqu_au_premier_crochet_de_la_droite();
        if (ch != (char)KEY_ESC && ch != (char)KEY_SUPPR) { // Si l'utilisateur décide de sortir de la fenêtre
            printw("%c", ch);
            char c[2] = {ch, '\0'}; // Crée un tableau de caractères pour stocker le caractère entré
            if (ch != (char)KEY_ESP) {
                strcat(mot, c);
            }
            else{
                FILE *fp;
                char searchword[MAX_WORD_LENGTH];
                fp = fopen("gutenberg.txt", "r");
                while (fscanf(fp, "%s", searchword) == 1) {
                    if (strcmp(searchword, mot) == 0) {
                        append_word_to_file(mot);
                        memset(mot, 0, sizeof(mot)); // vide le mot
                        create_occ();
                        tri_occ();
                    }
                }
                memset(mot, 0, sizeof(mot)); // vide le mot
            }
        }
        if (ch == (char)KEY_SUPPR){
            move(getcury(stdscr), getcurx(stdscr) - 1);
            delch();
            refresh();
            mot[strlen(mot) - 1] = '\0'; // Supprime le dernier caractère de la chaîne mot
        }
        
       WordCount word_counts[MAX_WORDS];
       suggest_words(mot,word_counts);

        if (ch == (char)KEY_1){
            effacer_jusqu_au_premier_espace();
            memset(mot, 0, sizeof(mot)); // vide le mot
            strcpy(mot,word_counts[0].word);
            append_word_to_file(mot);
            printw(" ");
            printw("%s", word_counts[0].word);
            memset(mot, 0, sizeof(mot)); // vide le mot
            suggest_words(mot,word_counts);
        }

        if (ch == (char)KEY_2){
            effacer_jusqu_au_premier_espace();
            memset(mot, 0, sizeof(mot)); // vide le mot
            strcpy(mot,word_counts[1].word);
            append_word_to_file(mot);
            printw(" ");
            printw("%s", word_counts[1].word);
            memset(mot, 0, sizeof(mot)); // vide le mot
            suggest_words(mot,word_counts);
        }

        if (ch == (char)KEY_3){
            effacer_jusqu_au_premier_espace();
            memset(mot, 0, sizeof(mot)); // vide le mot
            strcpy(mot,word_counts[2].word);
            append_word_to_file(mot);
            printw(" ");
            printw("%s", word_counts[2].word);
            memset(mot, 0, sizeof(mot)); // vide le mot
            suggest_words(mot,word_counts);
        }

    } while (ch != (char)KEY_ESC);
    endwin(); // Ferme ncurses
}

void supprimer_mot_au_fichier(char *nom_fichier, char *mot) {
    FILE *fichier = fopen(nom_fichier, "r"); // ouvrir le fichier en mode "lecture"
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return;
    }
    // créer un fichier temporaire pour stocker les lignes sans le mot
    char nom_fichier_temp[MAX_WORDS];
    sprintf(nom_fichier_temp, "%s.temp", nom_fichier);
    FILE *fichier_temp = fopen(nom_fichier_temp, "w"); // ouvrir le fichier temporaire en mode "écriture"
    if (fichier_temp == NULL) {
        printf("Impossible de créer le fichier temporaire %s\n", nom_fichier_temp);
        fclose(fichier);
        return;
    }

    char ligne[MAX_WORDS];
    while (fgets(ligne, MAX_WORDS, fichier) != NULL) { // lire chaque ligne du fichier
        char *position_mot = strstr(ligne, mot); // chercher le mot dans la ligne
        while (position_mot != NULL) { // tant que le mot est trouvé dans la ligne
            int decalage = position_mot - ligne; // calculer le décalage du mot dans la ligne
            for (int i = 0; i < strlen(mot); i++) {
                ligne[decalage + i] = ' '; // remplacer chaque caractère du mot par un espace
            }
            position_mot = strstr(ligne, mot); // chercher à nouveau le mot dans la ligne
        }
        fwrite(ligne, strlen(ligne), 1, fichier_temp); // écrire la ligne modifiée dans le fichier temporaire
    }

    // fermer les fichiers
    fclose(fichier);
    fclose(fichier_temp);

    // supprimer le fichier original
    remove(nom_fichier);

    // renommer le fichier temporaire en tant que fichier original
    rename(nom_fichier_temp, nom_fichier);
}


void choix_menu_supprimer(){
    char nom_fichier[MAX_WORD_LENGTH] = "mots_courants.txt";
    char mot[MAX_WORD_LENGTH];
    printf("\n Entrer le mot à supprimer : ");
    scanf("%s", mot);
    supprimer_mot_au_fichier(nom_fichier, mot);
    create_occ();
    tri_occ();
}

void ajouter_mot_au_fichier(char *nom_fichier, char *mot) {
    FILE *fichier = fopen(nom_fichier, "a"); // ouvrir le fichier en mode "ajout"
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return;
    }

    // déplacer le curseur de fichier à la fin du fichier
    fseek(fichier, 0, SEEK_END);

    // écrire le mot à la fin du fichier
    fprintf(fichier, "%s", mot);

    // fermer le fichier
    fclose(fichier);
}

void choix_menu_ajouter(){
    char nom_fichier[MAX_WORD_LENGTH] = "mots_courants.txt";
    char mot[MAX_WORD_LENGTH];
    printf("\n Entrer le mot à ajouter : ");
    scanf("%s", mot);
    ajouter_mot_au_fichier(nom_fichier, mot);
    create_occ();
    tri_occ();
}

void menu(){
    bool quitter = false;
    int choix;

    while (!quitter){
        printf("\n");
        printf("1  suprimer un mot du dictionnaire de prédiction\n");
        printf("2  ajouter un mot du dictionnaire de prédiction\n");
        printf("3  lancer l'application de saisie prédictive\n");
        printf("4  pour quitter\n");
        scanf("%d", &choix);

        switch (choix){
            case 1 :
                choix_menu_supprimer();
                break;
            case 2 :
                choix_menu_ajouter();
                break;
            case 3 :
                fenetre();;
                break;
            case 4 :
                quitter = true ;
                break ;
        }
    }
}

