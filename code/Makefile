opt = -c -Wall -Wextra -std=c99

grammaire.exe: grammaire.o main.o
	gcc -o grammaire grammaire.o main.o

grammaire.o: grammaire.c
	gcc ${opt} grammaire.c

main.o: main.c
	gcc ${opt} main.c