#ifndef ARRAYS_DINAMICOS_H
#define	ARRAYS_DINAMICOS_H

#define AD_ELEM_NAO_ENCONTRADO -1
#define AD_ELEM_EXISTE 1
#define AD_ELEM_NAO_EXISTE 0
#define AD_PAGINA_IMPOSSIVEL -1

typedef struct array_dinamico* ARRAY_DINAMICO;

typedef int ad_compara_elems(void *item_a, void *item_b);
typedef void ad_elimina_elems(void *item_a);

ARRAY_DINAMICO ad_inicializa();
ARRAY_DINAMICO ad_inicializa_cap(int);
void ad_insere_elemento(ARRAY_DINAMICO, void *);
int ad_procura_elemento(ARRAY_DINAMICO, void *,ad_compara_elems *);
int ad_existe_elemento(ARRAY_DINAMICO, void *, ad_compara_elems *);
void *ad_get_elemento(ARRAY_DINAMICO, int);
int ad_goto_pag(ARRAY_DINAMICO , int *, int , int);
void ad_remocao_rapida_elemento(ARRAY_DINAMICO, int);
void ad_remove_elemento_pos(ARRAY_DINAMICO, int);
void ad_remove_elemento(ARRAY_DINAMICO, void *, ad_compara_elems *);
void ad_substitui_elemento_pos(ARRAY_DINAMICO, int, void *);
void ad_substitui_elemento(ARRAY_DINAMICO, void *,void *, ad_compara_elems *);
int ad_get_tamanho(ARRAY_DINAMICO);
void ad_ordena(ARRAY_DINAMICO, ad_compara_elems *);
int ad_get_capacidade(ARRAY_DINAMICO);
void ad_deep_free(ARRAY_DINAMICO, ad_elimina_elems *);
void ad_free(ARRAY_DINAMICO);

#endif	/* ARRAYS_DINAMICOS_H */

