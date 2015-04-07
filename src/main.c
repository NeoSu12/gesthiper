#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "headers/interface.h"
#include "headers/erros.h"
#include "headers/compra.h"
#include "headers/cat_clientes.h"
#include "headers/cat_produtos.h"
#include "headers/compras.h"
#include "headers/contabilidade.h"

#define LINHA_CLIENTE_MAX 32
#define LINHA_PRODUTO_MAX 32
#define LINHA_COMPRA_MAX 64

int le_ficheiros(int, char **);
void le_clientes(FILE *, char *);
void le_produtos(FILE *, char *);
void le_compras(FILE *, char *);
int compra_valida(COMPRA);
int compra_valida_debug(COMPRA);
void mostra_compra(COMPRA);
void mostra_numero_codigos();
void testes();
void testes2();
void para_o_bruno();

CatClientes catalogo_clientes;
CatProdutos catalogo_produtos;
Contabilidade contabilidade;
Compras mod_compras;

/*VARIAVEIS TEMPORARIAS PARA TESTE*/
int cliente_errado = 0, produto_errado = 0;
FILE *fich_info_compras;

int main(int argc, char** argv) {
    catalogo_clientes = inicializa_catalogo_clientes();
    catalogo_produtos = inicializa_catalogo_produtos();
    contabilidade = inicializa_contabilidade();
    mod_compras = inicializa_compras();
    
    le_ficheiros(argc, argv);
    interface();
    
    free_catalogo_clientes(catalogo_clientes);
    free_catalogo_produtos(catalogo_produtos);
    free_contabilidade(contabilidade);
    free_compras(mod_compras);
    return (EXIT_SUCCESS);
}

int le_ficheiros(int argc, char **argv) {
    FILE *f_clientes = NULL;
    FILE *f_produtos = NULL;
    FILE *f_compras = NULL;
    char *nfclientes, *nfprodutos, *nfcompras;
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
            } else {
                error_msg(ERRO_FLAG_DESCONHECIDA, NULL);
                return EXIT_FAILURE;
            }
            break;
        case 4: nfclientes = argv[1];
            nfprodutos = argv[2];
            nfcompras = argv[3];
            break;
        default: error_msg(ERRO_ARGS, NULL);
            return EXIT_FAILURE;
    }

    f_clientes = fopen(nfclientes, "r");
    f_produtos = fopen(nfprodutos, "r");
    f_compras = fopen(nfcompras, "r");

    if (f_clientes == NULL || f_produtos == NULL || f_compras == NULL) {
        error_msg(ERRO_ABIR_FICH, NULL);
        return EXIT_FAILURE;
    }

    le_clientes(f_clientes, nfclientes);
    le_produtos(f_produtos, nfprodutos);
    le_compras(f_compras, nfcompras);

    cf = clock();
    printf("------------------\n");
    printf("Tempo total leitura: %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    printf("------------------\n");

    fclose(f_clientes);
    fclose(f_produtos);
    fclose(f_compras);

    return EXIT_SUCCESS;
}

