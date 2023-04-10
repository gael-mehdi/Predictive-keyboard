main.o: main.c fonctions.h
	gcc -c main.c

fonctions.o: fonctions.c fonctions.h
	gcc -c fonctions.c

main: main.o fonctions.o
	gcc main.o fonctions.o -o main -lm -lncurses