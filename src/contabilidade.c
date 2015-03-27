#include <stdlib.h>
#include <string.h>
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"
#include "headers/contabilidade.h"

#define NORMAL 0
#define PROMO 1

typedef enum campo {
    PROD_VENDAS_NORMAIS = 1, PROD_VENDAS_PROMO, PROD_VENDAS_AMBOS,
    PROD_FACT_NORMAL, PROD_FACT_PROMO, PROD_FACT_AMBOS,
    TOTAL_COMPRAS_NORMAL, TOTAL_COMPRAS_PROMO, TOTAL_COMPRAS_AMBOS,
    TOTAL_FACT_NORMAL, TOTAL_FACT_PROMO, TOTAL_FACT_AMBOS
} campo_t;

struct mod_contabilidade {
    int total_global_compras[12][2];
    double total_global_fact[12][2];
    ARVORE avl_produtos;
};

typedef struct produtos {
    char *cod_produto;
    int total_unidades_vendidas_prod;
    double total_facturacao_prod;
    int unidades_vendidas_prod[12][2];
    /* Quantidade dum produto vendida num determinado mes 0-Normal / 1- Promo */
    double facturacao_prod[12][2];
    /* Facturacao por mes */
}*NodoProdutos;

typedef struct produto_venda{
    char *codigo_produto;
    int n_vendas;
}*PRODUTO_VENDA;

NodoProdutos inicializa_nodo_produto(char*);
void free_nodo_produto_avl(void *, void *);
void free_nodo_produto(NodoProdutos);
PRODUTO_VENDA inicializa_produto_venda(char *, int);
int ordena_produto_venda_desc(void *, void *);
void free_produto_venda(PRODUTO_VENDA);
void free_produto_venda_ad(void *item);
int compara_vendas_produtos(const void *, const void *,void *);
NodoProdutos procura_nodo_avl_com_cod(Contabilidade, char *);
void troca_meses(int * , int *);
int total_vendas_produto_geral(Contabilidade, char*,int, int, campo_t);
double total_fact_produto_geral(Contabilidade, char*, int, int, campo_t);
int total_compras_geral(Contabilidade, int, int, campo_t);
double total_facturacao_geral(Contabilidade, int, int, campo_t);

/* 
 * INICIALIZACAO E GESTAO MEMORIA
 */

Contabilidade inicializa_contabilidade() {
    int i, j;
    Contabilidade res = (Contabilidade) malloc(sizeof (struct mod_contabilidade));
    
    res->avl_produtos = avl_create(compara_vendas_produtos, NULL, NULL);
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++){
            res->total_global_fact[i][j] = 0;
            res->total_global_compras[i][j] = 0;
        }
    
    return res;
}

void cont_regista_produto(Contabilidade cont, char *cod_prod){
    NodoProdutos nodo = inicializa_nodo_produto(cod_prod);
    avl_insert(cont->avl_produtos, nodo);
}

void cont_insere_compra(Contabilidade cont, COMPRA comp) {
    NodoProdutos nodo_pesquisa, prod;
    int promo, mes;
    double facturacao_compra;
    
    if(get_promo(comp) == 'N') promo = NORMAL;
    else promo = PROMO;
    
    mes = get_mes(comp);
    facturacao_compra = get_quantidade(comp) * get_preco_unit(comp);
    
    nodo_pesquisa = codigo_to_nodo(get_cod_produto(compra));
    prod = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);
    
    prod->total_unidades_vendidas_prod          += get_quantidade(comp);
    prod->unidades_vendidas_prod[mes-1][promo]  += get_quantidade(comp);
    prod->total_facturacao_prod                 += facturacao_compra;
    prod->facturacao_prod[mes - 1][promo]       += facturacao_compra;
    cont->total_global_compras[mes - 1][promo]  += 1;
    cont->total_global_fact[mes - 1][promo]     += facturacao_compra;
    
    free_nodo_produto(nodo_pesquisa);
}

void free_contabilidade(Contabilidade cont) {
    avl_destroy(cont->avl_produtos, free_nodo_produto_avl);
    free(cont);
}

/* 
 * CONSULTAS
 */

int existeCodigo(Contabilidade cont, char* cod_prod) {
    NodoProdutos res = procura_nodo_avl_com_cod(cont, cod_prod);
    return (res != NULL) ? 1 : 0;
}

int total_prods_comprados(Contabilidade cont){
    return avl_count(cont->avl_produtos);
}

/* 
 * FACTURACAO GLOBAL
 */

