VPATH = src:src/headers

objects = main.o erros.o compra.o avl.o cat_clientes.o \
	   cat_produtos.o interface.o queries.o compras.o \
		arrays_dinamicos.o contabilidade.o

CFLAGS = -ansi -pedantic -Wall -O2
VALFLAGS = -ansi -pedantic -Wall -O0 -g
CC = gcc

gesthiper : $(objects)
	$(CC) $(CFLAGS) -o gesthiper $(objects)

gesval: $(objects)
	CFLAGS = VALFLAGS
	$(CC) $(VALFLAGS) -o gesval $(objects)

runval: gesval meown
	valgrind --show-leak-kinds=all --leak-check=full ./gesval
	
valgrind: gesval
	valgrind --show-leak-kinds=all --leak-check=full ./gesval

main.o : erros.h compra.h interface.h cat_clientes.h cat_produtos.h compras.h
erros.o : erros.h
compra.o : compra.h
avl.o : avl.h
interface.o : interface.h queries.h
queries.o : interface.h queries.h
cat_clientes.o : avl.h cat_clientes.h
cat_produtos.o : avl.h cat_produtos.h
compras.o : compras.h avl.h compra.h 
arrays_dinamicos.o : arrays_dinamicos.h
contabilidade.o : compra.h avl.h arrays_dinamicos.h contabilidade.h


.PHONY : clean
clean :
	rm -f gesthiper
	rm -f $(objects)
	rm -f gesval

cleanobjs :
	rm -f $(objects)

#Faz-me dono dos ficheiros e actualiza permissoes.
#Util para trabalho em root.
meown :
	chown -R andre *
	chgrp -R andre *
	chmod -R o+r *
	chmod -R o-wx *
	chmod -R ug+wr *
	chgrp root gesval


