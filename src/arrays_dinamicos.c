#include <stdio.h>
#include <stdlib.h>
#include "headers/arrays_dinamicos.h"

#define AD_ELEMS_POR_PAG_DEFAULT 15


struct array_dinamico{
    void **elementos;
    int posicao;
    int capacidade;
    struct array_dinamico* garbage_collector;
};

struct ad_paginador{
    ARRAY_DINAMICO array_paginado;
    int num_pag;
    int posicao_inicial;
    int num_elems_pag;
    int elems_por_pag;
};

ARRAY_DINAMICO ad_inicializa_gc(int);
void ad_realloc_if_needed(ARRAY_DINAMICO);
void quicksort(void **, ad_compara_elems *, int, void *);


/*
 * INICIALIZACAO E LIBERTACAO MEMORIA
 */

ARRAY_DINAMICO ad_inicializa(){
    return ad_inicializa_cap(20);
}

ARRAY_DINAMICO ad_inicializa_cap(int capacidade){
    ARRAY_DINAMICO array_d = (ARRAY_DINAMICO) malloc(sizeof(struct array_dinamico));
    
    array_d->elementos = (void **) malloc(sizeof(void *) * capacidade);
    
    if(array_d->elementos != NULL){
        array_d->posicao = 0;
        array_d->capacidade = capacidade;
        array_d->garbage_collector = ad_inicializa_gc(capacidade>=10 ? capacidade/10 : 10);
    }
    
    return array_d;
}

void ad_clean_gc(ARRAY_DINAMICO ad, ad_elimina_elems *f_eliminacao) {
    int i;
    
    for (i = 0; i < ad->garbage_collector->posicao; i++)
        f_eliminacao(ad->garbage_collector->elementos[i]);

    ad_free(ad->garbage_collector);
    ad->garbage_collector = (ARRAY_DINAMICO) ad_inicializa_gc(8);
}

void ad_deep_free(ARRAY_DINAMICO ad, ad_elimina_elems *f_eliminacao) {
    int i;
    if (ad != NULL) {
        for (i = 0; i < ad->posicao; i++)
            f_eliminacao(ad->elementos[i]);

        for (i = 0; i < ad->garbage_collector->posicao; i++)
            f_eliminacao(ad->garbage_collector->elementos[i]);

        ad_free(ad->garbage_collector);
        free(ad->elementos);
    }
    free(ad);
}

void ad_free(ARRAY_DINAMICO ad) {
    if (ad != NULL) {
        free(ad->elementos);
        ad_free(ad->garbage_collector);
    }
    free(ad);
}

/*
 * TAMANHO E CAPACIDADE
 */

int ad_get_tamanho(ARRAY_DINAMICO ad){
    return ad->posicao;
}

int ad_get_capacidade(ARRAY_DINAMICO ad){
    return ad->capacidade;
}

/*
 * INSERCAO
 */

void ad_insere_elemento(ARRAY_DINAMICO ad, void *elemento){
    ad_insere_elemento_pos(ad, ad->posicao, elemento);
}

void ad_insere_elemento_pos(ARRAY_DINAMICO ad, int pos, void *elemento){
    ad_insere_elemento_pos_mode(ad, pos, elemento, INCREASE_SIZE);
}

void ad_insere_elemento_pos_mode(ARRAY_DINAMICO ad,int pos, void *elemento, insert_mode_t i_mode){
    int i;
    
    ad_realloc_if_needed(ad);
    
    for(i=ad->posicao;i>pos;i--){
        ad->elementos[i] = ad->elementos[i-1];
    }
    
    ad->elementos[pos] = elemento;
    ad->posicao++;
    
    if(i_mode == KEEP_SIZE)
        ad_remove_elemento_pos(ad, ad->posicao-1);
}

void ad_insere_elemento_ordenado(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara, void *param){
    ad_insere_elemento_ordenado_mode(ad, elemento,f_compara, INCREASE_SIZE, param);
}

