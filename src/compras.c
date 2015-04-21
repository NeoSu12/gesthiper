#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/compras.h"
#include "headers/compra.h"
#include "headers/avl.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL 0
#define PROMO 1

struct modulo_compras{
	ARVORE avl_clientes;
        ARVORE avl_produtos_nclientes;
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

struct compras_iterador_produtos_nclientes{
    TRAVERSER traverser;
};

struct compras_lista_clientes{
    ARRAY_DINAMICO lista_paginada;
};

struct compras_paginador_lista_clientes{
    COMPRAS_LISTA_CLIENTES lista_pag;
    AD_PAGINADOR paginador;
};

struct compras_lista_produtos{
    ARRAY_DINAMICO lista_paginada;
};

struct compras_paginador_lista_produtos{
    COMPRAS_LISTA_PRODUTOS lista_pag;
    AD_PAGINADOR paginador;
};

struct compras_num_clientes_mensais{
    int num_clientes[12];
};

struct compras_associacao_produto_clientes{
    char *cod_produto;
    ARRAY_DINAMICO associacoes;
};

struct compras_paginador_associacao_produto_clientes{
    AD_PAGINADOR paginador;
    ARRAY_DINAMICO lista_pag;
};

struct compras_produto_nclientes{
    char *cod_produto;
    int n_clientes;
};

struct compras_cliente_tipo_compra{
    char *cod_cliente;
    char tipo_compra;
};

COMPRAS_FICHA_PRODUTO compras_inicializa_ficha_produto(char *cod_prod);
COMPRAS_FICHA_CLIENTE compras_inicializa_ficha_cliente(char *cod_cli);
COMPRAS_CLIENTE_TIPO_COMPRA compras_inicializa_asoc_cliente_tipo_compra(char *cod_cliente, char tipo_compra);
COMPRAS_PRODUTO_NCLIENTES compras_inicializa_produto_nclientes(char *cod_prod);
COMPRAS_NUM_CLIENTES_MENSAIS compras_inicializa_num_compras_mensais(void);
COMPRAS_ASSOC_PROD_CLIENTES compras_inicializa_assoc_prod_clientes(char *cod_produto);
COMPRAS_FICHA_PRODUTO compras_clone_ficha_produto(COMPRAS_FICHA_PRODUTO src);
COMPRAS_FICHA_CLIENTE compras_clone_ficha_cliente(COMPRAS_FICHA_CLIENTE src);
COMPRAS_PRODUTO_NCLIENTES compras_clone_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES src);
COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha(char *cod_prod);
COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha(char *cod_cli);
COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha_noclone(char *cod_prod);
COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha_noclone(char *cod_cli);
COMPRAS_PRODUTO_NCLIENTES compras_codigo_produto_to_produto_nclientes_noclone(char *cod_prod);
COMPRAS_PRODUTO_NCLIENTES compras_codigo_produto_to_produto_nclientes(char *cod_prod);
COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl(Compras compras, char *cod_cli);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_cod_avl(Compras compras, char *cod_cli, char *cod_prod);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl(COMPRAS_FICHA_CLIENTE cliente, char *cod_prod);
COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl_noclone(Compras compras, char *cod_cli);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl_noclone(COMPRAS_FICHA_CLIENTE cliente, char *cod_prod);
COMPRAS_PRODUTO_NCLIENTES compras_procura_produto_ncli_com_cod_prod_avl_noclone(Compras compras, char *cod_prod);
int compras_compara_fichas_cli_por_cod_cli_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_fichas_prod_por_cod_prod_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_produtos_nclientes_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_fichas_prod_por_vendas_ad(void *ad_a, void *ad_b, void *param);
int compras_compara_fichas_prod_por_vendas_mes_ad(void *ad_a, void *ad_b, void *param);
void compras_free_produto_avl(void *avl_item, void *avl_param);
void compras_free_cliente_avl(void *avl_item, void *avl_param);
void compras_free_produto_nclientes_avl(void *avl_item, void *avl_param);
void compras_free_produto(COMPRAS_FICHA_PRODUTO produto);
void compras_free_cliente(COMPRAS_FICHA_CLIENTE cliente);
void compras_free_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli);
void compras_free_produto_ad(void *produto);
void compras_free_cliente_ad(void *cliente);
void compras_free_produto_nclientes_ad(void *ad_item);
void compras_free_asoc_cliente_tipo_compra_ad(void *item);
COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_proximo_noclone(IT_COMPRAS_CLIENTES it);
COMPRAS_FICHA_PRODUTO it_compras_fich_produto_proximo_noclone(IT_COMPRAS_PRODUTOS it);
COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_proximo_noclone(IT_COMPRAS_PRODUTOS_NCLIENTES it);
void compras_troca_meses(int *mes1, int *mes2);
void compras_insere_assoc_cli_compra_em_assoc_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES p_cli, COMPRAS_CLIENTE_TIPO_COMPRA cli_compra);

/*
 * COMPRAS
 */

Compras inicializa_compras(){
    Compras res = (Compras) malloc(sizeof(struct modulo_compras));
    res->avl_clientes = avl_create(compras_compara_fichas_cli_por_cod_cli_avl,NULL,NULL);
    res->avl_produtos_nclientes = avl_create(compras_compara_produtos_nclientes_avl, NULL, NULL);
    return res;
}

