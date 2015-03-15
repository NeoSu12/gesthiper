#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cat_produtos.h"
#include "avl.h"

struct catalogo_produtos {
    ARVORE indices[27];
};

struct iterador_produtos {
    TRAVERSER traverser;
    CatProdutos catalogo;
    int indice;
};

/*
 * Funções privadas ao módulo.
 */

int compara_produtos(const void *, const void *, void *);
void free_produto(void *item, void *param);
int calcula_indice_produto(char l);


/*
 ÁRVORE
 */

CatProdutos inicializa_catalogo_produtos() {
    int i = 0;
    CatProdutos res = (CatProdutos) malloc(sizeof (struct catalogo_produtos));

    for (i = 0; i <= 26; i++) {
        res->indices[i] = avl_create(compara_produtos, NULL, NULL);
    }

    return res;
}

int existe_produto(CatProdutos cat, char *elem) {
    int ind, res = 0;

    if (elem != NULL) {
        ind = calcula_indice_produto(*elem);
        if (avl_find(cat->indices[ind], elem) != NULL) res = 1;
        else res = 0;
    }

    return res;
}

char *procura_produto(CatProdutos cat, char *elem) {
    int ind;
    char *res;

    if (elem != NULL) {
        ind = calcula_indice_produto(*elem);
        res = (char *) avl_find(cat->indices[ind], elem);
    } else {
        res = NULL;
    }

    return res == NULL ? NULL : elem;
}

char *insere_produto(CatProdutos cat, char *str) {
    int ind = calcula_indice_produto(str[0]);
    int tamanho = strlen(str);
    char *res;
    char *new = (char *) malloc(tamanho + 1);

    strncpy(new, str, tamanho + 1);
    res = avl_insert(cat->indices[ind], new);

    return res == NULL ? NULL : str;
}

char *remove_produto(CatProdutos cat, char *str) {
    int ind = calcula_indice_produto(str[0]);
    return avl_delete(cat->indices[ind], str);
}

int total_produtos(CatProdutos cat) {
    size_t soma = 0;
    int i;

    for (i = 0; i <= 26; i++)
        soma += avl_count(cat->indices[i]);

    return soma;
}

int total_produtos_letra(CatProdutos cat, char letra) {
    int ind = calcula_indice_produto(letra);
    return avl_count(cat->indices[ind]);
}

void free_catalogo_produtos(CatProdutos cat) {
    int i = 0;

    for (i = 0; i <= 26; i++) {
        avl_destroy(cat->indices[i], free_produto);
    }

    free(cat);
}

/*
 IT_PRODUTOS
 */

IT_PRODUTOS inicializa_it_produtos_inicio(CatProdutos cat) {
    IT_PRODUTOS it = (IT_PRODUTOS) malloc(sizeof (struct iterador_produtos));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_PRODUTOS inicializa_it_produtos_fim(CatProdutos cat) {
    IT_PRODUTOS it;
    it = (IT_PRODUTOS) malloc(sizeof (struct iterador_produtos));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[26]);
    it->indice = 26;
    it->catalogo = cat;
    return it;
}

IT_PRODUTOS inicializa_it_produtos_elem(CatProdutos cat, char *st) {
    int indice;
    IT_PRODUTOS it;

    if (st != NULL) {
        it = (IT_PRODUTOS) malloc(sizeof (struct iterador_produtos));
        it->traverser = avl_t_alloc();
        it->catalogo = cat;
        indice = calcula_indice_produto(toupper(*st));
        avl_t_find(it->traverser, cat->indices[indice], st);
        it->indice = indice;
    } else {
        it = NULL;
    }

    return it;
}

IT_PRODUTOS inicializa_it_produtos_inicio_letra(CatProdutos cat, char c) {
    int indice;
    IT_PRODUTOS it = (IT_PRODUTOS) malloc(sizeof (struct iterador_produtos));
    it->traverser = avl_t_alloc();
    indice = calcula_indice_produto(toupper(c));
    avl_t_first(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_PRODUTOS inicializa_it_produtos_fim_letra(CatProdutos cat, char c) {
    int indice;
    IT_PRODUTOS it = (IT_PRODUTOS) malloc(sizeof (struct iterador_produtos));
    it->traverser = avl_t_alloc();
    indice = calcula_indice_produto(toupper(c));
    avl_t_last(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

int itera_n_produtos_proximos(IT_PRODUTOS it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = 0;

    if ((primeiro = it_produto_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i++;
    }

    while (i < n && (codigo = it_produto_proximo(it)) != NULL) {
        codigos[i] = codigo;
        i++;
    }
    return i;
}

int itera_n_produtos_anteriores(IT_PRODUTOS it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = n - 1;

    if ((primeiro = it_produto_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i--;
    }
    while (i >= 0 && (codigo = it_produto_anterior(it)) != NULL) {
        codigos[i] = codigo;
        i--;
    }
    return i < 0 ? n : n - i;
}

char *it_produto_proximo(IT_PRODUTOS it) {
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

char *it_produto_actual(IT_PRODUTOS it) {
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

char *it_produto_anterior(IT_PRODUTOS it) {
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

char *it_produto_proximo_letra(IT_PRODUTOS it) {
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

char *it_produto_anterior_letra(IT_PRODUTOS it) {
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

/*
 * Funções (privadas) auxiliares ao módulo.
 */

int compara_produtos(const void *avl_a, const void *avl_b, void *avl_param) {
    return strcmp((char *) avl_a, (char *) avl_b);
}

void free_produto(void *item, void *param) {
    free(item);
}

int calcula_indice_produto(char l) {
    int res = 0;
    char letra = toupper(l);

    if (isalpha(letra)) {
        res = letra - 'A';
    } else {
        res = 26;
    }
    return res;
}
