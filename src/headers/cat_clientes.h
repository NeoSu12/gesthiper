#ifndef CatClientes_H
#define	CatClientes_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1

typedef struct catalogo_clientes* CatClientes;
typedef struct iterador_clientes* IT_CLIENTES;
typedef struct lista_clientes* LISTA_CLIENTES;

/* CatClientes */

CatClientes inicializa_catalogo_clientes();
int existe_cliente(CatClientes, char *);
char *procura_cliente(CatClientes, char *);
char *insere_cliente(CatClientes, char *);
char *remove_cliente(CatClientes, char *);
int total_clientes(CatClientes);
int total_clientes_letra(CatClientes, char);
void free_catalogo_clientes(CatClientes);

/* IT_CLIENTES */

IT_CLIENTES inicializa_it_clientes(CatClientes cat);
IT_CLIENTES inicializa_it_clientes_letra(CatClientes cat, char);

IT_CLIENTES inicializa_it_clientes_primeiro(CatClientes);
IT_CLIENTES inicializa_it_clientes_ultimo(CatClientes);

IT_CLIENTES inicializa_it_clientes_primeiro_letra(CatClientes, char);
IT_CLIENTES inicializa_it_clientes_ultimo_letra(CatClientes, char);

IT_CLIENTES inicializa_it_clientes_elem(CatClientes, char *);

void free_it_cliente(IT_CLIENTES);

char *it_cliente_actual(IT_CLIENTES);

char *it_cliente_proximo(IT_CLIENTES);
char *it_cliente_anterior(IT_CLIENTES);

char *it_cliente_proximo_letra(IT_CLIENTES); 
char *it_cliente_anterior_letra(IT_CLIENTES);

/*
 * LISTA CLIENTES
 */

LISTA_CLIENTES lista_clientes_letra(CatClientes, char, int);
char *lista_cli_get_elemento(LISTA_CLIENTES,int p);
int lista_cli_get_pos_and_num_elems_pag(LISTA_CLIENTES, int *, int);
int lista_cli_get_num_pags(LISTA_CLIENTES);
int lista_cli_get_elems_por_pag(LISTA_CLIENTES);
int lista_cli_muda_elems_por_pag(LISTA_CLIENTES, int);
int lista_cli_get_num_elems(LISTA_CLIENTES);
void free_lista_clientes(LISTA_CLIENTES);


#endif	/* CatClientes_H */

