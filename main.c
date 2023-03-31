#include <ncurses.h>

#define KEY_ESC 27

int main()
{
    char ch;

    initscr(); // Initialise ncurses
    cbreak(); // Désactive le buffering de ligne
    noecho(); // Ne pas afficher les caractères saisis
    keypad(stdscr, TRUE); // Active les touches spéciales (F1, flèches, etc.)

    printw("Entrez du texte. Appuyez sur ESC pour quitter.");

    do {
        ch = getch();
        if (ch != (char)KEY_ESC) {
            printw("%c", ch); // Affiche le caractère saisi
        }
    } while (ch != (char)KEY_ESC);

    endwin(); // Ferme ncurses

    return 0;
}
