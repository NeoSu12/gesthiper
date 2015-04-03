#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "headers/interface.h"
#include "headers/queries.h"
#include "headers/cat_produtos.h"
#include "headers/cat_clientes.h"
#include "headers/contabilidade.h"
#include "headers/compras.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

#define TAM_PAGINA 15
#define TAM_INPUT 50

typedef enum mes {
    JANEIRO = 1, FEVEREIRO, MARCO, ABRIL, MAIO, JUNHO, JULHO, AGOSTO, SETEMBRO, OUTUBRO, NOVEMBRO, DEZEMBRO
} Mes;

int _05_aux_guarda_ficheiro(Compras mod_compras, char *cod_cli);

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;
extern Contabilidade contabilidade;
extern Compras mod_compras;

int _02_codigo_produtos_letra() {
    int i = 0, leitura = 0;
    int estado = QUERIE_2;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CAT_LISTA_PRODUTOS lista_prod = NULL;
    char letra;
    char input[50];
    
    while (estado == QUERIE_2) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 2                 |\n");
        printf("|                                                     |\n");
        printf("|  Listar produtos começados por letra                |\n");
        printf("| --------------------------------------------------- |\n");
        printf("| 1 - CATALOGOS | 2 - Menu Principal | 3 - Sair       |\n");
        printf(" ====================================================  \n");
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CATALOGOS;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                default:
                    break;
            }
        }
        if (isalpha(letra) && leitura > 0 && estado == QUERIE_2) {
            ci = clock();
            lista_prod = cat_lista_produtos_letra(catalogo_produtos, letra, TAM_PAGINA);
            resultados = cat_lista_prod_get_num_elems(lista_prod);
            total_pags = cat_lista_prod_get_num_pags(lista_prod);
            cf = clock();
            while (estado == QUERIE_2) {
                elems_pag = cat_lista_prod_get_pos_and_num_elems_pag(lista_prod, &inicio_pag, n_pagina);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("GESTHIPER >> CATALOGOS >> QUERIE 2                 \n");
                printf("A mostrar produtos começados pela letra %c\n", letra);
                printf("================================================= \n");
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n", cat_lista_prod_get_elemento(lista_prod, inicio_pag + i));

                printf("\n");
                printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
                printf("==================================================== \n");
                printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
                printf("<<  4  |  <  5  |  6  >  |  7  >>  |     9 - Pag...  \n");
                printf("==================================================== \n");
                printf("Insira nº da opcao >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '1':
                        estado = FACE_CATALOGOS;
                        break;
                    case '2':
                        estado = MENU_PRINCIPAL;
                        break;
                    case 'Q':
                    case '3':
                        estado = SAIR_PROGRAMA;
                        break;
                    case '4':
                        n_pagina = 1;
                        break;
                    case '5':
                        if (n_pagina > 1)
                            n_pagina--;
                        break;
                    case '6':
                        if (n_pagina < total_pags)
                            n_pagina++;
                        break;
                    case'7':
                        n_pagina = total_pags;
                        break;
                    case '9':
                        printf("Indique a pag para que quer ir >");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    default:
                        estado = SAIR_PROGRAMA;
                        break;
                }
            }
            cat_free_lista_produtos(lista_prod);

        }
    }
    return estado;
}

