#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "headers/compra.h"
#include "headers/cat_clientes.h"
#include "headers/cat_produtos.h"
#include "headers/compras.h"
#include "headers/contabilidade.h"

#define LINHA_CLIENTE_MAX 32
#define LINHA_PRODUTO_MAX 32
#define LINHA_COMPRA_MAX 64


extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;
extern Contabilidade contabilidade;
extern Compras mod_compras;


static void leitura_clientes(FILE *, char *);
static void leitura_produtos(FILE *, char *);
static void leitura_compras(FILE *, char *);
static int compra_valida(COMPRA);
static void mostra_ajuda();

int leitura_ficheiros(int argc, char **argv) {
    FILE *f_clientes = NULL;
    FILE *f_produtos = NULL;
    FILE *f_compras = NULL;
    char *nfclientes=NULL, *nfprodutos=NULL, *nfcompras=NULL;
    clock_t ci, cf;
    ci = clock();
    switch (argc) {
        case 1: nfclientes = "datasets/FichClientes.txt";
            nfprodutos = "datasets/FichProdutos.txt";
            nfcompras = "datasets/Compras.txt";
            break;

        case 2: if (strcmp(argv[1], "--mini") == 0) {
                nfclientes = "datasets/miniClientes.txt";
                nfprodutos = "datasets/miniProdutos.txt";
                nfcompras = "datasets/miniCompras.txt";
            }else {
                if(strcmp(argv[1], "--help") == 0){
                mostra_ajuda();
                return EXIT_FAILURE;
            }else{
                fprintf(stderr, "Apenas indicou um argumento, que não corresponde a nenhuma flag conhecida.\n");
                mostra_ajuda();
                return EXIT_FAILURE;
            }
            break;
        case 4: nfclientes = argv[1];
            nfprodutos = argv[2];
            nfcompras = argv[3];
            break;
        default:
            fprintf(stderr, "Nao foram especificados ficheiros de input.\n");
            mostra_ajuda();
            return EXIT_FAILURE;
    }
}

    if((f_clientes = fopen(nfclientes, "r"))==NULL){
        nfclientes = "FichClientes.txt";
        f_clientes = fopen(nfclientes, "r");
    }
    
    if((f_produtos = fopen(nfprodutos, "r"))==NULL){
        nfprodutos = "FichProdutos.txt";
        f_produtos = fopen(nfprodutos, "r");
    }
    
    if((f_compras = fopen(nfcompras, "r"))==NULL){
        nfcompras = "Compras.txt";
        f_compras = fopen(nfcompras, "r");
    }
    
    if (f_clientes == NULL || f_produtos == NULL || f_compras == NULL) {
        fprintf(stderr, "Erro ao abrir um dos ficheiros.\n");
        mostra_ajuda();
        return EXIT_FAILURE;
    }
    
    leitura_clientes(f_clientes, nfclientes);
    leitura_produtos(f_produtos, nfprodutos);
    leitura_compras(f_compras, nfcompras);

    cf = clock();
    printf("------------------\n");
    printf("Tempo total leitura: %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    printf("------------------\n");

    fclose(f_clientes);
    fclose(f_produtos);
    fclose(f_compras);

    return EXIT_SUCCESS;
}

static void leitura_clientes(FILE *f_cli, char *nf) {
    int clientes_validos = 0, total_linhas_clientes = 0;
    clock_t ci, cf;
    char *cliente, *linha_cliente;
    char *delim = " \n\r";
    
    
    ci = clock();
    linha_cliente = (char *) malloc(sizeof (char)*LINHA_CLIENTE_MAX);

    while (fgets(linha_cliente, LINHA_CLIENTE_MAX, f_cli) != NULL) {
        cliente = strtok(linha_cliente, delim);

        if (cliente != NULL) {
            cat_insere_cliente(catalogo_clientes, cliente);
            compras_regista_cliente(mod_compras, cliente);
            clientes_validos++;
        }

        total_linhas_clientes++;
    }

    cf = clock();
    printf("-----CLIENTES-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d (%d invalidas)\n", clientes_validos, total_linhas_clientes - clientes_validos);
    printf("Linhas lidas: %d\n", total_linhas_clientes);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);

    free(linha_cliente);
}

