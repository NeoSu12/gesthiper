#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/queries.h"
#include "headers/cat_produtos.h"
#include "headers/cat_clientes.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

#define TAM_PAGINA 20

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;

int _02_codigo_produtos_letra() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0;
    int iterados, i, n_pagina = 1, lim_inf = 1, lim_sup = 0, resultados;
    PagProdutos paginacao;
    char letra;
    char input[50];
    char *pagina[TAM_PAGINA];

    while (sair_menu == 0) {

        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);

        if (isalpha(letra) && leitura > 0) {
            paginacao = inicializa_pag_produtos_letra(catalogo_produtos, TAM_PAGINA, letra);
            resultados = total_produtos_letra(catalogo_produtos, letra);
            iterados = pag_produtos_goto_pag(paginacao, n_pagina, pagina);
            lim_sup += iterados;
            
            while (sair_menu == 0) {
                printf("\033[2J\033[1;1H");
                printf("-------------\n");
                printf("--Pagina %2d--\n", n_pagina);
                printf("-------------\n");

                for (i = 0; i < iterados; i++)
                    printf("%s\n", pagina[i]);

                printf("-------------\n");

                printf("A mostrar %d-%d de %d resultados\n", lim_inf, lim_sup, resultados);
                printf("---------------------------------------------------------\n");
                printf("1- Pag. Anterior | 2 - Proxima Pag | 0- Voltar | Q - Sair\n");
                printf("Escolha >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '0':
                        sair_menu = 1;
                        sair_programa = 0;
                        break;
                    case '1':
                        if (n_pagina != 1)
                            n_pagina--;
                        break;
                    case '2':
                        n_pagina++;
                        break;
                    case 'Q':
                        sair_menu = 1;
                        sair_programa = 1;
                        break;
                    default:
                        sair_menu = 1;
                        break;
                }
                iterados = pag_produtos_goto_pag(paginacao, n_pagina, pagina);

                if (iterados != PAGINA_IMPOSSIVEL) {
                    lim_sup += iterados;
                    lim_inf += iterados;
                } else {
                    /*Se a pagina nao existe, apenas pode acontecer por ter chegado ao
                     fim, nesse caso desfazer o incremento do switch.*/
                    n_pagina--;
                }
            }
        } else {
            sair_menu = 1;
        }


    }
    return sair_programa;
}

void _03_compras_mensais_prod() {
    printf("[QUERIE 3] Ainda não implementado!\n");
}

void _04_prods_nao_comprados() {
    printf("[QUERIE 4] Ainda não implementado!\n");
}

void _05_tabela_cliente() {
    printf("[QUERIE 5] Ainda não implementado!\n");
}

int _06_codigos_clientes_letra() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0;
    int iterados, i, n_pagina = 1, lim_inf = 1, lim_sup = 0, resultados;
    PagClientes paginacao;
    char letra;
    char input[50];
    char *pagina[TAM_PAGINA];

    while (sair_menu == 0) {

        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);

        if (isalpha(letra) && leitura > 0) {
            paginacao = inicializa_pag_clientes_letra(catalogo_clientes, TAM_PAGINA, letra);
            resultados = total_clientes_letra(catalogo_clientes, letra);
            iterados = pag_clientes_goto_pag(paginacao, n_pagina, pagina);
            lim_sup += iterados;
            
            while (sair_menu == 0) {
                printf("\033[2J\033[1;1H");
                printf("-------------\n");
                printf("--Pagina %2d--\n", n_pagina);
                printf("-------------\n");

                for (i = 0; i < iterados; i++)
                    printf("%s\n", pagina[i]);

                printf("-------------\n");

                printf("A mostrar %d-%d de %d resultados\n", lim_inf, lim_sup, resultados);
                printf("---------------------------------------------------------\n");
                printf("1- Pag. Anterior | 2 - Proxima Pag | 0- Voltar | Q - Sair\n");
                printf("Escolha >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '0':
                        sair_menu = 1;
                        sair_programa = 0;
                        break;
                    case '1':
                        if (n_pagina != 1)
                            n_pagina--;
                        break;
                    case '2':
                        n_pagina++;
                        break;
                    case 'Q':
                        sair_menu = 1;
                        sair_programa = 1;
                        break;
                    default:
                        sair_menu = 1;
                        break;
                }
                iterados = pag_clientes_goto_pag(paginacao, n_pagina, pagina);

                if (iterados != PAGINA_IMPOSSIVEL) {
                    lim_sup += iterados;
                    lim_inf += iterados;
                } else {
                    /*Se a pagina nao existe, apenas pode acontecer por ter chegado ao
                     fim, nesse caso desfazer o incremento do switch.*/
                    n_pagina--;
                }
            }
        } else {
            sair_menu = 1;
        }


    }
    return sair_programa;
}

void _07_compras_intervalo_meses() {
    printf("[QUERIE 7] Ainda não implementado!\n");
}

void _08_clientes_compraram_prod() {
    printf("[QUERIE 8] Ainda não implementado!\n");
}

void _09_produtos_mais_comprados_cliente() {
    printf("[QUERIE 9] Ainda não implementado!\n");
}

void _10_clientes_regulares() {
    printf("[QUERIE 10] Ainda não implementado!\n");
}

void _11_compras_CSV() {
    printf("[QUERIE 11] Ainda não implementado!\n");
}

void _12_prods_mais_vendidos() {
    printf("[QUERIE 12] Ainda não implementado!\n");
}

void _13_tres_prods_mais_comprados() {
    printf("[QUERIE 13] Ainda não implementado!\n");
}

void _14_clientes_prods_fantasma() {
    printf("[QUERIE 14] Ainda não implementado!\n");
}

