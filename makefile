# Makefile feita 'por nos' e que deve
# ser usada para correr no terminal.

objects = main.o

gesthiper : $(objects)
	gcc -o gesthiper $(objects)

main.o : main.c
	gcc -c main.c

.PHONY : clean

clean :
	rm gesthiper $(objects)

