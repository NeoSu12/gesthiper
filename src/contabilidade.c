#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"
#include "headers/contabilidade.h"

#define NORMAL 0
#define PROMO 1

struct mod_contabilidade {
    int total_global_compras[12][2];
    int total_global_fact[12][2];
    ARVORE avl_produtos;
};

struct produtos {
    char *cod_produto;
    int tabela_quantidade[12][2];
    /* Quantidade dum produto vendida num determinado mes 0-Normal / 1- Promo */
    int tabela_fact_produto[12][2];
    /* Facturacao por mes */

};


typedef struct produtos *NodoProdutos;
/*funcoes locais*/
int vendas_normal_mes();
int vendas_promo_mes();
int total_fact_normal_mes();
int total_fact_promo_mes(); 



Contabilidade inicializa_contabilidade() {
    int i, j;
    Contabilidade res = (Contabilidade) malloc(sizeof (struct mod_contabilidade));
    res->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++)
            res->total_global_fact[i][j] = 0;
    return res;
}

/*cod_cliente_t get_cod_cliente(COMPRA);
preco_unit_t get_preco_unit(COMPRA);
quantidade_t get_quantidade(COMPRA);
promo_t get_promo(COMPRA);
cod_produto_t get_cod_produto(COMPRA);
mes_t get_mes(COMPRA);*/
void inserir_produto(Contabilidade cont, COMPRA comp) {
    cod_cliente_t cod_cli = comp->cod_cliente;
    preco_unit_t preco_uni = comp->preco_unit;
    quantidade_t quant = comp->quantidade;
    int promo = (get_promo(comp) == 'N') ? NORMAL : PROMO;
    cod_produto_t cod_prod = comp->cod_produto;
    mes_t mes = comp->mes;
    NodoProdutos nodo = criaNodo(cod_prod);
    NodoProdutos prod;
    if ((prod = (NodoProdutos) avl_find(cont->avl_produtos, nodo)) == NULL) {
        avl_insert(cont, nodo);
        prod = nodo;
    }
    prod->tabela_quantidade[mes - 1][promo] += quant;
    prod->tabela_fact_produto[mes - 1][promo] += quant * preco_uni;
    cont->total_global_compras[mes - 1][promo]++;
    cont->total_global_fact[mes - 1][promo] += quant * preco_uni;
    return 0;
}

/*
int existeNodo(Contabilidade cont,NodoProdutos x){
    return (avl_find(cont->avl_produtos,x)==NULL)?0:1;
}
 */
int existeCodigo(Contabilidade cont, char* cod) {
    NodoProdutos val;
    NodoProdutos nodo;
    nodo = criaNodo(cod);
    val = (NodoProdutos) avl_find(cont->avl_produtos, nodo);
    free_nodo_produto(nodo);
    return (val == NULL) ? 0 : 1;
}

NodoProdutos criaNodo(char* cod) {
    int i, j;
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod) + 1));
    NodoProdutos temp = (NodoProdutos) malloc(sizeof (struct produtos));
    strcpy(copia, cod);
    temp->cod_produto = copia;
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++) {
            temp->tabela_fact_produto[i][j] = 0;
            temp->tabela_quantidade[i][j] = 0;
        }

    return temp;
}

/*querie 3 - dado um cod e um mes, saber o total facturado e total vendas*/
int total_vendas_mes(Contabilidade cont, int mes, char* cod) {
    int res;
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod) + 1));
    NodoProdutos prod = (NodoProdutos) malloc(sizeof (struct produtos));
    NodoProdutos temp = (NodoProdutos) malloc(sizeof (struct produtos));
    strcpy(copia, cod);
    temp->cod_produto = copia;
    prod = (NodoProdutos) avl_find(cont->avl_produtos, temp);
    res = vendas_normal_mes(mes, prod)+(vendas_promo_mes(mes, prod));
    free_nodo_produto(prod);
    free_nodo_produto(temp);
    return res;
}

int total_fact_mes(Contabilidade cont, int mes, char* cod) {
    int res;
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod) + 1));
    NodoProdutos prod = (NodoProdutos) malloc(sizeof (struct produtos));
    NodoProdutos temp = (NodoProdutos) malloc(sizeof (struct produtos));
    strcpy(copia, cod);
    temp->cod_produto = copia;
    prod = (NodoProdutos) avl_find(cont->avl_produtos, temp);
    res = total_fact_normal_mes(mes, prod)+(total_fact_promo_mes(mes, prod));
    free_nodo_produto(prod);
    free_nodo_produto(temp);
    return res;
}

/* funcoes locais - querie 3 */
int vendas_normal_mes(int mes, NodoProdutos prod) {
    return (prod->tabela_quantidade[mes - 1][NORMAL]);
}

int vendas_promo_mes(int mes, NodoProdutos prod) {
    return (prod->tabela_quantidade[mes - 1][PROMO]);
}

int total_fact_normal_mes(int mes, NodoProdutos prod) {
    return (prod->tabela_fact_produto[mes - 1][NORMAL]);
}

int total_fact_promo_mes(int mes, NodoProdutos prod) {
    return (prod->tabela_fact_produto[mes - 1][PROMO]);
}










/*querie 4 - Devolver lista e numeros de produtos com 0 vendas*/

/*querie 7 - Dado um intervalo de meses, devolver o numero de compras e total facturado - completo*/

/*entre mes1 e mes2, dá total de compras e total facturado*/

void compras_total_fact_intervalo(Contabilidade cont, int mes1, int mes2, int *compras, int *total_fact) {
    *compras = 0;
    *total_fact = 0;
    while (mes1 < mes2) {
        *compras += compras_total_mes(cont, mes1);
        *total_fact += total_fact_mes(cont, mes1);
        mes1++;
    }
}

/*dado um mes, da total de compras*/
int compras_total_mes(Contabilidade cont, int mes) {
    return (cont->total_global_compras[mes - 1][NORMAL] + cont->total_global_compras[mes - 1][PROMO]);
}

int total_fact_mes(Contabilidade cont, int mes) {
    return (cont->total_global_fact[mes - 1][NORMAL] + cont->total_global_fact[mes - 1][PROMO]);
}

/*querie 11 - Ficheiro em formato CSV, com mes | compras | clientes - FAZER NO MAIN REAPROVEITANDO AS FUNCOES PREVIAMENTE FEITAS*/

/*querie 12 - Devolver uma lista dos N produtos mais vendidos INCOMPLETO*/

ARRAY_DINAMICO lista_produtos_mais_comprados(Contabilidade cont) {
    NodoProdutos produto;
    TRAVERSER it = avl_t_alloc();
    avl_t_init(it, cont->avl_produtos);

    while ((produto = avl_t_next(it)) != NULL) {

    }

}

/*querie 14 - clientes zombie - FAZER NO MAIN CONJUNTAMENTE COM O MODULO COMPRAS*/




void free_nodo_produto(NodoProdutos prod) {
    free(prod->cod_produto);
    free(prod);
}

void free_contabilidade(Contabilidade cont) {
    avl_destroy(cont->avl_produtos, free_produtos);
    free(cont);
}