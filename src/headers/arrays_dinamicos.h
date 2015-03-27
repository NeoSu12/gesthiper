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

/*
 * INICIALIZACAO E LIBERTACAO MEMORIA
 */

ARRAY_DINAMICO ad_inicializa();
ARRAY_DINAMICO ad_inicializa_cap(int);
void ad_clean_gc(ARRAY_DINAMICO, ad_elimina_elems *);
void ad_deep_free(ARRAY_DINAMICO, ad_elimina_elems *);
void ad_free(ARRAY_DINAMICO);

/*
 * TAMANHO E CAPACIDADE
 */

int ad_get_tamanho(ARRAY_DINAMICO);
int ad_get_capacidade(ARRAY_DINAMICO);

/*
 * INSERCAO
 */

void ad_insere_elemento(ARRAY_DINAMICO, void *);
void ad_insere_elemento_pos(ARRAY_DINAMICO, int, void *);
void ad_insere_elemento_pos_mode(ARRAY_DINAMICO,int, void *, insert_mode_t);
void ad_insere_elemento_ordenado(ARRAY_DINAMICO, void *, ad_compara_elems *);
void ad_insere_elemento_ordenado_mode(ARRAY_DINAMICO, void *, ad_compara_elems *, insert_mode_t );

/*
 * PESQUISA
 */

void *ad_get_elemento(ARRAY_DINAMICO, int);
int ad_get_pos_elem(ARRAY_DINAMICO, void *,ad_compara_elems *);
int ad_existe_elemento(ARRAY_DINAMICO, void *, ad_compara_elems *);

/*
 * SUBSTITUICAO
 */

void ad_substitui_elemento(ARRAY_DINAMICO, void *,void *, ad_compara_elems *);
void ad_substitui_elemento_pos(ARRAY_DINAMICO, int, void *);

/*
 * REMOCAO
 */

void ad_remove_elemento(ARRAY_DINAMICO, void *, ad_compara_elems *);
void ad_remove_elemento_pos(ARRAY_DINAMICO, int);
void ad_remocao_rapida_elemento(ARRAY_DINAMICO, int);

/*
 * PAGINACAO
 */

int ad_goto_pag(ARRAY_DINAMICO, int *, int, int);

/*
 * ORDENACAO
 */

void ad_ordena(ARRAY_DINAMICO, ad_compara_elems *);



#endif	/* ARRAYS_DINAMICOS_H */

