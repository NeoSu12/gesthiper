#ifndef CATALOGO_H
#define	CATALOGO_H

typedef struct catalogo* CATALOGO;
typedef struct iterador* ITERADOR;


/* CATALOGO */
CATALOGO inicializa_catalogo();
void insere_item(CATALOGO, char *);
void free_catalogo(CATALOGO);
char *procura_elemento(CATALOGO, char *);

/* ITERADORES */
ITERADOR inicializa_iterador_inicio(CATALOGO);
ITERADOR inicializa_iterador_letra(CATALOGO, char);
ITERADOR inicializa_iterador_elem(CATALOGO, char *);
char *iterador_next(ITERADOR);
char *iterador_next_letra(ITERADOR);


#endif	/* CATALOGO_H */

