#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/compra.h"


struct compra{
    cod_cliente_t cod_cliente;
    preco_unit_t preco_unit;
    quantidade_t quantidade;
    promo_t promo;
    cod_produto_t cod_produto;
    mes_t mes;
};


COMPRA inicializa_compra(){
    COMPRA compra = (COMPRA) malloc(sizeof(struct compra));
    return compra;
}

COMPRA inicializa_compra_completo(cod_cliente_t cod_cli, cod_produto_t cod_produto,
                            preco_unit_t preco_unit, quantidade_t qtd, promo_t promo, mes_t mes){
    COMPRA compra = (COMPRA) malloc(sizeof(struct compra));
    compra->cod_cliente = cod_cli;
    compra->cod_produto = cod_produto;
    compra->preco_unit = preco_unit;
    compra->mes = mes;
    compra->quantidade = qtd;
    compra->promo = promo;
    return compra;
}

void actualiza_compra(COMPRA compra, cod_cliente_t cod_cli, cod_produto_t cod_produto, preco_unit_t preco_unit, quantidade_t qtd, promo_t promo, mes_t mes){
    compra->cod_cliente = cod_cli;
    compra->cod_produto = cod_produto;
    compra->preco_unit = preco_unit;
    compra->mes = mes;
    compra->quantidade = qtd;
    compra->promo = promo;
}

void free_compra(COMPRA compra){
    free(compra);
}


/*Get's*/
cod_cliente_t get_cod_cliente(COMPRA comp){
    return comp->cod_cliente;
}

preco_unit_t get_preco_unit(COMPRA comp){
    return comp->preco_unit;
}

quantidade_t get_quantidade(COMPRA comp){
    return comp->quantidade;
}

promo_t get_promo(COMPRA comp){
    return comp->promo;
}

cod_produto_t get_cod_produto(COMPRA comp){
    return comp->cod_produto;
}

mes_t get_mes(COMPRA comp){
    return comp->mes;
}

/* Set's*/
void set_cod_cliente(COMPRA comp, cod_cliente_t cod){
    comp->cod_cliente=cod;
}

void set_preco_unit(COMPRA comp, preco_unit_t p_unit){
    comp->preco_unit=p_unit;
}

void set_quantidade(COMPRA comp, quantidade_t qtd){
    comp->quantidade=qtd;
}

void set_promo(COMPRA comp,promo_t promo){
    comp->promo=promo;
}

void set_cod_produto(COMPRA comp,cod_produto_t prod){
    comp->cod_produto=prod;
}

void set_mes(COMPRA comp, mes_t mes){
    comp->mes=mes;
}



