#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cat_clientes.h"
#include "avl.h"

struct catalogo_clientes {
    ARVORE indices[27];
};

struct iterador {
    TRAVERSER tr;
    CatClientes c;
    int ind;
};

/*
 * Funções privadas ao módulo.
 */

int compara_clientes(const void *, const void *, void *);
void free_cliente(void *item, void *param);
int calcula_indice_cliente(char l);


/*
 ÁRVORE
 */

CatClientes inicializa_catalogo_clientes() {
    int i = 0;
    CatClientes res = (CatClientes) malloc(sizeof (struct catalogo_clientes));

    for (i = 0; i <= 26; i++) {
        res->indices[i] = avl_create(compara_clientes, NULL, NULL);
    }

    return res;
}

int existe_cliente(CatClientes cat, char *elem) {
    int ind, res = 0;

    if (elem != NULL) {
        ind = calcula_indice_cliente(*elem);
        if (avl_find(cat->indices[ind], elem) != NULL) res = 1;
        else res = 0;
    }

    return res;
}

char *procura_cliente(CatClientes cat, char *elem) {
    int ind;
    char *res;

    if (elem != NULL) {
        ind = calcula_indice_cliente(*elem);
        res = (char *) avl_find(cat->indices[ind], elem);
    } else {
        res = NULL;
    }

    return res == NULL ? NULL : elem;
}

char *insere_cliente(CatClientes cat, char *str) {
    int ind = calcula_indice_cliente(str[0]);
    int tamanho = strlen(str);
    char *res;
    char *new = (char *) malloc(tamanho + 1);

    strncpy(new, str, tamanho + 1);
    res = avl_insert(cat->indices[ind], new);

    return res == NULL ? NULL : str;
}

char *remove_cliente(CatClientes cat, char *str) {
    int ind = calcula_indice_cliente(str[0]);
    return avl_delete(cat->indices[ind], str);
}

int total_clientes(CatClientes cat) {
    size_t soma = 0;
    int i;

    for (i = 0; i <= 26; i++)
        soma += avl_count(cat->indices[i]);

    return soma;
}

int total_clientes_letra(CatClientes cat, char letra) {
    int ind = calcula_indice_cliente(letra);
    return avl_count(cat->indices[ind]);
}

void free_catalogo_clientes(CatClientes cat) {
    int i = 0;

    for (i = 0; i <= 26; i++) {
        avl_destroy(cat->indices[i], free_cliente);
    }

    free(cat);
}

/*
 IT_CLIENTES
 */

IT_CLIENTES inicializa_it_clientes_inicio(CatClientes cat) {
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    avl_t_first(it->tr, cat->indices[0]);
    it->ind = 0;
    it->c = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_fim(CatClientes cat) {
    IT_CLIENTES it;
    it = (IT_CLIENTES) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    avl_t_first(it->tr, cat->indices[26]);
    it->ind = 26;
    it->c = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_elem(CatClientes cat, char *st) {
    int indice;
    IT_CLIENTES it;

    if (st != NULL) {
        it = (IT_CLIENTES) malloc(sizeof (struct iterador));
        it->tr = avl_t_alloc();
        it->c = cat;
        indice = calcula_indice_cliente(toupper(*st));
        avl_t_find(it->tr, cat->indices[indice], st);
        it->ind = indice;
    } else {
        it = NULL;
    }

    return it;
}

IT_CLIENTES inicializa_it_clientes_inicio_letra(CatClientes cat, char c) {
    int indice;
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    indice = calcula_indice_cliente(toupper(c));
    avl_t_first(it->tr, cat->indices[indice]);
    it->ind = indice;
    it->c = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_fim_letra(CatClientes cat, char c) {
    int indice;
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    indice = calcula_indice_cliente(toupper(c));
    avl_t_last(it->tr, cat->indices[indice]);
    it->ind = indice;
    it->c = cat;
    return it;
}

int itera_n_clientes_proximos(IT_CLIENTES it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = 0;

    if ((primeiro = it_cliente_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i++;
    }

    while (i < n && (codigo = it_cliente_proximo(it)) != NULL) {
        codigos[i] = codigo;
        i++;
    }
    return i;
}

int itera_n_clientes_anteriores(IT_CLIENTES it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = n - 1;

    if ((primeiro = it_cliente_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i--;
    }
    while (i >= 0 && (codigo = it_cliente_anterior(it)) != NULL) {
        codigos[i] = codigo;
        i--;
    }
    return i < 0 ? n : n - 1;
}

char *it_cliente_proximo(IT_CLIENTES it) {
    int tamanho;
    int sair = 0;
    char *res = NULL;
    char *ret = NULL;

    while (res == NULL && sair == 0) {
        res = avl_t_next(it->tr);
        if (res != NULL || (res == NULL && it->ind >= 26)) {
            sair = 1;
        } else {
            /* res == NULL && it->ind<26 */
            it->ind++;
            res = avl_t_first(it->tr, it->c->indices[it->ind]);
        }
    }

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }
    return ret;
}

char *it_cliente_actual(IT_CLIENTES it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_cur(it->tr);

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }

    return ret;
}

char *it_cliente_anterior(IT_CLIENTES it) {
    int tamanho;
    int sair = 0;
    char *res = NULL;
    char *ret = NULL;

    while (res == NULL && sair == 0) {
        res = avl_t_prev(it->tr);
        if (res != NULL || (res == NULL && it->ind <= 0)) {
            sair = 1;
        } else {
            /* res == NULL && it->ind>=0 */
            it->ind--;
            res = avl_t_last(it->tr, it->c->indices[it->ind]);
        }
    }

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }
    return ret;
}

char *it_cliente_proximo_letra(IT_CLIENTES it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_next(it->tr);

    if (res != NULL) {
        tamanho = strlen(res) + 1;
        ret = (char *) malloc(sizeof (char)*tamanho);
        strncpy(ret, res, tamanho);
    }

    return ret;
}

char *it_cliente_anterior_letra(IT_CLIENTES it) {
    int tamanho;
    char *ret = NULL;
    char *res = avl_t_prev(it->tr);

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

int compara_clientes(const void *avl_a, const void *avl_b, void *avl_param) {
    return strcmp((char *) avl_a, (char *) avl_b);
}

void free_cliente(void *item, void *param) {
    free(item);
}

int calcula_indice_cliente(char l) {
    int res = 0;
    char letra = toupper(l);

    if (isalpha(letra)) {
        res = letra - 'A';
    } else {
        res = 26;
    }
    return res;
}