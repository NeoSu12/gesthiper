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
typedef struct compras_iterador_produtos_nclientes *IT_COMPRAS_PRODUTOS_NCLIENTES;
typedef struct compras_lista_clientes *COMPRAS_LISTA_CLIENTES;
typedef struct compras_lista_produtos *COMPRAS_LISTA_PRODUTOS;
typedef struct compras_num_clientes_mensais *COMPRAS_NUM_CLIENTES_MENSAIS;
typedef struct compras_associacao_produto_clientes *COMPRAS_ASSOC_PROD_CLIENTES;
typedef struct compras_cliente_tipo_compra *COMPRAS_CLIENTE_TIPO_COMPRA;
typedef struct compras_produto_nclientes *COMPRAS_PRODUTO_NCLIENTES;
typedef struct compras_paginador_lista_produtos *COMPRAS_PAG_LISTA_PRODUTOS;
typedef struct compras_paginador_lista_clientes *COMPRAS_PAG_LISTA_CLIENTES;
typedef struct compras_paginador_associacao_produto_clientes *COMPRAS_PAG_ASSOC_PROD_CLIENTES;

/*
 * COMPRAS
 */
Compras inicializa_compras(void);
void compras_regista_cliente(Compras compras, char *cod_cli);
void compras_regista_produto(Compras compras, char *cod_prod);
void compras_insere_compra(Compras compras, COMPRA comp);
void compras_remove_cliente(Compras compras, char *cod_cli);
void compras_remove_produto_de_cliente(Compras compras, char *cod_cli, char *cod_prod);
void compras_remove_produto(Compras compras, char *cod_prod);
int compras_num_cliente_sem_compras(Compras compras);
int compras_num_clientes_compraram_prod(Compras compras, char *cod_produto);
void free_compras(Compras compras);

/*
 * PRODUTO -> N CLIENTES
 */
char *compras_get_cod_produto_from_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli);
int compras_get_nclientes_from_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli);

/*
 * FICHA CLIENTES
 */
char *compras_get_cod_cli_ficha(COMPRAS_FICHA_CLIENTE cliente);
_Bool compras_cliente_comprou_em_todos_os_meses(COMPRAS_FICHA_CLIENTE cliente);

/*
 * COMPRAS CLIENTE
 */
int compras_total_produtos_comprados_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente);
int compras_total_unidades_compradas_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente);
int compras_produtos_comprados_ficha_cliente_meses(COMPRAS_FICHA_CLIENTE cliente, int mes_inf, int mes_sup);
int compras_produtos_comprados_ficha_cliente_mes(COMPRAS_FICHA_CLIENTE cliente, int mes);
int compras_total_produtos_comprados_codigo_cliente(Compras compras, char *cod_cliente);
int compras_total_unidades_compradas_codigo_cliente(Compras compras, char *cod_cliente);
int compras_produtos_comprados_cod_cliente_meses(Compras compras, char *cod_cliente, int mes_inf, int mes_sup);
int compras_produtos_comprados_cod_cliente_mes(Compras compras, char *cod_cliente, int mes);

/*
 * FICHA PRODUTOS
 */
char *compras_get_cod_prod_ficha(COMPRAS_FICHA_PRODUTO produto);

/*
 * FICHA PRODUTOS
 */
int compras_total_produtos_comprados_ficha(COMPRAS_FICHA_PRODUTO produto);
int compras_produtos_comprados_ficha_produto_meses_geral(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup, compras_campo_t campo);
int compras_produtos_comprados_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup);
int compras_produtos_comprados_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes);
int compras_produtos_comprados_normal_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup);
int compras_produtos_comprados_normal_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes);
int compras_produtos_comprados_promo_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup);
int compras_produtos_comprados_promo_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes);

/*
 * COMPRAS PRODUTO POR CODIGO
 */
int compras_total_produtos_comprados_codigo(Compras compras, char *cod_cli, char *cod_prod);
int compras_produtos_comprados_cod_produto_meses_geral(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup, compras_campo_t campo);
int compras_produtos_comprados_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup);
int compras_produtos_comprados_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes);
int compras_produtos_comprados_normal_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup);
int compras_produtos_comprados_normal_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes);
int compras_produtos_comprados_promo_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup);
int compras_produtos_comprados_promo_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes);

/*
 * LISTA CLIENTES
 */
COMPRAS_LISTA_CLIENTES compras_lista_clientes_regulares(Compras compras);
COMPRAS_FICHA_CLIENTE compras_lista_get_fich_cli(COMPRAS_LISTA_CLIENTES lista, int p);
int compras_lista_cli_get_num_elems(COMPRAS_LISTA_CLIENTES lista);
void compras_free_lista_clientes(COMPRAS_LISTA_CLIENTES lista);

/*
 * LISTA PRODUTOS
 */
COMPRAS_LISTA_PRODUTOS compras_produtos_mais_comprados_cliente_mes(Compras compras, char *cod_cliente, int mes);
COMPRAS_LISTA_PRODUTOS compras_top_n_produtos_mais_comprados_cliente(Compras compras, char *cod_cliente, int n);
COMPRAS_FICHA_PRODUTO compras_lista_get_fich_prod(COMPRAS_LISTA_PRODUTOS lista, int p);
void compras_free_lista_produtos(COMPRAS_LISTA_PRODUTOS lista);

/*
 * NUMERO CLIENTES POR MES
 */
