#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL 0
#define PROMO 1

struct modulo_compras{
	ARVORE avl_clientes;
};

struct compras_ficha_cliente{
	char *cod_cliente;
	ARVORE avl_produtos;
        int total_prods_comprados;
	int num_prods_comprados[12];
};

struct compras_ficha_produto{
	char *cod_produto;
	/* Numero de compras para cada mes,
         * distinguindo entre normal e promocao.*/
        int total_unidades_compradas;
	int num_unidades_compradas[12][2];
};

struct compras_iterador_clientes{
    TRAVERSER traverser;
};

struct compras_iterador_produtos{
    TRAVERSER traverser;
};

struct compras_lista_clientes{
    ARRAY_DINAMICO lista_paginada;
    int elems_por_pag;
};

struct compras_lista_produtos{
    ARRAY_DINAMICO lista_paginada;
    int elems_por_pag;
};

struct compras_num_clientes_mensais{
    int num_clientes[12];
};

struct compras_associacao_produto_clientes{
    char *cod_produto;
    int num_elems_pag;
    ARRAY_DINAMICO associacoes;
};

struct compras_cliente_tipo_compra{
    char *cod_cliente;
    char tipo_compra;
};

COMPRAS_FICHA_PRODUTO compras_inicializa_ficha_produto(char* cod_prod);
COMPRAS_FICHA_CLIENTE compras_inicializa_ficha_cliente(char* cod_cli);
COMPRAS_FICHA_PRODUTO compras_clone_ficha_produto(COMPRAS_FICHA_PRODUTO src);
COMPRAS_FICHA_CLIENTE compras_clone_ficha_cliente(COMPRAS_FICHA_CLIENTE src);
COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha(char* cod_prod);
COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha(char* cod_cli);
COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl(Compras compras, char *cod_cli);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_cod_avl(Compras compras, char *cod_cli, char *cod_prod);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl(COMPRAS_FICHA_CLIENTE cliente , char *cod_prod);
int compras_compara_fichas_cli_por_cod_cli_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_fichas_prod_por_cod_prod_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_fichas_prod_por_vendas_ad(void *ad_a, void *ad_b, void *param);
int compras_compara_fichas_prod_por_vendas_mes_ad(void *ad_a, void *ad_b, void *param);
void compras_free_produto_avl(void *avl_item, void *avl_param);
void compras_free_cliente_avl(void *avl_item, void *avl_param);
void compras_free_produto(COMPRAS_FICHA_PRODUTO produto);
void compras_free_cliente(COMPRAS_FICHA_CLIENTE cliente);
void compras_troca_meses(int *mes1 , int *mes2);
int compras_cliente_comprou_em_todos_os_meses(COMPRAS_FICHA_CLIENTE cliente);
void compras_free_produto_ad(void *produto);
void compras_free_cliente_ad(void *cliente);
COMPRAS_NUM_CLIENTES_MENSAIS compras_inicializa_num_compras_mensais();

COMPRAS_ASSOC_PROD_CLIENTES compras_inicializa_assoc_prod_clientes(char *cod_produto);
void compras_insere_assoc_cli_compra_em_assoc_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES p_cli, COMPRAS_CLIENTE_TIPO_COMPRA cli_compra);
COMPRAS_CLIENTE_TIPO_COMPRA compras_inicializa_asoc_cliente_tipo_compra(char *cod_cliente, char tipo_compra);
void compras_free_asoc_cliente_tipo_compra_ad(void *item);


/*
 * INCICIALIZACAO E GESTAO MEMORIA
 */

Compras inicializa_compras(){
    Compras res = (Compras) malloc(sizeof(struct modulo_compras));
    res->avl_clientes = avl_create(compras_compara_fichas_cli_por_cod_cli_avl,NULL,NULL);
    return res;
}

void compras_regista_cliente(Compras compras, char *cod_cli){
    COMPRAS_FICHA_CLIENTE cliente = compras_inicializa_ficha_cliente(cod_cli);
    avl_insert(compras->avl_clientes, cliente);
}

void compras_insere_compra(Compras compras, COMPRA comp){
    int indice_mes, promo;
    quantidade_t qtd;
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, get_cod_cliente(comp));
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_fichacli_avl(cliente, get_cod_produto(comp));
    
    if(produto==NULL){
        produto = compras_inicializa_ficha_produto(get_cod_produto(comp));
        avl_insert(cliente->avl_produtos,produto);
    }
    
    if(get_promo(comp) == 'N') promo = NORMAL;
    else promo = PROMO;
    
    indice_mes = get_mes(comp)-1;
    qtd = get_quantidade(comp);
    
    cliente->total_prods_comprados                      += qtd;
    cliente->num_prods_comprados[indice_mes]            += qtd;
    
    produto->total_unidades_compradas                   += qtd;
    produto->num_unidades_compradas[indice_mes][promo]  += qtd;
    
    
}

