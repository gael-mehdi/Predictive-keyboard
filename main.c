#include <ncurses.h>

int main() {
    // Initialiser ncurses
    initscr();
    // Créer une fenêtre de 10 lignes et 20 colonnes, en commençant aux coordonnées (2, 2)
    WINDOW* fenetre = newwin(10, 20, 2, 2);
    // Activer les couleurs
    start_color();
    // Définir une paire de couleurs
    init_pair(1, COLOR_RED, COLOR_WHITE);s
    // Appliquer la paire de couleurs à la fenêtre
    wbkgd(fenetre, COLOR_PAIR(1));
    // Rafraîchir l'écran pour afficher la fenêtre
    refresh();
    // Attendre une entrée utilisateur
    getch();
    // Fermer la fenêtre et quitter ncurses
    delwin(fenetre);
    endwin();
    return 0;
}

