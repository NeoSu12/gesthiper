# Makefile feita 'por nos' e que deve
# ser usada para correr no terminal.

objects = main.o erros.o compra.o

gesthiper : $(objects)
	gcc -o gesthiper $(objects)

main.o : main.c erros.h compra.h
	gcc -c main.c 

erros.o : erros.c erros.h
	gcc -c erros.c

compra.o : compra.c compra.h
	gcc -c compra.c


.PHONY : clean
clean :
	rm gesthiper $(objects)