void compras_regista_cliente(Compras compras, char *cod_cli){
    COMPRAS_FICHA_CLIENTE cliente = compras_inicializa_ficha_cliente(cod_cli);
    avl_insert(compras->avl_clientes, cliente);
}

void compras_regista_produto(Compras compras, char *cod_prod){
    COMPRAS_PRODUTO_NCLIENTES cliente = compras_inicializa_produto_nclientes(cod_prod);
    avl_insert(compras->avl_produtos_nclientes, cliente);
}

void compras_insere_compra(Compras compras, COMPRA comp){
    int indice_mes, promo;
    quantidade_t qtd;
    COMPRAS_PRODUTO_NCLIENTES produto_nclientes;
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, get_cod_cliente(comp));
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_fichacli_avl(cliente, get_cod_produto(comp));
    
    if(produto==NULL){
        produto = compras_inicializa_ficha_produto(get_cod_produto(comp));
        avl_insert(cliente->avl_produtos,produto);
        produto_nclientes = compras_procura_produto_ncli_com_cod_prod_avl_noclone(compras, get_cod_produto(comp));
        produto_nclientes->n_clientes++;
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

void compras_remove_cliente(Compras compras, char *cod_cli){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cli);
    COMPRAS_FICHA_PRODUTO produto_it = NULL;
    char *codigo_prod=NULL;
    COMPRAS_PRODUTO_NCLIENTES par = NULL;
    
    IT_COMPRAS_PRODUTOS it = inicializa_it_compras_fich_produtos(cliente);
    
    while((produto_it = it_compras_fich_produto_proximo_noclone(it)) != NULL){
        codigo_prod = compras_get_cod_prod_ficha(produto_it);
        par = compras_procura_produto_ncli_com_cod_prod_avl_noclone(compras, codigo_prod);
        par->n_clientes--;
    }
    free_it_compras_fich_produto(it);
    
    avl_destroy(cliente->avl_produtos, compras_free_produto_avl);
    compras_free_cliente((COMPRAS_FICHA_CLIENTE) avl_delete(compras->avl_clientes, cliente));
}

void compras_remove_produto_de_cliente(Compras compras, char *cod_cli, char *cod_prod){
    COMPRAS_PRODUTO_NCLIENTES par = NULL;
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl(compras, cod_cli);
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_fichacli_avl(cliente, cod_prod);
    
    if(produto != NULL){
    par = compras_procura_produto_ncli_com_cod_prod_avl_noclone(compras, cod_prod);
    par->n_clientes--;
    }
    
    compras_free_produto((COMPRAS_FICHA_PRODUTO)avl_delete(cliente->avl_produtos, produto));
}

void compras_remove_produto(Compras compras, char *cod_prod){
    COMPRAS_FICHA_PRODUTO produto = compras_codigo_produto_to_ficha(cod_prod);
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    COMPRAS_PRODUTO_NCLIENTES par = compras_codigo_produto_to_produto_nclientes_noclone(cod_prod);
    
    avl_delete(compras->avl_produtos_nclientes, par);
    
    while((cliente = it_compras_fich_cliente_proximo_noclone(it))!=NULL){
        compras_free_produto((COMPRAS_FICHA_PRODUTO)avl_delete(cliente->avl_produtos, produto));
    }
    
    free(par);
    free_it_compras_fich_cliente(it);
    compras_free_produto(produto);
}

int compras_num_cliente_sem_compras(Compras compras){
    int res=0;
    COMPRAS_FICHA_CLIENTE cliente;
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    
    while((cliente = it_compras_fich_cliente_proximo(it))!=NULL){
        if(compras_total_produtos_comprados_ficha_cliente(cliente)==0)
            res++;
        
        compras_free_cliente(cliente);
    }
    
    free_it_compras_fich_cliente(it);
    return res;
}

int compras_num_clientes_compraram_prod(Compras compras, char *cod_produto){
    COMPRAS_PRODUTO_NCLIENTES p_ncli = compras_procura_produto_ncli_com_cod_prod_avl_noclone(compras, cod_produto);
    return p_ncli->n_clientes;
}

void free_compras(Compras compras){
    if(compras!=NULL){
        avl_destroy(compras->avl_clientes, compras_free_cliente_avl);
        avl_destroy(compras->avl_produtos_nclientes, compras_free_produto_nclientes_avl);
    }
    
    free(compras);
}

/*
 * PRODUTO -> N CLIENTES
 */

char *compras_get_cod_produto_from_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli){
    return p_ncli->cod_produto;
}

int compras_get_nclientes_from_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli){
    return p_ncli->n_clientes;
}

/*
 * FICHA CLIENTES
 */

char *compras_get_cod_cli_ficha(COMPRAS_FICHA_CLIENTE cliente){
    return cliente->cod_cliente;
}

bool compras_cliente_comprou_em_todos_os_meses(COMPRAS_FICHA_CLIENTE cliente){
    int i=0;
    bool comprou = true;
    
    for(i=0;i<12 && comprou;i++){
        if(compras_produtos_comprados_ficha_cliente_mes(cliente, i+1)==0){
            comprou=false;
        }
    }
    return comprou;
}

