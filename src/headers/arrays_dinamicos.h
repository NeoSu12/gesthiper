#ifndef ARRAYS_DINAMICOS_H
#define	ARRAYS_DINAMICOS_H

#define AD_ELEM_NAO_ENCONTRADO -1
#define AD_ELEM_EXISTE 1
#define AD_ELEM_NAO_EXISTE 0
#define AD_PAGINA_IMPOSSIVEL -1


enum insert_mode{
    KEEP_SIZE=1,
    INCREASE_SIZE
};

typedef enum insert_mode insert_mode_t;

typedef struct array_dinamico* ARRAY_DINAMICO;


typedef int ad_compara_elems(void *item_a, void *item_b);
typedef void ad_elimina_elems(void *item_a);

ARRAY_DINAMICO ad_inicializa();
ARRAY_DINAMICO ad_inicializa_cap(int);

void ad_insere_elemento_pos(ARRAY_DINAMICO ad, int pos, void *elemento);
void ad_insere_elemento_ordenado(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara);
void ad_insere_elemento_pos_mode(ARRAY_DINAMICO ad,int pos, void *elemento, insert_mode_t i_mode);
void ad_insere_elemento(ARRAY_DINAMICO ad, void *elemento);
int ad_procura_elemento(ARRAY_DINAMICO ad, void *elemento,ad_compara_elems *f_compara);
int ad_existe_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara);
void ad_substitui_elemento_pos(ARRAY_DINAMICO ad, int pos, void *novo_elemento);
void ad_substitui_elemento(ARRAY_DINAMICO ad, void *novo_elemento,void *antigo_elemento, ad_compara_elems *f_compara);
void ad_remocao_rapida_elemento(ARRAY_DINAMICO ad, int pos);
void ad_remove_elemento_pos(ARRAY_DINAMICO ad, int pos);
void ad_remove_elemento(ARRAY_DINAMICO ad, void *elemento, ad_compara_elems *f_compara);
int ad_get_tamanho(ARRAY_DINAMICO ad);
int ad_get_capacidade(ARRAY_DINAMICO ad);
void *ad_get_elemento(ARRAY_DINAMICO ad, int pos);
int ad_goto_pag(ARRAY_DINAMICO ad, int *pos_inicial, int pag, int elems_por_pag);
void ad_ordena(ARRAY_DINAMICO ad, ad_compara_elems *f_comparacao);
void ad_deep_free(ARRAY_DINAMICO ad, ad_elimina_elems *f_eliminacao);
void ad_free(ARRAY_DINAMICO ad);

#endif	/* ARRAYS_DINAMICOS_H */

