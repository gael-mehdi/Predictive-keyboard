#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_SUPPR 127

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
    char mot[50];

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