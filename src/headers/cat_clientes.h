#ifndef CatClientes_H
#define	CatClientes_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1

typedef struct catalogo_clientes* CatClientes;
typedef struct iterador_clientes* IT_CLIENTES;
typedef struct paginacao_clientes* PagClientes;

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



#endif	/* CatClientes_H */

