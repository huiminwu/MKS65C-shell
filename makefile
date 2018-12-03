all: helpers.o okb.o
	gcc -o Shell helpers.o okb.o

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

okb.o: okb.c helpers.h
	gcc -c okb.c

run:
	./Shell
