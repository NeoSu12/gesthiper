#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "catalogo.h"
#include "avl.h"

struct catalogo{
    ARVORE indices[27];
};

struct iterador{
    TRAVERSER tr;
    CATALOGO c;
    int ind;
};

/*
 * Funções privadas ao módulo.
 */

int compara(const void *, const void *, void *);
int calcula_indice(char);
void free_string(void *, void *);

/*
 ÁRVORE
 */

CATALOGO inicializa_catalogo(){
    int i=0;
    CATALOGO res = (CATALOGO) malloc(sizeof(struct catalogo));
    
    for(i=0;i<=26;i++){
        res->indices[i] = avl_create(compara,NULL,NULL);
    }
    
    return res;
}

int existe_elemento(CATALOGO cat, char *elem){
    int ind, res=0;
    
    if(elem != NULL){
        ind = calcula_indice(*elem);
        if(avl_find(cat->indices[ind], elem) != NULL) res = 1;
        else res = 0;
    }
    
    return res;
}

char *procura_elemento(CATALOGO cat, char *elem){
    int ind;
    char *res;
    
    if(elem != NULL){
        ind = calcula_indice(*elem);
        res = (char *) avl_find (cat->indices[ind], elem);
    } else{
        res = NULL;
    }
    
    return res==NULL ? NULL : elem;
}

char *insere_item(CATALOGO cat, char *str){
    int ind = calcula_indice(str[0]);
    int tamanho = strlen(str);
    char *res;
    char *new = (char *) malloc(tamanho + 1);
    
    strncpy(new, str, tamanho +1);
    res = avl_insert(cat->indices[ind],new);
    
    return res==NULL ? NULL : str;
}

char *remove_item(CATALOGO cat, char *str){
    int ind = calcula_indice(str[0]);
    return avl_delete(cat->indices[ind],str);
}

int total_codigos(CATALOGO cat){
    size_t soma=0;
    int i;
    
    for(i=0;i<=26;i++)
        soma += avl_count(cat->indices[i]);
    
    return soma;
}

int total_codigos_letra(CATALOGO cat, char letra){
    int ind = calcula_indice(letra);
    return avl_count(cat->indices[ind]);
}

void free_catalogo(CATALOGO cat){
    int i=0;
    
    for(i=0;i<=26;i++){
        avl_destroy(cat->indices[i], free_string);
    }
    
    free(cat);
}
/*
 ITERADOR
 */

ITERADOR inicializa_it_inicio(CATALOGO cat) {
    ITERADOR it = (ITERADOR) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    avl_t_first(it->tr, cat->indices[0]);
    it->ind = 0;
    it->c = cat;
    return it;
}

ITERADOR inicializa_it_fim(CATALOGO cat) {
    ITERADOR it;
    it = (ITERADOR) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    avl_t_first(it->tr, cat->indices[26]);
    it->ind = 26;
    it->c = cat;
    return it;
}

ITERADOR inicializa_it_elem(CATALOGO cat, char *st) {
    int indice;
    ITERADOR it;
    
    if (st != NULL) {
        it = (ITERADOR) malloc(sizeof (struct iterador));
        it->tr = avl_t_alloc();
        it->c = cat;
        indice = calcula_indice(toupper(*st));
        avl_t_find(it->tr, cat->indices[indice], st);
        it->ind = indice;
    } else {
        it = NULL;
    }

    return it;
}

ITERADOR inicializa_it_inicio_letra(CATALOGO cat, char c) {
    int indice;
    ITERADOR it = (ITERADOR) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    indice = calcula_indice(toupper(c));
    avl_t_first(it->tr, cat->indices[indice]);
    it->ind = indice;
    it->c=cat;
    return it;
}

ITERADOR inicializa_it_fim_letra(CATALOGO cat, char c) {
    int indice;
    ITERADOR it = (ITERADOR) malloc(sizeof (struct iterador));
    it->tr = avl_t_alloc();
    indice = calcula_indice(toupper(c));
    avl_t_last(it->tr, cat->indices[indice]);
    it->ind = indice;
    it->c=cat;
    return it;
}

int itera_n_proximos(ITERADOR it, char *codigos[], int n){
    char *codigo, *primeiro;
    int i=0;
    
    if((primeiro  = iterador_actual(it)) != NULL ){
        codigos[i] = primeiro;
        i++;
    }
    
    while(i<n && (codigo = iterador_proximo(it)) != NULL){
        codigos[i] = codigo;
        i++;
    }
    return i;
}

int itera_n_anteriores(ITERADOR it, char *codigos[], int n){
    char *codigo, *primeiro;
    int i=n-1;
    
    if((primeiro  = iterador_actual(it)) != NULL ){
        codigos[i] = primeiro;
        i--;
    }
    while(i>=0 && (codigo = iterador_anterior(it)) != NULL){
        codigos[i] = codigo;
        i--;
    }
    return i<0 ? n : n-1;
}

char *iterador_proximo(ITERADOR it) {
    char *res=NULL;
    int sair = 0;

    while (res==NULL && sair == 0) {
        
        res = avl_t_next(it->tr);
        
        if (res != NULL || (res == NULL && it->ind >= 26)) {
            sair = 1;
        } else {
            /* res == NULL && it->ind<26 */
            it->ind++;
            res = avl_t_first(it->tr, it->c->indices[it->ind]);
        }
    }

    return res;
}

char *iterador_actual(ITERADOR it){
    return avl_t_cur(it->tr);
}

char *iterador_anterior(ITERADOR it) {
    char *res=NULL;
    int sair = 0;

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
    return res;
}

char *iterador_proximo_letra(ITERADOR it){
    return avl_t_next(it->tr);
}

char *iterador_anterior_letra(ITERADOR it){
    return avl_t_prev(it->tr);
}
       
/*
 * Funções (privadas) auxiliares ao módulo.
 */

int compara(const void *avl_a, const void *avl_b, void *avl_param){
    return strcmp((char *)avl_a, (char *)avl_b);
}

void free_string(void *item, void *param){
    free(item);
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