int _03_compras_e_fact_mensal_prod() {
    int i, leitura;
    char input[50];
    clock_t ci, cf;
    int total_vendas_n = 0, total_vendas_p = 0;
    double total_fact_n = 0, total_fact_p = 0;
    int estado = QUERIE_3;

    while (estado == QUERIE_3) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 3               |\n");
        printf("|                                                       |\n");
        printf("|  Nº vendas e facturação mensal produto                |\n");
        printf("| ----------------------------------------------------- |\n");
        printf("| 1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair     |\n");
        printf(" ======================================================  \n");
        printf("Insira o codigo de produto a procurar > ");
        leitura = scanf("%s", input);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CONTABILIDADE;
                    break;
                case'2':
                    estado = MENU_PRINCIPAL;
                    break;
                case'3':
                    estado = SAIR_PROGRAMA;
                default:
                    break;
            }
        }

        if (estado == QUERIE_3) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CATALOGOS >> QUERIE 3                 \n");
            printf("Nº vendas e facturação mensal produto              \n");
            printf("========================================================= \n");

            if (cat_existe_produto(catalogo_produtos, input)) {
                printf("Codigo de produto: %s\n", input);
                printf("========================================================= \n");
                printf("     |  Vendas   |       ||    Facturacao     |           |\n");
                printf(" Mes |  P  |  N  | Total ||    P    |    N    |   Total   |\n");
                printf("--------------------------------------------------------- \n");
                ci=clock();
                for (i = JANEIRO; i <= DEZEMBRO; i++) {
                    printf("%4d | %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                            i,
                            cont_total_vendas_normais_produto_mes(contabilidade, input, i),
                            cont_total_vendas_promo_produto_mes(contabilidade, input, i),
                            cont_total_vendas_normais_produto_mes(contabilidade, input, i) + cont_total_vendas_promo_produto_mes(contabilidade, input, i),
                            cont_total_fact_normal_produto_mes(contabilidade, input, i),
                            cont_total_fact_promo_produto_mes(contabilidade, input, i),
                            cont_total_fact_normal_produto_mes(contabilidade, input, i) + cont_total_fact_promo_produto_mes(contabilidade, input, i));


                    total_vendas_n += cont_total_vendas_normais_produto_mes(contabilidade, input, i);
                    total_vendas_p += cont_total_vendas_promo_produto_mes(contabilidade, input, i);

                    total_fact_n += cont_total_fact_normal_produto_mes(contabilidade, input, i);
                    total_fact_p += cont_total_fact_promo_produto_mes(contabilidade, input, i);

                }
                cf=clock();
                printf("---------------------------------------------------------- \n");
                printf("TOTAL| %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                        total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                        total_fact_n, total_fact_p, total_fact_n + total_fact_p);
                printf("========================================================== \n");
                printf("Tempo: %5.3f ============================================= \n", (float)(cf-ci)/CLOCKS_PER_SEC);
            } else {
                printf("O produto nao existe\n");
                printf("=========================================================== \n");
            }

            printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Procurar outro produto \n");
            printf("========================================================== \n");
            printf("Insira nº da opcao >");
            leitura = scanf("%s", input);
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CATALOGOS;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case 'Q':
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                case '4':
                    estado = QUERIE_3;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }

        }

    }/* END WHILE*/


    return estado;
}

int _04_prods_nao_comprados() {
    int i = 0, leitura = 0;
    int estado = QUERIE_4;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CONT_LISTA_PRODUTOS lista_prod = NULL;
    CONT_FICHA_PRODUTO ficha_prod = NULL;
    char input[50];
    
    ci = clock();
    
    lista_prod = cont_lista_prod_sem_compras(contabilidade);
    resultados = cont_lista_prod_get_num_elems(lista_prod);
    total_pags = cont_lista_prod_get_num_pags(lista_prod);

    cf= clock();
    while (estado == QUERIE_4) {
        elems_pag = cont_lista_prod_get_pos_and_num_elems_pag(lista_prod, &inicio_pag, n_pagina);
        fim_pag = inicio_pag + elems_pag;
        printf("\033[2J\033[1;1H");
        printf("GESTHIPER >> CATALOGOS >> QUERIE 4                 \n");
        printf("Lista de produtos nao comprados\n");
        printf("================================================= \n");

        if (resultados > 0) {
            printf("Pagina %2d/%d \n", n_pagina, total_pags);
            printf("\n");

            for (i = 0; i < elems_pag; i++) {
                ficha_prod = cont_lista_get_fich_prod(lista_prod, inicio_pag + i);
                printf("%s\n", cont_get_cod_prod_ficha(ficha_prod));
            }

            printf("\n");
            printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
        } else {
            printf("Nao ha resultados a mostrar.\n");
        }

        printf("==================================================== \n");
        printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair    \n");
        printf("<<  4  |  <  5  |  6  >  |  7  >>  |     9 - Pag...  \n");
        printf("==================================================== \n");
        printf("Insira nº da opcao >");
        leitura = scanf("%s", input);

        switch (toupper(input[0])) {
            case '1':
                estado = FACE_CONTABILIDADE;
                break;
            case '2':
                estado = MENU_PRINCIPAL;
                break;
            case 'Q':
            case '3':
                estado = SAIR_PROGRAMA;
                break;
            case '4':
                n_pagina = 1;
                break;
            case '5':
                if (n_pagina > 1)
                    n_pagina--;
                break;
            case '6':
                if (n_pagina < total_pags)
                    n_pagina++;
                break;
            case'7':
                n_pagina = total_pags;
                break;
            case '9':
                printf("Indique a pag para que quer ir >");
                leitura = scanf("%d", &escolha_pag);
                if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                    n_pagina = escolha_pag;
                break;
            default:
                estado = SAIR_PROGRAMA;
                break;
        }

    }
    cont_free_lista_produtos(lista_prod);
    return estado;
}

