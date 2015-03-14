#ifndef CATALOGO_H
#define	CATALOGO_H

typedef struct catalogo* CATALOGO;
typedef struct iterador* ITERADOR;

/* CATALOGO */
CATALOGO inicializa_catalogo();
int existe_elemento(CATALOGO, char *);
char *procura_elemento(CATALOGO, char *);
char *insere_item(CATALOGO, char *);
char *remove_item(CATALOGO, char *);
int total_codigos(CATALOGO);
int total_codigos_letra(CATALOGO, char);
void free_catalogo(CATALOGO);

/* ITERADORES */
ITERADOR inicializa_it_inicio(CATALOGO);
ITERADOR inicializa_it_fim(CATALOGO);
ITERADOR inicializa_it_elem(CATALOGO, char *);
ITERADOR inicializa_it_inicio_letra(CATALOGO, char);
ITERADOR inicializa_it_fim_letra(CATALOGO, char);
int itera_n_proximos(ITERADOR, char *[], int);
int itera_n_anteriores(ITERADOR, char *[], int );
char *iterador_proximo(ITERADOR);
char *iterador_actual(ITERADOR);
char *iterador_anterior(ITERADOR);
char *iterador_proximo_letra(ITERADOR);
char *iterador_anterior_letra(ITERADOR);


#endif	/* CATALOGO_H */

