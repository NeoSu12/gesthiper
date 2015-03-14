#ifndef CatClientes_H
#define	CatClientes_H

typedef struct catalogo_clientes* CatClientes;
typedef struct iterador* IT_CLIENTES;

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

IT_CLIENTES inicializa_it_clientes_inicio(CatClientes);
IT_CLIENTES inicializa_it_clientes_fim(CatClientes);
IT_CLIENTES inicializa_it_clientes_elem(CatClientes, char *);
IT_CLIENTES inicializa_it_clientes_inicio_letra(CatClientes, char);
IT_CLIENTES inicializa_it_clientes_fim_letra(CatClientes, char);
int itera_n_clientes_proximos(IT_CLIENTES, char *[], int);
int itera_n_clientes_anteriores(IT_CLIENTES, char *[], int);
char *it_cliente_proximo(IT_CLIENTES);
char *it_cliente_actual(IT_CLIENTES);
char *it_cliente_anterior(IT_CLIENTES);
char *it_cliente_proximo_letra(IT_CLIENTES); 
char *it_cliente_anterior_letra(IT_CLIENTES);


#endif	/* CatClientes_H */

