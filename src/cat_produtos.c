#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "headers/cat_produtos.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define APENAS_LETRA 1
#define TODO_CATALOGO 2


struct catalogo_produtos {
    ARVORE indices[27];
};

struct iterador_cat_produtos {
    TRAVERSER traverser;
    CatProdutos catalogo;
    int indice;
};

struct cat_lista_produtos{
    ARRAY_DINAMICO lista_paginada;
};

struct cat_paginador_lista_produtos{
    CAT_LISTA_PRODUTOS lista_pag;
    AD_PAGINADOR paginador;
};

/*
 * FUNÇÕES PRIVADAS AO MÓDULO
 */

int cat_compara_produtos_avl(const void *avl_a, const void *avl_b, void *avl_param);
void cat_free_produto_avl(void *item, void *param);
void cat_free_produto_ad(void *item);
int cat_calcula_indice_produto(char l);

/*
 * CATALOGO PRODUTOS
 */

CatProdutos inicializa_catalogo_produtos() {
    int i = 0;
    CatProdutos res = (CatProdutos) malloc(sizeof (struct catalogo_produtos));

    for (i = 0; i <= 26; i++) {
        res->indices[i] = avl_create(cat_compara_produtos_avl, NULL, NULL);
    }

    return res;
}

void cat_insere_produto(CatProdutos cat, char *str) {
    int ind = cat_calcula_indice_produto(str[0]);
    int tamanho = strlen(str);
    char *new = (char *) malloc(tamanho + 1);

    strncpy(new, str, tamanho + 1);
    avl_insert(cat->indices[ind], new);
    
}

void cat_remove_produto(CatProdutos cat, char *str) {
    int ind = cat_calcula_indice_produto(str[0]);
    free(avl_delete(cat->indices[ind], str));
}

void free_catalogo_produtos(CatProdutos cat) {
    int i = 0;
    
    if(cat != NULL){
    for (i = 0; i <= 26; i++) {
        avl_destroy(cat->indices[i], cat_free_produto_avl);
    }
}
    free(cat);
}

/*
 * PESQUISA/CONSULTA INFORMAÇÃO GLOBAL
 */

bool cat_existe_produto(CatProdutos cat, char *elem) {
    bool res = false;
    int ind;

    if (elem != NULL) {
        ind = cat_calcula_indice_produto(*elem);
        if (avl_find(cat->indices[ind], elem) != NULL) res = true;
        else res = false;
    }

    return res;
}

int cat_total_produtos(CatProdutos cat) {
    size_t soma = 0;
    int i;

    for (i = 0; i <= 26; i++)
        soma += avl_count(cat->indices[i]);

    return soma;
}

int cat_total_produtos_letra(CatProdutos cat, char letra) {
    int ind = cat_calcula_indice_produto(letra);
    return avl_count(cat->indices[ind]);
}


/*
 * LISTA PRODUTOS
 */

