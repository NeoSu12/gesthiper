#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "headers/cat_clientes.h"
#include "headers/avl.h"

#define APENAS_LETRA 1
#define TODO_CATALOGO 2


struct catalogo_clientes {
    ARVORE indices[27];
};

struct iterador_clientes {
    TRAVERSER traverser;
    CatClientes catalogo;
    int indice;
};

struct paginacao_clientes{
    IT_CLIENTES it;
    int posicao;
    int tamanho_pag;
    int letra_ou_catalogo;
};

/*
 * FUNÇÕES PRIVADAS AO MÓDULO
 */

int compara_clientes(const void *, const void *, void *);
void free_cliente(void *item, void *);
int calcula_indice_cliente(char l);
int ajusta_iterador_clientes(PagClientes, int);
int ajusta_iterador_clientes_letra(PagClientes, int);


/*
 * ÁRVORE
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
 * ITERADORES CLIENTES
 */

IT_CLIENTES inicializa_it_clientes_inicio(CatClientes cat) {
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador_clientes));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[0]);
    it->indice = 0;
    it->catalogo = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_fim(CatClientes cat) {
    IT_CLIENTES it;
    it = (IT_CLIENTES) malloc(sizeof (struct iterador_clientes));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cat->indices[26]);
    it->indice = 26;
    it->catalogo = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_elem(CatClientes cat, char *st) {
    int indice;
    IT_CLIENTES it;

    if (st != NULL) {
        it = (IT_CLIENTES) malloc(sizeof (struct iterador_clientes));
        it->traverser = avl_t_alloc();
        it->catalogo = cat;
        indice = calcula_indice_cliente(toupper(*st));
        avl_t_find(it->traverser, cat->indices[indice], st);
        it->indice = indice;
    } else {
        it = NULL;
    }

    return it;
}

