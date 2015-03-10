#ifndef CATALOGO_H
#define	CATALOGO_H

typedef struct catalogo CATALOGO;
typedef struct iterador ITERADOR;

CATALOGO *inicializa_catalogo(int);
void insere_item(CATALOGO *, char *);
void free_catalogo(CATALOGO *);
char *procura_elemento(CATALOGO *, char *);

ITERADOR *inicializa_iterador_null(CATALOGO *);
ITERADOR *inicializa_iterador_inicio(CATALOGO *);
ITERADOR *inicializa_iterador_letra(CATALOGO *, char);
ITERADOR *inicializa_iterador_elem(CATALOGO *, char *);
char *iterador_next(ITERADOR *);


#endif	/* CATALOGO_H */