int _05_tabela_cliente() {
    int i, leitura;
    char input[50];
    int cliente_existe=1;
    char *cod_cliente=NULL;
    clock_t ci, cf;
    int total_vendas=0;
    int estado = QUERIE_5;

    while (estado == QUERIE_5) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> COMPRAS >> QUERIE 5                     |\n");
        printf("|                                                       |\n");
        printf("|  Nº produtos comprados por cliente                    |\n");
        printf("| ----------------------------------------------------- |\n");
        printf("| 1 - COMPRAS | 2 - Menu Principal | 3 - Sair           |\n");
        printf(" ======================================================  \n");
        printf("Insira o codigo de cliente a procurar > ");
        leitura = scanf("%s", input);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_COMPRAS;
                    break;
                case'2':
                    estado = MENU_PRINCIPAL;
                    break;
                case'3':
                    estado = SAIR_PROGRAMA;
                default:
                    break;
            }
        }

        
        if (estado == QUERIE_5) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> COMPRAS >> QUERIE 5                 \n");
            printf("Nº produtos comprados por cliente                \n");
            printf("=========================== \n");

            if (cat_existe_cliente(catalogo_clientes, input)) {
                cliente_existe=1;
                cod_cliente = (char *) malloc(sizeof(char)*(strlen(input)+1));
                strcpy(cod_cliente, input);
                printf("Codigo de cliente: %s\n", cod_cliente);
                printf("=========================== \n");
                printf("     | Vendas | \n");
                printf(" Mes | Total  | \n");
                printf("--------------- \n");
                ci=clock();
                for (i = JANEIRO; i <= DEZEMBRO; i++) {
                    printf("%4d | %6d | \n",
                            i,
                            compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cliente, i));

                    total_vendas += compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cliente, i);
                    

                }
                cf=clock();
                printf("--------------- \n");
                printf("TOTAL| %6d |\n", total_vendas);
                printf("============== \n");
                printf("Tempo: %5.3f \n",
                        (float)(cf-ci)/CLOCKS_PER_SEC);
            } else {
                cliente_existe=0;
                printf("O cliente nao existe\n");
                printf("=========================================================== \n");
            }

            printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Ver outro cliente | 5 - Guardar em ficheiro \n");
            printf("========================================================== \n");
            printf("Insira nº da opcao >");
            leitura = scanf("%s", input);
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_COMPRAS;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case 'Q':
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                case '4':
                    estado = QUERIE_5;
                    break;
                case '5':
                    if(cliente_existe){
                        estado = _05_aux_guarda_ficheiro(mod_compras, cod_cliente);
                    } else{
                        printf("Nao é possivel criar nenhum ficheiro visto o cliente nao existir\n"
                                "Clique em qualquer tecla para continuar > ");
                        leitura = scanf("%s", input);
                        estado = QUERIE_5;
                    }
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }

        }

    }/* END WHILE*/

    free(cod_cliente);
    return estado;
}