COMPRAS_NUM_CLIENTES_MENSAIS compras_num_clientes_por_mes(Compras compras);
int compras_get_num_clientes_mes(COMPRAS_NUM_CLIENTES_MENSAIS compras_meses, int mes);
void compras_free_num_clientes_mensais(COMPRAS_NUM_CLIENTES_MENSAIS num_cli_mensais);

/*
 * ASSOCIACAO PRODUTO -> CLIENTES e CLIENTE -> TIPO COMPRA 
 */
COMPRAS_ASSOC_PROD_CLIENTES compras_get_associacao_produto_clientes_tipo_compra(Compras compras, char *cod_produto);
COMPRAS_CLIENTE_TIPO_COMPRA compras_get_cli_compra_from_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES lista, int p);
int compras_lista_assoc_prod_clis_get_num_elems(COMPRAS_ASSOC_PROD_CLIENTES lista);
char *compras_assoc_cli_compra_get_cod_cli(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra);
char compras_assoc_cli_compra_get_tipo_compra(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra);
void compras_free_assoc_prod_clientes(COMPRAS_ASSOC_PROD_CLIENTES assoc);

/*
 * PAGINACAO 
 * ASSOCIACAO PRODUTO -> CLIENTES e CLIENTE -> TIPO COMPRA
 */
COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_default(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc);
COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_primeira_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int elems_por_pag);
COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_ultima_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int elems_por_pag);
COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int n_pag, int elems_por_pag);
int compras_lista_assoc_pag_get_num_elems(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
int compras_assoc_prod_cli_pag_get_num_elems_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
int compras_assoc_goto_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int num_pag);
int compras_assoc_get_pos_inicio_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
int compras_assoc_get_num_pags(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
COMPRAS_CLIENTE_TIPO_COMPRA compras_assoc_get_elemento_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int n_elem);
void compras_assoc_set_num_elems_por_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int new_elems_por_pag);
int compras_assoc_get_elems_por_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
int compras_assoc_get_num_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);
void compras_assoc_free_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag);

/*
 * PAGINACAO LISTAS CLIENTE
 */
COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_default(COMPRAS_LISTA_CLIENTES lista_cli);
COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_primeira_pag(COMPRAS_LISTA_CLIENTES lista_cli, int elems_por_pag);
COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_ultima_pag(COMPRAS_LISTA_CLIENTES lista_cli, int elems_por_pag);
COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_pag(COMPRAS_LISTA_CLIENTES lista_cli, int n_pag, int elems_por_pag);
int compras_cli_goto_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int num_pag);
int compras_cli_pag_get_num_elems(COMPRAS_PAG_LISTA_CLIENTES pag);
int compras_cli_get_pos_inicio_pag(COMPRAS_PAG_LISTA_CLIENTES pag);
int compras_cli_get_num_pags(COMPRAS_PAG_LISTA_CLIENTES pag);
COMPRAS_FICHA_CLIENTE compras_cli_get_elemento_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int n_elem);
int compras_cli_pag_get_num_elems_pag(COMPRAS_PAG_LISTA_CLIENTES pag);
void compras_cli_set_num_elems_por_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int new_elems_por_pag);
int compras_cli_get_elems_por_pag(COMPRAS_PAG_LISTA_CLIENTES pag);
int compras_cli_get_num_pag(COMPRAS_PAG_LISTA_CLIENTES pag);
void compras_cli_free_pag(COMPRAS_PAG_LISTA_CLIENTES pag);

/*
 * PAGINACAO LISTAS PRODUTO
 */
COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_default(COMPRAS_LISTA_PRODUTOS lista_prod);
COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_primeira_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int elems_por_pag);
COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_ultima_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int elems_por_pag);
COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag);
int compras_prod_goto_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int num_pag);
int compras_lista_prod_get_num_elems(COMPRAS_LISTA_PRODUTOS lista);
int compras_prod_pag_get_num_elems(COMPRAS_PAG_LISTA_PRODUTOS pag);
int compras_prod_pag_get_num_elems_pag(COMPRAS_PAG_LISTA_PRODUTOS pag);
int compras_prod_get_pos_inicio_pag(COMPRAS_PAG_LISTA_PRODUTOS pag);
int compras_prod_get_num_pags(COMPRAS_PAG_LISTA_PRODUTOS pag);
COMPRAS_FICHA_PRODUTO compras_prod_get_elemento_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int n_elem);
void compras_prod_set_num_elems_por_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag);
int compras_prod_get_elems_por_pag(COMPRAS_PAG_LISTA_PRODUTOS pag);
int compras_prod_get_num_pag(COMPRAS_PAG_LISTA_PRODUTOS pag);
void compras_prod_free_pag(COMPRAS_PAG_LISTA_PRODUTOS pag);

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
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_primeiro(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_ultimo(Compras compras, char *cod_cliente);
IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_elem(Compras compras, char *cod_cliente, char *cod_produto);

/*
 * ITERADORES PRODUTOS -> NUMERO CLIENTES
 */
IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes(Compras compras);
IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_primeiro(Compras compras);
IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_ultimo(Compras compras);
IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_elem(Compras compras, char *st);
COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_actual(IT_COMPRAS_PRODUTOS_NCLIENTES it);
COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_proximo(IT_COMPRAS_PRODUTOS_NCLIENTES it);
COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_anterior(IT_COMPRAS_PRODUTOS_NCLIENTES it);
void free_it_compras_produtos_nclientes(IT_COMPRAS_PRODUTOS_NCLIENTES it);



#endif	/* COMPRAS_H */

