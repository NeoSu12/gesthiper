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
    int tabela_qtd_vendida[12][2];
    /* Quantidade dum produto vendida num determinado mes 0-Normal / 1- Promo */
    int tabela_fact_produto[12][2];
    /* Facturacao por mes */

};


typedef struct produtos *NodoProdutos;
/*funcoes locais*/
NodoProdutos criaNodo(char*);



Contabilidade inicializa_contabilidade() {
    int i, j;
    Contabilidade res = (Contabilidade) malloc(sizeof (struct mod_contabilidade));
    res->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++)
            res->total_global_fact[i][j] = 0;
    return res;
}


void inserir_produto(Contabilidade cont, COMPRA comp) {
    NodoProdutos prod;
    cod_cliente_t cod_cli   = get_cod_cliente(comp);
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


/*querie 3 - dado um cod e um mes, saber o total facturado e total vendas*/
int total_vendas_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    int res;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);

    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        res = produto->tabela_qtd_vendida[mes - 1][NORMAL] +
                produto->tabela_qtd_vendida[mes - 1][PROMO];
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
}

int total_vendas_normais_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    int res;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);

    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        res = produto->tabela_qtd_vendida[mes - 1][NORMAL];
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
}

int total_vendas_promo_produto_mes(Contabilidade cont, char* cod_prod, int mes) {
    int res;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);

    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        res = produto->tabela_qtd_vendida[mes - 1][PROMO];
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
}

int total_fact_produto_mes(Contabilidade cont, int mes, char* cod_prod) {
    int res;
    NodoProdutos produto;
    NodoProdutos nodo_pesquisa = criaNodo(cod_prod);

    produto = (NodoProdutos) avl_find(cont->avl_produtos, nodo_pesquisa);

    if (produto != NULL) {
        res = produto->tabela_fact_produto[mes - 1][NORMAL] +
                produto->tabela_fact_produto[mes - 1][PROMO];
    } else {
        res = PROD_NAO_VENDIDO;
    }

    free_nodo_produto(nodo_pesquisa);
    return res;
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

void free_nodo_produto(NodoProdutos prod) {
    free(prod->cod_produto);
    free(prod);
}

void free_contabilidade(Contabilidade cont) {
    avl_destroy(cont->avl_produtos, free_nodo_produto);
    free(cont);
}