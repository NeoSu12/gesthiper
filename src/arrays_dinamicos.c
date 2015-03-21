#include <stdio.h>
#include <stdlib.h>
#include "arrays_dinamicos.h"

struct array_dinamico{
    void **elementos;
    int posicao;
    int capacidade;
    struct array_dinamico* garbage_collector;
};



ARRAY_DINAMICO ad_inicializa_gc(int);
void quicksort(void **, ad_compara_elems *, int);

ARRAY_DINAMICO ad_inicializa(int capacidade){
    ARRAY_DINAMICO array_d = (ARRAY_DINAMICO) malloc(sizeof(struct array_dinamico));
    
    array_d->elementos = (void **) malloc(sizeof(void *) * capacidade);
    
    if(array_d->elementos != NULL){
        array_d->posicao = 0;
        array_d->capacidade = capacidade;
        array_d->garbage_collector = ad_inicializa_gc(capacidade>=10 ? capacidade/10 : 10);
    }
    
    return array_d;
}

ARRAY_DINAMICO ad_inicializa_gc(int capacidade){
    ARRAY_DINAMICO array_d = (ARRAY_DINAMICO) malloc(sizeof(struct array_dinamico));
    
    array_d->elementos = (void **) malloc(sizeof(void *) * capacidade);
    
    if(array_d->elementos != NULL){
        array_d->posicao = 0;
        array_d->capacidade = capacidade;
        array_d->garbage_collector = NULL;
    }
    
    return array_d;
}

void ad_insere_elemento(ARRAY_DINAMICO ad, void *elemento){
    int nova_cap;
    void **new_ptr;
    
    if(ad->posicao == ad->capacidade-1){
        nova_cap = ad->capacidade*2;
        /*Precisa de mais espaco, tenta realocar*/
        new_ptr = (void **) realloc(ad->elementos,sizeof(void *) * nova_cap);
        
        if (new_ptr != NULL){
            ad->elementos=new_ptr;
            ad->capacidade= nova_cap;
        }
        
    }
    
    ad->elementos[ad->posicao] = elemento;
    ad->posicao++;
    
}

int ad_procura_elemento(ARRAY_DINAMICO ad, void *elemento,ad_compara_elems *f_compara){
    int encontrado =0;
    int pos_encontrado=AD_ELEM_NAO_ENCONTRADO;
    int i=0;
    
    for(i=0;i<ad->posicao && !encontrado;i++)
        if(f_compara(elemento, ad->elementos[i])==0){
            encontrado = 1;
            pos_encontrado = i;
        }
    
    
    return pos_encontrado;
    
}

int ad_existe_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara){
    return ad_procura_elemento(ad, elemento,f_compara) == AD_ELEM_NAO_ENCONTRADO ? 
        AD_ELEM_NAO_EXISTE : AD_ELEM_EXISTE;
}

void ad_substitui_elemento_pos(ARRAY_DINAMICO ad, int pos, void *novo_elemento){
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    ad->elementos[pos] = novo_elemento;
}

void ad_substitui_elemento(ARRAY_DINAMICO ad, void *novo_elemento,void *antigo_elemento, ad_compara_elems *f_compara){
    
    int res_procura = ad_procura_elemento(ad, antigo_elemento,f_compara);
    if(res_procura != AD_ELEM_NAO_ENCONTRADO)
        ad_substitui_elemento_pos(ad,res_procura,novo_elemento);
        
}

void ad_remocao_rapida_elemento(ARRAY_DINAMICO ad, int pos){
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    ad->elementos[pos] = ad->elementos[ad->posicao -1];
    ad->posicao--;
}

void ad_remove_elemento_pos(ARRAY_DINAMICO ad, int pos){
    int i;
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    
    for(i=pos;i<ad->posicao;i++)
        ad->elementos[i] = ad->elementos[i + 1];
    
    ad->posicao--;
}

void ad_remove_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara){
    int posicao = ad_procura_elemento(ad,elemento,f_compara);
    
    if(posicao != AD_ELEM_NAO_ENCONTRADO)
        ad_remove_elemento_pos(ad, posicao);
}

int ad_get_tamanho(ARRAY_DINAMICO ad){
    return ad->posicao;
}

int ad_get_capacidade(ARRAY_DINAMICO ad){
    return ad->capacidade;
}

void *ad_get_elemento(ARRAY_DINAMICO ad, int pos){
    return pos < ad->posicao ? ad->elementos[pos] : NULL;
}

void **ad_goto_pag(ARRAY_DINAMICO ad, int *elems_pag, int pag, int elems_por_pag){
    int num_elems_pag;
    void **res=NULL;
    int diferenca=-1;
    int comeco_pag = elems_por_pag * (pag-1);
    
    if(comeco_pag < ad->posicao) {
        diferenca = ad->posicao-comeco_pag;
        num_elems_pag = (diferenca > elems_por_pag) ? elems_por_pag : diferenca;
        *elems_pag = num_elems_pag;
        res = ad->elementos + comeco_pag;
    }else{
        num_elems_pag = AD_PAGINA_IMPOSSIVEL;
    }
    
    
    return res;
}

void ad_ordena(ARRAY_DINAMICO ad, ad_compara_elems *f_comparacao) {
    quicksort(ad->elementos, f_comparacao, ad->posicao);
}

void quicksort(void **elems, ad_compara_elems *f_comparacao, int n){
    int i, j;
    void *p, *temp;
    
    if (n < 2) return;
    p = elems[n/2];
    
    for (i = 0, j = n - 1;; i++, j--) {
        
        while(f_comparacao(elems[i],p)<0) i++;
        while(f_comparacao(p,elems[j])<0)j--;
        if (i >= j) break;
        
        temp = elems[i];
        elems[i] = elems[j];
        elems[j] = temp;
    }
    quicksort(elems,f_comparacao, i);
    quicksort(elems+i,f_comparacao, n - i);
}

void ad_explicit_free(ARRAY_DINAMICO ad, ad_elimina_elems *f_eliminacao){
    int i;
    for(i=0;i<ad->posicao;i++)
        f_eliminacao(ad->elementos[i]);
    
    for(i=0;i<ad->garbage_collector->posicao;i++)
        f_eliminacao(ad->garbage_collector->elementos[i]);
    
    ad_free(ad->garbage_collector);
    ad_free(ad);
}

void ad_free(ARRAY_DINAMICO ad){
    free(ad->elementos);
    free(ad->garbage_collector->elementos);
    free(ad->garbage_collector);
    free(ad);
}

