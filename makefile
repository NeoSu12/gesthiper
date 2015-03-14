# Makefile feita 'por nos' e que deve
# ser usada para correr no terminal.

objects = main.o erros.o compra.o avl.o cat_clientes.o cat_produtos.o interface.o queries.o
flags = -ansi -pedantic -Wall -O2

gesthiper : $(objects)
	gcc $(flags) -o gesthiper $(objects)

main.o : main.c erros.h compra.h interface.h
	gcc $(flags) -c main.c 

erros.o : erros.c erros.h
	gcc $(flags) -c erros.c

compra.o : compra.c compra.h
	gcc $(flags) -c compra.c

avl.o : avl.c avl.h
	gcc $(flags) -c avl.c

interface.o : interface.c interface.h queries.h
	gcc $(flags) -c interface.c

queries.o : queries.c queries.h
	gcc $(flags) -c queries.c

cat_clientes.o : cat_clientes.c avl.h cat_clientes.h
	gcc $(flags) -c cat_clientes.c

cat_produtos.o : cat_produtos.c avl.h cat_produtos.h
	gcc $(flags) -c cat_produtos.c


.PHONY : clean
clean :
	rm gesthiper $(objects)

