#ifndef Contabilidade_H
#define	Contabilidade_H
#include "compra.h"

#define PROD_NAO_VENDIDO -1
#define CONT_ELEMS_PAG 10

typedef enum campo {
    PROD_VENDAS_NORMAIS = 1, PROD_VENDAS_PROMO, PROD_VENDAS_AMBOS,
    PROD_FACT_NORMAL, PROD_FACT_PROMO, PROD_FACT_AMBOS,
    TOTAL_COMPRAS_NORMAL, TOTAL_COMPRAS_PROMO, TOTAL_COMPRAS_AMBOS,
    TOTAL_FACT_NORMAL, TOTAL_FACT_PROMO, TOTAL_FACT_AMBOS
} campo_t;

typedef struct cont_ficha_produto *CONT_FICHA_PRODUTO;
typedef struct mod_contabilidade *Contabilidade;
typedef struct cont_lista_produtos* CONT_LISTA_PRODUTOS;
typedef struct iterador_cont* IT_CONT;
typedef struct cont_paginador_lista_produtos* CONT_PAG_LISTA_PRODUTOS;


/* 
 * INICIALIZACAO E GESTAO MEMORIA
 */

Contabilidade inicializa_contabilidade();
void cont_regista_produto(Contabilidade cont, char *cod_prod);
void cont_insere_compra(Contabilidade cont, COMPRA comp);
void cont_remove_produto(Contabilidade cont, char *cod_cliente);
void free_contabilidade(Contabilidade cont);
/* 
 * CONSULTAS
 */

int cont_existe_codigo_prod(Contabilidade cont, char* cod_prod);
int cont_total_prods_comprados(Contabilidade cont);

/* 
 * FACTURACAO GLOBAL
 */

double cont_total_facturacao(Contabilidade cont);
double cont_total_facturacao_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo);
double cont_total_facturacao_mes(Contabilidade cont, int mes);
double cont_total_facturacao_normal_mes(Contabilidade cont, int mes);
double cont_total_facturacao_promo_mes(Contabilidade cont, int mes);
double cont_total_facturacao_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double cont_total_facturacao_normal_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double cont_total_facturacao_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);

/* 
 * COMPRAS GLOBAIS
 */

int cont_total_compras(Contabilidade cont);
int cont_total_compras_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo);
int cont_total_compras_mes(Contabilidade cont, int mes);
int cont_total_compras_normais_mes(Contabilidade cont, int mes);
int cont_total_compras_promo_mes(Contabilidade cont, int mes);
int cont_total_compras_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int cont_total_compras_normais_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int cont_total_compras_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
/*
 * FUNCOES FICHA PRODUTO PUBLICAS
 */

char *cont_get_cod_prod_ficha(CONT_FICHA_PRODUTO ficha_prod);

/* 
 * FACTURACAO PRODUTO
 */

double cont_total_fact_fich_produto(CONT_FICHA_PRODUTO ficha_prod);
double cont_total_fact_fich_produto_geral(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup, campo_t campo);
double cont_total_fact_fich_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
double cont_total_fact_fich_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);
double cont_total_fact_fich_normal_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
double cont_total_fact_fich_normal_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);
double cont_total_fact_fich_promo_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
double cont_total_fact_fich_promo_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);

/*========================================*/

double cont_total_fact_produto_geral(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup, campo_t campo);
double cont_total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes);
double cont_total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup);
double cont_total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double cont_total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
double cont_total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double cont_total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);


/* 
 * VENDAS PRODUTO
 */

int cont_total_vendas_fich_produto(CONT_FICHA_PRODUTO ficha_prod);
int cont_total_vendas_fich_produto_geral(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup, campo_t campo);
int cont_total_vendas_fich_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
int cont_total_vendas_fich_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);
int cont_total_vendas_fich_normais_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
int cont_total_vendas_fich_normais_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);
int cont_total_vendas_fich_promo_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes);
int cont_total_vendas_fich_promo_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup);

/*===================================*/

int cont_total_vendas_produto_geral(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup, campo_t campo);
int cont_total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int cont_total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int cont_total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
/*
 * LISTAGENS / PAGINAÇÃO
 */

CONT_LISTA_PRODUTOS cont_lista_prod_sem_compras(Contabilidade cont);
int cont_num_prod_sem_compras(Contabilidade cont);
CONT_LISTA_PRODUTOS cont_top_produtos_comprados(Contabilidade cont, int n);
int cont_lista_get_num_elems(CONT_LISTA_PRODUTOS lista);
CONT_FICHA_PRODUTO cont_lista_get_fich_prod(CONT_LISTA_PRODUTOS lista,int p);
CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_default(CONT_LISTA_PRODUTOS lista_prod);
CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_primeira_pag(CONT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_ultima_pag(CONT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_pag(CONT_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag);
int cont_goto_pag(CONT_PAG_LISTA_PRODUTOS pag, int num_pag);
int cont_pag_get_num_elems(CONT_PAG_LISTA_PRODUTOS pag);
int cont_pag_get_num_elems_pag(CONT_PAG_LISTA_PRODUTOS pag);
int cont_get_pos_inicio_pag(CONT_PAG_LISTA_PRODUTOS pag);
int cont_get_num_pags(CONT_PAG_LISTA_PRODUTOS pag);
CONT_FICHA_PRODUTO cont_get_elemento_pag(CONT_PAG_LISTA_PRODUTOS pag, int n_elem);
void cont_set_num_elems_por_pag(CONT_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag);
int cont_get_elems_por_pag(CONT_PAG_LISTA_PRODUTOS pag);
int cont_get_num_pag(CONT_PAG_LISTA_PRODUTOS pag);
void cont_free_pag(CONT_PAG_LISTA_PRODUTOS pag);
void cont_free_lista_produtos(CONT_LISTA_PRODUTOS lista);

/*
 * ITERADORES
 */

IT_CONT inicializa_it_cont_fich_produtos(Contabilidade cont);
IT_CONT inicializa_it_cont_fich_produtos_primeiro(Contabilidade cont);
IT_CONT inicializa_it_cont_fich_produtos_ultimo(Contabilidade cont);
IT_CONT inicializa_it_cont_fich_produtos_elem(Contabilidade cont, char *st);
CONT_FICHA_PRODUTO it_cont_fich_produto_actual(IT_CONT it);
CONT_FICHA_PRODUTO it_cont_fich_produto_proximo(IT_CONT it);
CONT_FICHA_PRODUTO it_cont_fich_produto_anterior(IT_CONT it);
void free_it_cont_fich_prod(IT_CONT it);

#endif
