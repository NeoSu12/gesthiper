VPATH = src:src/headers

objects = main.o erros.o compra.o avl.o cat_clientes.o \
	   		cat_produtos.o interface.o queries.o compras.o \
				arrays_dinamicos.o contabilidade.o

CFLAGS = -ansi -pedantic -Wall -g -O2
CC = gcc

gesthiper : $(objects)
	$(CC) $(CFLAGS) -o gesthiper $(objects)

run: gesthiper
	./gesthiper

recompile:
	make clean
	make

runbase: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras.txt

run1m: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras1.txt

run3m: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras3.txt

valgrind:
	make --file=makefile-valgrind

runvalgrind:
	make --file=makefile-valgrind runval

runvalgrindbase:
	make --file=makefile-valgrind runvalbase

runvalgrind1m:
	make --file=makefile-valgrind runval1m

runvalgrind3m:
	make --file=makefile-valgrind runval3m

tarexe: gesthiper
	tar -cf gesthiper.tar datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

tarcomplete: gesthiper
	tar -cf gesthiper.tar makefile makefile-valgrind src/* datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

zipexe: gesthiper
	zip gesthiper.tar datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

zipcomplete: gesthiper
	zip gesthiper.tar makefile makefile-valgrind src/* datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

main.o : interface.h erros.h compra.h cat_clientes.h cat_produtos.h compras.h contabilidade.h
erros.o : erros.h
compra.o : compra.h
avl.o : avl.h
interface.o : erros.h interface.h queries.h
queries.o : interface.h queries.h cat_produtos.h cat_clientes.h contabilidade.h compras.h arrays_dinamicos.h
cat_clientes.o : avl.h cat_clientes.h arrays_dinamicos.h
cat_produtos.o : avl.h cat_produtos.h arrays_dinamicos.h
compras.o : compras.h avl.h compra.h arrays_dinamicos.h
arrays_dinamicos.o : arrays_dinamicos.h
contabilidade.o : compra.h avl.h arrays_dinamicos.h contabilidade.h


.PHONY : clean
clean :
	rm -f gesthiper
	rm -f $(objects)
	rm -f gesval


#Faz-me dono dos ficheiros e actualiza permissoes.
#Util para trabalho em root.
meown :
	chown -R andre *
	chgrp -R andre *
	chmod -R o+r *
	chmod -R o-wx *
	chmod -R ug+wr *
	chgrp root gesval


