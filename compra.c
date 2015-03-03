#include <stdio.h>
#include <stdlib.h>
#include "compra.h"

struct compra{
    char *cod_cliente;
    float preco_unit;
    unsigned short quantidade;
    char ispromo;
    char *cod_produto;
    unsigned char mes;
};

COMPRA *incializa_compra(){
    return (COMPRA *) malloc(sizeof(COMPRA));
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
    return comp->ispromo;
}

cod_produto_t get_cod_produto(COMPRA *comp){
    return comp->cod_produto;
}

mes_t get_mes(COMPRA *comp){
    return comp->mes;
}

/* Set's*/
void set_cod_cliente(COMPRA *comp, cod_cliente_t cod){
    comp->cod_cliente=cod;
}

void set_preco_unit(COMPRA *comp, preco_unit_t p_unit){
    comp->preco_unit=p_unit;
}

void set_quantidade(COMPRA *comp, quantidade_t qtd){
    comp->quantidade=qtd;
}

void set_promo(COMPRA *comp,promo_t promo){
    comp->ispromo=promo;
}

void set_cod_produto(COMPRA *comp,cod_produto_t prod){
    comp->cod_produto=prod;
}

void set_mes(COMPRA *comp, mes_t mes){
    comp->mes=mes;
}



