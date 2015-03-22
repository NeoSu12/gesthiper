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

#define TAM_PAGINA 10

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;

int _02_codigo_produtos_letra() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    int iterados, i, n_pagina = 1, lim_inf = 1, lim_sup = 0, resultados, total_pags, escolha_pag;
    PagProdutos paginacao;
    char letra;
    char input[50],r[50];
    char *pagina[TAM_PAGINA];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "             %sLISTA PRODUTOS%s               |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
 if(sair_menu==0){
    printf("Insira a letra a procurar > ");
    leitura = scanf("%s", input);
    letra = toupper(input[0]);

    if (isalpha(letra) && leitura > 0) {
        paginacao = inicializa_pag_produtos_letra(catalogo_produtos, TAM_PAGINA, letra);
        resultados = total_produtos_letra(catalogo_produtos, letra);
        total_pags = resultados % TAM_PAGINA == 0 ? resultados/TAM_PAGINA : resultados/TAM_PAGINA +1 ;
        
        iterados = pag_produtos_goto_pag(paginacao, n_pagina, pagina);
        lim_inf = (n_pagina-1)*TAM_PAGINA +1;
        lim_sup = lim_inf + iterados -1;

        while (sair_menu == 0) {
            printf("\033[2J\033[1;1H");
            printf("-----------------\n");
            printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
            printf("-----------------\n");

            for (i = 0; i < iterados; i++)
                printf("%s\n", pagina[i]);

            printf("\n");

            printf("A mostrar %d-%d de %d resultados\n", lim_inf, lim_sup, resultados);
            printf("---------------------------------------------------------\n");
            printf("1- Pag. Anterior | 2 - Proxima Pag | 3 - Ir Para pag... | 0- Voltar | Q - Sair\n");
            printf("Escolha >");
            leitura = scanf("%s", input);

            switch (toupper(input[0])) {
                case '0':
                    sair_menu=1;
                    break;
                case '1':
                    if (n_pagina > 1)
                        n_pagina--;
                    break;
                case '2':
                    n_pagina++;
                    break;
                case '3':
                    printf("Indique a pag para que quer ir >");
                    leitura = scanf("%d",&escolha_pag);
                    if(escolha_pag >0 && escolha_pag <= total_pags && leitura != 0)
                        n_pagina = escolha_pag;
                    break;
                case 'Q':
                    sair_menu = 1;
                    sair_programa = 1;
                    voltar=1;
                    break;
                default:
                    sair_menu = 1;
                    break;
            }
            iterados = pag_produtos_goto_pag(paginacao, n_pagina, pagina);

            if (iterados != PAGINA_IMPOSSIVEL) {
                lim_inf = (n_pagina-1)*TAM_PAGINA +1;
                lim_sup = lim_inf + iterados -1;
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
}
    return sair_programa;
}

int _03_compras_mensais_prod() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "             %sVENDAS PRODUTO%s               |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 3] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _04_prods_nao_comprados() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "           %sPRODUTOS NAO COMPRADOS%s         |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 4] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _05_tabela_cliente() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "     %sNºPRODUTOS COMPRADOS POR CLIENTE%s     |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 5] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _06_codigos_clientes_letra() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    int iterados, i, n_pagina = 1, lim_inf = 1, lim_sup = 0, resultados, total_pags, escolha_pag;
    PagClientes paginacao;
    char letra;
    char input[50],r[50];
    char *pagina[TAM_PAGINA];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "             %sLISTA CLIENTES%s               |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
if(sair_menu==0){    
    printf("Insira a letra a procurar > ");
    leitura = scanf("%s", input);
    letra = toupper(input[0]);

    if (isalpha(letra) && leitura > 0) {
        paginacao = inicializa_pag_clientes_letra(catalogo_clientes, TAM_PAGINA, letra);
        resultados = total_clientes_letra(catalogo_clientes, letra);
        total_pags = resultados % TAM_PAGINA == 0 ? resultados/TAM_PAGINA : resultados/TAM_PAGINA +1 ;
        
        iterados = pag_clientes_goto_pag(paginacao, n_pagina, pagina);
        lim_inf = (n_pagina-1)*TAM_PAGINA +1;
        lim_sup = lim_inf + iterados -1;

        while (sair_menu == 0) {
            printf("\033[2J\033[1;1H");
            printf("-----------------\n");
            printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
            printf("-----------------\n");

            for (i = 0; i < iterados; i++)
                printf("%s\n", pagina[i]);

            printf("\n");

            printf("A mostrar %d-%d de %d resultados\n", lim_inf, lim_sup, resultados);
            printf("---------------------------------------------------------\n");
            printf("1- Pag. Anterior | 2 - Proxima Pag | 3 - Ir Para pag... | 0- Voltar | Q - Sair\n");
            printf("Escolha >");
            leitura = scanf("%s", input);

            switch (toupper(input[0])) {
                case '0':
                    sair_menu=1;
                    break;
                case '1':
                    if (n_pagina > 1)
                        n_pagina--;
                    break;
                case '2':
                    n_pagina++;
                    break;
                case '3':
                    printf("Indique a pag para que quer ir >");
                    leitura = scanf("%d",&escolha_pag);
                    if(escolha_pag >0 && escolha_pag <= total_pags && leitura != 0)
                        n_pagina = escolha_pag;
                    break;
                case 'Q':
                    sair_menu = 1;
                    sair_programa = 1;
                    voltar=1;
                    break;
                default:
                    sair_menu = 1;
                    break;
            }
            iterados = pag_clientes_goto_pag(paginacao, n_pagina, pagina);

            if (iterados != PAGINA_IMPOSSIVEL) {
                lim_inf = (n_pagina-1)*TAM_PAGINA +1;
                lim_sup = lim_inf + iterados -1;
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
}
    return sair_programa;
}

int _07_compras_intervalo_meses() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "         %sNºCOMPRAS INTERVALO MESES%s        |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 7] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _08_clientes_compraram_prod() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "        %sQUAIS CLIENTES COMPRARAM?  %s       |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 8] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _09_produtos_mais_comprados_cliente() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "          %sPRODUTO MAIS COMPRADO%s           |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 9] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _10_clientes_regulares() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "            %sCLIENTES REGULARES%s            |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 10] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _11_compras_CSV() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "               %sTABELA CSV%s                 |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 11] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _12_prods_mais_vendidos() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "          %sPRODUTOS MAIS VENDIDOS%s          |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 12] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _13_tres_prods_mais_comprados() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "               %sTOP 3 PRODUTOS%s             |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 13] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

int _14_clientes_prods_fantasma() {
    int leitura = 0;
    int sair_menu = 0, sair_programa = 0, voltar = 0;
    char r[50];
while(voltar==0){
    sair_menu=0;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "                                          |\n|"
                "       %sCLIENTES E PRODUTOS FANTASMA%s       |\n|"
                "                                          |\n|"
                " 1 - Iniciar                              |\n| "
                "0 - Voltar                      Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, GREEN, WHITE, RED, WHITE, NORMAL);
    printf("[QUERIE 14] Ainda não implementado!\n//O que procura(?): ");
    leitura = scanf("%s", r);
    
    switch (toupper(r[0])) {
            case '0': voltar = 1;
                      sair_menu = 1;
                      sair_programa = 0;
                      break;
            case '1': 
                      sair_menu = 1;
                      break;
            case 'Q': 
                      voltar=1;
                      sair_menu = 1;
                      sair_programa = 1;
                      break;
             default: 
                      sair_menu = 1;
                      break;
        }
}
return sair_programa;
}

