#include <stdio.h>
#include <string.h>

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

int main() {
    char s1[] = "hello"; // Première chaîne de caractères
    char s2[] = "world"; // Deuxième chaîne de caractères
    printf("Distance: %d\n", levenshtein_distance(s1, s2)); // Affiche la distance de Levenshtein entre les deux chaînes
    return 0;
}
