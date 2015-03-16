#ifndef CatProdutos_H
#define	CatProdutos_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1

typedef struct catalogo_produtos* CatProdutos;
typedef struct iterador_produtos* IT_PRODUTOS;
typedef struct paginacao_produtos* PagProdutos;

/* CatProdutos */

CatProdutos inicializa_catalogo_produtos();
int existe_produto(CatProdutos, char *);
char *procura_produto(CatProdutos, char *);
char *insere_produto(CatProdutos, char *);
char *remove_produto(CatProdutos, char *);
int total_produtos(CatProdutos);
int total_produtos_letra(CatProdutos, char);
void free_catalogo_produtos(CatProdutos);

/* IT_PRODUTOS */

IT_PRODUTOS inicializa_it_produtos_inicio(CatProdutos);
IT_PRODUTOS inicializa_it_produtos_fim(CatProdutos);
IT_PRODUTOS inicializa_it_produtos_elem(CatProdutos, char *);
IT_PRODUTOS inicializa_it_produtos_inicio_letra(CatProdutos, char);
IT_PRODUTOS inicializa_it_produtos_fim_letra(CatProdutos, char);
int itera_n_produtos_proximos(IT_PRODUTOS, char *[], int);
int itera_n_produtos_anteriores(IT_PRODUTOS, char *[], int);
char *it_produto_proximo(IT_PRODUTOS);
char *it_produto_actual(IT_PRODUTOS);
char *it_produto_anterior(IT_PRODUTOS);
char *it_produto_proximo_letra(IT_PRODUTOS); 
char *it_produto_anterior_letra(IT_PRODUTOS);

/* PAGINAÇÃO */

PagProdutos inicializa_pag_produtos(CatProdutos cat, int tam_pag);
PagProdutos inicializa_pag_produtos_letra(CatProdutos cat, int tam_pag, char letra);
int pag_produtos_goto_pag(PagProdutos pag_produtos ,int n_pagina, char *pagina[]);


#endif	/* CatProdutos_H */