void ad_insere_elemento_ordenado_mode(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara, insert_mode_t i_mode, void *param){
    int keep_looking=1;
    int i;
    
    if(ad->posicao==0){
        ad_insere_elemento_pos_mode(ad, 0, elemento, i_mode);
        keep_looking=0;
    }
    
    if(keep_looking && f_compara(elemento, ad_get_elemento(ad,0), param)<=0)
        keep_looking=0;
    
    if(keep_looking && f_compara(elemento, ad_get_elemento(ad,ad->posicao-1),param)>0){
        ad_insere_elemento_pos_mode(ad, ad->posicao-1,elemento,i_mode);
        keep_looking=0;
    }
    
    for(i=0;i<ad->posicao-1 && keep_looking;i++){
        if(f_compara(elemento, ad_get_elemento(ad, i),param)>=0
                &&f_compara(elemento, ad_get_elemento(ad, i+1),param)<0){
            ad_insere_elemento_pos_mode(ad, i+1, elemento, i_mode);
            keep_looking = 0;
        }
    }
       
}

/*
 * PESQUISA
 */

void *ad_get_elemento(ARRAY_DINAMICO ad, int pos){
    return pos < ad->posicao ? ad->elementos[pos] : NULL;
}

int ad_get_pos_elem(ARRAY_DINAMICO ad, void *elemento,ad_compara_elems *f_compara, void *param){
    int encontrado =0;
    int pos_encontrado=AD_ELEM_NAO_ENCONTRADO;
    int i=0;
    
    for(i=0;i<ad->posicao && !encontrado;i++){
        if(f_compara(elemento, ad->elementos[i], param)==0){
            encontrado = 1;
            pos_encontrado = i;
        }
    }
    
    return pos_encontrado;
    
}

int ad_existe_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara, void *param){
    return ad_get_pos_elem(ad, elemento,f_compara, param) == AD_ELEM_NAO_ENCONTRADO ? 
        AD_ELEM_NAO_EXISTE : AD_ELEM_EXISTE;
}

/*
 * SUBSTITUICAO
 */

void ad_substitui_elemento(ARRAY_DINAMICO ad, void *novo_elemento,void *antigo_elemento, ad_compara_elems *f_compara, void *param){
    
    int res_procura = ad_get_pos_elem(ad, antigo_elemento,f_compara, param);
    if(res_procura != AD_ELEM_NAO_ENCONTRADO)
        ad_substitui_elemento_pos(ad,res_procura,novo_elemento);
        
}

void ad_substitui_elemento_pos(ARRAY_DINAMICO ad, int pos, void *novo_elemento){
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    ad->elementos[pos] = novo_elemento;
}

/*
 * REMOCAO
 */

void ad_remove_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara, void *param){
    int posicao = ad_get_pos_elem(ad,elemento,f_compara, param);
    
    if(posicao != AD_ELEM_NAO_ENCONTRADO)
        ad_remove_elemento_pos(ad, posicao);
}

void ad_remove_elemento_pos(ARRAY_DINAMICO ad, int pos){
    int i;
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    
    for(i=pos;i<ad->posicao;i++)
        ad->elementos[i] = ad->elementos[i + 1];
    
    ad->posicao--;
}

void ad_remocao_rapida_elemento(ARRAY_DINAMICO ad, int pos){
    ad_insere_elemento(ad->garbage_collector,ad->elementos[pos]);
    ad->elementos[pos] = ad->elementos[ad->posicao -1];
    ad->posicao--;
}

/*
 * PAGINACAO
 */

AD_PAGINADOR ad_inicializa_paginador_default(ARRAY_DINAMICO ad) {
    AD_PAGINADOR pag_res = (AD_PAGINADOR) malloc(sizeof (struct ad_paginador));
    pag_res->array_paginado     = ad;
    pag_res->posicao_inicial    = AD_PAGINA_IMPOSSIVEL;
    pag_res->num_elems_pag      = AD_PAGINA_IMPOSSIVEL;
    pag_res->num_pag            = AD_PAGINA_IMPOSSIVEL;
    pag_res->elems_por_pag      = AD_ELEMS_POR_PAG_DEFAULT;
    return pag_res;
}

AD_PAGINADOR ad_inicializa_paginador_primeira_pag(ARRAY_DINAMICO ad, int elems_por_pag) {
    AD_PAGINADOR pag_res = ad_inicializa_paginador_pag(ad, 1, elems_por_pag);
    return pag_res;
}

AD_PAGINADOR ad_inicializa_paginador_ultima_pag(ARRAY_DINAMICO ad, int elems_por_pag) {
    AD_PAGINADOR pag_res = ad_inicializa_paginador_pag(ad, 1, elems_por_pag);
    ad_goto_pag(pag_res, ad_get_num_pags(pag_res));
    return pag_res;
}

