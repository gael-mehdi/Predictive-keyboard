 #include <ncurses.h>

 #define KEY_ESC 27
 #define KEY_SUPPR 127

int main()
{
    char ch;

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
        if (ch == (char)KEY_SUPPR){ // Si l'utilisateur veut supprimer un caractère
            move(getcury(stdscr), getcurx(stdscr) - 1); // Se déplace d'un caractère à gauche
            delch(); // Supprime le caractère précédent le curseur
            refresh(); // Rafraîchit l'écran
        }
    } while (ch != (char)KEY_ESC);

    endwin(); // Ferme ncurses

    return 0;
}