/*
 * COMPRAS CLIENTE
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

int compras_total_produtos_comprados_codigo_cliente(Compras compras, char *cod_cliente){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl_noclone(compras, cod_cliente);
    return compras_total_produtos_comprados_ficha_cliente(cliente);
}

int compras_total_unidades_compradas_codigo_cliente(Compras compras, char *cod_cliente){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl_noclone(compras, cod_cliente);
    return compras_total_unidades_compradas_ficha_cliente(cliente);
}

int compras_produtos_comprados_cod_cliente_meses(Compras compras, char *cod_cliente, int mes_inf, int mes_sup){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl_noclone(compras, cod_cliente);
    return compras_produtos_comprados_ficha_cliente_meses(cliente, mes_inf, mes_sup);
}

int compras_produtos_comprados_cod_cliente_mes(Compras compras, char *cod_cliente, int mes){
    COMPRAS_FICHA_CLIENTE cliente = compras_procura_ficha_cliente_com_cod_avl_noclone(compras, cod_cliente);
    return compras_produtos_comprados_ficha_cliente_meses(cliente, mes, mes);
}

/*
 * FICHA PRODUTOS
 */

char *compras_get_cod_prod_ficha(COMPRAS_FICHA_PRODUTO produto){
    return produto->cod_produto;
}

/*
 * COMPRAS PRODUTO POR FICHA
 */

int compras_total_produtos_comprados_ficha(COMPRAS_FICHA_PRODUTO produto){
    return produto->total_unidades_compradas;
}

int compras_produtos_comprados_ficha_produto_meses_geral(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup, compras_campo_t campo){
    
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

int compras_produtos_comprados_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_AMBOS);
}

int compras_produtos_comprados_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_AMBOS);
}

int compras_produtos_comprados_normal_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_NORMAL);
}

int compras_produtos_comprados_normal_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_NORMAL);
}

int compras_produtos_comprados_promo_ficha_produto_meses(COMPRAS_FICHA_PRODUTO produto, int mes_inf, int mes_sup){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, COMPRAS_PROMO);
}

int compras_produtos_comprados_promo_ficha_produto_mes(COMPRAS_FICHA_PRODUTO produto, int mes){
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes, mes, COMPRAS_PROMO);
}

/*
 * COMPRAS PRODUTO POR CODIGO
 */

int compras_total_produtos_comprados_codigo(Compras compras, char *cod_cli, char *cod_prod){
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_cod_avl(compras, cod_cli, cod_prod);
    return compras_total_produtos_comprados_ficha(produto);
}

int compras_produtos_comprados_cod_produto_meses_geral(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup, compras_campo_t campo){
    
    COMPRAS_FICHA_PRODUTO produto = compras_procura_ficha_produto_com_cod_avl(compras, cod_cli, cod_prod);
    return compras_produtos_comprados_ficha_produto_meses_geral(produto, mes_inf, mes_sup, campo);
}

int compras_produtos_comprados_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_AMBOS);
}

int compras_produtos_comprados_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_AMBOS);
}

int compras_produtos_comprados_normal_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_NORMAL);
}

int compras_produtos_comprados_normal_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_NORMAL);
}

int compras_produtos_comprados_promo_cod_produto_meses(Compras compras, char *cod_cli, char *cod_prod, int mes_inf, int mes_sup){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes_inf, mes_sup, COMPRAS_PROMO);
}

int compras_produtos_comprados_promo_cod_produto_mes(Compras compras, char *cod_cli, char *cod_prod, int mes){
    return compras_produtos_comprados_cod_produto_meses_geral(compras, cod_cli, cod_prod, mes, mes, COMPRAS_PROMO);
}


/*
 * LISTA CLIENTES
 */

COMPRAS_LISTA_CLIENTES compras_lista_clientes_regulares(Compras compras){
    COMPRAS_FICHA_CLIENTE cliente_it, cliente;
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    COMPRAS_LISTA_CLIENTES l_clientes = (COMPRAS_LISTA_CLIENTES) malloc(sizeof(struct compras_lista_clientes));
    ARRAY_DINAMICO ad = ad_inicializa_cap(1000);
    
    while((cliente_it = it_compras_fich_cliente_proximo_noclone(it))!=NULL){
        if(compras_cliente_comprou_em_todos_os_meses(cliente_it)){
            cliente = compras_clone_ficha_cliente(cliente_it);
            ad_insere_elemento(ad,cliente);
        }
    }
    
    l_clientes->lista_paginada=ad;
    free_it_compras_fich_cliente(it);
    return l_clientes;
}

COMPRAS_FICHA_CLIENTE compras_lista_get_fich_cli(COMPRAS_LISTA_CLIENTES lista,int p){
    return (COMPRAS_FICHA_CLIENTE) ad_get_elemento(lista->lista_paginada, p);
}

int compras_lista_cli_get_num_elems(COMPRAS_LISTA_CLIENTES lista){
    return ad_get_tamanho(lista->lista_paginada);
}

void compras_free_lista_clientes(COMPRAS_LISTA_CLIENTES lista){
    if(lista != NULL)
        ad_deep_free(lista->lista_paginada, compras_free_cliente_ad);
    
    free(lista);
}