AD_PAGINADOR ad_inicializa_paginador_pag(ARRAY_DINAMICO ad, int n_pag, int elems_por_pag) {
    AD_PAGINADOR pag_res = (AD_PAGINADOR) malloc(sizeof (struct ad_paginador));
    pag_res->array_paginado     = ad;
    pag_res->elems_por_pag      = (elems_por_pag > 0) ? elems_por_pag : AD_ELEMS_POR_PAG_DEFAULT;
    pag_res->num_elems_pag      = ad_goto_pag(pag_res, n_pag);
    return pag_res;
}

int ad_goto_pag(AD_PAGINADOR pag, int num_pag){
    int comeco_pag = pag->elems_por_pag * (num_pag-1);
    int diferenca = pag->array_paginado->posicao-comeco_pag;
    
    if(diferenca > 0 && num_pag > 0) {
        pag->posicao_inicial = comeco_pag;
        pag->num_elems_pag   = (diferenca > pag->elems_por_pag) ? pag->elems_por_pag : diferenca;
        pag->num_pag         =  num_pag;
    }else{
        pag->posicao_inicial = AD_PAGINA_IMPOSSIVEL;
        pag->num_elems_pag   = AD_PAGINA_IMPOSSIVEL;
        pag->num_pag         = AD_PAGINA_IMPOSSIVEL;
    }
    
    return pag->posicao_inicial;
}

int ad_get_num_elems_pag(AD_PAGINADOR pag){
    return pag->num_elems_pag;
}

int ad_get_pos_inicio_pag(AD_PAGINADOR pag){
    return pag->posicao_inicial;
}

int ad_get_num_pags(AD_PAGINADOR pag){
    int num_elems = pag->array_paginado->posicao;
    int elems_por_pag = pag->elems_por_pag;
    return ((num_elems%elems_por_pag) == 0) ? num_elems / elems_por_pag : (num_elems / elems_por_pag) +1;
}

void *ad_get_elemento_pag(AD_PAGINADOR pag, int n_elem){
    int indice;
    void *resultado = NULL;
    
    if(pag->posicao_inicial != AD_PAGINA_IMPOSSIVEL 
            && n_elem <= pag->num_elems_pag){
        indice = pag->posicao_inicial + (n_elem-1);
        resultado = pag->array_paginado->elementos[indice];
    }else{
        resultado = NULL;
    }
    
    return resultado;
}

void ad_set_num_elems_por_pag(AD_PAGINADOR pag, int new_elems_por_pag){
    if(new_elems_por_pag > 0){
        pag->elems_por_pag = new_elems_por_pag;
        ad_goto_pag(pag, pag->num_pag);
    }
}

int ad_get_elems_por_pag(AD_PAGINADOR pag){
    return pag->elems_por_pag;
}

int ad_get_num_pag(AD_PAGINADOR pag){
    return pag->num_pag;
}

void ad_free_pag(AD_PAGINADOR pag){
    free(pag);
}



/*
 * ORDENACAO
 */

void ad_ordena(ARRAY_DINAMICO ad, ad_compara_elems *f_comparacao, void *param) {
    quicksort(ad->elementos, f_comparacao, ad->posicao, param);
}


/*
 * FUNCOES PRIVADAS AO MODULO
 */

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

void ad_realloc_if_needed(ARRAY_DINAMICO ad){
    int nova_cap;
    void **new_ptr;
    
    if(ad->posicao == ad->capacidade){
        nova_cap = ad->capacidade*2;
        new_ptr = (void **) realloc(ad->elementos,sizeof(void *) * nova_cap);
        
        if (new_ptr != NULL){
            ad->elementos=new_ptr;
            ad->capacidade= nova_cap;
        }
    }
}

void quicksort(void **elems, ad_compara_elems *f_comparacao, int n, void *param){
    int i, j;
    void *p, *temp;
    
    if (n < 2) return;
    p = elems[n/2];
    
    for (i = 0, j = n - 1;; i++, j--) {
        
        while(f_comparacao(elems[i],p, param)<0) i++;
        while(f_comparacao(p,elems[j], param)<0)j--;
        if (i >= j) break;
        
        temp = elems[i];
        elems[i] = elems[j];
        elems[j] = temp;
    }
    quicksort(elems,f_comparacao, i, param);
    quicksort(elems+i,f_comparacao, n - i, param);
}

