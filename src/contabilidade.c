#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"
#include "headers/contabilidade.h"

#define NORMAL 0
#define PROMO 1

typedef enum campo {
    CMP_VENDAS_NORMAIS = 1, CMP_VENDAS_PROMO, CMP_VENDAS_AMBOS,
    CMP_FACT_NORMAL, CMP_FACT_PROMO, CMP_FACT_AMBOS
} campo_t;



struct mod_contabilidade {
    int total_global_compras[12][2];
    double total_global_fact[12][2];
    ARVORE avl_produtos;
};

struct produtos {
    char *cod_produto;
    int tabela_qtd_vendida[12][2];
    /* Quantidade dum produto vendida num determinado mes 0-Normal / 1- Promo */
    double tabela_fact_produto[12][2];
    /* Facturacao por mes */

};


typedef struct produtos *NodoProdutos;
/*funcoes locais*/
int total_vendas_produto(Contabilidade, char*,int, int, campo_t);
double total_fact_produto(Contabilidade, char*, int, int, campo_t);

NodoProdutos criaNodo(char*);
void troca_meses(int *, int *);
void free_nodo_produto(NodoProdutos);


Contabilidade inicializa_contabilidade() {
    int i, j;
    Contabilidade res = (Contabilidade) malloc(sizeof (struct mod_contabilidade));
    res->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++)
            res->total_global_fact[i][j] = 0;
    return res;
}


void insere_compra(Contabilidade cont, COMPRA comp) {
    NodoProdutos prod;
    preco_unit_t preco_uni  = get_preco_unit(comp);
    quantidade_t quant      = get_quantidade(comp);
    promo_t promo_ch        = get_promo(comp);
    cod_produto_t cod_prod  = get_cod_produto(comp);
    mes_t mes               = get_mes(comp);
    int promo = (promo_ch == 'N') ? NORMAL : PROMO;
    NodoProdutos nodo = criaNodo(cod_prod);
    
    if ((prod = (NodoProdutos) avl_find(cont->avl_produtos, nodo)) == NULL) {
        avl_insert(cont, nodo);
        prod = nodo;
    }
    prod->tabela_qtd_vendida[mes - 1][promo] += quant;
    prod->tabela_fact_produto[mes - 1][promo] += quant * preco_uni;
    cont->total_global_compras[mes - 1][promo]++;
    cont->total_global_fact[mes - 1][promo] += quant * preco_uni;
    return 0;
}


int existeCodigo(Contabilidade cont, char* cod_prod) {
    NodoProdutos res;
    NodoProdutos nodo_aux;
    nodo_aux = criaNodo(cod_prod);
    res = (NodoProdutos) avl_find(cont->avl_produtos, nodo_aux);
    free_nodo_produto(nodo_aux);
    
    return (res == NULL) ? 0 : 1;
}

int total_prods_comprados(Contabilidade cont){
    return avl_count(cont->avl_produtos);
}

/* 
 * VENDAS 
 */

int total_vendas_produto(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    int res=0, i=0;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);
    
    if(mes_inf>mes_sup) troca_meses(&mes_inf, &mes_sup);
    
    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        
        for(i=mes_inf; i<=mes_sup;i++){
            switch(campo){
                case CMP_VENDAS_NORMAIS: 
                    res += produto->tabela_qtd_vendida[i-1][NORMAL];
                    break;
                case CMP_VENDAS_PROMO: 
                    res += produto->tabela_qtd_vendida[i-1][PROMO];
                    break;
                case CMP_VENDAS_AMBOS: 
                    res += produto->tabela_qtd_vendida[i-1][NORMAL]+
                        produto->tabela_qtd_vendida[i-1][PROMO];
                    break;
                default:
                    break;
            }
        }
        
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
}

int total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto(cont, cod_prod, mes, mes,CMP_VENDAS_AMBOS);
}