void free_compras(Compras compras){
    avl_destroy(compras->avl_clientes, compras_free_cliente_avl);
    free(compras);
}

/*
 * TOTAL PRODUTOS COMPRADOS CLIENTE
 */

int compras_total_produtos_comprados_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente){
    return avl_count(cliente->avl_produtos);
}

int compras_total_unidades_compradas_ficha_cliente(COMPRAS_FICHA_CLIENTE cliente){
    return cliente->total_prods_comprados;
}

int compras_produtos_comprados_ficha_cliente_meses(COMPRAS_FICHA_CLIENTE cliente, int mes_inf, int mes_sup){
    int i=0, res=0;
    if (mes_inf > mes_sup) compras_troca_meses(&mes_inf, &mes_sup);
    
    for(i=mes_inf;i<=mes_sup;i++){
        res += cliente->num_prods_comprados[i-1];
    }
    return res;
}

int compras_produtos_comprados_ficha_cliente_mes(COMPRAS_FICHA_CLIENTE cliente, int mes){
    return compras_produtos_comprados_ficha_cliente_meses(cliente, mes, mes);
}

int compras_produtos_comprados_cod_cliente_meses(Compras compras, char *cod_cliente, int mes_inf, int mes_sup){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return compras_produtos_comprados_ficha_cliente_meses(cliente, mes_inf, mes_sup);
}

int compras_produtos_comprados_cod_cliente_mes(Compras compras, char *cod_cliente, int mes){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return compras_produtos_comprados_ficha_cliente_meses(cliente, mes, mes);
}

/*
 * PRODUTO COMPRADO POR CLIENTE
 */

int compras_total_produtos_comprados_ficha(COMPRAS_FICHA_PRODUTO produto){
    return produto->total_unidades_compradas;
}

int compras_produtos_comprados_ficha_produto_meses_geral(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup, 
                                                            compras_campo_t campo){
    
    int i=0, res=0;
    if (mes_inf > mes_sup) compras_troca_meses(&mes_inf, &mes_sup);
    
    
    switch (campo) {
            case COMPRAS_NORMAL:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->num_unidades_compradas[i - 1][NORMAL];
                break;
            case COMPRAS_PROMO:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->num_unidades_compradas[i - 1][PROMO];
                break;
            case COMPRAS_AMBOS:
                for (i = mes_inf; i <= mes_sup; i++)
                    res += produto->num_unidades_compradas[i - 1][NORMAL] +
                        produto->num_unidades_compradas[i - 1][PROMO];
                break;
            default:
                break;
        }
    
    return res;
}

int compras_produtos_comprados_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_AMBOS);
}

int compras_produtos_comprados_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_AMBOS);
}

int compras_produtos_comprados_normal_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_NORMAL);
}

int compras_produtos_comprados_normal_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_NORMAL);
}

int compras_produtos_comprados_promo_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_PROMO);
}

int compras_produtos_comprados_promo_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, 
                                                            int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_PROMO);
}

/*===================================================*/

int compras_produtos_comprados_cod_produto_meses_geral(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup, 
                                                            compras_campo_t campo){
    
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_cod_avl(compras, cod_cli, cod_prod);
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, campo);
}

int compras_produtos_comprados_cod_produto_meses(Compras compras, 
                                                    char *cod_cli, char *cod_prod, 
                                                    int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_AMBOS);
}

int compras_produtos_comprados_cod_produto_mes(Compras compras, 
                                                    char *cod_cli, char *cod_prod, 
                                                    int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_AMBOS);
}

int compras_produtos_comprados_normal_cod_produto_meses(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_NORMAL);
}

int compras_produtos_comprados_normal_cod_produto_mes(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_NORMAL);
}

int compras_produtos_comprados_promo_cod_produto_meses(Compras compras, 
                                                            char *cod_cli, char *cod_prod, 
                                                            int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_PROMO);
}

int compras_produtos_comprados_promo_cod_produto_mes(Compras compras, 
                                                        char *cod_cli, char *cod_prod, 
                                                        int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_PROMO);
}

/*
 * ITERADORES CLIENTES
 */

IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes(Compras compras) {
    IT_COMPRAS_CLIENTES it = (IT_COMPRAS_CLIENTES) malloc(sizeof (struct compras_iterador_clientes));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, compras->avl_clientes);
    return it;
}

IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_primeiro(Compras compras) {
    IT_COMPRAS_CLIENTES it = (IT_COMPRAS_CLIENTES) malloc(sizeof (struct compras_iterador_clientes));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, compras->avl_clientes);
    return it;
}

IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_ultimo(Compras compras) {
    IT_COMPRAS_CLIENTES it;
    it = (IT_COMPRAS_CLIENTES) malloc(sizeof (struct compras_iterador_clientes));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, compras->avl_clientes);
    return it;
}

IT_COMPRAS_CLIENTES inicializa_it_compras_fich_clientes_elem(Compras compras, char *st) {
    IT_COMPRAS_CLIENTES it = NULL;
    COMPRAS_FICHA_CLIENTE procura = NULL;
    
    if (st != NULL) {
        it = (IT_COMPRAS_CLIENTES) malloc(sizeof (struct compras_iterador_clientes));
        it->traverser = avl_t_alloc();
        procura = compras_codigo_cliente_to_ficha(st);
        avl_t_find(it->traverser, compras->avl_clientes, procura);
        compras_free_cliente(procura);
    }
    
    return it;
}

COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_actual(IT_COMPRAS_CLIENTES it) {
    COMPRAS_FICHA_CLIENTE ret = NULL;
    COMPRAS_FICHA_CLIENTE res = avl_t_cur(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_cliente(res);

    return ret;
}

COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_proximo(IT_COMPRAS_CLIENTES it) {
    COMPRAS_FICHA_CLIENTE ret = NULL;
    COMPRAS_FICHA_CLIENTE res = avl_t_next(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_cliente(res);

    return ret;
}

COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_anterior(IT_COMPRAS_CLIENTES it) {
    COMPRAS_FICHA_CLIENTE ret = NULL;
    COMPRAS_FICHA_CLIENTE res = avl_t_prev(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_cliente(res);

    return ret;
}

void free_it_compras_fich_cliente(IT_COMPRAS_CLIENTES it){
    avl_t_free(it->traverser);
    free(it);
}

/*
 * ITERADORES PRODUTOS
 */

IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos(COMPRAS_FICHA_CLIENTE cliente) {
    IT_COMPRAS_PRODUTOS it = (IT_COMPRAS_PRODUTOS) malloc(sizeof (struct compras_iterador_produtos));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, cliente->avl_produtos);
    return it;
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_primeiro(COMPRAS_FICHA_CLIENTE cliente) {
    IT_COMPRAS_PRODUTOS it = (IT_COMPRAS_PRODUTOS) malloc(sizeof (struct compras_iterador_produtos));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, cliente->avl_produtos);
    return it;
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_ultimo(COMPRAS_FICHA_CLIENTE cliente) {
    IT_COMPRAS_PRODUTOS it;
    it = (IT_COMPRAS_PRODUTOS) malloc(sizeof (struct compras_iterador_produtos));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, cliente->avl_produtos);
    return it;
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_fich_produtos_elem(COMPRAS_FICHA_CLIENTE cliente, char *st) {
    IT_COMPRAS_PRODUTOS it = NULL;
    COMPRAS_FICHA_PRODUTO procura = NULL;
    
    if (st != NULL) {
        it = (IT_COMPRAS_PRODUTOS) malloc(sizeof (struct compras_iterador_produtos));
        it->traverser = avl_t_alloc();
        procura = compras_codigo_produto_to_ficha(st);
        avl_t_find(it->traverser, cliente->avl_produtos, procura);
        compras_free_produto(procura);
    }
    
    return it;
}

COMPRAS_FICHA_PRODUTO it_compras_fich_produto_actual(IT_COMPRAS_PRODUTOS it) {
    COMPRAS_FICHA_PRODUTO ret = NULL;
    COMPRAS_FICHA_PRODUTO res = avl_t_cur(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_produto(res);

    return ret;
}

COMPRAS_FICHA_PRODUTO it_compras_fich_produto_proximo(IT_COMPRAS_PRODUTOS it) {
    COMPRAS_FICHA_PRODUTO ret = NULL;
    COMPRAS_FICHA_PRODUTO res = avl_t_next(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_produto(res);

    return ret;
}

COMPRAS_FICHA_PRODUTO it_compras_fich_produto_anterior(IT_COMPRAS_PRODUTOS it) {
    COMPRAS_FICHA_PRODUTO ret = NULL;
    COMPRAS_FICHA_PRODUTO res = avl_t_prev(it->traverser);

    if (res != NULL)
        ret = compras_clone_ficha_produto(res);

    return ret;
}

void free_it_compras_fich_produto(IT_COMPRAS_PRODUTOS it){
    avl_t_free(it->traverser);
    free(it);
}

/*
 * ITERADORES PRODUTOS COM CODIGO
 */

IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos(Compras compras, char *cod_cliente) {
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return inicializa_it_compras_fich_produtos(cliente);
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_primeiro(Compras compras, char *cod_cliente) {
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return inicializa_it_compras_fich_produtos_primeiro(cliente);
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_ultimo(Compras compras, char *cod_cliente) {
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return inicializa_it_compras_fich_produtos_ultimo(cliente);
}

IT_COMPRAS_PRODUTOS inicializa_it_compras_cod_produtos_elem(Compras compras, char *cod_cliente, char *cod_produto) {
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    return inicializa_it_compras_fich_produtos_elem(cliente, cod_produto);
}


/*
 * Pedidos aos modulos
 */

COMPRAS_LISTA_CLIENTES compras_lista_clientes_regulares(Compras compras){
    COMPRAS_FICHA_CLIENTE cliente;
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    COMPRAS_LISTA_CLIENTES l_clientes = (COMPRAS_LISTA_CLIENTES) malloc(sizeof(struct compras_lista_clientes));
    ARRAY_DINAMICO ad = ad_inicializa_cap(1000);
    
    while((cliente = it_compras_fich_cliente_proximo(it))!=NULL){
        if(compras_cliente_comprou_em_todos_os_meses(cliente)){
            ad_insere_elemento(ad,cliente);
        }else{
            compras_free_cliente(cliente);
        }
    }
    
    l_clientes->lista_paginada=ad;
    l_clientes->elems_por_pag=20;
    free_it_compras_fich_cliente(it);
    return l_clientes;
}

COMPRAS_LISTA_PRODUTOS compras_produtos_mais_comprados_cliente_mes(Compras compras, char *cod_cliente, int mes){
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    COMPRAS_FICHA_PRODUTO produto = NULL;
    IT_COMPRAS_PRODUTOS it = NULL;
    COMPRAS_LISTA_PRODUTOS l_produtos = (COMPRAS_LISTA_PRODUTOS) malloc(sizeof(struct compras_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa_cap(1000);
    
    cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    it = inicializa_it_compras_fich_produtos(cliente);
    
    
    while((produto = it_compras_fich_produto_proximo(it))!=NULL){
            ad_insere_elemento(ad,produto);
    }
    
    ad_ordena(ad, compras_compara_fichas_prod_por_vendas_ad, &mes);
    l_produtos->lista_paginada=ad;
    l_produtos->elems_por_pag=20;
    return l_produtos;
}

COMPRAS_LISTA_PRODUTOS compras_top_n_produtos_mais_comprados_cliente(Compras compras, char *cod_cliente, int n){
    int i, total_prods_comprados;
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    COMPRAS_FICHA_PRODUTO produto = NULL;
    IT_COMPRAS_PRODUTOS it = NULL;
    COMPRAS_FICHA_PRODUTO *tops = (COMPRAS_FICHA_PRODUTO *) malloc(sizeof(struct compras_ficha_produto)*n);
    COMPRAS_LISTA_PRODUTOS l_produtos = (COMPRAS_LISTA_PRODUTOS) malloc(sizeof(struct compras_lista_produtos));
    ARRAY_DINAMICO ad = ad_inicializa_cap(2048);
    ARRAY_DINAMICO lista_top = ad_inicializa_cap(64);
    
    cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cliente);
    total_prods_comprados = compras_total_produtos_comprados_ficha_cliente(cliente);
    it = inicializa_it_compras_fich_produtos(cliente);
    
    while((produto = it_compras_fich_produto_proximo(it))!=NULL){
            ad_insere_elemento(ad,produto);
    }
    
    ad_ordena(ad, compras_compara_fichas_prod_por_vendas_ad, NULL);
    
    for(i=0;i<n && i< total_prods_comprados;i++){
        tops[i] = compras_clone_ficha_produto(ad_get_elemento(ad,i));
        ad_insere_elemento(lista_top, tops[i]);
    }

    l_produtos->lista_paginada=lista_top;
    l_produtos->elems_por_pag=20;
    
    ad_deep_free(ad, compras_free_produto_ad);
    return l_produtos;
}

COMPRAS_NUM_CLIENTES_MENSAIS compras_num_clientes_por_mes(Compras compras){
    int i, n_compras;
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    IT_COMPRAS_CLIENTES it = NULL;
    COMPRAS_NUM_CLIENTES_MENSAIS res = compras_inicializa_num_compras_mensais();
    
    it = inicializa_it_compras_fich_clientes(compras);
    
    while((cliente = it_compras_fich_cliente_proximo(it))!=NULL){
        for(i=0;i<12;i++){
            n_compras = compras_produtos_comprados_ficha_cliente_mes(cliente,i+1);
            
            if(n_compras > 0) 
                res->num_clientes[i]++;
        }
        compras_free_cliente(cliente);
    }
    
    free_it_compras_fich_cliente(it);
    return res;
}

int compras_get_num_clientes_mes(COMPRAS_NUM_CLIENTES_MENSAIS compras_meses, int mes){
    return (mes >=1 && mes<=12)? compras_meses->num_clientes[mes-1] : 0;
}

int compras_num_cliente_sem_compras(Compras compras){
    int res=0;
    COMPRAS_FICHA_CLIENTE cliente;
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    
    while((cliente = it_compras_fich_cliente_proximo(it))!=NULL){
        if(compras_total_produtos_comprados_ficha_cliente(cliente))
            res++;
        
        compras_free_cliente(cliente);
    }
    
    free_it_compras_fich_cliente(it);
    return res;
}

/*
 * ASSOCIACOES
 */

COMPRAS_ASSOC_PROD_CLIENTES compras_get_associacao_produto_clientes_tipo_compra(Compras compras, char *cod_produto){
    int soma_normal=0, soma_promo=0;
    char tipo_compra = 'X';
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    COMPRAS_FICHA_PRODUTO produto = NULL;
    COMPRAS_CLIENTE_TIPO_COMPRA assoc_cli_compra = NULL;
    COMPRAS_ASSOC_PROD_CLIENTES assoc_prod_cli = compras_inicializa_assoc_prod_clientes(cod_produto);        
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    
    
    while((cliente = it_compras_fich_cliente_proximo(it))!=NULL){
        produto = compras_procura_ficha_produto_com_fichacli_avl(cliente, cod_produto);
        
        if(produto != NULL){
            soma_normal = compras_produtos_comprados_normal_ficha_produto_meses(produto, 1, 12);
            soma_promo = compras_produtos_comprados_promo_ficha_produto_meses(produto, 1, 12);
            
            if(soma_normal>0 && soma_promo>0) tipo_compra = 'A';
            else if(soma_normal>0) tipo_compra = 'N';
            else if(soma_promo>0) tipo_compra = 'P';
            
            assoc_cli_compra = compras_inicializa_asoc_cliente_tipo_compra(cliente->cod_cliente, tipo_compra);
            compras_insere_assoc_cli_compra_em_assoc_prod_cli(assoc_prod_cli, assoc_cli_compra);
            
        }
        
        compras_free_cliente(cliente);
    }
    
    assoc_prod_cli->num_elems_pag=20;
    free_it_compras_fich_cliente(it);
    return assoc_prod_cli;
}

COMPRAS_CLIENTE_TIPO_COMPRA compras_get_cli_compra_from_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES lista,int p){
    return (COMPRAS_CLIENTE_TIPO_COMPRA) ad_get_elemento(lista->associacoes, p);
}

int compras_assoc_prod_cli_get_pos_and_num_elems_pag(COMPRAS_ASSOC_PROD_CLIENTES lista, int *pos_inicial, int pag){
    return ad_goto_pag(lista->associacoes, pos_inicial, pag, lista->num_elems_pag);
}

int compras_assoc_prod_cli_get_num_pags(COMPRAS_ASSOC_PROD_CLIENTES lista){
    return ad_get_num_pags(lista->associacoes, lista->num_elems_pag);
}

int compras_assoc_prod_cli_get_elems_por_pag(COMPRAS_ASSOC_PROD_CLIENTES lista){
    return lista->num_elems_pag;
}

void compras_assoc_prod_cli_muda_elems_por_pag(COMPRAS_ASSOC_PROD_CLIENTES lista, int n){
    lista->num_elems_pag=n;
}

int compras_assoc_prod_cli_get_num_elems(COMPRAS_ASSOC_PROD_CLIENTES lista){
    return ad_get_tamanho(lista->associacoes);
}

void compras_free_assoc_prod_clientes(COMPRAS_ASSOC_PROD_CLIENTES assoc){
    ad_deep_free(assoc->associacoes, compras_free_asoc_cliente_tipo_compra_ad);
    free(assoc->cod_produto);
    free(assoc);
}

char *compras_assoc_cli_compra_get_cod_cli(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra){
    return cliente_compra->cod_cliente;
}

char compras_assoc_cli_compra_get_tipo_compra(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra){
    return cliente_compra->tipo_compra;
}


/*
 * LISTAS CLIENTE
 */

char *compras_get_cod_cli_ficha(COMPRAS_FICHA_CLIENTE cliente){
    return cliente->cod_cliente;
}

COMPRAS_FICHA_CLIENTE compras_lista_get_fich_cli(COMPRAS_LISTA_CLIENTES lista,int p){
    return (COMPRAS_FICHA_CLIENTE) ad_get_elemento(lista->lista_paginada, p);
}

int compras_lista_cli_get_pos_and_num_elems_pag(COMPRAS_LISTA_CLIENTES lista, int *pos_inicial, int pag){
    return ad_goto_pag(lista->lista_paginada, pos_inicial, pag, lista->elems_por_pag);
}

int compras_lista_cli_get_num_pags(COMPRAS_LISTA_CLIENTES lista){
    return ad_get_num_pags(lista->lista_paginada, lista->elems_por_pag);
}

int compras_lista_cli_get_elems_por_pag(COMPRAS_LISTA_CLIENTES lista){
    return lista->elems_por_pag;
}

void compras_lista_cli_muda_elems_por_pag(COMPRAS_LISTA_CLIENTES lista, int n){
    lista->elems_por_pag=n;
}

int compras_lista_cli_get_num_elems(COMPRAS_LISTA_CLIENTES lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void compras_free_lista_clientes(COMPRAS_LISTA_CLIENTES lista){
    ad_deep_free(lista->lista_paginada, compras_free_cliente_ad);
    free(lista);
}


/*
 * LISTAS PRODUTO
 */


char *compras_get_cod_prod_ficha(COMPRAS_FICHA_PRODUTO produto){
    return produto->cod_produto;
}

COMPRAS_FICHA_PRODUTO compras_lista_get_fich_prod(COMPRAS_LISTA_PRODUTOS lista,int p){
    return (COMPRAS_FICHA_PRODUTO) ad_get_elemento(lista->lista_paginada, p);
}

int compras_lista_prod_get_pos_and_num_elems_pag(COMPRAS_LISTA_PRODUTOS lista, int *pos_inicial, int pag){
    return ad_goto_pag(lista->lista_paginada, pos_inicial, pag, lista->elems_por_pag);
}

int compras_lista_prod_get_num_pags(COMPRAS_LISTA_PRODUTOS lista){
    return ad_get_num_pags(lista->lista_paginada, lista->elems_por_pag);
}

int compras_lista_prod_get_elems_por_pag(COMPRAS_LISTA_PRODUTOS lista){
    return lista->elems_por_pag;
}

void compras_lista_prod_muda_elems_por_pag(COMPRAS_LISTA_PRODUTOS lista, int n){
    lista->elems_por_pag=n;
}

int compras_lista_prod_get_num_elems(COMPRAS_LISTA_PRODUTOS lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void compras_free_lista_produtos(COMPRAS_LISTA_PRODUTOS lista){
    ad_deep_free(lista->lista_paginada, compras_free_produto_ad);
    free(lista);
}


/*
 * FUNCOES AUXILIARES PRIVADAS AO MODULO
 */

COMPRAS_FICHA_PRODUTO compras_inicializa_ficha_produto(char* cod_prod) {
    int i, j;
    COMPRAS_FICHA_PRODUTO prod = (COMPRAS_FICHA_PRODUTO) malloc(sizeof (struct compras_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    
    for (i = 0; i < 12; i++){
        for (j = 0; j < 2; j++) {
            prod->num_unidades_compradas[i][j] = 0;
        }
    }
    prod->total_unidades_compradas=0;
    return prod;
}

COMPRAS_FICHA_CLIENTE compras_inicializa_ficha_cliente(char* cod_cli) {
    int i;
    COMPRAS_FICHA_CLIENTE cli = (COMPRAS_FICHA_CLIENTE) malloc(sizeof (struct compras_ficha_cliente));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_cli) + 1));
    strcpy(copia, cod_cli);
    
    cli->cod_cliente = copia;
    cli->avl_produtos = avl_create(compras_compara_fichas_prod_por_cod_prod_avl, NULL, NULL);
    
    for (i = 0; i < 12; i++) 
        cli->num_prods_comprados[i]=0;
    
    cli->total_prods_comprados=0;
    
    
    return cli;
}

COMPRAS_FICHA_PRODUTO compras_clone_ficha_produto(COMPRAS_FICHA_PRODUTO src){
    int i, j;
    COMPRAS_FICHA_PRODUTO dest = (COMPRAS_FICHA_PRODUTO) malloc(sizeof (struct compras_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(src->cod_produto) + 1));
    strcpy(copia, src->cod_produto);
    dest->cod_produto = copia;
    
    for (i = 0; i < 12; i++){
        for (j = 0; j < 2; j++) {
            dest->num_unidades_compradas[i][j] = src->num_unidades_compradas[i][j];
        }
    }
    
    dest->total_unidades_compradas = src->total_unidades_compradas;
    return dest;
    
}

COMPRAS_FICHA_CLIENTE compras_clone_ficha_cliente(COMPRAS_FICHA_CLIENTE src){
    int i;
    COMPRAS_FICHA_CLIENTE dest = (COMPRAS_FICHA_CLIENTE) malloc(sizeof (struct compras_ficha_cliente));
    char *copia = (char*) malloc(sizeof (char)*(strlen(src->cod_cliente) + 1));
    strcpy(copia, src->cod_cliente);
    dest->cod_cliente = copia;
    
    for (i = 0; i < 12; i++){
            dest->num_prods_comprados[i] = src->num_prods_comprados[i];
    }
    
    dest->total_prods_comprados = src->total_prods_comprados;
    dest->avl_produtos=src->avl_produtos;
    return dest;
    
}

COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha(char* cod_prod) {
    COMPRAS_FICHA_PRODUTO prod = (COMPRAS_FICHA_PRODUTO) malloc(sizeof (struct compras_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    return prod;
}

COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha(char* cod_cli) {
    COMPRAS_FICHA_CLIENTE cli = (COMPRAS_FICHA_CLIENTE) malloc(sizeof (struct compras_ficha_cliente));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_cli) + 1));
    strcpy(copia, cod_cli);
    cli->cod_cliente = copia;
    return cli;
}

COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl(Compras compras, char *cod_cli){
    COMPRAS_FICHA_CLIENTE nodo_aux = compras_codigo_cliente_to_ficha(cod_cli);
    COMPRAS_FICHA_CLIENTE res = (COMPRAS_FICHA_CLIENTE) avl_find(compras->avl_clientes, nodo_aux);
    free(nodo_aux->cod_cliente);
    free(nodo_aux);
    return res;
}

COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_cod_avl(Compras compras, char *cod_cli, char *cod_prod){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cli);
    
    COMPRAS_FICHA_PRODUTO nodo_aux = compras_codigo_produto_to_ficha(cod_prod);
    COMPRAS_FICHA_PRODUTO res = (COMPRAS_FICHA_PRODUTO) avl_find(cliente->avl_produtos, nodo_aux);
    free(nodo_aux->cod_produto);
    free(nodo_aux);
    return res;
}

COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl(COMPRAS_FICHA_CLIENTE cliente , char *cod_prod){
    COMPRAS_FICHA_PRODUTO nodo_aux = compras_codigo_produto_to_ficha(cod_prod);
    COMPRAS_FICHA_PRODUTO res = (COMPRAS_FICHA_PRODUTO) avl_find(cliente->avl_produtos, nodo_aux);
    free(nodo_aux->cod_produto);
    free(nodo_aux);
    return res;
}

int compras_compara_fichas_cli_por_cod_cli_avl(const void *avl_a, const void *avl_b, void *param){
    COMPRAS_FICHA_CLIENTE a = (COMPRAS_FICHA_CLIENTE) avl_a;
    COMPRAS_FICHA_CLIENTE b = (COMPRAS_FICHA_CLIENTE) avl_b;
    return strcmp(a->cod_cliente, b->cod_cliente);
}

int compras_compara_fichas_prod_por_cod_prod_avl(const void *avl_a, const void *avl_b, void *param){
    COMPRAS_FICHA_PRODUTO a = (COMPRAS_FICHA_PRODUTO) avl_a;
    COMPRAS_FICHA_PRODUTO b = (COMPRAS_FICHA_PRODUTO) avl_b;
    return strcmp(a->cod_produto, b->cod_produto);
}

int compras_compara_fichas_prod_por_vendas_ad(void *ad_a, void *ad_b, void *param){
    int res=1;
    COMPRAS_FICHA_PRODUTO a = (COMPRAS_FICHA_PRODUTO) ad_a;
    COMPRAS_FICHA_PRODUTO b = (COMPRAS_FICHA_PRODUTO) ad_b;
    
    if(a->total_unidades_compradas > b->total_unidades_compradas) res = -1;
    else if(a->total_unidades_compradas == b->total_unidades_compradas)res = 0;
    else res = 1;
    return res;
}

int compras_compara_fichas_prod_por_vendas_mes_ad(void *ad_a, void *ad_b, void *param){
    int res=1;
    COMPRAS_FICHA_PRODUTO a = (COMPRAS_FICHA_PRODUTO) ad_a;
    COMPRAS_FICHA_PRODUTO b = (COMPRAS_FICHA_PRODUTO) ad_b;
    int mes =  *((int *)param);
    int comprados_a = compras_produtos_comprados_ficha_produto_mes(a, mes);
    int comprados_b = compras_produtos_comprados_ficha_produto_mes(b, mes);
    
    if(comprados_a > comprados_b) res = -1;
    else if(comprados_a == comprados_b)res = 0;
    else res = 1;
    return res;
}

void compras_free_produto_avl(void *avl_item, void *avl_param){
    COMPRAS_FICHA_PRODUTO produto = (COMPRAS_FICHA_PRODUTO) avl_item;
    free(produto->cod_produto);
    free(produto);
}

void compras_free_cliente_avl(void *avl_item, void *avl_param){
    COMPRAS_FICHA_CLIENTE cliente = (COMPRAS_FICHA_CLIENTE) avl_item;
    free(cliente->cod_cliente);
    avl_destroy(cliente->avl_produtos, compras_free_produto_avl);
    free(cliente);
}

void compras_free_produto(COMPRAS_FICHA_PRODUTO produto){
    free(produto->cod_produto);
    free(produto);
}

void compras_free_cliente(COMPRAS_FICHA_CLIENTE cliente){
    free(cliente->cod_cliente);
    free(cliente);
}

void compras_free_produto_ad(void *produto){
    COMPRAS_FICHA_PRODUTO p = (COMPRAS_FICHA_PRODUTO) produto;
    free(p->cod_produto);
    free(p);
}

void compras_free_cliente_ad(void *cliente){
    COMPRAS_FICHA_CLIENTE c = (COMPRAS_FICHA_CLIENTE) c;
    free(c->cod_cliente);
    free(c);
}

void compras_troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}

int compras_cliente_comprou_em_todos_os_meses(COMPRAS_FICHA_CLIENTE cliente){
    int i=0, comprou=1;
    
    for(i=0;i<12 && comprou;i++){
        if(compras_produtos_comprados_ficha_cliente_mes(cliente, i+1)==0){
            comprou=0;
        }
    }
    return comprou;
}

COMPRAS_NUM_CLIENTES_MENSAIS compras_inicializa_num_compras_mensais(){
    int i;
    COMPRAS_NUM_CLIENTES_MENSAIS res = (COMPRAS_NUM_CLIENTES_MENSAIS) malloc(sizeof(struct compras_num_clientes_mensais));
    for(i=0;i<12;i++) res->num_clientes[i]=0;
    return res;
}


COMPRAS_ASSOC_PROD_CLIENTES compras_inicializa_assoc_prod_clientes(char *cod_produto){
    char *copia;
    COMPRAS_ASSOC_PROD_CLIENTES res = (COMPRAS_ASSOC_PROD_CLIENTES) malloc(sizeof(struct compras_associacao_produto_clientes));
    copia = (char *) malloc(sizeof(char)*(strlen(cod_produto)+1));
    strcpy(copia, cod_produto);
    res->cod_produto = copia;
    res->associacoes = ad_inicializa_cap(1024);
    return res;
}

void compras_insere_assoc_cli_compra_em_assoc_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES p_cli, COMPRAS_CLIENTE_TIPO_COMPRA cli_compra){
    ad_insere_elemento(p_cli->associacoes, cli_compra);
}

COMPRAS_CLIENTE_TIPO_COMPRA compras_inicializa_asoc_cliente_tipo_compra(char *cod_cliente, char tipo_compra) {
    char *copia;
    COMPRAS_CLIENTE_TIPO_COMPRA res = (COMPRAS_CLIENTE_TIPO_COMPRA) malloc(sizeof (struct compras_cliente_tipo_compra));
    copia = (char *) malloc(sizeof (char)*(strlen(cod_cliente) + 1));
    strcpy(copia, cod_cliente);
    res->cod_cliente = copia;
    res->tipo_compra = tipo_compra;
    return res;
}

void compras_free_asoc_cliente_tipo_compra_ad(void *item) {
    COMPRAS_CLIENTE_TIPO_COMPRA p_apagar = (COMPRAS_CLIENTE_TIPO_COMPRA) item;
    free(p_apagar->cod_cliente);
    free(p_apagar);
}


