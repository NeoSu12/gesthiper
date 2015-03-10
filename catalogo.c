#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "catalogo.h"
#include "avl.h"

struct catalogo{
    int tam;
    ARVORE *indices[27];
};

struct iterador{
    TRAVERSER *tr;
    CATALOGO *c;
    int ind;
};

int compara(const void *, const void *, void *);
int calcula_indice(char);
void free_string(void *, void *);

/*
 ÁRVORE
 */

CATALOGO inicializa_catalogo(int tam){
    int i=0;
    CATALOGO *res = (CATALOGO *) malloc(sizeof(CATALOGO));
    res->tam=tam;
    for(i=0;i<=26;i++){
        res->indices[i] = avl_create(compara,NULL,NULL);
    }
    return *res;
}

char *procura_elemento(CATALOGO cat, char *elem){
    int ind;
    char *res;
    
    if(elem != NULL){
        ind = calcula_indice(*elem);
        res = (char *) avl_find (cat.indices[ind], elem);
    } else{
        res = NULL;
    }
    
    return res;
}

void insere_item(CATALOGO cat, char *str){
    int ind = calcula_indice(str[0]);
    char *new = (char *) malloc(cat.tam + 1);
    strncpy(new, str, cat.tam +1);
    avl_insert(cat.indices[ind],new);
}

int compara(const void *avl_a, const void *avl_b, void *avl_param){
    return strcmp((char *)avl_a, (char *)avl_b);
}

void free_string(void *item, void *param){
    free(item);
}

void free_catalogo(CATALOGO cat){
    int i=0;
    
    for(i=0;i<=26;i++){
        avl_destroy(cat.indices[i], free_string);
    }
    
}

int calcula_indice(char l){
    int res=0;
    char letra = toupper(l);
    
    if(isalpha(letra)){
        res = letra -'A';
    }else{
        res = 26;
    }
    return res;
}


/*
 ITERADOR
 */

ITERADOR inicializa_iterador_null(CATALOGO cat){
    ITERADOR *it = (ITERADOR *) malloc(sizeof(ITERADOR));
    it->tr = avl_t_alloc();
    avl_t_init(it->tr,cat.indices[0]);
    it->ind = 0;
    it->c=&cat;
    return *it;
}


ITERADOR inicializa_iterador_inicio(CATALOGO cat){
    ITERADOR *it = (ITERADOR *) malloc(sizeof(ITERADOR));
    it->tr = avl_t_alloc();
    avl_t_first(it->tr,cat.indices[0]);
    it->ind=0;
    return *it;
}

ITERADOR inicializa_iterador_elem(CATALOGO cat, char *st) {
    int indice;
    ITERADOR *it = (ITERADOR *) malloc(sizeof(ITERADOR));
    it->tr = avl_t_alloc();
    it->c=&cat;

    if (st != NULL) {
        indice = calcula_indice(toupper(*st));
        avl_t_find(it->tr, cat.indices[indice], st);
        it->ind = indice;
    }else{
        it = NULL;
    }
    
    return *it;
}

ITERADOR inicializa_iterador_letra(CATALOGO cat, char c) {
    int indice;
    ITERADOR *it = (ITERADOR *) malloc(sizeof (ITERADOR));
    it->tr = avl_t_alloc();
    indice = calcula_indice(toupper(c));
    avl_t_first(it->tr, cat.indices[indice]);
    it->ind = indice;
    it->c=&cat;
    return *it;
}

char *iterador_next(ITERADOR it){
    return avl_t_next(it.tr);
}


char *iterador_next__(ITERADOR it){
    char *res;
    int sair=0;
    
    while(sair==0){
        res = avl_t_next(it.tr);
        if(res != NULL || (res==NULL && it.ind>=26)){
            sair=1;
        }else{
        /* res == NULL && it->ind<26 */
            it.ind++;
            avl_t_first(it.tr,it.c->indices[it.ind]);
        }
    }
    return res;
    }
       