int total_vendas_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto(cont, cod_prod, mes_inf, mes_sup,CMP_VENDAS_AMBOS);
}

int total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto(cont, cod_prod, mes, mes,CMP_VENDAS_NORMAIS);
}

int total_vendas_normais_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto(cont, cod_prod, mes_inf, mes_sup,CMP_VENDAS_NORMAIS);
}

int total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_vendas_produto(cont, cod_prod, mes, mes,CMP_VENDAS_PROMO);
}

int total_vendas_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_vendas_produto(cont, cod_prod, mes_inf, mes_sup,CMP_VENDAS_PROMO);
}

/* 
 * FACTURACAO 
 */

double total_fact_produto(Contabilidade cont, char* cod_prod,
                            int mes_inf, int mes_sup, campo_t campo) {
    double res=0, i=0;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);
    
    if(mes_inf>mes_sup) troca_meses(&mes_inf, &mes_sup);
    
    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        
        for(i=mes_inf; i<=mes_sup;i++){
            switch(campo){
                case CMP_FACT_NORMAL: 
                    res += produto->tabela_fact_produto[i-1][NORMAL];
                    break;
                case CMP_FACT_PROMO: 
                    res += produto->tabela_fact_produto[i-1][PROMO];
                    break;
                case CMP_FACT_AMBOS: 
                    res += produto->tabela_fact_produto[i-1][NORMAL]+
                        produto->tabela_fact_produto[i-1][PROMO];
                    break;
                default:
                    break;
            }
        }
        
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
}

double total_fact_produto_mes(Contabilidade cont,char* cod_prod, int mes) {
    return total_fact_produto(cont, cod_prod, mes, mes,CMP_FACT_AMBOS);
}

double total_fact_produto_int_meses(Contabilidade cont,char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto(cont, cod_prod, mes_inf, mes_sup,CMP_FACT_AMBOS);
}

double total_fact_normal_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_fact_produto(cont, cod_prod, mes, mes,CMP_FACT_NORMAL);
}

double total_fact_normal_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto(cont, cod_prod, mes_inf, mes_sup,CMP_FACT_NORMAL);
}

double total_fact_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    return total_fact_produto(cont, cod_prod, mes, mes,CMP_FACT_PROMO);
}

double total_fact_promo_produto_int_meses(Contabilidade cont, char* cod_prod, int mes_inf, int mes_sup) {
    return total_fact_produto(cont, cod_prod, mes_inf, mes_sup,CMP_FACT_PROMO);
}


int compras_total_mes(Contabilidade cont, int mes) {
    return (cont->total_global_compras[mes - 1][NORMAL] + cont->total_global_compras[mes - 1][PROMO]);
}

int total_fact_mes(Contabilidade cont, int mes) {
    return (cont->total_global_fact[mes - 1][NORMAL] + cont->total_global_fact[mes - 1][PROMO]);
}


ARRAY_DINAMICO lista_produtos_mais_comprados(Contabilidade cont) {
    NodoProdutos produto;
    ARRAY_DINAMICO lista = ad_inicializa_cap();
    TRAVERSER it = avl_t_alloc();
    avl_t_init(it, cont->avl_produtos);

    while ((produto = avl_t_next(it)) != NULL) {

    }

}

void free_contabilidade(Contabilidade cont) {
    avl_destroy(cont->avl_produtos, free_nodo_produto);
    free(cont);
}


/* 
 * FUNCOES AUXILIARES PRIVADAS AO MODULO 
 */

NodoProdutos criaNodo(char* cod_prod) {
    int i, j;
    NodoProdutos temp = (NodoProdutos) malloc(sizeof (struct produtos));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    temp->cod_produto = copia;
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++) {
            temp->tabela_fact_produto[i][j] = 0;
            temp->tabela_qtd_vendida[i][j] = 0;
        }

    return temp;
}

void troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}

void free_nodo_produto(NodoProdutos prod) {
    free(prod->cod_produto);
    free(prod);
}

