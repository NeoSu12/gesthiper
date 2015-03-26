#ifndef Contabilidade_H
#define	Contabilidade_H

#define PROD_NAO_VENDIDO -1

typedef struct mod_contabilidade *Contabilidade;


Contabilidade inicializa_contabilidade();
void inserir_produto(Contabilidade, COMPRA);
int existeCodigo(Contabilidade, char*);
int total_vendas_produto_mes(Contabilidade, char*, int);
int total_fact_produto_mes(Contabilidade, int, char*);

#endif