/*
 * LISTA PRODUTOS
 */

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
    free_it_compras_fich_produto(it);
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
    
    while((produto = it_compras_fich_produto_proximo_noclone(it))!=NULL){
            ad_insere_elemento(ad,produto);
    }
    
    ad_ordena(ad, compras_compara_fichas_prod_por_vendas_ad, NULL);
    
    for(i=0;i<n && i< total_prods_comprados;i++){
        tops[i] = compras_clone_ficha_produto(ad_get_elemento(ad,i));
        ad_insere_elemento(lista_top, tops[i]);
    }

    l_produtos->lista_paginada=lista_top;
    
    ad_free(ad);
    free_it_compras_fich_produto(it);
    free(tops);
    return l_produtos;
}

COMPRAS_FICHA_PRODUTO compras_lista_get_fich_prod(COMPRAS_LISTA_PRODUTOS lista,int p){
    return (COMPRAS_FICHA_PRODUTO) ad_get_elemento(lista->lista_paginada, p);
}

void compras_free_lista_produtos(COMPRAS_LISTA_PRODUTOS lista){
    if(lista != NULL)
        ad_deep_free(lista->lista_paginada, compras_free_produto_ad);
    free(lista);
}



/*
 * NUMERO CLIENTES POR MES
 */

COMPRAS_NUM_CLIENTES_MENSAIS compras_num_clientes_por_mes(Compras compras){
    int i, n_compras;
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    IT_COMPRAS_CLIENTES it = NULL;
    COMPRAS_NUM_CLIENTES_MENSAIS res = compras_inicializa_num_compras_mensais();
    
    it = inicializa_it_compras_fich_clientes(compras);
    
    while((cliente = it_compras_fich_cliente_proximo_noclone(it))!=NULL){
        for(i=0;i<12;i++){
            n_compras = compras_produtos_comprados_ficha_cliente_mes(cliente,i+1);   
            if(n_compras > 0)  res->num_clientes[i]++;
        }
    }
    
    free_it_compras_fich_cliente(it);
    return res;
}

int compras_get_num_clientes_mes(COMPRAS_NUM_CLIENTES_MENSAIS compras_meses, int mes){
    return (mes >=1 && mes<=12)? compras_meses->num_clientes[mes-1] : 0;
}

void compras_free_num_clientes_mensais(COMPRAS_NUM_CLIENTES_MENSAIS num_cli_mensais){
    free(num_cli_mensais);
}


/*
 * ASSOCIACAO PRODUTO -> CLIENTES e CLIENTE -> TIPO COMPRA 
 */

COMPRAS_ASSOC_PROD_CLIENTES compras_get_associacao_produto_clientes_tipo_compra(Compras compras, char *cod_produto){
    int soma_normal=0, soma_promo=0;
    char tipo_compra = 'X';
    COMPRAS_FICHA_CLIENTE cliente = NULL;
    COMPRAS_FICHA_PRODUTO produto = NULL;
    COMPRAS_CLIENTE_TIPO_COMPRA assoc_cli_compra = NULL;
    COMPRAS_ASSOC_PROD_CLIENTES assoc_prod_cli = compras_inicializa_assoc_prod_clientes(cod_produto);        
    IT_COMPRAS_CLIENTES it = inicializa_it_compras_fich_clientes(compras);
    
    
    while((cliente = it_compras_fich_cliente_proximo_noclone(it))!=NULL){
        produto = compras_procura_ficha_produto_com_fichacli_avl_noclone(cliente, cod_produto);
        
        if(produto != NULL){
            soma_normal = compras_produtos_comprados_normal_ficha_produto_meses(produto, 1, 12);
            soma_promo = compras_produtos_comprados_promo_ficha_produto_meses(produto, 1, 12);
            
            if(soma_normal>0 && soma_promo>0) tipo_compra = 'A';
            else if(soma_normal>0) tipo_compra = 'N';
            else if(soma_promo>0) tipo_compra = 'P';
            
            assoc_cli_compra = compras_inicializa_asoc_cliente_tipo_compra(cliente->cod_cliente, tipo_compra);
            compras_insere_assoc_cli_compra_em_assoc_prod_cli(assoc_prod_cli, assoc_cli_compra);
            
        }
        
    }
    
    free_it_compras_fich_cliente(it);
    return assoc_prod_cli;
}

COMPRAS_CLIENTE_TIPO_COMPRA compras_get_cli_compra_from_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES lista,int p){
    return (COMPRAS_CLIENTE_TIPO_COMPRA) ad_get_elemento(lista->associacoes, p);
}

int compras_lista_assoc_prod_clis_get_num_elems(COMPRAS_ASSOC_PROD_CLIENTES lista) {
    return ad_get_tamanho(lista->associacoes);
}

char *compras_assoc_cli_compra_get_cod_cli(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra){
    return cliente_compra->cod_cliente;
}

char compras_assoc_cli_compra_get_tipo_compra(COMPRAS_CLIENTE_TIPO_COMPRA cliente_compra){
    return cliente_compra->tipo_compra;
}

void compras_free_assoc_prod_clientes(COMPRAS_ASSOC_PROD_CLIENTES assoc) {
    if (assoc != NULL) {
        ad_deep_free(assoc->associacoes, compras_free_asoc_cliente_tipo_compra_ad);
        free(assoc->cod_produto);
    }
    free(assoc);
}


/*
 * PAGINACAO 
 * ASSOCIACAO PRODUTO -> CLIENTES e CLIENTE -> TIPO COMPRA
 */

COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_default(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc) {
    COMPRAS_PAG_ASSOC_PROD_CLIENTES pag_res = (COMPRAS_PAG_ASSOC_PROD_CLIENTES) malloc(sizeof (struct compras_paginador_associacao_produto_clientes));
    pag_res->lista_pag          = lista_assoc->associacoes;
    pag_res->paginador          = ad_inicializa_paginador_default(pag_res->lista_pag);
    return pag_res;
}

COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_primeira_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int elems_por_pag) {
    COMPRAS_PAG_ASSOC_PROD_CLIENTES pag_res = (COMPRAS_PAG_ASSOC_PROD_CLIENTES) malloc(sizeof (struct compras_paginador_associacao_produto_clientes));
    pag_res->lista_pag          = lista_assoc->associacoes;;
    pag_res->paginador          = ad_inicializa_paginador_primeira_pag(pag_res->lista_pag, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_ultima_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int elems_por_pag) {
    COMPRAS_PAG_ASSOC_PROD_CLIENTES pag_res = (COMPRAS_PAG_ASSOC_PROD_CLIENTES) malloc(sizeof (struct compras_paginador_associacao_produto_clientes));
    pag_res->lista_pag          = lista_assoc->associacoes;;
    pag_res->paginador          = ad_inicializa_paginador_ultima_pag(pag_res->lista_pag, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_ASSOC_PROD_CLIENTES compras_assoc_inicializa_paginador_pag(COMPRAS_ASSOC_PROD_CLIENTES lista_assoc, int n_pag, int elems_por_pag) {
    COMPRAS_PAG_ASSOC_PROD_CLIENTES pag_res = (COMPRAS_PAG_ASSOC_PROD_CLIENTES) malloc(sizeof (struct compras_paginador_associacao_produto_clientes));
    pag_res->lista_pag          = lista_assoc->associacoes;;
    pag_res->paginador          = ad_inicializa_paginador_pag(pag_res->lista_pag ,n_pag, elems_por_pag);
    return pag_res;
}

int compras_lista_assoc_pag_get_num_elems(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag) {
    return ad_get_tamanho(pag->lista_pag);
}

int compras_assoc_prod_cli_pag_get_num_elems_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    return ad_get_num_elems_pag(pag->paginador);
}

int compras_assoc_goto_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int num_pag){
    return ad_goto_pag(pag->paginador, num_pag);
}

int compras_assoc_get_pos_inicio_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    return ad_get_pos_inicio_pag(pag->paginador);
}

int compras_assoc_get_num_pags(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    return ad_get_num_pags(pag->paginador);
}

COMPRAS_CLIENTE_TIPO_COMPRA compras_assoc_get_elemento_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int n_elem){
    return (COMPRAS_CLIENTE_TIPO_COMPRA) ad_get_elemento_pag(pag->paginador, n_elem);
}

void compras_assoc_set_num_elems_por_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag, int new_elems_por_pag){
    ad_set_num_elems_por_pag(pag->paginador, new_elems_por_pag);
}

int compras_assoc_get_elems_por_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    return ad_get_elems_por_pag(pag->paginador);
}

int compras_assoc_get_num_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    return ad_get_num_pag(pag->paginador);
}

void compras_assoc_free_pag(COMPRAS_PAG_ASSOC_PROD_CLIENTES pag){
    if(pag != NULL)
        ad_free_pag(pag->paginador);
    
    free(pag);
}



/*
 * PAGINACAO LISTAS CLIENTE
 */

COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_default(COMPRAS_LISTA_CLIENTES lista_cli) {
    COMPRAS_PAG_LISTA_CLIENTES pag_res = (COMPRAS_PAG_LISTA_CLIENTES) malloc(sizeof (struct compras_paginador_lista_clientes));
    pag_res->lista_pag          = lista_cli;
    pag_res->paginador          = ad_inicializa_paginador_default(pag_res->lista_pag->lista_paginada);
    return pag_res;
}

COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_primeira_pag(COMPRAS_LISTA_CLIENTES lista_cli, int elems_por_pag) {
    COMPRAS_PAG_LISTA_CLIENTES pag_res = (COMPRAS_PAG_LISTA_CLIENTES) malloc(sizeof (struct compras_paginador_lista_clientes));
    pag_res->lista_pag          = lista_cli;
    pag_res->paginador          = ad_inicializa_paginador_primeira_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_ultima_pag(COMPRAS_LISTA_CLIENTES lista_cli, int elems_por_pag) {
    COMPRAS_PAG_LISTA_CLIENTES pag_res = (COMPRAS_PAG_LISTA_CLIENTES) malloc(sizeof (struct compras_paginador_lista_clientes));
    pag_res->lista_pag          = lista_cli;
    pag_res->paginador          = ad_inicializa_paginador_ultima_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_LISTA_CLIENTES compras_cli_inicializa_paginador_pag(COMPRAS_LISTA_CLIENTES lista_cli, int n_pag, int elems_por_pag) {
    COMPRAS_PAG_LISTA_CLIENTES pag_res = (COMPRAS_PAG_LISTA_CLIENTES) malloc(sizeof (struct compras_paginador_lista_clientes));
    pag_res->lista_pag          = lista_cli;
    pag_res->paginador          = ad_inicializa_paginador_pag(pag_res->lista_pag->lista_paginada,n_pag, elems_por_pag);
    return pag_res;
}

int compras_cli_goto_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int num_pag){
    return ad_goto_pag(pag->paginador, num_pag);
}

int compras_cli_pag_get_num_elems(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_tamanho(pag->lista_pag->lista_paginada);
}

int compras_cli_get_pos_inicio_pag(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_pos_inicio_pag(pag->paginador);
}

int compras_cli_get_num_pags(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_num_pags(pag->paginador);
}

COMPRAS_FICHA_CLIENTE compras_cli_get_elemento_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int n_elem){
    return (COMPRAS_FICHA_CLIENTE) ad_get_elemento_pag(pag->paginador, n_elem);
}

