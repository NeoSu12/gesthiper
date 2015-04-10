#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"
#include "headers/contabilidade.h"

#define NORMAL 0
#define PROMO 1

struct mod_contabilidade {
    int total_global_compras[12][2];
    double total_global_fact[12][2];
    ARVORE avl_produtos;
};

struct cont_ficha_produto {
    char *cod_produto;
    int unidades_vendidas_prod[12][2];
    int total_unidades_vendidas_prod;
    double facturacao_prod[12][2];
    double total_facturacao_prod;
};

struct cont_lista_produtos{
    ARRAY_DINAMICO lista_paginada;
};

struct cont_paginador_lista_produtos{
    CONT_LISTA_PRODUTOS lista_pag;
    AD_PAGINADOR paginador;
};

struct iterador_cont {
    TRAVERSER traverser;
};


CONT_FICHA_PRODUTO inicializa_ficha_produto(char* cod_prod);
CONT_FICHA_PRODUTO cont_ficha_prod_clone(CONT_FICHA_PRODUTO src);
void free_ficha_prod(CONT_FICHA_PRODUTO prod);
CONT_FICHA_PRODUTO codigo_to_ficha(char* cod_prod);
CONT_FICHA_PRODUTO codigo_to_ficha_noclone(char* cod_prod) ;
CONT_FICHA_PRODUTO cont_procura_ficha_com_cod_avl(Contabilidade cont, char *cod_prod);
int cont_compara_ficha_por_cod_avl(const void *avl_a, const void *avl_b, void *avl_param);
int cont_compara_ficha_por_vendas_ad(void *avl_a,void *avl_b, void *);
void cont_free_string_ad(void *item);
void free_ficha_prod_avl(void *item, void *avl_param);
void troca_meses(int *mes1 , int *mes2);
void free_ficha_prod_ad(void *item);



/* 
 * CONTABILIDADE
 */

Contabilidade inicializa_contabilidade() {
    int i, j;
    Contabilidade res = (Contabilidade) malloc(sizeof (struct mod_contabilidade));
    
    res->avl_produtos = avl_create(cont_compara_ficha_por_cod_avl, NULL, NULL);
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++){
            res->total_global_fact[i][j] = 0;
            res->total_global_compras[i][j] = 0;
        }
    
    return res;
}

void cont_regista_produto(Contabilidade cont, char *cod_prod){
    CONT_FICHA_PRODUTO nodo = inicializa_ficha_produto(cod_prod);
    avl_insert(cont->avl_produtos, nodo);
}

void cont_insere_compra(Contabilidade cont, COMPRA comp) {
    CONT_FICHA_PRODUTO prod;
    int promo, mes;
    double facturacao_compra;
    
    if(get_promo(comp) == 'N') promo = NORMAL;
    else promo = PROMO;
    
    mes = get_mes(comp);
    facturacao_compra = get_quantidade(comp) * get_preco_unit(comp);
    
    prod = cont_procura_ficha_com_cod_avl(cont, get_cod_produto(comp));
    
    prod->total_unidades_vendidas_prod          += get_quantidade(comp);
    prod->total_facturacao_prod                 += facturacao_compra;
    
    prod->unidades_vendidas_prod[mes-1][promo]  += get_quantidade(comp);
    prod->facturacao_prod[mes - 1][promo]       += facturacao_compra;
    
    cont->total_global_compras[mes - 1][promo]  += 1;
    cont->total_global_fact[mes - 1][promo]     += facturacao_compra;
    
}

void cont_remove_produto(Contabilidade cont, char *cod_cliente){
    CONT_FICHA_PRODUTO produto = cont_procura_ficha_com_cod_avl(cont, cod_cliente);
    avl_delete(cont->avl_produtos, produto);
}

bool cont_existe_codigo_prod(Contabilidade cont, char* cod_prod) {
    CONT_FICHA_PRODUTO res = cont_procura_ficha_com_cod_avl(cont, cod_prod);
    return (res != NULL) ? true : false;
}

int cont_num_prod_sem_compras(Contabilidade cont) {
    int res = 0;
    CONT_FICHA_PRODUTO produto;
    IT_CONT iterador = inicializa_it_cont_fich_produtos(cont);

    while ((produto = it_cont_fich_produto_proximo(iterador)) != NULL) {
        if (cont_total_vendas_fich_produto(produto) == 0)
            res++;

        free_ficha_prod(produto);
    }

    free_it_cont_fich_prod(iterador);
    return res;
}

