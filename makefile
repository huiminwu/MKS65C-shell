all: helpers.o okb.o main.o
	gcc -o Shell helpers.o okb.o main.o

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

okb.o: okb.c okb.h helpers.h
	gcc -c okb.c

main.o: main.c okb.h
	gcc -c main.c

run:
	./Shell
