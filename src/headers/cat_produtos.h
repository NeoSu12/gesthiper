#ifndef CatProdutos_H
#define	CatProdutos_H

#include <stdbool.h>

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1
#define CAT_PRODUTOS_ELEMS_PAG 10

typedef struct catalogo_produtos *CatProdutos;
typedef struct iterador_cat_produtos *IT_CAT_PRODUTOS;
typedef struct cat_lista_produtos *CAT_LISTA_PRODUTOS;
typedef struct cat_paginador_lista_produtos *CAT_PAG_LISTA_PRODUTOS;


/*
 * CATALOGO PRODUTOS
 */

CatProdutos inicializa_catalogo_produtos(void);
void cat_insere_produto(CatProdutos cat, char *str);
void cat_remove_produto(CatProdutos cat, char *str);
void free_catalogo_produtos(CatProdutos cat);

/*
 * PESQUISA/CONSULTA INFORMAÇÃO GLOBAL
 */
bool cat_existe_produto(CatProdutos cat, char *elem);
int cat_total_produtos(CatProdutos cat);
int cat_total_produtos_letra(CatProdutos cat, char letra);

/*
 * LISTA PRODUTOS
 */
CAT_LISTA_PRODUTOS cat_lista_produtos_letra(CatProdutos catalogo_produtos, char letra);
char *cat_lista_prod_get_elemento(CAT_LISTA_PRODUTOS lista, int p);
int cat_lista_prod_get_num_elems(CAT_LISTA_PRODUTOS lista);
void cat_free_lista_produtos(CAT_LISTA_PRODUTOS lista);

/*
 * PAGINADOR LISTA DE PRODUTOS
 */
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_default(CAT_LISTA_PRODUTOS lista_prod);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_primeira_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_ultima_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_pag(CAT_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag);
void cat_prod_set_num_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag);
int cat_prod_goto_pag(CAT_PAG_LISTA_PRODUTOS pag, int num_pag);
int cat_prod_pag_get_num_elems(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_pag_get_num_elems_pag(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_get_pos_inicio_pag(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_get_num_pags(CAT_PAG_LISTA_PRODUTOS pag);
char *cat_prod_get_elemento_pag(CAT_PAG_LISTA_PRODUTOS pag, int n_elem);
int cat_prod_get_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_get_num_pag(CAT_PAG_LISTA_PRODUTOS pag);
void cat_prod_free_pag(CAT_PAG_LISTA_PRODUTOS pag);

/*
 * ITERADORES PRODUTOS
 */
IT_CAT_PRODUTOS inicializa_it_cat_produtos(CatProdutos cat);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_letra(CatProdutos cat, char c);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro(CatProdutos cat);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo(CatProdutos cat);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_elem(CatProdutos cat, char *st);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro_letra(CatProdutos cat, char c);
IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo_letra(CatProdutos cat, char c);
char *it_cat_produto_proximo(IT_CAT_PRODUTOS it);
char *it_cat_produto_actual(IT_CAT_PRODUTOS it);
char *it_cat_produto_anterior(IT_CAT_PRODUTOS it);
char *it_cat_produto_proximo_letra(IT_CAT_PRODUTOS it);
char *it_cat_produto_anterior_letra(IT_CAT_PRODUTOS it);
void free_it_cat_produto(IT_CAT_PRODUTOS it);


#endif	/* CatProdutos_H */

