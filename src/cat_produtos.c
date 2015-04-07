#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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
    int elems_por_pag;
};

/*
 * FUNÇÕES PRIVADAS AO MÓDULO
 */

int cat_compara_produtos_av(const void *, const void *, void *);
void cat_free_produto_avl(void *item, void *);
int cat_calcula_indice_produto(char l);
void cat_free_produto_ad(void *);

/*
 * ÁRVORE
 */

CatProdutos inicializa_catalogo_produtos() {
    int i = 0;
    CatProdutos res = (CatProdutos) malloc(sizeof (struct catalogo_produtos));

    for (i = 0; i <= 26; i++) {
        res->indices[i] = avl_create(cat_compara_produtos_av, NULL, NULL);
    }

    return res;
}

int cat_existe_produto(CatProdutos cat, char *elem) {
    int ind, res = 0;

    if (elem != NULL) {
        ind = cat_calcula_indice_produto(*elem);
        if (avl_find(cat->indices[ind], elem) != NULL) res = 1;
        else res = 0;
    }

    return res;
}

char *cat_procura_produto(CatProdutos cat, char *elem) {
    int ind;
    char *res;

    if (elem != NULL) {
        ind = cat_calcula_indice_produto(*elem);
        res = (char *) avl_find(cat->indices[ind], elem);
    } else {
        res = NULL;
    }

    return res == NULL ? NULL : elem;
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
    avl_delete(cat->indices[ind], str);
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

void free_catalogo_produtos(CatProdutos cat) {
    int i = 0;

    for (i = 0; i <= 26; i++) {
        avl_destroy(cat->indices[i], cat_free_produto_avl);
    }

    free(cat);
}

/*
 * PAGINACAO PRODUTOS
 */

CAT_LISTA_PRODUTOS cat_lista_produtos_letra(CatProdutos catalogo_produtos, char letra){
    char *produto;
    CAT_LISTA_PRODUTOS pag = (CAT_LISTA_PRODUTOS) malloc(sizeof(struct cat_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa(8000);
    IT_CAT_PRODUTOS it = inicializa_it_cat_produtos_letra(catalogo_produtos, letra);

    while ((produto = it_cat_produto_proximo_letra(it)) != NULL) {
        ad_insere_elemento(ad, produto);
    }
    
    pag->elems_por_pag = CAT_PRODUTOS_ELEMS_PAG;
    pag->lista_paginada = ad;
    free_it_cat_produto(it);
    return pag;
}

char *cat_lista_prod_get_elemento(CAT_LISTA_PRODUTOS lista,int p){
    return (char *) ad_get_elemento(lista->lista_paginada, p);
}

int cat_lista_prod_get_pos_and_num_elems_pag(CAT_LISTA_PRODUTOS lista, int *pos_inicial, int pag){
    return ad_goto_pag(lista->lista_paginada, pos_inicial, pag, lista->elems_por_pag);
}

int cat_lista_prod_get_num_pags(CAT_LISTA_PRODUTOS lista){
    return ad_get_num_pags(lista->lista_paginada, lista->elems_por_pag);
}

int cat_lista_prod_get_elems_por_pag(CAT_LISTA_PRODUTOS lista){
    return lista->elems_por_pag;
}

void cat_lista_prod_muda_elems_por_pag(CAT_LISTA_PRODUTOS lista, int n){
    lista->elems_por_pag=n;
}

int cat_lista_prod_get_num_elems(CAT_LISTA_PRODUTOS lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void cat_free_lista_produtos(CAT_LISTA_PRODUTOS lista){
    ad_deep_free(lista->lista_paginada, cat_free_produto_ad);
    free(lista);
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
    avl_t_free(it->traverser);
    free(it);
}


/*
 * Funções (privadas) auxiliares ao módulo.
 */

int cat_compara_produtos_av(const void *avl_a, const void *avl_b, void *avl_param) {
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
