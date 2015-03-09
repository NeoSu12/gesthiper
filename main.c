#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "erros.h"
#include "compra.h"

#define LINHA_CLIENTE_MAX 20
#define LINHA_PRODUTO_MAX 20
#define LINHA_COMPRA_MAX 40

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

void interface();
void faceprodutos();
void faceclientes();
void facecompras();
void listaprodutos_2();
void naocomprados_4();
void quemcomprou_8();

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
    time_t tempo_i, tempo_f;
    clock_t ci, cf;
    char *cliente;

    ci = clock();
    tempo_i = time(NULL);
    cliente = (char *) malloc(sizeof (char)*LINHA_CLIENTE_MAX);

    while (fgets(cliente, LINHA_CLIENTE_MAX, f_cli) != NULL) {
        cliente[strlen(cliente) - 1] = '\0';
        if (cliente_valido(cliente) != -1) clientes_validos++;
        /* printf("%s\n",cliente); */
        total_linhas_clientes++;
    }
    tempo_f = time(NULL);
    cf = clock();

    printf("-----CLIENTES-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", clientes_validos);
    printf("Linhas lidas: %d\n", total_linhas_clientes);
    printf("Leitura em %f segundos.\n", difftime(tempo_f, tempo_i));
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(cliente);
}

void le_produtos(FILE *f_prod, char *nf) {
    int produtos_validos = 0, total_linhas_produtos = 0;
    time_t tempo_i, tempo_f;
    clock_t ci, cf;
    char *produto;

    ci = clock();
    tempo_i = time(NULL);
    produto = (char *) malloc(sizeof (char)*LINHA_PRODUTO_MAX);

    while (fgets(produto, LINHA_PRODUTO_MAX, f_prod) != NULL) {
        produto[strlen(produto) - 1] = '\0';
        if (produto_valido(produto) != -1) produtos_validos++;
        /* printf("%s\n",produto); */
        total_linhas_produtos++;
    }
    cf = clock();
    tempo_f = time(NULL);
    printf("-----PRODUTOS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", produtos_validos);
    printf("Linhas lidas: %d\n", total_linhas_produtos);
    printf("Leitura em %f segundos.\n", difftime(tempo_f, tempo_i));
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(produto);
}

void le_compras(FILE *f_comp, char *nf) {
    int compras_validas = 0, total_linhas_compras = 0;
    time_t tempo_i, tempo_f;
    clock_t ci, cf;
    char *linha_compra, *delim, *token;
    COMPRA *compra;

    ci = clock();
    tempo_i = time(NULL);
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
    tempo_f = time(NULL);
    printf("------COMPRAS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", compras_validas);
    printf("Linhas lidas: %d\n", total_linhas_compras);
    printf("Leitura em %f segundos.\n", difftime(tempo_f, tempo_i));
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

/* Interface utilizador */

void interface() {
    char r[50];
    int sair = 0, leitura;
    printf("%s ------------------------------------------\n|"
            "%sGESTHIPER%s                                 |\n|"
            "                                          |\n|"
            "   1 - Produtos                           |\n|"
            "   2 - Clientes                           |\n|"
            "   3 - Compras/Contabilidade              |\n|"
            "                                          |\n| "
            "BEM-VINDO                       Q - %sSair%s |\n "
            "------------------------------------------\n%s",
            WHITE, BLUE, WHITE, RED, WHITE, NORMAL);


    while (sair == 0) {

        printf("//O que procura(?): ");
        leitura = scanf("%s", r);
        
        switch (toupper(r[0])) {
            case '1': faceprodutos();break;
            case '2': faceclientes();break;
            case '3': facecompras();break;
            case 'Q': sair = 1;break;
            default: 
                /*TODO: Tratamento de erro temporario.
                 * Melhorar este tratamento depois de saber resposta do professor
                 sobre o uso do scanf.*/
                if(leitura!=0) sair = 1;
                else fprintf(stderr, "[ERRO] Nao foi possivel ler a opcao escolhida."); 
                break;
        }
    }


}

void faceprodutos() {
    char r[50];
    int sair=0,leitura;
    
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
            "%s*1* %sPRODUTOS%s                              |\n|"
            "                                          |\n|"
            "   1 - Listar produtos (2)                |\n|"
            "   2 - Nao comprados (4)                  |\n|"
            "   3 - Quem comprou? (8)                  |\n|"
            "                                          |\n|"
            "  0 - Voltar                     Q - %sSair%s |\n"
            " ------------------------------------------\n%s", 
            WHITE, YELLOW, BLUE, WHITE, RED, WHITE, NORMAL);
    
    
    while (sair == 0) {

        printf("//O que procura(?): ");
        leitura = scanf("%s", r);
        
        switch (toupper(r[0])) {
            case '0': printf("\033[2J\033[1;1H");
                      interface();break;
            case '1': listaprodutos_2();break;
            case '2': naocomprados_4();break;
            case '3': quemcomprou_8();break;
            case 'Q': sair = 1;break;
            default: 
                /*TODO: Tratamento de erro temporario.
                 * Melhorar este tratamento depois de saber resposta do professor
                 sobre o uso do scanf.*/
                if(leitura!=0) sair = 1;
                else fprintf(stderr, "[ERRO] Nao foi possivel ler a opcao escolhida."); 
                break;
        }
    }
    
}

