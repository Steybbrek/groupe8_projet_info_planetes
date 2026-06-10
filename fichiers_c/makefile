vplanetes: main.o planetes.o
	clear && gcc -lm -o planetes main.o planetes.o && rm *.o && valgrind ./planetes
planetes: main.o planetes.o
	clear && gcc -lm -o planetes main.o planetes.o && rm *.o && ./planetes
main.o:
	gcc -c main.c
planetes.o:
	gcc -c planetes.c