int _06_codigos_clientes_letra() {
    int i = 0, leitura = 0;
    int estado = QUERIE_6;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CAT_LISTA_CLIENTES lista_cli = NULL;
    char letra;
    char input[50];

    while (estado == QUERIE_6) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 6                 |\n");
        printf("|                                                     |\n");
        printf("|  Listar clientes começados por letra                |\n");
        printf("| --------------------------------------------------- |\n");
        printf("| 1 - CATALOGOS | 2 - Menu Principal | 3 - Sair       |\n");
        printf(" ====================================================  \n");
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CATALOGOS;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                default:
                    break;
            }
        }
        if (isalpha(letra) && leitura > 0 && estado == QUERIE_6) {
            ci=clock();
            lista_cli = cat_lista_clientes_letra(catalogo_clientes, letra, TAM_PAGINA);
            resultados = cat_lista_cli_get_num_elems(lista_cli);
            total_pags = cat_lista_cli_get_num_pags(lista_cli);
            cf=clock();
            while (estado == QUERIE_6) {
                elems_pag = cat_lista_cli_get_pos_and_num_elems_pag(lista_cli, &inicio_pag, n_pagina);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("GESTHIPER >> CATALOGOS >> QUERIE 6                 \n");
                printf("A mostrar clientes começados pela letra %c\n", letra);
                printf("================================================= \n");
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n", cat_lista_cli_get_elemento(lista_cli, inicio_pag + i));

                printf("\n");
                printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
                printf("==================================================== \n");
                printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
                printf("<<  4  |  <  5  |  6  >  |  7  >>  |     9 - Pag...  \n");
                printf("==================================================== \n");
                printf("Insira nº da opcao >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '1':
                        estado = FACE_CATALOGOS;
                        break;
                    case '2':
                        estado = MENU_PRINCIPAL;
                        break;
                    case 'Q':
                    case '3':
                        estado = SAIR_PROGRAMA;
                        break;
                    case '4':
                        n_pagina = 1;
                        break;
                    case '5':
                        if (n_pagina > 1)
                            n_pagina--;
                        break;
                    case '6':
                        if (n_pagina < total_pags)
                            n_pagina++;
                        break;
                    case'7':
                        n_pagina = total_pags;
                        break;
                    case '9':
                        printf("Indique a pag para que quer ir >");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    default:
                        estado = SAIR_PROGRAMA;
                        break;
                }
            }
            cat_free_lista_clientes(lista_cli);

        }
    }
    return estado;
}

