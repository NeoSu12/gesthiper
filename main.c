#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "interface.h"
#include "erros.h"
#include "compra.h"
#include "catalogo.h"

#define LINHA_CLIENTE_MAX 20
#define LINHA_PRODUTO_MAX 20
#define LINHA_COMPRA_MAX 40

int le_ficheiros(int, char **);
void le_clientes(FILE *, char *);
void le_produtos(FILE *, char *);
void le_compras(FILE *, char *);
int compra_valida(COMPRA *);
void mostra_compra(COMPRA *);
void testes();

CATALOGO catalogo_clientes;
CATALOGO catalogo_produtos;

int main(int argc, char** argv) {
    catalogo_clientes = inicializa_catalogo();
    catalogo_produtos = inicializa_catalogo();
    testes();
    /*le_ficheiros(argc, argv);
    
    interface();*/
    return (EXIT_SUCCESS);
}

int le_ficheiros(int argc, char **argv) {
    FILE *f_clientes = NULL;
    FILE *f_produtos = NULL;
    FILE *f_compras = NULL;
    char *nfclientes, *nfprodutos, *nfcompras;

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
    linha_cliente = (char *) malloc(sizeof(char)*LINHA_CLIENTE_MAX);

    while (fgets(linha_cliente, LINHA_CLIENTE_MAX, f_cli) != NULL) {
        cliente = strtok(linha_cliente, delim);
        insere_item(catalogo_clientes, cliente);
        clientes_validos++;
        total_linhas_clientes++;
    }

    cf = clock();

    printf("-----CLIENTES-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", clientes_validos);
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

    linha_produto = (char *) malloc(sizeof(char)*LINHA_PRODUTO_MAX);

    while (fgets(linha_produto, LINHA_PRODUTO_MAX, f_prod) != NULL) {
        produto = strtok(linha_produto, delim);
        insere_item(catalogo_produtos, produto);
        produtos_validos++;
        total_linhas_produtos++;
    }

    cf = clock();

    printf("-----PRODUTOS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", produtos_validos);
    printf("Linhas lidas: %d\n", total_linhas_produtos);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(linha_produto);
}

void le_compras(FILE *f_comp, char *nf) {
    COMPRA *compra;
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
            compras_validas++;
        }
        total_linhas_compras++;
    }
    cf = clock();
    
    printf("------COMPRAS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", compras_validas);
    printf("Linhas lidas: %d\n", total_linhas_compras);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);

    free_compra(compra);
    free(linha_compra);
}


/*
 * TODO: Verificar se produto e cliente existem, depois de ter catalogos prontos.
 */
int compra_valida(COMPRA *compra) {
    return get_mes(compra) >= 1 && get_mes(compra) <= 12
            && get_preco_unit(compra) >= 0
            && (get_promo(compra) == 'N' || get_promo(compra) == 'P');
}

void mostra_compra(COMPRA *compra) {
    printf("Produto: %s | ", get_cod_produto(compra));
    printf("Preco: %5.2f | ", get_preco_unit(compra));
    printf("Quantidade: %2d | ", get_quantidade(compra));
    printf("Promo: %c | ", get_promo(compra));
    printf("Cliente: %s | ", get_cod_cliente(compra));
    printf("Mes: %2d\n", get_mes(compra));

}

void testes() {
    char *res_it;
    ITERADOR it_fim = inicializa_it_fim(catalogo_clientes);
    ITERADOR it_inicio = inicializa_it_inicio(catalogo_clientes);
    
    /* Testa inserções */
    insere_item(catalogo_clientes, "AA");
    insere_item(catalogo_clientes, "BC");
    insere_item(catalogo_clientes, "BA");
    insere_item(catalogo_clientes, "CA");
    insere_item(catalogo_clientes, "DX");
    
    /* 
     * Testa se a funcao iterador_proximo() consegue pegar num
     * iterador a começar no inicio da arvore e percorrer todos os elementos.
     */
    printf("Imprimir o catalogo do inicio para o fim:\n");
    while ((res_it = iterador_proximo(it_inicio)) != NULL) {
        printf("%s\n", res_it);
    }
    
    /*
     * Testa se o mesmo iterador que foi inicializado no inicio do catalogo
     * e levado até ao fim no ciclo anterior agora consegue fazer o caminho inverso,
     * de volta ao inicio.
     */
    printf("Imprimir o catalogo do fim para o inicio:\n");
    while ((res_it = iterador_anterior(it_inicio)) != NULL) {
        printf("%s\n", res_it);
    }
    
    /*
     * Testa se um iterador inicializado no fim do catalogo consegue ser levado até
     * ao principio.
     */
    printf("Imprimir o catalogo do fim para o inicio:\n");
    while ((res_it = iterador_anterior(it_fim)) != NULL) {
        printf("%s\n", res_it);
    }
    
    /*
     * Testa a remoção de um elemento.
     */
    remove_item(catalogo_clientes,"AA");
    printf("Imprimir o catalogo do inicio para o fim:\n");
    while ((res_it = iterador_proximo(it_inicio)) != NULL) {
        printf("%s\n", res_it);
    }
    
    /*
     * Testa função total_codigos.
     */
    printf("Total codigos: %d\n", total_codigos(catalogo_clientes));
}