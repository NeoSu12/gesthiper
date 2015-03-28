#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "headers/cat_clientes.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define APENAS_LETRA 1
#define TODO_CATALOGO 2


struct catalogo_clientes {
    ARVORE indices[27];
};

struct iterador_cat_clientes {
    TRAVERSER traverser;
    CatClientes catalogo;
    int indice;
};

struct cat_lista_clientes{
    ARRAY_DINAMICO lista_paginada;
    int elems_por_pag;
};

/*
 * FUNÇÕES PRIVADAS AO MÓDULO
 */

int cat_compara_clientes_av(const void *, const void *, void *);
void cat_free_cliente_avl(void *item, void *);
int cat_calcula_indice_cliente(char l);
void cat_free_cliente_ad(void *);

/*
 * ÁRVORE
 */

CatClientes inicializa_catalogo_clientes() {
    int i = 0;
    CatClientes res = (CatClientes) malloc(sizeof (struct catalogo_clientes));

    for (i = 0; i <= 26; i++) {
        res->indices[i] = avl_create(cat_compara_clientes_av, NULL, NULL);
    }

    return res;
}

int cat_existe_cliente(CatClientes cat, char *elem) {
    int ind, res = 0;

    if (elem != NULL) {
        ind = cat_calcula_indice_cliente(*elem);
        if (avl_find(cat->indices[ind], elem) != NULL) res = 1;
        else res = 0;
    }

    return res;
}

char *cat_procura_cliente(CatClientes cat, char *elem) {
    int ind;
    char *res;

    if (elem != NULL) {
        ind = cat_calcula_indice_cliente(*elem);
        res = (char *) avl_find(cat->indices[ind], elem);
    } else {
        res = NULL;
    }

    return res == NULL ? NULL : elem;
}

char *cat_insere_cliente(CatClientes cat, char *str) {
    int ind = cat_calcula_indice_cliente(str[0]);
    int tamanho = strlen(str);
    char *res;
    char *new = (char *) malloc(tamanho + 1);

    strncpy(new, str, tamanho + 1);
    res = avl_insert(cat->indices[ind], new);

    return res == NULL ? NULL : str;
}

char *cat_remove_cliente(CatClientes cat, char *str) {
    int ind = cat_calcula_indice_cliente(str[0]);
    return avl_delete(cat->indices[ind], str);
}

int cat_total_clientes(CatClientes cat) {
    size_t soma = 0;
    int i;

    for (i = 0; i <= 26; i++)
        soma += avl_count(cat->indices[i]);

    return soma;
}

int cat_total_clientes_letra(CatClientes cat, char letra) {
    int ind = cat_calcula_indice_cliente(letra);
    return avl_count(cat->indices[ind]);
}

void free_catalogo_clientes(CatClientes cat) {
    int i = 0;

    for (i = 0; i <= 26; i++) {
        avl_destroy(cat->indices[i], cat_free_cliente_avl);
    }

    free(cat);
}

/*
 * ITERADORES CLIENTES
 */

