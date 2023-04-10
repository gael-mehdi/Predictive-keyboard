#include "fonctions.h"

int main()
{
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

    return 0;
}
