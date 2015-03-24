#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL 0
#define PROMO 1

struct mod_contabilidade{
        int total[12][2];
	ARVORE avl_nao_vendido;
        ARVORE avl_vendido;
};

struct nao_vendido{
    char *cod_produto;
};

struct vendido{
    char *cod_produto;
    int tabela_compras[12][4];
    /* Numero de compras por */
};

typedef struct nao_vendido *NodoNaovendido;
typedef struct vendido *NodoVendido;

Contabilidade inicializa_contabilidade(){
    Contabilidade res = (Contabilidade) malloc(sizeof(struct mod_contabilidade));
    res->avl_nao_vendido = avl_create(compara_nodo_produtos, NULL, NULL);
    res->avl_vendido = avl_create(compara_nodo_produtos, NULL, NULL);
    for(i=0;i<24;i++)
    res->total[i][i] = 0;
    return res;
}

void free_contabilidade(Contabilidade cont){
    avl_destroy(conta->avl_nao_vendido, free_nao_vendido);
    avl_destroy(conta->avl_vendido, free_vendido);
    free(cont);
}