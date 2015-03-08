#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compra.h"

#define NBYTES_CLIENTE sizeof(char)*(5+1)
#define NBYTES_PRODUTO sizeof(char)*(6+1)

struct compra{
    cod_cliente_t cod_cliente;
    preco_unit_t preco_unit;
    quantidade_t quantidade;
    promo_t promo;
    cod_produto_t cod_produto;
    mes_t mes;
};

COMPRA *inicializa_compra(){
    COMPRA *compra = (COMPRA *) malloc(sizeof(COMPRA));
    compra->cod_cliente = (cod_cliente_t) malloc(NBYTES_CLIENTE);
    compra->cod_produto = (cod_produto_t) malloc(NBYTES_PRODUTO);
    return compra;
}

COMPRA *inicializa_compra_completo(cod_cliente_t cod_cliente,preco_unit_t preco_unit,
    quantidade_t quantidade, promo_t promo, cod_produto_t cod_produto, mes_t mes){
    
    COMPRA *compra = (COMPRA *) malloc(sizeof(COMPRA));
    compra->cod_cliente = (cod_cliente_t) malloc(NBYTES_CLIENTE);
    compra->cod_produto = (cod_produto_t) malloc(NBYTES_PRODUTO);
    strncpy(compra->cod_cliente,cod_cliente,NBYTES_CLIENTE);
    strncpy(compra->cod_produto,cod_produto,NBYTES_PRODUTO);
    compra->preco_unit= preco_unit;
    compra->quantidade= quantidade;
    compra->promo = promo;
    compra->mes=mes;
    
    return compra;
}

COMPRA *clone_compra(COMPRA *compra){
    COMPRA *compra_clone = (COMPRA *) malloc(sizeof(COMPRA));
    
    compra_clone->cod_cliente = (cod_cliente_t) malloc(NBYTES_CLIENTE);
    compra_clone->cod_produto = (cod_produto_t) malloc(NBYTES_PRODUTO);
    strncpy(compra_clone->cod_cliente,compra->cod_cliente,NBYTES_CLIENTE);
    strncpy(compra_clone->cod_produto,compra->cod_produto,NBYTES_PRODUTO);
    compra_clone->preco_unit=compra->preco_unit;
    compra_clone->quantidade=compra->quantidade;
    compra_clone->promo=compra->promo;
    compra_clone->mes=compra->mes;
    
    return compra_clone;
}

int equals_compra(COMPRA *compra1, COMPRA *compra2){
    
    if ((strncmp(compra1->cod_cliente,compra2->cod_cliente,NBYTES_CLIENTE))&&
            (strncmp(compra1->cod_produto,compra2->cod_produto,NBYTES_PRODUTO))&&
            (compra1->preco_unit==compra2->preco_unit)&&
            (compra1->quantidade==compra2->quantidade)&&
            (compra1->promo==compra2->promo)&&
            (compra1->mes==compra2->mes)) return 1;
    else return 0;
}

void free_compra(COMPRA *compra){
    free(compra->cod_cliente);
    free(compra->cod_produto);
    free(compra);
}

/*Get's*/
cod_cliente_t get_cod_cliente(COMPRA *comp){
    return comp->cod_cliente;
}

preco_unit_t get_preco_unit(COMPRA *comp){
    return comp->preco_unit;
}

quantidade_t get_quantidade(COMPRA *comp){
    return comp->quantidade;
}

promo_t get_promo(COMPRA *comp){
    return comp->promo;
}

cod_produto_t get_cod_produto(COMPRA *comp){
    return comp->cod_produto;
}

mes_t get_mes(COMPRA *comp){
    return comp->mes;
}

/* Set's*/
void set_cod_cliente(COMPRA *comp, cod_cliente_t cod){
    strncpy(comp->cod_cliente,cod,NBYTES_CLIENTE);
}

void set_preco_unit(COMPRA *comp, preco_unit_t p_unit){
    comp->preco_unit=p_unit;
}

void set_quantidade(COMPRA *comp, quantidade_t qtd){
    comp->quantidade=qtd;
}

void set_promo(COMPRA *comp,promo_t promo){
    comp->promo=promo;
}

void set_cod_produto(COMPRA *comp,cod_produto_t prod){
    strncpy(comp->cod_produto,prod,NBYTES_PRODUTO);
}

void set_mes(COMPRA *comp, mes_t mes){
    comp->mes=mes;
}



