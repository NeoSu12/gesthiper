#ifndef COMPRA_H
#define	COMPRA_H

typedef struct compra COMPRA;

typedef char * cod_cliente_t;
typedef float preco_unit_t;
typedef unsigned short quantidade_t;
typedef char promo_t;
typedef char * cod_produto_t;
typedef unsigned short mes_t;

COMPRA *incializa_compra();

/*Get's*/
cod_cliente_t get_cod_cliente(COMPRA *);
preco_unit_t get_preco_unit(COMPRA *);
quantidade_t get_quantidade(COMPRA *);
promo_t get_promo(COMPRA *);
cod_produto_t get_cod_produto(COMPRA *);
mes_t get_mes(COMPRA *);

/* Set's*/
void set_cod_cliente(COMPRA *, cod_cliente_t);
void set_preco_unit(COMPRA *, preco_unit_t);
void set_quantidade(COMPRA *, quantidade_t);
void set_promo(COMPRA *,promo_t);
void set_cod_produto(COMPRA *,cod_produto_t);
void set_mes(COMPRA *, mes_t);



#endif	/* COMPRA_H */

