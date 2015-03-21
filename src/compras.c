#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

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
void free_nodo_produto(void *, void *);
void free_nodo_cliente(void *, void *);

int compara_nodo_clientes(void *, void *);
int compara_nodo_produtos(void *, void *);


Compras inicializa_compras(){
    Compras res = (Compras) malloc(sizeof(struct modulo_compras));
    res->avl_clientes = avl_create(compara_nodo_clientes, NULL, NULL);
    return res;
}

void free_compras(Compras compras){
    avl_destroy(compras->avl_clientes, free_nodo_cliente);
    free(compras);
}

void insere_compra(Compras compras, COMPRA compra){
    int i=0, j=0;
    char *novo_cliente=NULL, *novo_produto=NULL;
    NodoCliente cliente;
    NodoProduto produto;
    
    cliente = (NodoCliente) avl_find(compras->avl_clientes, get_cod_cliente(compra));
    
    if(cliente == NULL){
        cliente = (NodoCliente) malloc(sizeof(struct nodo_avl_clientes));
        
        strcpy(novo_cliente,get_cod_cliente(compra));
        cliente->cod_cliente = novo_cliente;
        cliente->avl_produtos = avl_create(compara_nodo_produtos, NULL, NULL);
        
        for(i=0;i<12;i++) cliente->num_prods_comprados[i]=0;
        for(i=0;i<3;i++) cliente->num_vendas_top3[i]=0;
        
        avl_insert(compras->avl_clientes,cliente);
    }
    
    cliente->num_prods_comprados[get_mes(compra) - 1] += get_quantidade(compra);
    produto = (NodoProduto) avl_find(cliente->avl_produtos, get_cod_produto(compra));
    
    if(produto == NULL){
        produto = (NodoProduto) malloc(sizeof(struct nodo_avl_produtos));
        strcpy(novo_produto,get_cod_produto(compra));
        produto->cod_produto = novo_produto;
        
        for(i=0;i<12;i++){
            for(j=0;j<2;j++) produto->num_compras[i][j] = 0;
        }
        
        avl_insert(cliente->avl_produtos,produto);
    }
    
    if(get_promo(compra) == 'N') 
        produto->num_compras[get_mes(compra) - 1][NORMAL] += get_quantidade(compra);
    
    if(get_promo(compra) == 'P') 
        produto->num_compras[get_mes(compra) - 1][PROMO] += get_quantidade(compra);
    
    
}

int num_produtos_comprados_cliente(Compras compras, char *cod_cliente){
    NodoCliente cliente_procura, cliente;
    int resultado=0, i=0;
    
    cliente_procura = (NodoCliente) malloc(sizeof(struct nodo_avl_clientes));
    cliente_procura->cod_cliente = cod_cliente;
    
    cliente = (NodoCliente) avl_find(compras->avl_clientes,cliente_procura);
    
    if(cliente==NULL) resultado = -1;
    else{
        for(i=0;i<12;i++) 
            resultado += cliente->num_prods_comprados[i];
    }
    
    return resultado;
}

int num_produtos_comprados_cliente_mes(Compras compras, char *cod_cliente, char mes){
    int resultado;
    NodoCliente cliente_procura, cliente;
    
    cliente_procura = (NodoCliente) malloc(sizeof(struct nodo_avl_clientes));
    cliente_procura->cod_cliente = cod_cliente;
    
    cliente = (NodoCliente) avl_find(compras->avl_clientes,cliente_procura);
    
    if(cliente==NULL) resultado = -1;
    else resultado = cliente->num_prods_comprados[mes-1];
    
    return resultado;
}

ARRAY_DINAMICO clientes_compraram_prod(Compras compras, char *cod_cliente){
    NodoCliente cliente;
    char *cliente_copia;
    int tamanho_cliente;
    ARRAY_DINAMICO lista_clientes = ad_inicializa(100);
    TRAVERSER it = avl_t_alloc();
    
    avl_t_init(it, compras->avl_clientes);
    
    while((cliente = (NodoCliente) avl_t_next(it)) != NULL){
        if(strcmp(cliente->cod_cliente,cod_cliente)==0){
            tamanho_cliente = strlen(cod_cliente) + 1;
            cliente_copia = (char *) malloc(sizeof(char)*tamanho_cliente)
            strcpy(cliente_copia,cod_cliente);
            ad_insere_elemento(lista_clientes,cliente_copia);
        }
    }
    
    avl_t_free(it);
    return lista_clientes;
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

void free_nodo_cliente(void *avl_item, void *avl_param){
    NodoCliente cliente = (NodoCliente) avl_item;
    avl_destroy(cliente->avl_produtos,free_nodo_produto);
    free(cliente->cod_cliente);
}

void free_nodo_produto(void *avl_item, void *avl_param){
    NodoProduto produto = (NodoProduto) avl_item;
    free(produto->cod_produto);
}



int compara_nodo_clientes(void *item_a, void *item_b){
    NodoCliente c_a = (NodoCliente) item_a;
    NodoCliente c_b = (NodoCliente) item_b;
    return strcmp((char *)c_a->cod_cliente, (char *)c_b->cod_cliente);
}

int compara_nodo_produtos(void *item_a, void *item_b){
    NodoProduto c_a = (NodoProduto) item_a;
    NodoProduto c_b = (NodoProduto) item_b;
    return strcmp((char *)c_a->cod_produto, (char *)c_b->cod_produto);
}