int cont_total_prods_comprados(Contabilidade cont){
    return avl_count(cont->avl_produtos);
}

void free_contabilidade(Contabilidade cont) {
    if(cont != NULL)
        avl_destroy(cont->avl_produtos, free_ficha_prod_avl);
    free(cont);
}


/* 
 * FACTURACAO GLOBAL
 */

double cont_total_facturacao(Contabilidade cont){
    return cont_total_facturacao_geral(cont, 1, 12, TOTAL_FACT_AMBOS);
}

double cont_total_facturacao_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo) {
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

double cont_total_facturacao_mes(Contabilidade cont, int mes){
    return cont_total_facturacao_geral(cont, mes, mes, TOTAL_FACT_AMBOS);
}

double cont_total_facturacao_normal_mes(Contabilidade cont, int mes){
    return cont_total_facturacao_geral(cont, mes, mes, TOTAL_FACT_NORMAL);
}

double cont_total_facturacao_promo_mes(Contabilidade cont, int mes){
    return cont_total_facturacao_geral(cont, mes, mes, TOTAL_FACT_PROMO);
}

double cont_total_facturacao_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return cont_total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_AMBOS);
}

double cont_total_facturacao_normal_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return cont_total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_NORMAL);
}

double cont_total_facturacao_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup){
    return cont_total_facturacao_geral(cont, mes_inf, mes_sup, TOTAL_FACT_PROMO);
}

/* 
 * COMPRAS GLOBAIS
 */

int cont_total_compras(Contabilidade cont){
    return cont_total_compras_geral(cont, 1, 12, TOTAL_COMPRAS_AMBOS);
}

int cont_total_compras_geral(Contabilidade cont, int mes_inf, int mes_sup, campo_t campo) {
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

int cont_total_compras_mes(Contabilidade cont, int mes) {
    return cont_total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_AMBOS);
}

int cont_total_compras_normais_mes(Contabilidade cont, int mes) {
    return cont_total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_NORMAL);
}

int cont_total_compras_promo_mes(Contabilidade cont, int mes) {
    return cont_total_compras_geral(cont, mes, mes, TOTAL_COMPRAS_NORMAL);
}

int cont_total_compras_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return cont_total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_AMBOS);
}

int cont_total_compras_normais_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return cont_total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_NORMAL);
}

int cont_total_compras_promo_int_meses(Contabilidade cont, int mes_inf, int mes_sup) {
    return cont_total_compras_geral(cont, mes_inf, mes_sup, TOTAL_COMPRAS_NORMAL);
}


/*
 * FUNCOES FICHA PRODUTO
 */

char *cont_get_cod_prod_ficha(CONT_FICHA_PRODUTO ficha_prod) {
    return ficha_prod->cod_produto;
}


/* 
 * FACTURACAO PRODUTO
 */

double cont_total_fact_fich_produto(CONT_FICHA_PRODUTO ficha_prod){
    return ficha_prod->total_facturacao_prod;
}

double cont_total_fact_fich_produto_geral(CONT_FICHA_PRODUTO fich_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    double res = 0;
    int i = 0;

    if (mes_inf > mes_sup) 
        troca_meses(&mes_inf, &mes_sup);


    if (fich_prod != NULL) {

        switch (campo) {
            case PROD_FACT_NORMAL:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->facturacao_prod[i - 1][NORMAL];
                break;
            case PROD_FACT_PROMO:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->facturacao_prod[i - 1][PROMO];
                break;
            case PROD_FACT_AMBOS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->facturacao_prod[i - 1][NORMAL] +
                        fich_prod->facturacao_prod[i - 1][PROMO];
                break;
            default:
                break;
        }

    } else {
        res = PROD_NAO_VENDIDO;
    }

    return res;
}

double cont_total_fact_fich_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes, mes,PROD_FACT_AMBOS);
}

double cont_total_fact_fich_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_FACT_AMBOS);
}

double cont_total_fact_fich_normal_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes, mes,PROD_FACT_NORMAL);
}

double cont_total_fact_fich_normal_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_FACT_NORMAL);
}

double cont_total_fact_fich_promo_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes, mes,PROD_FACT_PROMO);
}

double cont_total_fact_fich_promo_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_FACT_PROMO);
}

/*========================================*/

double cont_total_fact_produto(Contabilidade cont, char *cod_prod){
     CONT_FICHA_PRODUTO produto = cont_procura_ficha_com_cod_avl(cont, cod_prod);
     return cont_total_fact_fich_produto(produto);
}

