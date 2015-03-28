#ifndef Contabilidade_H
#define	Contabilidade_H
#include "compra.h"

#define PROD_NAO_VENDIDO -1

typedef struct mod_contabilidade *Contabilidade;


Contabilidade inicializa_contabilidade();
void cont_regista_produto(Contabilidade, char *);
void cont_insere_compra(Contabilidade, COMPRA);
void free_contabilidade(Contabilidade);



/* 
 * VENDAS PRODUTO
 */

int cont_total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int cont_total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int cont_total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int cont_total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);

/* 
 * COMPRAS GLOBAIS
 */

int cont_total_compras_mes(Contabilidade cont, int mes);
int cont_total_compras_normais_mes(Contabilidade cont, int mes);
int cont_total_compras_promo_mes(Contabilidade cont, int mes);
int cont_total_compras_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int cont_total_compras_normais_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int cont_total_compras_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);


/* 
 * FACTURACAO PRODUTO
 */

double cont_total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes);
double cont_total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup);
double cont_total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double cont_total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
double cont_total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double cont_total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);

/* 
 * FACTURACAO GLOBAL
 */

double cont_total_facturacao_mes(Contabilidade cont, int mes);
double cont_total_facturacao_normal_mes(Contabilidade cont, int mes);
double cont_total_facturacao_promo_mes(Contabilidade cont, int mes);
double cont_total_facturacao_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double cont_total_facturacao_normal_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double cont_total_facturacao_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);

#endif