IT_CLIENTES inicializa_it_clientes_inicio_letra(CatClientes cat, char c) {
    int indice;
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador_clientes));
    it->traverser = avl_t_alloc();
    indice = calcula_indice_cliente(toupper(c));
    avl_t_first(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

IT_CLIENTES inicializa_it_clientes_fim_letra(CatClientes cat, char c) {
    int indice;
    IT_CLIENTES it = (IT_CLIENTES) malloc(sizeof (struct iterador_clientes));
    it->traverser = avl_t_alloc();
    indice = calcula_indice_cliente(toupper(c));
    avl_t_last(it->traverser, cat->indices[indice]);
    it->indice = indice;
    it->catalogo = cat;
    return it;
}

int itera_n_clientes_proximos(IT_CLIENTES it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = 0;
    
    
    if(it_cliente_actual(it)==NULL) 
        it_cliente_proximo(it);
    
    if ((primeiro = it_cliente_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i++;
    }

    while (i < n && (codigo = it_cliente_proximo(it)) != NULL) {
        codigos[i] = codigo;
        i++;
    }
    
    it_cliente_proximo(it);
    return i;
    
}

int itera_n_clientes_proximos_letra(IT_CLIENTES it, char *codigos[], int n) {
    char *codigo, *primeiro;
    int i = 0;
    
    
    if(it_cliente_actual(it)==NULL) 
        it_cliente_proximo_letra(it);
    
    if ((primeiro = it_cliente_actual(it)) != NULL) {
        codigos[i] = primeiro;
        i++;
    }

    while (i < n && (codigo = it_cliente_proximo_letra(it)) != NULL) {
        codigos[i] = codigo;
        i++;
    }
    
    it_cliente_proximo(it);
    return i;
    
}

int itera_n_clientes_anteriores(IT_CLIENTES it, char *codigos[], int n) {
    int i = 0;
    
    while (i < n && it_cliente_anterior(it) != NULL) i++;
    
    itera_n_clientes_proximos(it,codigos,i);
    
    return i;
}

char *it_cliente_proximo(IT_CLIENTES it) {
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

char *it_cliente_actual(IT_CLIENTES it) {
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

char *it_cliente_anterior(IT_CLIENTES it) {
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

char *it_cliente_proximo_letra(IT_CLIENTES it) {
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

char *it_cliente_anterior_letra(IT_CLIENTES it) {
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
 * PAGINAÇÃO
 */

PagClientes inicializa_pag_clientes(CatClientes cat, int tam_pag){
    PagClientes res = (PagClientes) malloc(sizeof(struct paginacao_clientes));
    IT_CLIENTES iterador = inicializa_it_clientes_inicio(cat);
    
    res->letra_ou_catalogo = TODO_CATALOGO;
    res->it=iterador;
    res->tamanho_pag=tam_pag;

    if (avl_t_first(iterador->traverser, iterador->catalogo->indices[iterador->indice]) == NULL)
        res->posicao = 0;
    else
        res->posicao = 1;
    
    return res;
}

PagClientes inicializa_pag_clientes_letra(CatClientes cat, int tam_pag, char letra){
    PagClientes res = (PagClientes) malloc(sizeof(struct paginacao_clientes));
    IT_CLIENTES iterador = inicializa_it_clientes_inicio_letra(cat, letra);
    
    res->letra_ou_catalogo = APENAS_LETRA;
    res->it=iterador;
    res->tamanho_pag=tam_pag;
    
    if(avl_t_first(iterador->traverser, iterador->catalogo->indices[iterador->indice])==NULL)
        res->posicao=0;
    else 
        res->posicao=1;
    
    return res;
}

int pag_clientes_goto_pag(PagClientes pag_clientes ,int n_pagina, char *pagina[]){
    int ajuste;
    int elems;
    
    if(pag_clientes->letra_ou_catalogo==TODO_CATALOGO){
        ajuste = ajusta_iterador_clientes(pag_clientes, n_pagina);
    }else{
        ajuste = ajusta_iterador_clientes_letra(pag_clientes, n_pagina);
    }
    
    if(ajuste == PAGINA_POSSIVEL){
        if(pag_clientes->letra_ou_catalogo==TODO_CATALOGO){
            elems = itera_n_clientes_proximos(pag_clientes->it, pagina ,pag_clientes->tamanho_pag);
        }else{ 
            elems = itera_n_clientes_proximos_letra(pag_clientes->it, pagina ,pag_clientes->tamanho_pag);
        }
        
        pag_clientes->posicao+=elems;
    } else {
        elems = PAGINA_IMPOSSIVEL;
    }
    
    return elems;
    
}

int ajusta_iterador_clientes(PagClientes pag_clientes, int n_pagina){
    int i, retorno;
    int tam_pag = pag_clientes->tamanho_pag;
    int pos_f = (n_pagina-1)*tam_pag+1;
    int diferenca = pos_f - pag_clientes->posicao;
    
    if(diferenca>0){
        for(i=0;i<diferenca && it_cliente_proximo(pag_clientes->it)!=NULL;i++)
            pag_clientes->posicao++;
    }else{
        diferenca = abs(diferenca);
        for(i=0;i<diferenca && it_cliente_anterior(pag_clientes->it)!=NULL;i++)
            pag_clientes->posicao--;
    }
    
    if(i==diferenca) retorno = PAGINA_POSSIVEL;
    else retorno = PAGINA_IMPOSSIVEL;
    
    return retorno;
}

int ajusta_iterador_clientes_letra(PagClientes pag_clientes, int n_pagina){
    int i, retorno;
    int tam_pag = pag_clientes->tamanho_pag;
    int pos_f = (n_pagina-1)*tam_pag+1;
    int diferenca = pos_f - pag_clientes->posicao;
    
    if(diferenca>0){
        for(i=0;i<diferenca && it_cliente_proximo_letra(pag_clientes->it)!=NULL;i++)
            pag_clientes->posicao++;
    }else{
        diferenca = abs(diferenca);
        for(i=0;i<diferenca && it_cliente_anterior_letra(pag_clientes->it)!=NULL;i++)
            pag_clientes->posicao--;
    }
    
    if(i==diferenca) retorno = PAGINA_POSSIVEL;
    else retorno = PAGINA_IMPOSSIVEL;
    
    return retorno;
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