static void leitura_produtos(FILE *f_prod, char *nf) {
    int produtos_validos = 0, total_linhas_produtos = 0;
    clock_t ci, cf;
    char *produto, *linha_produto;
    char *delim = " \n\r";
    
    ci = clock();

    linha_produto = (char *) malloc(sizeof (char)*LINHA_PRODUTO_MAX);

    while (fgets(linha_produto, LINHA_PRODUTO_MAX, f_prod) != NULL) {
        produto = strtok(linha_produto, delim);

        if (produto != NULL) {
            cat_insere_produto(catalogo_produtos, produto);
            cont_regista_produto(contabilidade, produto);
            compras_regista_produto(mod_compras, produto);
            produtos_validos++;
        }

        total_linhas_produtos++;
    }

    cf = clock();
    
    printf("-----PRODUTOS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d (%d invalidas)\n", produtos_validos, total_linhas_produtos - produtos_validos);
    printf("Linhas lidas: %d\n", total_linhas_produtos);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(linha_produto);
}

static void leitura_compras(FILE *f_comp, char *nf) {
    COMPRA compra;
    int compras_validas = 0, total_linhas_compras = 0;
    clock_t ci, cf;
    char *linha_compra, *token;
    char *delim = " \n\r";
    
    cod_cliente_t cod_cliente;
    preco_unit_t preco_unit;
    quantidade_t quantidade;
    promo_t promo;
    cod_produto_t cod_produto;
    mes_t mes;
    
    
    ci = clock();

    linha_compra = (char *) malloc(sizeof (char)*LINHA_COMPRA_MAX);
    compra = inicializa_compra();
    
    while (fgets(linha_compra, LINHA_COMPRA_MAX, f_comp) != NULL) {
        
        /*Produto*/
        token = strtok(linha_compra, delim);
        cod_produto = token;
        /*Preco*/
        token = strtok(NULL, delim);
        preco_unit= atof(token);
        /*Quantidade*/
        token = strtok(NULL, delim);
        quantidade = atoi(token);
        /*Promo*/
        token = strtok(NULL, delim);
        promo = token[0];
        /*Cliente*/
        token = strtok(NULL, delim);
        cod_cliente = token;
        /*Mes*/
        token = strtok(NULL, delim);
        mes = atoi(token);
        
        actualiza_compra(compra, cod_cliente, cod_produto, preco_unit, quantidade, promo, mes);
        
        if (compra_valida(compra)) {
            cont_insere_compra(contabilidade, compra);
            compras_insere_compra(mod_compras, compra);
            compras_validas++;
        }
        total_linhas_compras++;
    }

    cf = clock();
    printf("------COMPRAS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d (%d invalidas)\n", compras_validas, total_linhas_compras - compras_validas);
    printf("Linhas lidas: %d\n", total_linhas_compras);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    
    
    /*
    printf("------------------\n");
    printf("MOTIVO DAS COMPRAS INVALIDAS:\n");
    printf("------------------\n");
    printf("Codigo Cliente errado: %d\n", cliente_errado);
    printf("Codigo Produto errado: %d\n", produto_errado);
    printf("Facturacao Anual: %.2f€\n", facturacao_total);
     */

    free_compra(compra);
    free(linha_compra);
}

static int compra_valida(COMPRA compra) {
    return cat_existe_cliente(catalogo_clientes, get_cod_cliente(compra))
            && cat_existe_produto(catalogo_produtos, get_cod_produto(compra))
            && get_mes(compra) >= 1 && get_mes(compra) <= 12
            && get_preco_unit(compra) >= 0
            && (get_quantidade(compra) > 0)
            && (get_promo(compra) == 'N' || get_promo(compra) == 'P');
}


static void mostra_ajuda(){
        fprintf(stderr, "\tModo de executar:\n");
        fprintf(stderr, "\t./gesthiper - (Caso os ficheiros de clientes, produtos e compras estejam na directoria)\n");
        fprintf(stderr, "\tOU\n");
        fprintf(stderr, "\t./gesthiper <ficheiro_clientes> <ficheiro_produtos> <ficheiro_compras>\n");
}
