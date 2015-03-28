#ifndef Contabilidade_H
#define	Contabilidade_H

#define PROD_NAO_VENDIDO -1

typedef struct mod_contabilidade *Contabilidade;


Contabilidade inicializa_contabilidade();
void cont_regista_produto(Contabilidade, char *);
void cont_insere_compra(Contabilidade, COMPRA);
void free_contabilidade(Contabilidade);

/* 
 * VENDAS PRODUTO
 */

int total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);

/* 
 * COMPRAS GLOBAIS
 */

int total_compras_mes(Contabilidade cont, int mes);
int total_compras_normais_mes(Contabilidade cont, int mes);
int total_compras_promo_mes(Contabilidade cont, int mes);
int total_compras_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int total_compras_normais_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
int total_compras_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);


/* 
 * FACTURACAO PRODUTO
 */

double total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes);
double total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup);
double total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
double total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);

/* 
 * FACTURACAO GLOBAL
 */

double total_facturacao_mes(Contabilidade cont, int mes);
double total_facturacao_normal_mes(Contabilidade cont, int mes);
double total_facturacao_promo_mes(Contabilidade cont, int mes);
double total_facturacao_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double total_facturacao_normal_int_meses(Contabilidade cont, int mes_inf, int mes_sup);
double total_facturacao_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup);

#endif
