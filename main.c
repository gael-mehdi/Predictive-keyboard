#include <ncurses.h>

int main()
{
    initscr(); // Initialiser ncurses
    cbreak(); // Désactiver la mise en mémoire tampon de la ligne
    noecho(); // Désactiver l'affichage des caractères saisis
    keypad(stdscr, TRUE); // Activer les touches de fonction et les touches fléchées

    // Créer la fenêtre parente
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* parent_win = newwin(max_y, max_x, 0, 0);
    box(parent_win, 0, 0); // Ajouter une bordure à la fenêtre parente

    // Créer une sous-fenêtre dans la fenêtre parente
    int sub_y = max_y / 2;
    int sub_x = max_x / 2;
    int sub_pos_y = (max_y - sub_y) / 2;
    int sub_pos_x = (max_x - sub_x) / 2;
    WINDOW* sub_win = subwin(parent_win, sub_y, sub_x, sub_pos_y, sub_pos_x);
    box(sub_win, 0, 0); // Ajouter une bordure à la sous-fenêtre

    // Afficher du texte dans la fenêtre parente et dans la sous-fenêtre
    mvwprintw(parent_win, 1, 1, "Ceci est une fenetre parente.");
    mvwprintw(sub_win, 1, 1, "Ceci est une sous-fenetre.");

    wrefresh(parent_win); // Rafraîchir la fenêtre parente
    wrefresh(sub_win); // Rafraîchir la sous-fenêtre
    getch(); // Attendre que l'utilisateur appuie sur une touche

    delwin(sub_win); // Supprimer la sous-fenêtre
    delwin(parent_win); // Supprimer la fenêtre parente
    endwin(); // Terminer ncurses

    return 0;
}
