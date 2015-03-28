#ifndef CatProdutos_H
#define	CatProdutos_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1

typedef struct catalogo_produtos* CatProdutos;
typedef struct iterador_cat_produtos* IT_CAT_PRODUTOS;
typedef struct cat_lista_produtos* CAT_LISTA_PRODUTOS;

/* CatProdutos */

CatProdutos inicializa_catalogo_produtos();
int cat_existe_produto(CatProdutos, char *);
char *cat_procura_produto(CatProdutos, char *);
char *cat_insere_produto(CatProdutos, char *);
char *cat_remove_produto(CatProdutos, char *);
int cat_total_produtos(CatProdutos);
int cat_total_produtos_letra(CatProdutos, char);
void free_catalogo_produtos(CatProdutos);

/* IT_CAT_PRODUTOS */

IT_CAT_PRODUTOS inicializa_it_cat_produtos(CatProdutos cat);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_letra(CatProdutos cat, char);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro(CatProdutos);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo(CatProdutos);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro_letra(CatProdutos, char);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo_letra(CatProdutos, char);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_elem(CatProdutos, char *);

void free_it_cat_produto(IT_CAT_PRODUTOS);
char *it_cat_produto_actual(IT_CAT_PRODUTOS);
char *it_cat_produto_proximo(IT_CAT_PRODUTOS);
char *it_cat_produto_anterior(IT_CAT_PRODUTOS);
char *it_cat_produto_proximo_letra(IT_CAT_PRODUTOS); 
char *it_cat_produto_anterior_letra(IT_CAT_PRODUTOS);

/*
 * LISTA PRODUTOS
 */

CAT_LISTA_PRODUTOS cat_lista_produtos_letra(CatProdutos, char, int);
char *cat_lista_prod_get_elemento(CAT_LISTA_PRODUTOS,int p);
int cat_lista_prod_get_pos_and_num_elems_pag(CAT_LISTA_PRODUTOS, int *, int);
int cat_lista_prod_get_num_pags(CAT_LISTA_PRODUTOS);
int cat_lista_prod_get_elems_por_pag(CAT_LISTA_PRODUTOS);
int cat_lista_prod_muda_elems_por_pag(CAT_LISTA_PRODUTOS, int);
int cat_lista_prod_get_num_elems(CAT_LISTA_PRODUTOS);
void cat_free_lista_produtos(CAT_LISTA_PRODUTOS);

#endif	/* CatProdutos_H */