IT_CAT_CLIENTES inicializa_it_cat_clientes(CatClientes cat) {
    IT_CAT_CLIENTES it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_letra(CatClientes cat, char c) {
    int indice;
    IT_CAT_CLIENTES it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_cliente(toupper(c));
    avl_t_init(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_primeiro(CatClientes cat) {
    IT_CAT_CLIENTES it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_ultimo(CatClientes cat) {
    IT_CAT_CLIENTES it;
    it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, cat->indices[26]);
    it->indice = 26;
    it->catalogo = cat;
    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_elem(CatClientes cat, char *st) {
    int indice;
    IT_CAT_CLIENTES it;

    if (st != NULL) {
        it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
        it->traverser = avl_t_alloc();
        it->catalogo = cat;
        indice = cat_calcula_indice_cliente(toupper(*st));
        avl_t_find(it->traverser, cat->indices[indice], st);
        it->indice = indice;
    } else {
        it = NULL;
    }

    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_primeiro_letra(CatClientes cat, char c) {
    int indice;
    IT_CAT_CLIENTES it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_cliente(toupper(c));
    avl_t_first(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_CAT_CLIENTES inicializa_it_cat_clientes_ultimo_letra(CatClientes cat, char c) {
    int indice;
    IT_CAT_CLIENTES it = (IT_CAT_CLIENTES) malloc(sizeof (struct iterador_cat_clientes));
    it->traverser = avl_t_alloc();
    indice = cat_calcula_indice_cliente(toupper(c));
    avl_t_last(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

char *it_cat_cliente_proximo(IT_CAT_CLIENTES it) {
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

char *it_cat_cliente_actual(IT_CAT_CLIENTES it) {
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

char *it_cat_cliente_anterior(IT_CAT_CLIENTES it) {
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

char *it_cat_cliente_proximo_letra(IT_CAT_CLIENTES it) {
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

char *it_cat_cliente_anterior_letra(IT_CAT_CLIENTES it) {
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

void free_it_cat_cliente(IT_CAT_CLIENTES it){
    avl_t_free(it->traverser);
    free(it);
}

/*
 * PAGINACAO CLIENTES
 */

CAT_LISTA_CLIENTES cat_lista_clientes_letra(CatClientes catalogo_clientes, char letra, int elems_por_pag){
    char *cliente;
    CAT_LISTA_CLIENTES pag = (CAT_LISTA_CLIENTES) malloc(sizeof(struct cat_lista_clientes));
    ARRAY_DINAMICO ad = ad_inicializa(8000);
    IT_CAT_CLIENTES it = inicializa_it_cat_clientes_letra(catalogo_clientes, letra);

    while ((cliente = it_cat_cliente_proximo_letra(it)) != NULL) {
        ad_insere_elemento(ad, cliente);
    }
    
    pag->elems_por_pag = elems_por_pag;
    pag->lista_paginada = ad;
    free_it_cat_cliente(it);
    return pag;
}

char *cat_lista_cli_get_elemento(CAT_LISTA_CLIENTES lista,int p){
    return (char *) ad_get_elemento(lista->lista_paginada, p);
}

int cat_lista_cli_get_pos_and_num_elems_pag(CAT_LISTA_CLIENTES lista, int *pos_inicial, int pag){
    return ad_goto_pag(lista->lista_paginada, pos_inicial, pag, lista->elems_por_pag);
}

int cat_lista_cli_get_num_pags(CAT_LISTA_CLIENTES lista){
    return (cat_lista_cli_get_num_elems(lista) / cat_lista_cli_get_elems_por_pag(lista)) + 1;
}

int cat_lista_cli_get_elems_por_pag(CAT_LISTA_CLIENTES lista){
    return lista->elems_por_pag;
}

int cat_lista_cli_muda_elems_por_pag(CAT_LISTA_CLIENTES lista, int n){
    return lista->elems_por_pag=n;
}

int cat_lista_cli_get_num_elems(CAT_LISTA_CLIENTES lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void cat_free_lista_clientes(CAT_LISTA_CLIENTES lista){
    ad_deep_free(lista->lista_paginada, cat_free_cliente_ad);
    free(lista);
}


/*
 * Funções (privadas) auxiliares ao módulo.
 */

int cat_compara_clientes_av(const void *avl_a, const void *avl_b, void *avl_param) {
    return strcmp((char *) avl_a, (char *) avl_b);
}

void cat_free_cliente_avl(void *item, void *param) {
    free(item);
}

void cat_free_cliente_ad(void *item) {
    free(item);
}

int cat_calcula_indice_cliente(char l) {
    int res = 0;
    char letra = toupper(l);

    if (isalpha(letra)) {
        res = letra - 'A';
    } else {
        res = 26;
    }
    return res;
}
