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


COMPRAS_FICHA_PRODUTO compras_inicializa_ficha_produto(char* cod_prod);
COMPRAS_FICHA_CLIENTE compras_inicializa_ficha_cliente(char* cod_cli);
COMPRAS_FICHA_PRODUTO compras_codigo_produto_to_ficha(char* cod_prod);
COMPRAS_FICHA_CLIENTE compras_codigo_cliente_to_ficha(char* cod_cli);
COMPRAS_FICHA_CLIENTE compras_procura_ficha_cliente_com_cod_avl(Compras compras, char *cod_cli);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_cod_avl(Compras compras, char *cod_cli, char *cod_prod);
COMPRAS_FICHA_PRODUTO compras_procura_ficha_produto_com_fichacli_avl(COMPRAS_FICHA_CLIENTE cliente , char *cod_prod);
int compras_compara_fichas_cli_por_cod_cli_avl(const void *avl_a, const void *avl_b, void *param);
int compras_compara_fichas_prod_por_cod_prod_avl(const void *avl_a, const void *avl_b, void *param);
void compras_free_produto(void *avl_item, void *avl_param);
void compras_free_cliente(void *avl_item, void *avl_param);
void compras_troca_meses(int *mes1 , int *mes2);

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
    avl_destroy(compras->avl_clientes, compras_free_cliente);
    free(compras);
}

/*
 * TOTAL PRODUTOS COMPRADOS CLIENTE
 */

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
 * FUNCOES AUXILIARES PRIVADAS AO MODULO
 */

COMPRAS_FICHA_PRODUTO compras_inicializa_ficha_produto(char* cod_prod) {
    int i, j;
    COMPRAS_FICHA_PRODUTO prod = (COMPRAS_FICHA_PRODUTO) malloc(sizeof (struct compras_ficha_produto));
    char *copia = (char*) malloc(sizeof (char)*(strlen(cod_prod) + 1));
    strcpy(copia, cod_prod);
    prod->cod_produto = copia;
    
    for (i = 0; i < 12; i++)
        for (j = 0; j < 2; j++) {
            prod->num_unidades_compradas[i][j] = 0;
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

void compras_free_produto(void *avl_item, void *avl_param){
    COMPRAS_FICHA_PRODUTO produto = (COMPRAS_FICHA_PRODUTO) avl_item;
    free(produto->cod_produto);
    free(produto);
}

void compras_free_cliente(void *avl_item, void *avl_param){
    COMPRAS_FICHA_CLIENTE cliente = (COMPRAS_FICHA_CLIENTE) avl_item;
    free(cliente->cod_cliente);
    avl_destroy(cliente->avl_produtos, compras_free_produto);
    free(cliente);
    
}

void compras_troca_meses(int *mes1 , int *mes2){
    int temp = *mes1;
    *mes1 = *mes2;
    *mes2 = temp;
}


