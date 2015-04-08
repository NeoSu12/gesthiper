#ifndef CatProdutos_H
#define	CatProdutos_H

#define PAGINA_POSSIVEL 0
#define PAGINA_IMPOSSIVEL -1
#define CAT_PRODUTOS_ELEMS_PAG 10

typedef struct catalogo_produtos *CatProdutos;
typedef struct iterador_cat_produtos *IT_CAT_PRODUTOS;
typedef struct cat_lista_produtos *CAT_LISTA_PRODUTOS;
typedef cat_paginador_lista_produtos *CAT_PAG_LISTA_PRODUTOS;


/* CatProdutos */

CatProdutos inicializa_catalogo_produtos();
int cat_existe_produto(CatProdutos, char *);
char *cat_procura_produto(CatProdutos, char *);
void cat_insere_produto(CatProdutos, char *);
void cat_remove_produto(CatProdutos, char *);
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

CAT_LISTA_PRODUTOS cat_lista_produtos_letra(CatClientes catalogo_produtos, char letra);
char *cat_lista_prod_get_elemento(CAT_LISTA_PRODUTOS lista,int p);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_default(CAT_LISTA_PRODUTOS lista_prod);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_primeira_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_ultima_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag);
CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_pag(CAT_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag);
void cat_prod_goto_pag(CAT_PAG_LISTA_PRODUTOS pag, int num_pag);
int cat_prod_get_pos_inicio_pag(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_get_num_pags(CAT_PAG_LISTA_PRODUTOS pag);
char *cat_prod_get_elemento_pag(CAT_PAG_LISTA_PRODUTOS pag, int n_elem);
void cat_prod_set_num_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag);
int cat_prod_get_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag);
int cat_prod_get_num_pag(CAT_PAG_LISTA_PRODUTOS pag);
void cat_prod_free_pag(CAT_PAG_LISTA_PRODUTOS pag);
void cat_free_lista_produtos(CAT_LISTA_PRODUTOS lista);


#endif	/* CatProdutos_H */

