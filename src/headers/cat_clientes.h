#ifndef CatClientes_H
#define	CatClientes_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1

typedef struct catalogo_clientes* CatClientes;
typedef struct iterador_cat_clientes* IT_CAT_CLIENTES;
typedef struct cat_lista_clientes* CAT_LISTA_CLIENTES;

/* CatClientes */

CatClientes inicializa_catalogo_clientes();
int cat_existe_cliente(CatClientes, char *);
char *cat_procura_cliente(CatClientes, char *);
char *cat_insere_cliente(CatClientes, char *);
char *cat_remove_cliente(CatClientes, char *);
int cat_total_clientes(CatClientes);
int cat_total_clientes_letra(CatClientes, char);
void free_catalogo_clientes(CatClientes);

/* IT_CAT_CLIENTES */

IT_CAT_CLIENTES inicializa_it_cat_clientes(CatClientes cat);
IT_CAT_CLIENTES inicializa_it_cat_clientes_letra(CatClientes cat, char);
IT_CAT_CLIENTES inicializa_it_cat_clientes_primeiro(CatClientes);
IT_CAT_CLIENTES inicializa_it_cat_clientes_ultimo(CatClientes);
IT_CAT_CLIENTES inicializa_it_cat_clientes_primeiro_letra(CatClientes, char);
IT_CAT_CLIENTES inicializa_it_cat_clientes_ultimo_letra(CatClientes, char);
IT_CAT_CLIENTES inicializa_it_cat_clientes_elem(CatClientes, char *);

void free_it_cat_cliente(IT_CAT_CLIENTES);
char *it_cat_cliente_actual(IT_CAT_CLIENTES);
char *it_cat_cliente_proximo(IT_CAT_CLIENTES);
char *it_cat_cliente_anterior(IT_CAT_CLIENTES);
char *it_cat_cliente_proximo_letra(IT_CAT_CLIENTES); 
char *it_cat_cliente_anterior_letra(IT_CAT_CLIENTES);

/*
 * LISTA CLIENTES
 */

CAT_LISTA_CLIENTES cat_lista_clientes_letra(CatClientes, char, int);
char *cat_lista_cli_get_elemento(CAT_LISTA_CLIENTES,int p);
int cat_lista_cli_get_pos_and_num_elems_pag(CAT_LISTA_CLIENTES, int *, int);
int cat_lista_cli_get_num_pags(CAT_LISTA_CLIENTES);
int cat_lista_cli_get_elems_por_pag(CAT_LISTA_CLIENTES);
int cat_lista_cli_muda_elems_por_pag(CAT_LISTA_CLIENTES, int);
int cat_lista_cli_get_num_elems(CAT_LISTA_CLIENTES);
void cat_free_lista_clientes(CAT_LISTA_CLIENTES);

#endif	/* CatClientes_H */