int _07_compras_intervalo_meses() {
    int i, leitura;
    char input[50];
    int mes_inf=1, mes_sup=1;
    clock_t ci, cf;
    int total_vendas_n = 0, total_vendas_p = 0;
    double total_fact_n = 0, total_fact_p = 0;
    int estado = QUERIE_7;

    while (estado == QUERIE_7) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 7               |\n");
        printf("|                                                       |\n");
        printf("|  Nº vendas e facturação mensal produto                |\n");
        printf("| ----------------------------------------------------- |\n");
        printf("| 1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair     |\n");
        printf(" ======================================================  \n");
        printf("Insira o codigo de produto a procurar > ");
        leitura = scanf("%s", input);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CONTABILIDADE;
                    break;
                case'2':
                    estado = MENU_PRINCIPAL;
                    break;
                case'3':
                    estado = SAIR_PROGRAMA;
                default:
                    printf("Insira o intervalo de meses (formato xx-yy) > ");
                    leitura = scanf("%d-%d", &mes_inf, &mes_sup);

                    if (leitura == 1) {
                        switch (mes_inf) {
                            case 1:
                                estado = FACE_CONTABILIDADE;
                                break;
                            case 2:
                                estado = MENU_PRINCIPAL;
                                break;
                            case 3:
                                estado = SAIR_PROGRAMA;
                            default:
                                break;
                        }
                    } else {
                        if (mes_inf > mes_sup) {
                            i = mes_inf;
                            mes_inf = mes_sup;
                            mes_sup = 1;
                        }
                    }
                    /*END SWITCH*/
                    break;
            }
        }
        
        if (estado == QUERIE_7) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CONTABILIDADE >> QUERIE 7                 \n");
            printf("Nº vendas e facturação mensal produto              \n");
            printf("========================================================= \n");

            if (cat_existe_produto(catalogo_produtos, input)) {
                ci = clock();
                printf("Codigo de produto: %s\n", input);
                printf("========================================================= \n");
                printf("     |  Vendas   |       ||    Facturacao     |           |\n");
                printf(" Mes |  P  |  N  | Total ||    P    |    N    |   Total   |\n");
                printf("--------------------------------------------------------- \n");
                for (i = mes_inf; i <= mes_sup; i++) {
                    printf("%4d | %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                            i,
                            cont_total_vendas_normais_produto_mes(contabilidade, input, i),
                            cont_total_vendas_promo_produto_mes(contabilidade, input, i),
                            cont_total_vendas_normais_produto_mes(contabilidade, input, i) + cont_total_vendas_promo_produto_mes(contabilidade, input, i),
                            cont_total_fact_normal_produto_mes(contabilidade, input, i),
                            cont_total_fact_promo_produto_mes(contabilidade, input, i),
                            cont_total_fact_normal_produto_mes(contabilidade, input, i) + cont_total_fact_promo_produto_mes(contabilidade, input, i));


                    total_vendas_n += cont_total_vendas_normais_produto_mes(contabilidade, input, i);
                    total_vendas_p += cont_total_vendas_promo_produto_mes(contabilidade, input, i);

                    total_fact_n += cont_total_fact_normal_produto_mes(contabilidade, input, i);
                    total_fact_p += cont_total_fact_promo_produto_mes(contabilidade, input, i);

                }
                cf=clock();
                printf("---------------------------------------------------------- \n");
                printf("TOTAL| %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                        total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                        total_fact_n, total_fact_p, total_fact_n + total_fact_p);
                printf("========================================================== \n");
                printf("Tempo: %5.3f ============================================= \n", (float)(cf-ci)/CLOCKS_PER_SEC);
            } else {
                printf("O produto nao existe\n");
                printf("========================================================== \n");
            }

            printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Procurar outro produto \n");
            printf("========================================================= \n");
            printf("Insira nº da opcao >");
            leitura = scanf("%s", input);
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CONTABILIDADE;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case 'Q':
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                case '4':
                    estado = QUERIE_7;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }

        }

    }/* END WHILE*/


    return estado;
}

int _08_clientes_compraram_prod() {
    return 1;
}

int _09_produtos_mais_comprados_cliente() {
    return 1;
}

