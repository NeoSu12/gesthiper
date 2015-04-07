#ifndef COMPRAS_H
#define	COMPRAS_H

#define COMPRAS_ELEMS_PAG 10
#include "compra.h"

typedef enum compras_campo {
    COMPRAS_NORMAL, COMPRAS_PROMO, COMPRAS_AMBOS
}compras_campo_t;

typedef struct modulo_compras *Compras;
typedef struct compras_ficha_produto *COMPRAS_FICHA_PRODUTO;
typedef struct compras_ficha_cliente *COMPRAS_FICHA_CLIENTE;
typedef struct compras_iterador_clientes *IT_COMPRAS_CLIENTES;
typedef struct compras_iterador_produtos *IT_COMPRAS_PRODUTOS;
typedef struct compras_lista_clientes *COMPRAS_LISTA_CLIENTES;
typedef struct compras_lista_produtos *COMPRAS_LISTA_PRODUTOS;
typedef struct compras_num_clientes_mensais *COMPRAS_NUM_CLIENTES_MENSAIS;
typedef struct compras_associacao_produto_clientes *COMPRAS_ASSOC_PROD_CLIENTES;
typedef struct compras_cliente_tipo_compra *COMPRAS_CLIENTE_TIPO_COMPRA;

/*
 * INCICIALIZACAO E GESTAO MEMORIA
 */

Compras inicializa_compras();
void compras_regista_cliente(Compras compras, char *cod_cli);
void compras_insere_compra(Compras compras, COMPRA comp);
void compras_remove_cliente(Compras compras, char *cod_cli);
void compras_remove_produto_de_cliente(Compras compras, char *cod_cli, char *cod_prod);
void compras_remove_produto(Compras compras, char *cod_prod);
void free_compras(Compras compras);

/*
 * TOTAL PRODUTOS COMPRADOS CLIENTE
 */

int compras_total_produtos_comprados_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente);
int compras_total_unidades_compradas_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente);
int compras_produtos_comprados_ficha_cliente_meses(COMPRAS_FICHA_CLIENTE cliente, int mes_inf, int mes_sup);
int compras_produtos_comprados_ficha_cliente_mes(COMPRAS_FICHA_CLIENTE cliente, int mes);
int compras_produtos_comprados_cod_cliente_meses(Compras compras, char *cod_cliente, int mes_inf, int mes_sup);
int compras_produtos_comprados_cod_cliente_mes(Compras compras, char *cod_cliente, int mes);

/*
 * PRODUTO COMPRADO POR CLIENTE
 */

int compras_total_produtos_comprados_ficha(COMPRAS_FICHA_PRODUTO produto);

int compras_produtos_comprados_ficha_produto_meses_geral(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup, 
                                                            compras_campo_t campo);
int compras_produtos_comprados_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup);
int compras_produtos_comprados_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes);
int compras_produtos_comprados_normal_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup);
int compras_produtos_comprados_normal_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes);
int compras_produtos_comprados_promo_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup);
int compras_produtos_comprados_promo_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes);

/*===================================================*/

int compras_produtos_comprados_cod_produto_meses_geral(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup, 
                                                            compras_campo_t campo);

int compras_produtos_comprados_cod_produto_meses(Compras compras, 
                                                    char *cod_cli, char *cod_prod, 
                                                    int mes_inf, int mes_sup);
int compras_produtos_comprados_cod_produto_mes(Compras compras, 
                                                    char *cod_cli, char *cod_prod, 
                                                    int mes);
int compras_produtos_comprados_normal_cod_produto_meses(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup);
int compras_produtos_comprados_normal_cod_produto_mes(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes);
int compras_produtos_comprados_promo_cod_produto_meses(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup);
int compras_produtos_comprados_promo_cod_produto_mes(Compras compras, 
                                                        char *cod_cli, char *cod_prod, 
                                                        int mes);

/*
 * ITERADORES
 */

/*
 * ITERADORES CLIENTES
 */

IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes(Compras compras);
IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_primeiro(Compras compras);
IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_ultimo(Compras compras);
IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_elem(Compras compras, char *st);
COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_actual(IT_COMPRAS_CLIENTES it);
COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_proximo(IT_COMPRAS_CLIENTES it);
COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_anterior(IT_COMPRAS_CLIENTES it);
void free_it_compras_fich_cliente(IT_COMPRAS_CLIENTES it);

/*
 * ITERADORES PRODUTOS
 */

IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos(COMPRAS_FICHA_CLIENTE cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_primeiro(COMPRAS_FICHA_CLIENTE cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_ultimo(COMPRAS_FICHA_CLIENTE cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_elem(COMPRAS_FICHA_CLIENTE cliente, char *st);
COMPRAS_FICHA_PRODUTO it_compras_fich_produto_actual(IT_COMPRAS_PRODUTOS it);
COMPRAS_FICHA_PRODUTO it_compras_fich_produto_proximo(IT_COMPRAS_PRODUTOS it);
COMPRAS_FICHA_PRODUTO it_compras_fich_produto_anterior(IT_COMPRAS_PRODUTOS it);
void free_it_compras_fich_produto(IT_COMPRAS_PRODUTOS it);

/*
 * ITERADORES PRODUTOS COM CODIGO
 */

IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_primeiro(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_ultimo(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_elem(Compras compras, char *cod_cliente, char *cod_produto);


/*
 * Pedidos aos modulos
 */

COMPRAS_LISTA_CLIENTES compras_lista_clientes_regulares(Compras compras);
COMPRAS_LISTA_PRODUTOS compras_produtos_mais_comprados_cliente_mes(Compras compras, char *cod_cliente, int mes);
COMPRAS_LISTA_PRODUTOS compras_top_n_produtos_mais_comprados_cliente(Compras compras, char *cod_cliente, int n);
COMPRAS_NUM_CLIENTES_MENSAIS compras_num_clientes_por_mes(Compras compras);
void compras_free_num_clientes_mensais(COMPRAS_NUM_CLIENTES_MENSAIS num_cli_mensais);
int compras_get_num_clientes_mes(COMPRAS_NUM_CLIENTES_MENSAIS compras_meses, int mes);
int compras_num_cliente_sem_compras(Compras compras);
int compras_num_clientes_compraram_prod(Compras compras, char *cod_produto);

/*
 * ASSOCIACOES
 */


COMPRAS_ASSOC_PROD_CLIENTES compras_get_associacao_produto_clientes_tipo_compra(Compras compras, char *cod_produto);
COMPRAS_CLIENTE_TIPO_COMPRA compras_get_cli_compra_from_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES lista,int p);
int compras_assoc_prod_cli_get_pos_and_num_elems_pag(COMPRAS_ASSOC_PROD_CLIENTES lista, int *pos_inicial, int pag);
int compras_assoc_prod_cli_get_num_pags(COMPRAS_ASSOC_PROD_CLIENTES lista);
int compras_assoc_prod_cli_get_elems_por_pag(COMPRAS_ASSOC_PROD_CLIENTES lista);
void compras_assoc_prod_cli_muda_elems_por_pag(COMPRAS_ASSOC_PROD_CLIENTES lista, int n);
int compras_assoc_prod_cli_get_num_elems(COMPRAS_ASSOC_PROD_CLIENTES lista);
void compras_free_assoc_prod_clientes(COMPRAS_ASSOC_PROD_CLIENTES assoc);
char *compras_assoc_cli_compra_get_cod_cli(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra);
char compras_assoc_cli_compra_get_tipo_compra(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra);


/*
 * LISTAS CLIENTE
 */

char *compras_get_cod_cli_ficha(COMPRAS_FICHA_CLIENTE cliente);
COMPRAS_FICHA_CLIENTE compras_lista_get_fich_cli(COMPRAS_LISTA_CLIENTES lista,int p);
int compras_lista_cli_get_pos_and_num_elems_pag(COMPRAS_LISTA_CLIENTES lista, int *pos_inicial, int pag);
int compras_lista_cli_get_num_pags(COMPRAS_LISTA_CLIENTES lista);
int compras_lista_cli_get_elems_por_pag(COMPRAS_LISTA_CLIENTES lista);
void compras_lista_cli_muda_elems_por_pag(COMPRAS_LISTA_CLIENTES lista, int n);
int compras_lista_cli_get_num_elems(COMPRAS_LISTA_CLIENTES lista);
void compras_free_lista_clientes(COMPRAS_LISTA_CLIENTES lista);


/*
 * LISTAS PRODUTO
 */

char *compras_get_cod_prod_ficha(COMPRAS_FICHA_PRODUTO produto);
COMPRAS_FICHA_PRODUTO compras_lista_get_fich_prod(COMPRAS_LISTA_PRODUTOS lista,int p);
int compras_lista_prod_get_pos_and_num_elems_pag(COMPRAS_LISTA_PRODUTOS lista, int *pos_inicial, int pag);
int compras_lista_prod_get_num_pags(COMPRAS_LISTA_PRODUTOS lista);
int compras_lista_prod_get_elems_por_pag(COMPRAS_LISTA_PRODUTOS lista);
void compras_lista_prod_muda_elems_por_pag(COMPRAS_LISTA_PRODUTOS lista, int n);
int compras_lista_prod_get_num_elems(COMPRAS_LISTA_PRODUTOS lista);
void compras_free_lista_produtos(COMPRAS_LISTA_PRODUTOS lista);



#endif	/* COMPRAS_H */