double cont_total_fact_produto_geral(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    CONT_FICHA_PRODUTO produto = cont_procura_ficha_com_cod_avl(cont, cod_prod);
    return cont_total_fact_fich_produto_geral(produto, mes_inf, mes_sup, campo);
}

double cont_total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_AMBOS);
}

double cont_total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_AMBOS);
}

double cont_total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_NORMAL);
}

double cont_total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_NORMAL);
}

double cont_total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes, mes,PROD_FACT_PROMO);
}

double cont_total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_fact_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_FACT_PROMO);
}



/* 
 * VENDAS PRODUTO
 */

int cont_total_vendas_fich_produto(CONT_FICHA_PRODUTO ficha_prod){
    return ficha_prod->total_unidades_vendidas_prod;
}

int cont_total_vendas_fich_produto_geral(CONT_FICHA_PRODUTO fich_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    int res = 0, i = 0;
    
    if (mes_inf > mes_sup) troca_meses(&mes_inf, &mes_sup);
    
    if (fich_prod != NULL) {

        switch (campo) {
            case PROD_VENDAS_NORMAIS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->unidades_vendidas_prod[i - 1][NORMAL];
                break;
            case PROD_VENDAS_PROMO:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->unidades_vendidas_prod[i - 1][PROMO];
                break;
            case PROD_VENDAS_AMBOS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += fich_prod->unidades_vendidas_prod[i - 1][NORMAL] +
                        fich_prod->unidades_vendidas_prod[i - 1][PROMO];
                break;
            default:
                break;
        }

    } else {
        res = PROD_NAO_VENDIDO;
    }

    return res;
}

int cont_total_vendas_fich_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes, mes,PROD_VENDAS_AMBOS);
}

int cont_total_vendas_fich_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_VENDAS_AMBOS);
}

int cont_total_vendas_fich_normais_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes, mes,PROD_VENDAS_NORMAIS);
}

int cont_total_vendas_fich_normais_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_VENDAS_NORMAIS);
}

int cont_total_vendas_fich_promo_produto_mes(CONT_FICHA_PRODUTO fich_prod, int mes) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes, mes,PROD_VENDAS_PROMO);
}

int cont_total_vendas_fich_promo_produto_int_meses(CONT_FICHA_PRODUTO fich_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_fich_produto_geral(fich_prod, mes_inf, mes_sup,PROD_VENDAS_PROMO);
}

/*===================================*/

int cont_total_vendas_produto(Contabilidade cont, char *cod_prod){
    CONT_FICHA_PRODUTO produto = cont_procura_ficha_com_cod_avl(cont, cod_prod);
    return cont_total_vendas_fich_produto(produto);
}

int cont_total_vendas_produto_geral(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    CONT_FICHA_PRODUTO produto = cont_procura_ficha_com_cod_avl(cont, cod_prod);
    return cont_total_vendas_fich_produto_geral(produto, mes_inf, mes_sup, campo);
}

int cont_total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_AMBOS);
}

int cont_total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_AMBOS);
}

int cont_total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_NORMAIS);
}

int cont_total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_NORMAIS);
}

int cont_total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes, mes,PROD_VENDAS_PROMO);
}

int cont_total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return cont_total_vendas_produto_geral(cont, cod_prod, mes_inf, mes_sup,PROD_VENDAS_PROMO);
}

/*
 * LISTAS PRODUTOS
 */

