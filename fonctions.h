#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_SUPPR 127

int min(int x, int y, int z);
int levenshtein_distance(char *s1, char *s2);