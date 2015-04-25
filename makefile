VPATH = src:src/headers

objects = main.o leitura_ficheiros.o compra.o avl.o cat_clientes.o \
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

run100m: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras100m.txt

run1m: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras1.txt

run3m: gesthiper
	./gesthiper datasets/FichClientes.txt datasets/FichProdutos.txt datasets/Compras3.txt

500k: gesthiper
	./gesthiper FichClientes.txt FichProdutos.txt Compras.txt

1m: gesthiper
	./gesthiper FichClientes.txt FichProdutos.txt Compras1.txt

3m: gesthiper
	./gesthiper FichClientes.txt FichProdutos.txt Compras3.txt

valgrind:
	make clean
	make --file=makefile-valgrind

runvalgrind:
	make clean
	make --file=makefile-valgrind runval

runvalgrindbase:
	make clean
	make --file=makefile-valgrind runvalbase

runvalgrind100m:
	make clean
	make --file=makefile-valgrind runval100m

runvalgrind1m:
	make clean
	make --file=makefile-valgrind runval1m

runvalgrind3m:
	make clean
	make --file=makefile-valgrind runval3m

prototipos:
	cproto -o prototipos.h src/*.c

tarexe: recompile
	tar -cf gesthiper.tar datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

tarcomplete: recompile
	tar -cf gesthiper.tar makefile makefile-valgrind src/* src/headers/* datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

zipexe: recompile
	zip gesthiper.zip datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

zipentrega: recompile
	zip LI3_Grupo8.zip makefile makefile-valgrind src/* src/headers/*

zipcomplete: recompile
	zip gesthiper.zip makefile makefile-valgrind src/* src/headers/* datasets/Compras.txt datasets/Compras1.txt datasets/Compras3.txt gesthiper

main.o : leitura_ficheiros.h interface.h cat_clientes.h cat_produtos.h compras.h contabilidade.h
leitura_ficheiros.o : compra.h cat_clientes.h cat_produtos.h compras.h contabilidade.h
compra.o : compra.h
avl.o : avl.h
interface.o : interface.h queries.h
queries.o : interface.h queries.h cat_produtos.h cat_clientes.h contabilidade.h compras.h
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


