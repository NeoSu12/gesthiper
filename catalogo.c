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

int compara(const void *, const void *, void *);
int calcula_indice(char);

CATALOGO *inicializa(int tam){
    int i=0;
    CATALOGO *res = (CATALOGO *) malloc(sizeof(CATALOGO));
    res->tam=tam;
    for(i=0;i<=26;i++){
        res->indices[i] = avl_create(compara,NULL,NULL);
    }
    return res;
}

void free_catalogo(CATALOGO *cat){
    int i=0;
    for(i=0;i<=26;i++){
        avl_destroy(cat->indices[i], destroy_item);  /* Existe aqui um erro :/ */
    }
}

void insere_item(CATALOGO *cat, char *str){
    int ind = calcula_indice(str[0]);
    char *new = (char *) malloc(cat->tam + 1);
    strncpy(new, str, cat->tam +1);
    avl_insert(cat->indices[ind],new);
}

int compara(const void *avl_a, const void *avl_b, void *avl_param){
    return strcmp((char *)avl_a, (char *)avl_b);
}

void destroy_item(void *item, void *param){
    free(item);
}

int calcula_indice(char c){
    int res=0;
    char cl = toupper(c);
    if(isalpha(cl)){
        res = cl -'A';
    }else{
        res = 26;
    }
    return res;
}
