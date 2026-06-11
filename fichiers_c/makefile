vplanetes: main.o planetes.o
	clear && gcc -lm -o planetes main.o planetes.o && rm *.o && valgrind ./planetes && echo "Suppression du fichier planetes" && rm ./planetes
planetes: main.o planetes.o
	clear && gcc -lm -o planetes main.o planetes.o && rm *.o && ./planetes && echo "Suppression du fichier planetes" && rm ./planetes
main.o:
	gcc -c main.c
planetes.o:
	gcc -c planetes.c