void faceclientes() {
    char r[50];
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
            "%s*2* %sCLIENTES%s                              |\n|"
            "                                          |\n|"
            "   1 - Listar clientes (6)                |\n|"
            "   2 - Quantos produtos? (5)              |\n|"
            "   3 - Mais comprados (9)                 |\n|"
            "   4 - Clientes do ano! (10)              |\n|"
            "   5 - Top 3 produtos (13)                |\n", WHITE, YELLOW, BLUE, WHITE);
    
    printf("|                                          |\n|"
            "  0 - Voltar                     Q - %sSair%s |\n"
            " ------------------------------------------\n%s", RED, WHITE, NORMAL);
    
    while (r[0] != 48 || r[0] != 49 || r[0] != 50 || r[0] != 51 || r[0] != 52 || r[0] != 53 || r[0] != 81 || r[0] != 113) {
        printf("//Que deseja fazer(?): ");
        if (scanf("%s", r) == 1) {
            if (strlen(r) == 1 && (r[0] == 48 || r[0] == 49 || r[0] == 50 || r[0] == 51 || r[0] == 52 || r[0] == 53 || r[0] == 81 || r[0] == 113)) break;
            else printf("\n[ERRO] Valor invalido\n\n");
        } else {
            printf("[ERRO] Falha de Leitura\n\n");
        }
    }
    if (r[0] == 48) {
        printf("\033[2J\033[1;1H");
        interface();
    }
    if (r[0] == 49); /* Valor temporario !!     listaclientes_6();*/
    if (r[0] == 50); /* Valor temporario !!     quantosprodutos_5();*/
    if (r[0] == 51); /* Valor temporario !!     maiscomprados_9();*/
    if (r[0] == 52); /* Valor temporario !!     clientesdoano_10();*/
    if (r[0] == 53); /* Valor temporario !!     top3produtos_13();*/
    return;
}