int _10_clientes_regulares() {
    int i = 0, leitura = 0;
    int estado = QUERIE_10;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    COMPRAS_LISTA_CLIENTES lista_cli = NULL;
    COMPRAS_FICHA_CLIENTE ficha_cli = NULL;
    char input[50];
    
    ci = clock();
    
    lista_cli = compras_lista_clientes_regulares(mod_compras);
    resultados = compras_lista_cli_get_num_elems(lista_cli);
    total_pags = compras_lista_cli_get_num_pags(lista_cli);

    cf= clock();
    while (estado == QUERIE_10) {
        elems_pag = compras_lista_cli_get_pos_and_num_elems_pag(lista_cli, &inicio_pag, n_pagina);
        fim_pag = inicio_pag + elems_pag;
        printf("\033[2J\033[1;1H");
        printf("GESTHIPER >> COMPRAS >> QUERIE 10                 \n");
        printf("Cliente que compraram todos os meses \n");
        printf("================================================= \n");

        if (resultados > 0) {
            printf("Pagina %2d/%d \n", n_pagina, total_pags);
            printf("\n");

            for (i = 0; i < elems_pag; i++) {
                ficha_cli = compras_lista_get_fich_cli(lista_cli, inicio_pag + i);
                printf("%s\n", compras_get_cod_cli_ficha(ficha_cli));
            }

            printf("\n");
            printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
        } else {
            printf("Nao ha resultados a mostrar.\n");
        }

        printf("==================================================== \n");
        printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair    \n");
        printf("<<  4  |  <  5  |  6  >  |  7  >>  |     9 - Pag...  \n");
        printf("==================================================== \n");
        printf("Insira nº da opcao >");
        leitura = scanf("%s", input);

        switch (toupper(input[0])) {
            case '1':
                estado = FACE_COMPRAS;
                break;
            case '2':
                estado = MENU_PRINCIPAL;
                break;
            case 'Q':
            case '3':
                estado = SAIR_PROGRAMA;
                break;
            case '4':
                n_pagina = 1;
                break;
            case '5':
                if (n_pagina > 1)
                    n_pagina--;
                break;
            case '6':
                if (n_pagina < total_pags)
                    n_pagina++;
                break;
            case'7':
                n_pagina = total_pags;
                break;
            case '9':
                printf("Indique a pag para que quer ir >");
                leitura = scanf("%d", &escolha_pag);
                if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                    n_pagina = escolha_pag;
                break;
            default:
                estado = SAIR_PROGRAMA;
                break;
        }

    }
    compras_free_lista_clientes(lista_cli);
    return estado;
}

int _11_compras_CSV() {
    return 1;
}

int _12_prods_mais_vendidos() {
    int i = 0, leitura = 0;
    int estado = QUERIE_4;
    clock_t ci, cf;
    int n_top = 0;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CONT_LISTA_PRODUTOS lista_prod = NULL;
    CONT_FICHA_PRODUTO ficha_prod = NULL;
    char input[50];


    printf("\033[2J\033[1;1H");
    printf(" ======================================================  \n");
    printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 12              |\n");
    printf("|                                                       |\n");
    printf("|  Lista de produtos mais comprados                     |\n");
    printf("| ----------------------------------------------------- |\n");
    printf("| w - CONTABILIDADE | e - Menu Principal | q - Sair     |\n");
    printf(" ======================================================  \n");
    printf("Indique quantos produtos quer ver > ");
    leitura = scanf("%s", input);

    if (leitura > 0) {
        switch (toupper(input[0])) {
            case 'W':
                estado = FACE_CONTABILIDADE;
                break;
            case'E':
                estado = MENU_PRINCIPAL;
                break;
            case'Q':
                estado = SAIR_PROGRAMA;
            default:
                n_top = atoi(input);
                if (n_top <= 0 || n_top > INT_MAX) {
                    n_top = 100;
                    printf("Numero invalido.\n"
                            "Será mostrado o top 100 de produtos mais comprados\n"
                            "Carregue em qualquer tecla para continuar > ");
                    leitura = scanf("%s", input);
                }
                /*END SWITCH*/
                break;
        }
    }


    if (estado == QUERIE_4) {
        ci = clock();
        lista_prod = cont_top_produtos_comprados(contabilidade, n_top);
        resultados = cont_lista_prod_get_num_elems(lista_prod);
        total_pags = cont_lista_prod_get_num_pags(lista_prod);
        cf = clock();


        while (estado == QUERIE_4) {
            elems_pag = cont_lista_prod_get_pos_and_num_elems_pag(lista_prod, &inicio_pag, n_pagina);
            fim_pag = inicio_pag + elems_pag;
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CONTABILIDADE >> QUERIE 12                 \n");
            printf("Lista de produtos mais comprados\n");
            printf("================================================= \n");
            if (resultados > 0) {
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("------------------------\n");
                printf(" #  | Produto | Vendas |\n");
                printf("------------------------\n");
                for (i = 0; i < elems_pag; i++) {
                    ficha_prod = cont_lista_get_fich_prod(lista_prod, inicio_pag + i);
                    printf("%3d | %7s | %6d |\n", inicio_pag + i + 1,
                            cont_get_cod_prod_ficha(ficha_prod),
                            cont_total_vendas_fich_produto(ficha_prod));
                }

                printf("------------------------\n");
                printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float) (cf - ci) / CLOCKS_PER_SEC);
            } else {
                printf("Nao ha resultados a mostrar.\n");
            }

            printf("==================================================== \n");
            printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair    \n");
            printf("<<  4  |  <  5  |  6  >  |  7  >>  |     9 - Pag...  \n");
            printf("==================================================== \n");
            printf("Insira nº da opcao >");
            leitura = scanf("%s", input);

            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_CONTABILIDADE;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case 'Q':
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                case '4':
                    n_pagina = 1;
                    break;
                case '5':
                    if (n_pagina > 1)
                        n_pagina--;
                    break;
                case '6':
                    if (n_pagina < total_pags)
                        n_pagina++;
                    break;
                case'7':
                    n_pagina = total_pags;
                    break;
                case '9':
                    printf("Indique a pag para que quer ir >");
                    leitura = scanf("%d", &escolha_pag);
                    if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                        n_pagina = escolha_pag;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }

        }
        cont_free_lista_produtos(lista_prod);
    }

    return estado;
}

