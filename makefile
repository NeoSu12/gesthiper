VPATH = src:src/headers

objects = main.o erros.o compra.o avl.o cat_clientes.o \
	   cat_produtos.o interface.o queries.o compras.o

CFLAGS = -ansi -pedantic -Wall -O2
CC = gcc

gesthiper : $(objects)
	$(CC) $(CFLAGS) -o gesthiper $(objects)

main.o : erros.h compra.h interface.h cat_clientes.h cat_produtos.h compras.h
erros.o : erros.h
compra.o : compra.h
avl.o : avl.h
interface.o : interface.h queries.h
queries.o : queries.h
cat_clientes.o : avl.h cat_clientes.h
cat_produtos.o : avl.h cat_produtos.h
compras.o : compras.h avl.h compra.h 

.PHONY : clean
clean :
	rm gesthiper $(objects)