CONT_LISTA_PRODUTOS cont_lista_prod_sem_compras(Contabilidade cont){
    CONT_FICHA_PRODUTO produto;
    CONT_LISTA_PRODUTOS lista = (CONT_LISTA_PRODUTOS) malloc(sizeof(struct cont_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa_cap(1000);
    IT_CONT iterador = inicializa_it_cont_fich_produtos(cont);
    
    while((produto = it_cont_fich_produto_proximo(iterador))!=NULL){
        if(cont_total_vendas_fich_produto(produto)==0){
            ad_insere_elemento(ad,produto);
        }else{
            free_ficha_prod(produto);
        }
    }
    lista->lista_paginada=ad;
    free_it_cont_fich_prod(iterador);
    return lista;
}

CONT_LISTA_PRODUTOS cont_top_produtos_comprados(Contabilidade cont, int n) {
    int i;
    CONT_FICHA_PRODUTO produto;
    CONT_LISTA_PRODUTOS lista = (CONT_LISTA_PRODUTOS) malloc(sizeof (struct cont_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa_cap(1000);
    IT_CONT iterador = inicializa_it_cont_fich_produtos(cont);


    for (i = 0; (produto = it_cont_fich_produto_proximo(iterador)) != NULL; i++) {
        ad_insere_elemento(ad, produto);
    }
    ad_ordena(ad, cont_compara_ficha_por_vendas_ad, NULL);

    if (n < ad_get_tamanho(ad)) {
        for (i = ad_get_tamanho(ad) - 1; i >= n; i--) {
            ad_remove_elemento_pos(ad, i);
        }
    }

    lista->lista_paginada = ad;
    free_it_cont_fich_prod(iterador);
    return lista;
}

CONT_FICHA_PRODUTO cont_lista_get_fich_prod(CONT_LISTA_PRODUTOS lista,int p){
    return (CONT_FICHA_PRODUTO) ad_get_elemento(lista->lista_paginada, p);
}

int cont_lista_get_num_elems(CONT_LISTA_PRODUTOS lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void cont_free_lista_produtos(CONT_LISTA_PRODUTOS lista){
    if(lista != NULL)
        ad_deep_free(lista->lista_paginada, free_ficha_prod_ad);
    
    free(lista);
}


/*
 * PAGINCAO LISTA PRODUTOS
 */

CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_default(CONT_LISTA_PRODUTOS lista_prod) {
    CONT_PAG_LISTA_PRODUTOS pag_res = (CONT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cont_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_default(pag_res->lista_pag->lista_paginada);
    return pag_res;
}

CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_primeira_pag(CONT_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    CONT_PAG_LISTA_PRODUTOS pag_res = (CONT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cont_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_primeira_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_ultima_pag(CONT_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    CONT_PAG_LISTA_PRODUTOS pag_res = (CONT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cont_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_ultima_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

CONT_PAG_LISTA_PRODUTOS cont_inicializa_paginador_pag(CONT_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag) {
    CONT_PAG_LISTA_PRODUTOS pag_res = (CONT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cont_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_pag(pag_res->lista_pag->lista_paginada,n_pag, elems_por_pag);
    return pag_res;
}

int cont_goto_pag(CONT_PAG_LISTA_PRODUTOS pag, int num_pag){
    return ad_goto_pag(pag->paginador, num_pag);
}

int cont_pag_get_num_elems(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_tamanho(pag->lista_pag->lista_paginada);
}

int cont_pag_get_num_elems_pag(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_elems_pag(pag->paginador);
}

int cont_get_pos_inicio_pag(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_pos_inicio_pag(pag->paginador);
}

int cont_get_num_pags(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pags(pag->paginador);
}

CONT_FICHA_PRODUTO cont_get_elemento_pag(CONT_PAG_LISTA_PRODUTOS pag, int n_elem){
    return (CONT_FICHA_PRODUTO) ad_get_elemento_pag(pag->paginador, n_elem);
}

void cont_set_num_elems_por_pag(CONT_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag){
    ad_set_num_elems_por_pag(pag->paginador, new_elems_por_pag);
}

int cont_get_elems_por_pag(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_elems_por_pag(pag->paginador);
}

int cont_get_num_pag(CONT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pag(pag->paginador);
}

void cont_free_pag(CONT_PAG_LISTA_PRODUTOS pag){
    if(pag != NULL)
        ad_free_pag(pag->paginador);
    
    free(pag);
}


/*
 * ITERADORES FICHA PRODUTOS
 */

IT_CONT inicializa_it_cont_fich_produtos(Contabilidade cont) {
    IT_CONT it = (IT_CONT) malloc(sizeof (struct iterador_cont));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, cont->avl_produtos);
    return it;
}

IT_CONT inicializa_it_cont_fich_produtos_primeiro(Contabilidade cont) {
    IT_CONT it = (IT_CONT) malloc(sizeof (struct iterador_cont));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cont->avl_produtos);
    return it;
}

IT_CONT inicializa_it_cont_fich_produtos_ultimo(Contabilidade cont) {
    IT_CONT it;
    it = (IT_CONT) malloc(sizeof (struct iterador_cont));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, cont->avl_produtos);
    return it;
}

IT_CONT inicializa_it_cont_fich_produtos_elem(Contabilidade cont, char *st) {
    IT_CONT it = NULL;
    CONT_FICHA_PRODUTO procura = NULL;
    
    if (st != NULL) {
        it = (IT_CONT) malloc(sizeof (struct iterador_cont));
        it->traverser = avl_t_alloc();
        procura = codigo_to_ficha(st);
        avl_t_find(it->traverser, cont->avl_produtos, procura);
        free_ficha_prod(procura);
    }
    
    return it;
}

CONT_FICHA_PRODUTO it_cont_fich_produto_actual(IT_CONT it) {
    CONT_FICHA_PRODUTO ret = NULL;
    CONT_FICHA_PRODUTO res = avl_t_cur(it->traverser);

    if (res != NULL)
        ret = cont_ficha_prod_clone(res);

    return ret;
}

CONT_FICHA_PRODUTO it_cont_fich_produto_proximo(IT_CONT it) {
    CONT_FICHA_PRODUTO ret = NULL;
    CONT_FICHA_PRODUTO res = avl_t_next(it->traverser);

    if (res != NULL)
        ret = cont_ficha_prod_clone(res);

    return ret;
}

CONT_FICHA_PRODUTO it_cont_fich_produto_anterior(IT_CONT it) {
    CONT_FICHA_PRODUTO ret = NULL;
    CONT_FICHA_PRODUTO res = avl_t_prev(it->traverser);

    if (res != NULL)
        ret = cont_ficha_prod_clone(res);

    return ret;
}

void free_it_cont_fich_prod(IT_CONT it){
    if(it != NULL)
        avl_t_free(it->traverser);
    
    free(it);
}


/* 
 * FUNCOES AUXILIARES PRIVADAS AO MODULO 
 */

CONT_FICHA_PRODUTO inicializa_ficha_produto(char* cod_prod) {
    int i, j;
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) malloc(sizeof (struct cont_ficha_produto));
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

CONT_FICHA_PRODUTO cont_ficha_prod_clone(CONT_FICHA_PRODUTO src){
    int i,j;
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) malloc(sizeof (struct cont_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(src->cod_produto) + 1));
    strcpy(copia, src->cod_produto);
    prod->cod_produto = copia;
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++) {
            prod->facturacao_prod[i][j] = src->facturacao_prod[i][j];
            prod->unidades_vendidas_prod[i][j] = src->unidades_vendidas_prod[i][j];
        }
    
    prod->total_facturacao_prod=src->total_facturacao_prod;
    prod->total_unidades_vendidas_prod=src->total_unidades_vendidas_prod;
    return prod;
}
 
void free_ficha_prod(CONT_FICHA_PRODUTO prod) {
    if(prod != NULL)
        free(prod->cod_produto);
    
    free(prod);
}

CONT_FICHA_PRODUTO codigo_to_ficha(char* cod_prod) {
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) malloc(sizeof (struct cont_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    return prod;
}

CONT_FICHA_PRODUTO codigo_to_ficha_noclone(char* cod_prod) {
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) malloc(sizeof (struct cont_ficha_produto));
    prod->cod_produto = cod_prod;
    return prod;
}

CONT_FICHA_PRODUTO cont_procura_ficha_com_cod_avl(Contabilidade cont, char *cod_prod){
    CONT_FICHA_PRODUTO nodo_aux = codigo_to_ficha(cod_prod);
    CONT_FICHA_PRODUTO res = (CONT_FICHA_PRODUTO) avl_find(cont->avl_produtos, nodo_aux);
    free(nodo_aux->cod_produto);
    free(nodo_aux);
    return res;
}

int cont_compara_ficha_por_cod_avl(const void *avl_a, const void *avl_b,
                                 void *avl_param){
    CONT_FICHA_PRODUTO a = (CONT_FICHA_PRODUTO) avl_a;
    CONT_FICHA_PRODUTO b = (CONT_FICHA_PRODUTO) avl_b;
    return strcmp(a->cod_produto, b->cod_produto);
}

int cont_compara_ficha_por_vendas_ad(void *avl_a, void *avl_b, void *param){
    int res=0;
    CONT_FICHA_PRODUTO a = (CONT_FICHA_PRODUTO) avl_a;
    CONT_FICHA_PRODUTO b = (CONT_FICHA_PRODUTO) avl_b;
    
    if(a->total_unidades_vendidas_prod > b->total_unidades_vendidas_prod) res = -1;
    else if(a->total_unidades_vendidas_prod == b->total_unidades_vendidas_prod)res = 0;
    else res = 1;
    
    return res;
}

void cont_free_string_ad(void *item){
    free(item);
}

void free_ficha_prod_avl(void *item, void *avl_param) {
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) item;
    free(prod->cod_produto);
    free(prod);
}

void free_ficha_prod_ad(void *item) {
    CONT_FICHA_PRODUTO prod = (CONT_FICHA_PRODUTO) item;
    free(prod->cod_produto);
    free(prod);
}

void troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}

