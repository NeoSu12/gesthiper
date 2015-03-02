# Makefile feita 'por nos' e que deve
# ser usada para correr no terminal.

objects = main.o erros.o

gesthiper : $(objects)
	gcc -o gesthiper $(objects)

main.o : main.c erros.o
	gcc -c main.c

erros.o : erros.c
	gcc -c erros.c

.PHONY : clean

clean :
	rm gesthiper $(objects)