void facecompras() {
    char r[50];
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|%s*3* %sCONTABILIDADE%s                         |\n|                                          |\n|   1 - Numero vendas (3)                  |\n|   2 - Numero compras (7)                 |\n|   3 - 'N' produtos mais vendidos (12)    |\n|   4 - Clientes/Produtos ignorados (14)   |\n|   5 - Informacao geral (11)              |\n", WHITE, YELLOW, BLUE, WHITE);
    printf("|                                          |\n|  0 - Voltar                     Q - %sSair%s |\n ------------------------------------------\n%s", RED, WHITE, NORMAL);
    while (r[0] != 48 || r[0] != 49 || r[0] != 50 || r[0] != 51 || r[0] != 52 || r[0] != 53 || r[0] != 81 || r[0] != 113) {
        printf("//Que deseja fazer(?): ");
        if (scanf("%s", r) == 1) {
            if (strlen(r) == 1 && (r[0] == 48 || r[0] == 49 || r[0] == 50 || r[0] == 51 || r[0] == 52 || r[0] == 53 || r[0] == 81 || r[0] == 113)) break;
            else printf("\n[ERRO] Valor invalido\n\n");
        } else {
            printf("[ERRO] Falha de Leitura\n\n");
        }
    }
    if (r[0] == 48) {
        printf("\033[2J\033[1;1H");
        interface();
    }
    if (r[0] == 49); /* Valor temporario !!     numerovendas_3();*/
    if (r[0] == 50); /* Valor temporario !!     numerocompras_7();*/
    if (r[0] == 51); /* Valor temporario !!     maisvendidos_12();*/
    if (r[0] == 52); /* Valor temporario !!     cliproignorados_14();*/
    if (r[0] == 53); /* Valor temporario !!     infogeral_11();*/
    printf("\033[2J\033[1;1H");
    return;
}

void listaprodutos_2() {
    char r[50];
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|%sGESTHIPER%s                                 |\n|                                          |\n|                                          |\n|             %sLISTAR PRODUTOS%s              |\n|                                          |\n|                                          |\n|  1 - Iniciar                             |\n|  0 - Voltar                     Q - %sSair%s |\n ------------------------------------------\n%s", WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    while (r[0] != 48 || r[0] != 49 || r[0] != 81 || r[0] != 113) {
        printf("//Que deseja fazer(?): ");
        if (scanf("%s", r) == 1) {
            if (strlen(r) == 1 && (r[0] == 48 || r[0] == 49 || r[0] == 81 || r[0] == 113)) break;
            else printf("\n[ERRO] Valor invalido\n\n");
        } else {
            printf("[ERRO] Falha de Leitura\n\n");
        }
    }
    if (r[0] == 48) faceprodutos();
    if (r[0] == 49) printf("//Insira a letra: \n"); /* lista os produtos temporario!!*/
    return;
}

void naocomprados_4() {
    char r[50];
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|%sGESTHIPER%s                                 |\n|                                          |\n|                                          |\n|             %sNAO COMPRADOS%s                |\n|                                          |\n|                                          |\n|  1 - Iniciar                             |\n|  0 - Voltar                     Q - %sSair%s |\n ------------------------------------------\n%s", WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    while (r[0] != 48 || r[0] != 49 || r[0] != 81 || r[0] != 113) {
        printf("//Que deseja fazer(?): ");
        if (scanf("%s", r) == 1) {
            if (strlen(r) == 1 && (r[0] == 48 || r[0] == 49 || r[0] == 81 || r[0] == 113)) break;
            else printf("\n[ERRO] Valor invalido\n\n");
        } else {
            printf("[ERRO] Falha de Leitura\n\n");
        }
    }
    if (r[0] == 48) faceprodutos();
    if (r[0] == 49) {
        printf("//Insira codigo do cliente: \n"); /* mostra tabela blah blah temporario!!*/
        printf("//Deseja guardar a informacao(?): \n"); /* temporario!!*/
    }
    return;
}

void quemcomprou_8() {
    char r[50];
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|%sGESTHIPER%s                                 |\n|                                          |\n|                                          |\n|             %sQUEM COMPROU?%s                |\n|                                          |\n|                                          |\n|  1 - Iniciar                             |\n|  0 - Voltar                     Q - %sSair%s |\n ------------------------------------------\n%s", WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    while (r[0] != 48 || r[0] != 49 || r[0] != 81 || r[0] != 113) {
        printf("//Que deseja fazer(?): ");
        if (scanf("%s", r) == 1) {
            if (strlen(r) == 1 && (r[0] == 48 || r[0] == 49 || r[0] == 81 || r[0] == 113)) break;
            else printf("\n[ERRO] Valor invalido\n\n");
        } else {
            printf("[ERRO] Falha de Leitura\n\n");
        }
    }
    if (r[0] == 48) faceprodutos();
    if (r[0] == 49) printf("//Insira codigo de produto: \n"); /* mostra info blah blah temporario!!*/
    return;
}
