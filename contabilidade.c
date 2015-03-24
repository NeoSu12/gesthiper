#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL 0
#define PROMO 1

struct mod_contabilidade{
        int total_fact[12][2];
        ARVORE avl_produtos;
};



struct produtos{
    char *cod_produto;
    int tabela_quantidade[12][2];
    /* Numero de quantidade dum produto num determinado mes */
    int tabela_fact_produto[12][2];
    /* Facturacao por mes */
    
};

typedef struct produtos *NodoProdutos;

Contabilidade inicializa_contabilidade(){
    Contabilidade res = (Contabilidade) malloc(sizeof(struct mod_contabilidade));
    res->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
    for(i=0;i<12;i++)
        for(j=0;j<2;j++)
            res->total_fact[i][j] = 0;
    return res;
}

void inserir_produto(Contabilidade cont, COMPRA comp){
    if(existeCodigo(cont,comp->cod_produto)) {
        return 0;
    }else{
        avl_insert(cont,comp->cod_produto);
    }
    return 0;
}

/*querie 3 - dado um cod e um mes, saber o total facturado e total vendas*/
int total_fact_mes(char* cod, int mes){
    return 0;
}

int total_vendas_mes(char* cod, int mes){
    return 0;
}

/*querie 4 - Devolver lista e numeros de produtos com 0 vendas*/

/*querie 7 - Dado um intervalo de meses, devolver o numero de compras e total facturado*/

/*querie 11 - Ficheiro em formato CSV, com mes | compras | clientes*/

/*querie 12 - Devolver uma lista dos N produtos mais vendidos*/

/*querie 14 - clientes zombie*/



int existeNodo(Contabilidade cont,NodoProdutos x){
    return (avl_find(cont->avl_produtos,x)==NULL)?0:1;
}

int existeCodigo(Contabilidade cont, char* cod){
    char *copia =(char*)malloc(sizeof(char)*(strlen(cod)+1));
    NodoProdutos temp = (NodoProdutos)malloc(sizeof(struct produtos));
    strcpy(copia,cod);
    temp->cod_produto=copia;
    val=existeNodo(cont,copia);
    free_nodo_produto(temp);
    return val;
}

void free_nodo_produto(NodoProdutos prod){
    free(prod->cod_produto);
    free(prod);
}

void free_contabilidade(Contabilidade cont){
    avl_destroy(cont->avl_produtos, free_produtos);
    free(cont);
}