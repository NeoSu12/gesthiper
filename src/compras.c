#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/avl.h"

#define NORMAL 0
#define PROMO 1

struct modulo_compras{
	ARVORE avl_clientes;
};

struct nodo_avl_clientes{
	char *cod_cliente;
	ARVORE avl_produtos;
	int num_prods_comprados[12];
	char *top3[3];
	int num_vendas_top3[3];
};

struct nodo_avl_produtos{
	char *cod_produto;
	/* Numero de compras para cada mes,
         * distinguindo entre normal e promocao.*/
	int num_compras[12][2];
};

typedef struct nodo_avl_produtos *NodoProduto;
typedef struct nodo_avl_clientes *NodoCliente;

int compara_nodo_clientes(const void *,const void *, void *);
int compara_nodo_produtos(const void *,const void *, void *);

Compras inicializa_compras(){
    Compras res = (Compras) malloc(sizeof(struct modulo_compras));
    res->avl_clientes = avl_create(compara_nodo_clientes, NULL, NULL);
    return res;
}

void insere_compra(Compras compras, COMPRA compra){
    char *novo_cliente, *novo_produto;
    int i=0, j=0;
    NodoCliente cliente;
    NodoProduto produto;
    
    cliente = (NodoCliente) avl_find(compras->avl_clientes, compra->cod_cliente);
    
    if(cliente == NULL){
        cliente = (NodoCliente) malloc(sizeof(struct nodo_avl_clientes));
        
        strcpy(novo_cliente,compra->cod_cliente);
        cliente->cod_cliente = novo_cliente;
        cliente->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
        
        for(i=0;i<12;i++) cliente->num_prods_comprados[i]=0;
        for(i=0;i<3;i++) cliente->num_vendas_top3[i]=0;
        
        avl_insert(compras->avl_clientes,cliente);
    }
    
    cliente->num_prods_comprados[compra->mes - 1] += compra->quantidade;
    produto = (NodoProduto) avl_find(cliente->avl_produtos, compra->cod_produto);
    
    if(produto == NULL){
        produto = (NodoProduto) malloc(sizeof(struct nodo_avl_produtos));
        strcpy(novo_produto,compra->cod_produto);
        produto->cod_produto = novo_produto;
        
        for(i=0;i<12;i++){
            for(j=0;j<2;j++) produto->num_compras[i][j] = 0;
        }
        
        avl_insert(cliente->avl_produtos,produto);
    }
    
    if(compra->promo == 'N') 
        produto->num_compras[NORMAL][compra->mes - 1] += compra->quantidade;
    
    if(compra->promo == 'P') 
        produto->num_compras[PROMO][compra->mes - 1] += compra->quantidade;
    
    
}


int compara_nodo_clientes(const void *avl_a,const void *avl_b, void *param){
    NodoCliente item_a = (NodoCliente) avl_a;
    NodoCliente item_b = (NodoCliente) avl_b;
    return strcmp(item_a->cod_cliente, item_b->cod_cliente);
}

int compara_nodo_produtos(const void *avl_a,const void *avl_b, void *param){
    NodoProduto item_a = (NodoProduto) avl_a;
    NodoProduto item_b = (NodoProduto) avl_b;
    return strcmp(item_a->cod_produto, item_b->cod_produto);
}