int compras_cli_pag_get_num_elems_pag(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_num_elems_pag(pag->paginador);
}

void compras_cli_set_num_elems_por_pag(COMPRAS_PAG_LISTA_CLIENTES pag, int new_elems_por_pag){
    ad_set_num_elems_por_pag(pag->paginador, new_elems_por_pag);
}

int compras_cli_get_elems_por_pag(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_elems_por_pag(pag->paginador);
}

int compras_cli_get_num_pag(COMPRAS_PAG_LISTA_CLIENTES pag){
    return ad_get_num_pag(pag->paginador);
}

void compras_cli_free_pag(COMPRAS_PAG_LISTA_CLIENTES pag){
    if(pag != NULL)
        ad_free_pag(pag->paginador);
    
    free(pag);
}


/*
 * PAGINACAO LISTAS PRODUTO
 */

COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_default(COMPRAS_LISTA_PRODUTOS lista_prod) {
    COMPRAS_PAG_LISTA_PRODUTOS pag_res = (COMPRAS_PAG_LISTA_PRODUTOS) malloc(sizeof (struct compras_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_default(pag_res->lista_pag->lista_paginada);
    return pag_res;
}

COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_primeira_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    COMPRAS_PAG_LISTA_PRODUTOS pag_res = (COMPRAS_PAG_LISTA_PRODUTOS) malloc(sizeof (struct compras_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_primeira_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_ultima_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int elems_por_pag) {
    COMPRAS_PAG_LISTA_PRODUTOS pag_res = (COMPRAS_PAG_LISTA_PRODUTOS) malloc(sizeof (struct compras_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_ultima_pag(pag_res->lista_pag->lista_paginada, elems_por_pag);
    return pag_res;
}

COMPRAS_PAG_LISTA_PRODUTOS compras_prod_inicializa_paginador_pag(COMPRAS_LISTA_PRODUTOS lista_prod, int n_pag, int elems_por_pag) {
    COMPRAS_PAG_LISTA_PRODUTOS pag_res = (COMPRAS_PAG_LISTA_PRODUTOS) malloc(sizeof (struct compras_paginador_lista_produtos));
    pag_res->lista_pag          = lista_prod;
    pag_res->paginador          = ad_inicializa_paginador_pag(pag_res->lista_pag->lista_paginada,n_pag, elems_por_pag);
    return pag_res;
}

int compras_prod_goto_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int num_pag){
    return ad_goto_pag(pag->paginador, num_pag);
}

int compras_lista_prod_get_num_elems(COMPRAS_LISTA_PRODUTOS lista) {
    return ad_get_tamanho(lista->lista_paginada);
}

int compras_prod_pag_get_num_elems(COMPRAS_PAG_LISTA_PRODUTOS pag) {
    return ad_get_tamanho(pag->lista_pag->lista_paginada);
}

int compras_prod_pag_get_num_elems_pag(COMPRAS_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_elems_pag(pag->paginador);
}

int compras_prod_get_pos_inicio_pag(COMPRAS_PAG_LISTA_PRODUTOS pag){
    return ad_get_pos_inicio_pag(pag->paginador);
}

int compras_prod_get_num_pags(COMPRAS_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pags(pag->paginador);
}

COMPRAS_FICHA_PRODUTO compras_prod_get_elemento_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int n_elem){
    return (COMPRAS_FICHA_PRODUTO) ad_get_elemento_pag(pag->paginador, n_elem);
}

void compras_prod_set_num_elems_por_pag(COMPRAS_PAG_LISTA_PRODUTOS pag, int new_elems_por_pag){
    ad_set_num_elems_por_pag(pag->paginador, new_elems_por_pag);
}

int compras_prod_get_elems_por_pag(COMPRAS_PAG_LISTA_PRODUTOS pag){
    return ad_get_elems_por_pag(pag->paginador);
}

int compras_prod_get_num_pag(COMPRAS_PAG_LISTA_PRODUTOS pag){
    return ad_get_num_pag(pag->paginador);
}

void compras_prod_free_pag(COMPRAS_PAG_LISTA_PRODUTOS pag){
    if(pag != NULL)
        ad_free_pag(pag->paginador);
    
    free(pag);
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
    if(it != NULL)
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
    if(it!= NULL)
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
 * ITERADORES PRODUTOS -> NUMERO CLIENTES
 */

IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes(Compras compras) {
    IT_COMPRAS_PRODUTOS_NCLIENTES it = (IT_COMPRAS_PRODUTOS_NCLIENTES) malloc(sizeof (struct compras_iterador_produtos_nclientes));
    it->traverser = avl_t_alloc();
    avl_t_init(it->traverser, compras->avl_produtos_nclientes);
    return it;
}

IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_primeiro(Compras compras) {
    IT_COMPRAS_PRODUTOS_NCLIENTES it = (IT_COMPRAS_PRODUTOS_NCLIENTES) malloc(sizeof (struct compras_iterador_produtos_nclientes));
    it->traverser = avl_t_alloc();
    avl_t_first(it->traverser, compras->avl_produtos_nclientes);
    return it;
}

IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_ultimo(Compras compras) {
    IT_COMPRAS_PRODUTOS_NCLIENTES it;
    it = (IT_COMPRAS_PRODUTOS_NCLIENTES) malloc(sizeof (struct compras_iterador_produtos_nclientes));
    it->traverser = avl_t_alloc();
    avl_t_last(it->traverser, compras->avl_produtos_nclientes);
    return it;
}

IT_COMPRAS_PRODUTOS_NCLIENTES inicializa_it_compras_produtos_nclientes_elem(Compras compras, char *st) {
    IT_COMPRAS_PRODUTOS_NCLIENTES it = NULL;
    COMPRAS_PRODUTO_NCLIENTES procura = NULL;
    
    if (st != NULL) {
        it = (IT_COMPRAS_PRODUTOS_NCLIENTES) malloc(sizeof (struct compras_iterador_produtos_nclientes));
        it->traverser = avl_t_alloc();
        procura = compras_codigo_produto_to_produto_nclientes_noclone(st);
        avl_t_find(it->traverser, compras->avl_produtos_nclientes, procura);
        compras_free_produto_nclientes(procura);
    }
    
    return it;
}

COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_actual(IT_COMPRAS_PRODUTOS_NCLIENTES it) {
    COMPRAS_PRODUTO_NCLIENTES ret = NULL;
    COMPRAS_PRODUTO_NCLIENTES res = avl_t_cur(it->traverser);

    if (res != NULL)
        ret = compras_clone_produto_nclientes(res);

    return ret;
}

COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_proximo(IT_COMPRAS_PRODUTOS_NCLIENTES it) {
    COMPRAS_PRODUTO_NCLIENTES ret = NULL;
    COMPRAS_PRODUTO_NCLIENTES res = avl_t_next(it->traverser);

    if (res != NULL)
        ret = compras_clone_produto_nclientes(res);

    return ret;
}

COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_anterior(IT_COMPRAS_PRODUTOS_NCLIENTES it) {
    COMPRAS_PRODUTO_NCLIENTES ret = NULL;
    COMPRAS_PRODUTO_NCLIENTES res = avl_t_prev(it->traverser);

    if (res != NULL)
        ret = compras_clone_produto_nclientes(res);

    return ret;
}

void free_it_compras_produtos_nclientes(IT_COMPRAS_PRODUTOS_NCLIENTES it){
    if(it != NULL)
        avl_t_free(it->traverser);
    free(it);
}



/*
 * FUNCOES AUXILIARES PRIVADAS AO MODULO
 */

/*
 * INICIALIZACOES
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

COMPRAS_CLIENTE_TIPO_COMPRA compras_inicializa_asoc_cliente_tipo_compra(char *cod_cliente, char tipo_compra) {
    char *copia;
    COMPRAS_CLIENTE_TIPO_COMPRA res = (COMPRAS_CLIENTE_TIPO_COMPRA) malloc(sizeof (struct compras_cliente_tipo_compra));
    copia = (char *) malloc(sizeof (char)*(strlen(cod_cliente) + 1));
    strcpy(copia, cod_cliente);
    res->cod_cliente = copia;
    res->tipo_compra = tipo_compra;
    return res;
}

COMPRAS_PRODUTO_NCLIENTES compras_inicializa_produto_nclientes(char* cod_prod){
    COMPRAS_PRODUTO_NCLIENTES res = (COMPRAS_PRODUTO_NCLIENTES) malloc(sizeof(struct compras_produto_nclientes));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    res->cod_produto = copia;
    res->n_clientes =0;
    return res;
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

COMPRAS_PRODUTO_NCLIENTES compras_clone_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES src){
    COMPRAS_PRODUTO_NCLIENTES dest = (COMPRAS_PRODUTO_NCLIENTES) malloc(sizeof(struct compras_produto_nclientes));
    char *copia = (char*) malloc(sizeof (char)*(strlen(src->cod_produto) + 1));
    strcpy(copia, src->cod_produto);
    dest->cod_produto   = copia;
    dest->n_clientes    = src->n_clientes;
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

COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha_noclone(char* cod_prod) {
    COMPRAS_FICHA_PRODUTO prod = (COMPRAS_FICHA_PRODUTO) malloc(sizeof (struct compras_ficha_produto));
    prod->cod_produto = cod_prod;
    return prod;
}

COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha_noclone(char* cod_cli) {
    COMPRAS_FICHA_CLIENTE cli = (COMPRAS_FICHA_CLIENTE) malloc(sizeof (struct compras_ficha_cliente));
    cli->cod_cliente = cod_cli;
    return cli;
}

COMPRAS_PRODUTO_NCLIENTES compras_codigo_produto_to_produto_nclientes_noclone(char* cod_prod){
    COMPRAS_PRODUTO_NCLIENTES dest = (COMPRAS_PRODUTO_NCLIENTES) malloc(sizeof(struct compras_produto_nclientes));
    dest->cod_produto = cod_prod;
    return dest;
}

COMPRAS_PRODUTO_NCLIENTES compras_codigo_produto_to_produto_nclientes(char* cod_prod){
    COMPRAS_PRODUTO_NCLIENTES res = (COMPRAS_PRODUTO_NCLIENTES) malloc(sizeof(struct compras_produto_nclientes));
    res->cod_produto = cod_prod;
    return res;
}

/*
 * PROCURAS
 */

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

COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl_noclone(Compras compras, char *cod_cli){
    COMPRAS_FICHA_CLIENTE nodo_aux = compras_codigo_cliente_to_ficha_noclone(cod_cli);
    COMPRAS_FICHA_CLIENTE res = (COMPRAS_FICHA_CLIENTE) avl_find(compras->avl_clientes, nodo_aux);
    free(nodo_aux);
    return res;
}

COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl_noclone(COMPRAS_FICHA_CLIENTE cliente , char *cod_prod){
    COMPRAS_FICHA_PRODUTO nodo_aux = compras_codigo_produto_to_ficha_noclone(cod_prod);
    COMPRAS_FICHA_PRODUTO res = (COMPRAS_FICHA_PRODUTO) avl_find(cliente->avl_produtos, nodo_aux);
    free(nodo_aux);
    return res;
}

COMPRAS_PRODUTO_NCLIENTES compras_procura_produto_ncli_com_cod_prod_avl_noclone(Compras compras, char *cod_prod){
    COMPRAS_PRODUTO_NCLIENTES nodo_aux = compras_codigo_produto_to_produto_nclientes_noclone(cod_prod);
    COMPRAS_PRODUTO_NCLIENTES res = (COMPRAS_PRODUTO_NCLIENTES) avl_find(compras->avl_produtos_nclientes, nodo_aux);
    free(nodo_aux);
    return res;
}

/*
 * FUNCOES COMPARACAO
 */

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

int compras_compara_produtos_nclientes_avl(const void *avl_a, const void *avl_b, void *param){
    COMPRAS_PRODUTO_NCLIENTES a = (COMPRAS_PRODUTO_NCLIENTES) avl_a;
    COMPRAS_PRODUTO_NCLIENTES b = (COMPRAS_PRODUTO_NCLIENTES) avl_b;
    
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

/*
 * LIBERTACAO MEMORIA
 */

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

void compras_free_produto_nclientes_avl(void *avl_item, void *avl_param){
    COMPRAS_PRODUTO_NCLIENTES p_ncli = (COMPRAS_PRODUTO_NCLIENTES) avl_item;
    free(p_ncli->cod_produto);
    free(p_ncli);
}

void compras_free_produto(COMPRAS_FICHA_PRODUTO produto) {
    if (produto != NULL)
        free(produto->cod_produto);

    free(produto);
}

void compras_free_cliente(COMPRAS_FICHA_CLIENTE cliente) {
    if (cliente != NULL)
        free(cliente->cod_cliente);
    
    free(cliente);
}

void compras_free_produto_nclientes(COMPRAS_PRODUTO_NCLIENTES p_ncli) {
    if (p_ncli != NULL)
        free(p_ncli->cod_produto);
    free(p_ncli);
       
}

void compras_free_produto_ad(void *produto){
    COMPRAS_FICHA_PRODUTO p = (COMPRAS_FICHA_PRODUTO) produto;
    free(p->cod_produto);
    free(p);
}

void compras_free_cliente_ad(void *cliente){
    COMPRAS_FICHA_CLIENTE c = (COMPRAS_FICHA_CLIENTE) cliente;
    free(c->cod_cliente);
    free(c);
}

void compras_free_produto_nclientes_ad(void *ad_item){
    COMPRAS_PRODUTO_NCLIENTES p_ncli = (COMPRAS_PRODUTO_NCLIENTES) ad_item;
    free(p_ncli->cod_produto);
    free(p_ncli);
}

void compras_free_asoc_cliente_tipo_compra_ad(void *item) {
    COMPRAS_CLIENTE_TIPO_COMPRA p_apagar = (COMPRAS_CLIENTE_TIPO_COMPRA) item;
    free(p_apagar->cod_cliente);
    free(p_apagar);
}


/*
 * OPTIMIZACOES
 */

COMPRAS_FICHA_CLIENTE it_compras_fich_cliente_proximo_noclone(IT_COMPRAS_CLIENTES it) {
    return avl_t_next(it->traverser);
}

COMPRAS_FICHA_PRODUTO it_compras_fich_produto_proximo_noclone(IT_COMPRAS_PRODUTOS it) {
    return avl_t_next(it->traverser);
}

COMPRAS_PRODUTO_NCLIENTES it_compras_produtos_nclientes_proximo_noclone(IT_COMPRAS_PRODUTOS_NCLIENTES it) {
    return avl_t_next(it->traverser);
}

/*
 * OUTRAS
 */

void compras_troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}

void compras_insere_assoc_cli_compra_em_assoc_prod_cli(COMPRAS_ASSOC_PROD_CLIENTES p_cli, COMPRAS_CLIENTE_TIPO_COMPRA cli_compra){
    ad_insere_elemento(p_cli->associacoes, cli_compra);
}



