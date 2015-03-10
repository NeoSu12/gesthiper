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
int cliente_valido(char *);
int produto_valido(char *);
int compra_valida(COMPRA *);
void mostra_compra(COMPRA *);

int main(int argc, char** argv) {
    le_ficheiros(argc, argv);
    interface();
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
    char *cliente, *cliente2;
    ITERADOR *it;
    CATALOGO *clientes = inicializa_catalogo(5);
    
    ci = clock();
    cliente = (char *) malloc(sizeof (char)*LINHA_CLIENTE_MAX);
    cliente2 = (char *) malloc(sizeof (char)*LINHA_CLIENTE_MAX);

    while (fgets(cliente, LINHA_CLIENTE_MAX, f_cli) != NULL) {
        
        cliente[strlen(cliente) - 1] = '\0';
        
        if (cliente_valido(cliente) != -1){
            insere_item(clientes,cliente);
            clientes_validos++;
        }
        
        /* printf("%s\n",cliente); */
        total_linhas_clientes++;
    }
    
    it = inicializa_iterador_letra(clientes,'c');
    printf("----------");
    printf("Nomes comecados por B:");
    while((cliente2 = iterador_next(it)) != NULL){
        printf("%s\n",cliente2);
    }
    printf("----------");
    
    
    cf = clock();
    
    
    printf("-----CLIENTES-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", clientes_validos);
    printf("Linhas lidas: %d\n", total_linhas_clientes);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    
    free(clientes);
    free(cliente);
}

void le_produtos(FILE *f_prod, char *nf) {
    int produtos_validos = 0, total_linhas_produtos = 0;
    clock_t ci, cf;
    char *produto;

    ci = clock();
    produto = (char *) malloc(sizeof (char)*LINHA_PRODUTO_MAX);

    while (fgets(produto, LINHA_PRODUTO_MAX, f_prod) != NULL) {
        produto[strlen(produto) - 1] = '\0';
        if (produto_valido(produto) != -1) produtos_validos++;
        /* printf("%s\n",produto); */
        total_linhas_produtos++;
    }
    
    cf = clock();
    
    printf("-----PRODUTOS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", produtos_validos);
    printf("Linhas lidas: %d\n", total_linhas_produtos);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(produto);
}

void le_compras(FILE *f_comp, char *nf) {
    int compras_validas = 0, total_linhas_compras = 0;
    clock_t ci, cf;
    char *linha_compra, *delim, *token;
    COMPRA *compra;

    ci = clock();
    linha_compra = (char *) malloc(sizeof (char)*LINHA_COMPRA_MAX);
    compra = inicializa_compra();
    delim = " ";

    while (fgets(linha_compra, LINHA_COMPRA_MAX, f_comp) != NULL) {
        linha_compra[strlen(linha_compra) - 1] = '\0';
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
            /*mostra_compra(compra);*/
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

int cliente_valido(char *cl) {
    int res = 0;
    int tam = strlen(cl);

    if (tam != 5) {
        res = -1;
    } else {
        res = isupper(cl[0]) && isupper(cl[1]) &&
                isdigit(cl[2]) && isdigit(cl[3]) && isdigit(cl[4]);
    }

    return res;
}

int produto_valido(char *prod) {
    int res = 0;
    int tam = strlen(prod);

    if (tam != 6) {
        res = -1;
    } else {
        res = isupper(prod[0]) && isupper(prod[1]) &&
                isdigit(prod[2]) && isdigit(prod[3]) &&
                isdigit(prod[4]) && isdigit(prod[5]);
    }

    return res;
}

int compra_valida(COMPRA *compra) {
    return (cliente_valido(get_cod_cliente(compra)) != -1)
            && (produto_valido(get_cod_produto(compra)) != -1)
            && get_mes(compra) >= 1 && get_mes(compra) <= 12
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