int _13_tres_prods_mais_comprados() {
    return 1;
}

int _14_clientes_prods_fantasma() {
    return 1;
}
/*
 * FUNCOES AUXILIARES
 */

int _05_aux_guarda_ficheiro(Compras mod_compras, char *cod_cli) {
    int estado = QUERIE_5;
    int i = 0, total_vendas = 0, escolha=0;
    FILE *fich = NULL;
    char input[TAM_INPUT];
    int leitura = -1;

    printf("\033[2J\033[1;1H");
    printf("GESTHIPER >> COMPRAS >> QUERIE 5 >> Guardar ficheiro \n");
    printf("Nº produtos comprados por cliente                \n");
    printf("------------------------------------------------------- \n");
    printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
    printf("4 - Procurar outro cliente\n");
    printf("======================================================= \n");
    printf("Indique o nome do ficheiro > ");
    leitura = scanf("%s", input);

    if (strcmp(input, "1") == 0) {
        estado = FACE_COMPRAS;
    }

    if (strcmp(input, "2") == 0) {
        estado = MENU_PRINCIPAL;
    }

    if (strcmp(input, "3") == 0) {
        estado = SAIR_PROGRAMA;
    }

    if (estado == QUERIE_5 && leitura > 0) {
        fich = fopen(input, "w");

        if (fich != NULL) {
            fprintf(fich, "Codigo de cliente: %s\n", cod_cli);
            fprintf(fich, "============== \n");
            fprintf(fich, "     | Vendas |\n");
            fprintf(fich, " Mes | Total  |\n");
            fprintf(fich, "--------------- \n");
            for (i = JANEIRO; i <= DEZEMBRO; i++) {
                fprintf(fich, "%4d | %6d |\n", i,
                        compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cli, i));

                total_vendas += compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cli, i);
            }
            
            fprintf(fich, "--------------- \n");
            fprintf(fich, "TOTAL| %6d |\n", total_vendas);
            fprintf(fich, "============== \n");

            fclose(fich);
            printf("Ficheiro guardado com sucesso.\n");
        } else {
            printf("Houve um erro ao abrir o ficheiro.\n");
        }

        printf("Insira o nº da opcao > ");
        leitura = scanf("%d", &escolha);

        switch (escolha){
            case 1:
                estado = FACE_COMPRAS;
                break;
            case 2:
                estado = MENU_PRINCIPAL;
                break;
            case 3:
                estado = SAIR_PROGRAMA;
                break;
            default:
                estado = QUERIE_5;
                break;
        }
    }

    return estado;
}


