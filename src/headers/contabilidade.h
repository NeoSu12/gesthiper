#ifndef Contabilidade_H
#define	Contabilidade_H

#define PROD_NAO_VENDIDO -1

typedef struct mod_contabilidade *Contabilidade;


Contabilidade inicializa_contabilidade();
void insere_compra(Contabilidade, COMPRA);
int existeCodigo(Contabilidade, char*);


int total_prods_comprados(Contabilidade);


/* 
 * VENDAS 
 */

int total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
int total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
int total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);

/* 
 * FACTURACAO 
 */

double total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes);
double total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup);
double total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);
double total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes);
double total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup);


#endif