double total_facturacao_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo) {
    double total = 0;
    int i = 0;

    if (mes_inf > mes_sup) troca_meses(&mes_inf, &mes_sup);

    switch (campo) {
        case TOTAL_FACT_NORMAL:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_fact[i - 1][NORMAL];
            break;
        case TOTAL_FACT_PROMO:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_fact[i - 1][PROMO];
            break;
        case TOTAL_FACT_AMBOS:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_fact[i - 1][NORMAL] +
                    cont->total_global_fact[i - 1][PROMO];
            break;
        default:
            break;
    }
    
    return total;
}

double total_facturacao_mes(Contabilidade cont, int mes){
    return total_facturacao_geral(cont, mes, mes, TOTAL_FACT_AMBOS);
}

double total_facturacao_normal_mes(Contabilidade cont, int mes){
    return total_facturacao_geral(cont, mes, mes, TOTAL_FACT_NORMAL);
}

double total_facturacao_promo_mes(Contabilidade cont, int mes){
    return total_facturacao_geral(cont, mes, mes, TOTAL_FACT_PROMO);
}

double total_facturacao_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_AMBOS);
}

double total_facturacao_normal_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_NORMAL);
}

double total_facturacao_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_PROMO);
}

/* 
 * COMPRAS GLOBAIS
 */

int total_compras_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo) {
    int total = 0, i = 0;

    if (mes_inf > mes_sup) troca_meses(&mes_inf, &mes_sup);

    switch (campo) {
        case TOTAL_COMPRAS_NORMAL:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_compras[i - 1][NORMAL];
            break;
        case TOTAL_COMPRAS_PROMO:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_compras[i - 1][PROMO];
            break;
        case TOTAL_COMPRAS_AMBOS:
            for (i = mes_inf; i <= mes_sup; i++)
                total += cont->total_global_compras[i - 1][NORMAL] +
                    cont->total_global_compras[i - 1][PROMO];
            break;
        default:
            break;
    }

    return total;
}

int total_compras_mes(Contabilidade cont, int mes) {
    return total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_AMBOS);
}

int total_compras_normais_mes(Contabilidade cont, int mes) {
    return total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_NORMAL);
}

int total_compras_promo_mes(Contabilidade cont, int mes) {
    return total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_NORMAL);
}

int total_compras_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_AMBOS);
}

int total_compras_normais_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_NORMAL);
}

int total_compras_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_NORMAL);
}

/* 
 * FACTURACAO PRODUTO
 */

double total_fact_produto_geral(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    double res = 0;
    int i = 0;
    NodoProdutos produto = procura_nodo_avl_com_cod(cont, cod_prod);

    if (mes_inf > mes_sup) 
        troca_meses(&mes_inf, &mes_sup);


    if (produto != NULL) {

        switch (campo) {
            case PROD_FACT_NORMAL:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->facturacao_prod[i - 1][NORMAL];
                break;
            case PROD_FACT_PROMO:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->facturacao_prod[i - 1][PROMO];
                break;
            case PROD_FACT_AMBOS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->facturacao_prod[i - 1][NORMAL] +
                        produto->facturacao_prod[i - 1][PROMO];
                break;
            default:
                break;
        }

    } else {
        res = PROD_NAO_VENDIDO;
    }

    return res;
}

double total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes) {
    return total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_AMBOS);
}

double total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_AMBOS);
}

double total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_NORMAL);
}

double total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_NORMAL);
}

double total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_PROMO);
}

double total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_PROMO);
}


/* 
 * VENDAS PRODUTO
 */

int total_vendas_produto_geral(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    int res = 0, i = 0;
    NodoProdutos produto = procura_nodo_avl_com_cod(cont, cod_prod);

    if (mes_inf > mes_sup) 
        troca_meses(&mes_inf, &mes_sup);


    if (produto != NULL) {

        switch (campo) {
            case PROD_VENDAS_NORMAIS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->unidades_vendidas_prod[i - 1][NORMAL];
                break;
            case PROD_VENDAS_PROMO:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->unidades_vendidas_prod[i - 1][PROMO];
                break;
            case PROD_VENDAS_AMBOS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->unidades_vendidas_prod[i - 1][NORMAL] +
                        produto->unidades_vendidas_prod[i - 1][PROMO];
                break;
            default:
                break;
        }

    } else {
        res = PROD_NAO_VENDIDO;
    }

    return res;
}

int total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_AMBOS);
}

int total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_AMBOS);
}

int total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_NORMAIS);
}

int total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_NORMAIS);
}

int total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_PROMO);
}

