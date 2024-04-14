opt = -c -Wall -Wextra -std=c99

main.exe: analyse.o main.o
	gcc -o main analyse.o main.o

analyse.o: analyse.c
	gcc ${opt} analyse.c

main.o: main.c
	gcc ${opt} main.c

clean:
	rm -f *.o *.exe