void le_clientes(FILE *f_cli, char *nf) {
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

void le_produtos(FILE *f_prod, char *nf) {
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

void le_compras(FILE *f_comp, char *nf) {
    COMPRA compra;
    int compras_validas = 0, total_linhas_compras = 0;
    clock_t ci, cf;
    char *linha_compra, *token;
    char *delim = " \n\r";
    
    ci = clock();

    linha_compra = (char *) malloc(sizeof (char)*LINHA_COMPRA_MAX);
    compra = inicializa_compra();
    
    while (fgets(linha_compra, LINHA_COMPRA_MAX, f_comp) != NULL) {
        
        /*Produto*/
        token = strtok(linha_compra, delim);
        set_cod_produto(compra, token);
        /*Preco*/
        token = strtok(NULL, delim);
        set_preco_unit(compra, atof(token));
        /*Quantidade*/
        token = strtok(NULL, delim);
        set_quantidade(compra, atoi(token));
        /*Promo*/
        token = strtok(NULL, delim);
        set_promo(compra, token[0]);
        /*Cliente*/
        token = strtok(NULL, delim);
        set_cod_cliente(compra, token);
        /*Mes*/
        token = strtok(NULL, delim);
        set_mes(compra, atoi(token));
                
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

void mostra_numero_codigos() {
    char letra = 'A';
    int tot_prod = 0, tot_cli = 0;
    printf("----------------------------\n");
    printf("Nº DE CODIGOS COMECADOS POR...\n");
    printf("CLIENTES\tPRODUTOS\n");
    for (letra = 'A'; letra <= 'Z'; letra++) {
        printf("%c: %5d\t%c: %5d\n",
                letra, cat_total_clientes_letra(catalogo_clientes, letra),
                letra, cat_total_produtos_letra(catalogo_produtos, letra));
        tot_cli += cat_total_clientes_letra(catalogo_clientes, letra);
        tot_prod += cat_total_produtos_letra(catalogo_produtos, letra);

    }
    printf("Tot: %d\tTot:%d\n", tot_cli, tot_prod);
    printf("----------------------------\n");
}


int compra_valida(COMPRA compra) {
    return cat_existe_cliente(catalogo_clientes, get_cod_cliente(compra))
            && cat_existe_produto(catalogo_produtos, get_cod_produto(compra))
            && get_mes(compra) >= 1 && get_mes(compra) <= 12
            && get_preco_unit(compra) >= 0
            && (get_quantidade(compra) > 0)
            && (get_promo(compra) == 'N' || get_promo(compra) == 'P');
}



int compra_valida_debug(COMPRA compra) {
    int res = 1;

    if (!cat_existe_cliente(catalogo_clientes, get_cod_cliente(compra))) {
        res = 0;
        cliente_errado++;
    }

    if (res != 0) {
        if (!cat_existe_produto(catalogo_produtos, get_cod_produto(compra))) {
            res = 0;
            produto_errado++;
        }
    }

    return res;
}

void mostra_compra(COMPRA compra) {
    printf("Produto: %s | ", get_cod_produto(compra));
    printf("Preco: %5.2f | ", get_preco_unit(compra));
    printf("Quantidade: %2d | ", get_quantidade(compra));
    printf("Promo: %c | ", get_promo(compra));
    printf("Cliente: %s | ", get_cod_cliente(compra));
    printf("Mes: %2d\n", get_mes(compra));

}

void para_o_bruno() {
    int i;
    FILE *fich = fopen("compras_clientes.txt", "w");
    CONT_FICHA_PRODUTO produto;
    IT_CONT it = inicializa_it_cont_fich_produtos(contabilidade);
    int vendas_n, vendas_p, total_vendas_n, total_vendas_p;
    double fact_n, fact_p, total_fact_n, total_fact_p;
    
    while ((produto = it_cont_fich_produto_proximo(it)) != NULL) {
        total_vendas_n = 0;
        total_vendas_p = 0;
        total_fact_n=0;
        total_fact_p=0;
        
        fprintf(fich,"Codigo de produto: %s\n", cont_get_cod_prod_ficha(produto));
        fprintf(fich,"========================================================= \n");
        fprintf(fich,"     |  Vendas   |       ||    Facturacao     |           |\n");
        fprintf(fich," Mes |  P  |  N  | Total ||    P    |    N    |   Total   |\n");
        fprintf(fich,"--------------------------------------------------------- \n");
        for (i = 1; i <= 12; i++) {
            vendas_n=cont_total_vendas_fich_normais_produto_mes(produto,i);
            vendas_p=cont_total_vendas_fich_promo_produto_mes(produto,i);
            fact_n = cont_total_fact_fich_normal_produto_mes(produto,i);
            fact_p = cont_total_fact_fich_promo_produto_mes(produto,i);
            fprintf(fich,"%4d | %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                    i,
                    vendas_n,
                    vendas_p,
                    vendas_n+vendas_p,
                    fact_n,
                    fact_p,
                    fact_n + fact_p);


            total_vendas_n += vendas_n;
            total_vendas_p += vendas_p;

            total_fact_n += fact_n;
            total_fact_p += fact_p;

        }
        fprintf(fich,"---------------------------------------------------------- \n");
        fprintf(fich,"TOTAL| %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                total_fact_n, total_fact_p, total_fact_n + total_fact_p);
        fprintf(fich,"========================================================== \n");
    }

    fclose(fich);
}