CAT_LISTA_PRODUTOS cat_lista_produtos_letra(CatProdutos catalogo_produtos, char letra){
    char *produto;
    CAT_LISTA_PRODUTOS pag = (CAT_LISTA_PRODUTOS) malloc(sizeof(struct cat_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa(8000);
    IT_CAT_PRODUTOS it = inicializa_it_cat_produtos_letra(catalogo_produtos, letra);

    while ((produto = it_cat_produto_proximo_letra(it)) != NULL) {
        ad_insere_elemento(ad, produto);
    }
    
    pag->lista_paginada = ad;
    free_it_cat_produto(it);
    return pag;
}

char *cat_lista_prod_get_elemento(CAT_LISTA_PRODUTOS lista,int p){
    return (char *) ad_get_elemento(lista->lista_paginada, p);
}

int cat_lista_prod_get_num_elems(CAT_LISTA_PRODUTOS lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void cat_free_lista_produtos(CAT_LISTA_PRODUTOS lista){
    if(lista != NULL)
        ad_deep_free(lista->lista_paginada, cat_free_produto_ad);
    
    free(lista);
}


/*
 * PAGINADOR LISTA DE PRODUTOS
 */


CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_default(CAT_LISTA_PRODUTOS lista_prod) {
    CAT_PAG_LISTA_PRODUTOS pag_res = (CAT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cat_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_default(pag_res->lista_pag->lista_paginada);
    return pag_res;
}

CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_primeira_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    CAT_PAG_LISTA_PRODUTOS pag_res = (CAT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cat_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_primeira_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_ultima_pag(CAT_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    CAT_PAG_LISTA_PRODUTOS pag_res = (CAT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cat_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_ultima_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

CAT_PAG_LISTA_PRODUTOS cat_prod_inicializa_paginador_pag(CAT_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag) {
    CAT_PAG_LISTA_PRODUTOS pag_res = (CAT_PAG_LISTA_PRODUTOS) malloc(sizeof (struct cat_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_pag(pag_res->lista_pag->lista_paginada,n_pag, elems_por_pag);
    return pag_res;
}

void cat_prod_set_num_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag){
    ad_set_num_elems_por_pag(pag->paginador, new_elems_por_pag);
}

int cat_prod_goto_pag(CAT_PAG_LISTA_PRODUTOS pag, int num_pag){
    return ad_goto_pag(pag->paginador, num_pag);
}

int cat_prod_pag_get_num_elems(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_tamanho(pag->lista_pag->lista_paginada);
}

int cat_prod_pag_get_num_elems_pag(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_elems_pag(pag->paginador);
}

int cat_prod_get_pos_inicio_pag(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_pos_inicio_pag(pag->paginador);
}

int cat_prod_get_num_pags(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pags(pag->paginador);
}

char *cat_prod_get_elemento_pag(CAT_PAG_LISTA_PRODUTOS pag, int n_elem){
    return (char *) ad_get_elemento_pag(pag->paginador, n_elem);
}

int cat_prod_get_elems_por_pag(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_elems_por_pag(pag->paginador);
}

int cat_prod_get_num_pag(CAT_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pag(pag->paginador);
}

void cat_prod_free_pag(CAT_PAG_LISTA_PRODUTOS pag){
    if(pag != NULL)
        ad_free_pag(pag->paginador);
    
    free(pag);
}


/*
 * ITERADORES PRODUTOS
 */

IT_CAT_PRODUTOS inicializa_it_cat_produtos(CatProdutos cat) {
    IT_CAT_PRODUTOS it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_letra(CatProdutos cat, char c) {
    int indice;
    IT_CAT_PRODUTOS it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_produto(toupper(c));
    avl_t_init(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro(CatProdutos cat) {
    IT_CAT_PRODUTOS it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo(CatProdutos cat) {
    IT_CAT_PRODUTOS it;
    it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, cat->indices[26]);
    it->indice = 26;
    it->catalogo = cat;
    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_elem(CatProdutos cat, char *st) {
    int indice;
    IT_CAT_PRODUTOS it;

    if (st != NULL) {
        it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
        it->traverser = avl_t_alloc();
        it->catalogo = cat;
        indice = cat_calcula_indice_produto(toupper(*st));
        avl_t_find(it->traverser, cat->indices[indice], st);
        it->indice = indice;
    } else {
        it = NULL;
    }

    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_primeiro_letra(CatProdutos cat, char c) {
    int indice;
    IT_CAT_PRODUTOS it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_produto(toupper(c));
    avl_t_first(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_CAT_PRODUTOS inicializa_it_cat_produtos_ultimo_letra(CatProdutos cat, char c) {
    int indice;
    IT_CAT_PRODUTOS it = (IT_CAT_PRODUTOS) malloc(sizeof (struct iterador_cat_produtos));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_produto(toupper(c));
    avl_t_last(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

char *it_cat_produto_proximo(IT_CAT_PRODUTOS it) {
    int tamanho;
    int sair = 0;
    char *res = NULL;
    char *ret = NULL;

    while (res == NULL && sair == 0) {
        res = avl_t_next(it->traverser);
        if (res != NULL || (res == NULL && it->indice >= 26)) {
            sair = 1;
        } else {
            /* res == NULL && it->indice<26 */
            it->indice++;
            res = avl_t_first(it->traverser, it->catalogo->indices[it->indice]);
        }
    }

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }
    return ret;
}

char *it_cat_produto_actual(IT_CAT_PRODUTOS it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_cur(it->traverser);

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }

    return ret;
}

char *it_cat_produto_anterior(IT_CAT_PRODUTOS it) {
    int tamanho;
    int sair = 0;
    char *res = NULL;
    char *ret = NULL;

    while (res == NULL && sair == 0) {
        res = avl_t_prev(it->traverser);
        if (res != NULL || (res == NULL && it->indice <= 0)) {
            sair = 1;
        } else {
            /* res == NULL && it->indice>=0 */
            it->indice--;
            res = avl_t_last(it->traverser, it->catalogo->indices[it->indice]);
        }
    }

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }
    return ret;
}

char *it_cat_produto_proximo_letra(IT_CAT_PRODUTOS it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_next(it->traverser);
    
    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }

    return ret;
}

char *it_cat_produto_anterior_letra(IT_CAT_PRODUTOS it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_prev(it->traverser);

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }

    return ret;
}

void free_it_cat_produto(IT_CAT_PRODUTOS it){
    if(it != NULL)
        avl_t_free(it->traverser);
    
    free(it);
}


/*
 * Funções (privadas) auxiliares ao módulo.
 */

int cat_compara_produtos_avl(const void *avl_a, const void *avl_b, void *avl_param) {
    return strcmp((char *) avl_a, (char *) avl_b);
}

void cat_free_produto_avl(void *item, void *param) {
    free(item);
}

void cat_free_produto_ad(void *item) {
    free(item);
}

int cat_calcula_indice_produto(char l) {
    int res = 0;
    char letra = toupper(l);

    if (isalpha(letra)) {
        res = letra - 'A';
    } else {
        res = 26;
    }
    return res;
}