int total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_PROMO);
}



ARRAY_DINAMICO lista_produtos_mais_comprados(Contabilidade cont, int top_n) {
    int i;
    NodoProdutos produto;
    PRODUTO_VENDA prod_venda;
    ARRAY_DINAMICO lista = ad_inicializa_cap(top_n);
    TRAVERSER it = avl_t_alloc();
    avl_t_init(it, cont->avl_produtos);

    while ((produto = avl_t_next(it)) != NULL) {
        prod_venda = inicializa_produto_venda(produto->cod_produto, produto->total_unidades_vendidas_prod);
        ad_insere_elemento(lista,prod_venda);
    }

    ad_ordena(lista, ordena_produto_venda_desc);
    
    for(i=ad_get_tamanho()-1;i>top_n;i--){
        ad_remove_elemento_pos(lista, i);
    }
    
    avl_t_free(it);
    return lista;
}

ARRAY_DINAMICO produtos_sem_vendas(Contabilidade cont){
    NodoProdutos produto;
    PRODUTO_VENDA prod_venda;
    ARRAY_DINAMICO lista = ad_inicializa_cap(2000);
    TRAVERSER it = avl_t_alloc();
    avl_t_init(it, cont->avl_produtos);

    while ((produto = avl_t_next(it)) != NULL) {
        prod_venda = inicializa_produto_venda(produto->cod_produto, produto->total_unidades_vendidas_prod);
        ad_insere_elemento(lista, prod_venda);
    }
    
    ad_ordena(lista, ordena_produto_venda_desc);

    avl_t_free(it);
    return lista;
    
}



/* 
 * FUNCOES AUXILIARES PRIVADAS AO MODULO 
 */

NodoProdutos inicializa_nodo_produto(char* cod_prod) {
    int i, j;
    NodoProdutos prod = (NodoProdutos) malloc(sizeof (struct produtos));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++) {
            prod->facturacao_prod[i][j] = 0;
            prod->unidades_vendidas_prod[i][j] = 0;
        }
    
    prod->total_facturacao_prod=0;
    prod->total_unidades_vendidas_prod=0;
    return prod;
}

PRODUTO_VENDA inicializa_produto_venda(char *cod, int vendas){
    PRODUTO_VENDA res = (PRODUTO_VENDA) malloc(sizeof(struct produto_venda));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod) + 1));
    strcpy(copia, cod);
    res->codigo_produto=copia;
    res->n_vendas = vendas;
    return res;
}

int ordena_produto_venda_desc(void *item_a, void *item_b){
    int res=0;
    PRODUTO_VENDA a = (PRODUTO_VENDA) item_a;
    PRODUTO_VENDA b = (PRODUTO_VENDA) item_b;
    
    if(a->n_vendas > b->n_vendas) res=-1;
    else if(a->n_vendas == b->n_vendas) res=0;
    else res=1;
    
    return res;
}

void free_produto_venda(PRODUTO_VENDA pv){
    free(pv->codigo_produto);
    free(pv);
}

void free_produto_venda_ad(void *item){
    PRODUTO_VENDA pv = (PRODUTO_VENDA) item;
    free(pv->codigo_produto);
    free(pv);
}

NodoProdutos codigo_to_nodo(char* cod_prod) {
    int i, j;
    NodoProdutos prod = (NodoProdutos) malloc(sizeof (struct produtos));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    return prod;
}


void free_nodo_produto_avl(void *item, void *avl_param) {
    NodoProdutos prod = (NodoProdutos) item;
    free(prod->cod_produto);
    free(prod);
}

void free_nodo_produto(NodoProdutos prod) {
    free(prod->cod_produto);
    free(prod);
}

int compara_vendas_produtos(const void *avl_a, const void *avl_b,
                                 void *avl_param){
    int res=0;
    NodoProdutos a = (NodoProdutos) avl_a;
    NodoProdutos b = (NodoProdutos) avl_b;
    
    if(a->total_unidades_vendidas_prod > b->total_unidades_vendidas_prod) res = 1;
    else if(a->total_unidades_vendidas_prod == b->total_unidades_vendidas_prod)res = 0;
    else res = -1;
    
    return res;
}

NodoProdutos procura_nodo_avl_com_cod(Contabilidade cont, char *cod_prod){
    NodoProdutos nodo_aux = inicializa_nodo_produto(cod_prod);
    NodoProdutos res = (NodoProdutos) avl_find(cont->avl_produtos, nodo_aux);
    free_nodo_produto(nodo_aux);
    return res;
}

void troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}

