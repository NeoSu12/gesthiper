# Makefile feita 'por nos' e que deve
# ser usada para correr no terminal.

objects = main.o erros.o compra.o
flags = -ansi -pedantic -Wall -O2

gesthiper : $(objects)
	gcc $(flags) -o gesthiper $(objects)

main.o : main.c erros.h compra.h
	gcc $(flags) -c main.c 

erros.o : erros.c erros.h
	gcc $(flags) -c erros.c

compra.o : compra.c compra.h
	gcc $(flags) -c compra.c


.PHONY : clean
clean :
	rm gesthiper $(objects)

