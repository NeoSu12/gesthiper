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

#define TAM_PAGINA 10
#define TAM_INPUT 50

typedef enum mes {
    JANEIRO = 1, FEVEREIRO, MARCO, ABRIL, MAIO, JUNHO, JULHO, AGOSTO, SETEMBRO, OUTUBRO, NOVEMBRO, DEZEMBRO
} Mes;

static int _05_aux_guarda_ficheiro(Compras mod_compras, char *cod_cli);
static int _11_aux_cria_CSV(COMPRAS_NUM_CLIENTES_MENSAIS n_clientes_meses, int n_compras[]);

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;
extern Contabilidade contabilidade;
extern Compras mod_compras;

int _02_codigo_produtos_letra() {
    int i = 0, leitura = 0;
    int estado = QUERIE_2;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    clock_t ci, cf;
    CAT_LISTA_PRODUTOS lista_prod = NULL;
    CAT_PAG_LISTA_PRODUTOS paginador = NULL;
    char letra;
    char input[50];

    while (estado == QUERIE_2) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 2                 |\n");
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
            lista_prod = cat_lista_produtos_letra(catalogo_produtos, letra);
            paginador = cat_prod_inicializa_paginador_pag(lista_prod, 1, TAM_PAGINA);
            resultados = cat_prod_pag_get_num_elems(paginador);
            total_pags = cat_prod_get_num_pags(paginador);
            cf = clock();
            while (estado == QUERIE_2) {
                inicio_pag = cat_prod_goto_pag(paginador, n_pagina);
                elems_pag = cat_prod_pag_get_num_elems_pag(paginador);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("GESTHIPER >> CATALOGOS >> QUERIE 2                 \n");
                printf("A mostrar produtos começados pela letra %c\n", letra);
                printf("================================================= \n");
                if (resultados > 0) {
                    printf("Pagina %2d/%d \n", n_pagina, total_pags);
                    printf("------------------- \n");
                    printf("|   #   |  Codigo  |\n");
                    printf("------------------- \n");
                    for (i = 0; i < elems_pag; i++)
                        printf("| %5d | %8s |\n", inicio_pag + i+1, cat_lista_prod_get_elemento(lista_prod, inicio_pag + i));
                    printf("------------------- \n");
                    printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n",
                            inicio_pag + 1, fim_pag, resultados, (float) (cf - ci) / CLOCKS_PER_SEC);
                } else {
                    printf("Nao existem produtos comecados pela letra %c\n", letra);
                }
                printf("==================================================== \n");
                printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
                printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
                printf("==================================================== \n");
                printf("Insira nº da opcao > ");
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
                        printf("Indique a pag para que quer ir > ");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    default:
                        estado = SAIR_PROGRAMA;
                        break;
                }
            }
            
        }
    }
    cat_free_lista_produtos(lista_prod);
    cat_prod_free_pag(paginador);
    return estado;
}

int _03_compras_e_fact_mensal_prod() {
    int leitura, mes = -1;
    char input[50];
    clock_t ci, cf;
    int total_vendas_n = 0, total_vendas_p = 0;
    double total_fact_n = 0, total_fact_p = 0;
    int estado = QUERIE_3;

    while (estado == QUERIE_3) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 3               |\n");
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
                    estado = QUERIE_3;
                    while (mes < 1 || mes > 12) {
                        printf("Indique o mes > ");
                        leitura = scanf("%d", &mes);
                        if (mes < 1 || mes > 12) printf("Mes invalido\n");
                    }
            }
        }

        if (estado == QUERIE_3) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CONTABILIDADE >> QUERIE 3                 \n");
            printf("Nº vendas e facturação mensal produto              \n");
            printf("============================================================= \n");

            if (cat_existe_produto(catalogo_produtos, input)) {
                printf("Codigo de produto: %s\n", input);
                printf("============================================================= \n");
                printf("------------------------------------------------------------- \n");
                printf("|       |  Vendas   |       ||    Facturacao     |           |\n");
                printf("|  Mes  |  N  |  P  | Total ||    N    |    P    |   Total   |\n");
                printf("------------------------------------------------------------- \n");
                ci = clock();
                printf("| %5d | %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                        mes,
                        cont_total_vendas_normais_produto_mes(contabilidade, input, mes),
                        cont_total_vendas_promo_produto_mes(contabilidade, input, mes),
                        cont_total_vendas_normais_produto_mes(contabilidade, input, mes) + cont_total_vendas_promo_produto_mes(contabilidade, input, mes),
                        cont_total_fact_normal_produto_mes(contabilidade, input, mes),
                        cont_total_fact_promo_produto_mes(contabilidade, input, mes),
                        cont_total_fact_normal_produto_mes(contabilidade, input, mes) + cont_total_fact_promo_produto_mes(contabilidade, input, mes));


                total_vendas_n += cont_total_vendas_normais_produto_mes(contabilidade, input, mes);
                total_vendas_p += cont_total_vendas_promo_produto_mes(contabilidade, input, mes);

                total_fact_n += cont_total_fact_normal_produto_mes(contabilidade, input, mes);
                total_fact_p += cont_total_fact_promo_produto_mes(contabilidade, input, mes);

                cf = clock();
                printf("------------------------------------------------------------ \n");
                printf("Tempo: %5.5f ============================================= \n", (float) (cf - ci) / CLOCKS_PER_SEC);
            } else {
                printf("O produto nao existe\n");
                printf("============================================================= \n");
            }

            printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Procurar outro produto \n");
            printf("============================================================= \n");
            printf("Insira nº da opcao > ");
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

int _03_compras_e_fact_mensal_prod_old() {
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
                    estado = MENU_PRINCIPAL;
                default:
                    break;
            }
        }

        if (estado == QUERIE_3) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CONTABILIDADE >> QUERIE 3                 \n");
            printf("Nº vendas e facturação mensal produto              \n");
            printf("========================================================== \n");

            if (cat_existe_produto(catalogo_produtos, input)) {
                printf("Codigo de produto: %s\n", input);
                printf("========================================================== \n");
                printf("---------------------------------------------------------- \n");
                printf("     |  Vendas   |       ||    Facturacao     |           |\n");
                printf(" Mes |  N  |  P  | Total ||    N    |    P    |   Total   |\n");
                printf("---------------------------------------------------------- \n");
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
                printf("----------------------------------------------------------- \n");
                printf("TOTAL| %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                        total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                        total_fact_n, total_fact_p, total_fact_n + total_fact_p);
                printf("----------------------------------------------------------- \n");
                printf("Tempo: %5.5f ============================================= \n", (float)(cf-ci)/CLOCKS_PER_SEC);
            } else {
                printf("O produto nao existe\n");
                printf("=========================================================== \n");
            }

            printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Procurar outro produto \n");
            printf("=========================================================== \n");
            printf("Insira nº da opcao > ");
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
                    estado = QUERIE_3;
                    break;
                default:
                    estado = MENU_PRINCIPAL;
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
    CONT_PAG_LISTA_PRODUTOS paginador = NULL;
    char input[50];
    
    ci = clock();
    
    lista_prod = cont_lista_prod_sem_compras(contabilidade);
    paginador = cont_inicializa_paginador_pag(lista_prod, 1, TAM_PAGINA);
    resultados = cont_pag_get_num_elems(paginador);
    total_pags = cont_get_num_pags(paginador);

    cf= clock();
    while (estado == QUERIE_4) {
        inicio_pag = cont_goto_pag(paginador, n_pagina);
        elems_pag = cont_pag_get_num_elems_pag(paginador);
        fim_pag = inicio_pag + elems_pag;
        printf("\033[2J\033[1;1H");
        printf("================================================= \n");
        printf("GESTHIPER >> CONTABILIDADE >> QUERIE 4            \n");
        printf("Lista de produtos nao comprados                   \n");
        printf("================================================= \n");

        if (resultados > 0) {
            printf("Pagina %2d/%d \n", n_pagina, total_pags);
            printf("--------------------\n");
            printf("|   #   |  Codigo  |\n");
            printf("--------------------\n");
            for (i = 0; i < elems_pag; i++) {
                ficha_prod = cont_lista_get_fich_prod(lista_prod, inicio_pag + i);
                printf("| %5d | %8s |\n", inicio_pag + i+1,cont_get_cod_prod_ficha(ficha_prod));
            }

            printf("--------------------\n");
            printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
        } else {
            printf("Nao ha resultados a mostrar.\n");
        }

        printf("==================================================== \n");
        printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair    \n");
        printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
        printf("==================================================== \n");
        printf("Insira nº da opcao > ");
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
                printf("Indique a pag para que quer ir > ");
                leitura = scanf("%d", &escolha_pag);
                if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                    n_pagina = escolha_pag;
                break;
            default:
                estado = SAIR_PROGRAMA;
                break;
        }

    }
    cont_free_pag(paginador);
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
                    estado = MENU_PRINCIPAL;
                default:
                    break;
            }
        }

        
        if (estado == QUERIE_5) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> COMPRAS >> QUERIE 5                 \n");
            printf("Nº produtos comprados por cliente                \n");
            printf("================================================ \n");

            if (cat_existe_cliente(catalogo_clientes, input)) {
                cliente_existe=1;
                cod_cliente = (char *) malloc(sizeof(char)*(strlen(input)+1));
                strcpy(cod_cliente, input);
                printf("Codigo de cliente: %s\n", cod_cliente);
                printf("================================================ \n");
                printf("------------------  \n");
                printf("|       | Vendas | \n");
                printf("|  Mes  | Total  | \n");
                printf("------------------  \n");
                ci=clock();
                for (i = JANEIRO; i <= DEZEMBRO; i++) {
                    printf("| %5d | %6d | \n",
                            i,
                            compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cliente, i));

                    total_vendas += compras_produtos_comprados_cod_cliente_mes(mod_compras, cod_cliente, i);
                    

                }
                cf=clock();
                printf("------------------  \n");
                printf("| TOTAL | %6d |\n", total_vendas);
                printf("------------------  \n");
                printf("Tempo: %5.5f ================================= \n",
                        (float)(cf-ci)/CLOCKS_PER_SEC);
            } else {
                cliente_existe=0;
                printf("O cliente nao existe\n");
                printf("================================================ \n");
            }

            printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Ver outro cliente | 5 - Guardar em ficheiro \n");
            printf("================================================ \n");
            printf("Insira nº da opcao > ");
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
                    estado = MENU_PRINCIPAL;
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
    CAT_PAG_LISTA_CLIENTES paginador = NULL;
    char letra;
    char input[50];

    while (estado == QUERIE_6) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 6                 |\n");
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
            lista_cli = cat_lista_clientes_letra(catalogo_clientes, letra);
            paginador = cat_cli_inicializa_paginador_pag(lista_cli,1, TAM_PAGINA);
            resultados = cat_lista_cli_get_num_elems(lista_cli);
            total_pags = cat_cli_get_num_pags(paginador);
            cf=clock();
            while (estado == QUERIE_6) {
                inicio_pag = cat_cli_goto_pag(paginador, n_pagina);
                elems_pag = cat_cli_pag_get_num_elems_pag(paginador);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("GESTHIPER >> CATALOGOS >> QUERIE 6                 \n");
                printf("A mostrar clientes começados pela letra %c\n", letra);
                printf("================================================= \n");
                if(resultados > 0){
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("------------------- \n");
                printf("|   #   |  Codigo  |\n");
                printf("------------------- \n");
                for (i = 0; i < elems_pag; i++)
                    printf("| %5d | %8s |\n", inicio_pag +i+1, cat_lista_cli_get_elemento(lista_cli, inicio_pag + i));

                printf("------------------- \n");
                printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
                }else{
                    printf("Nao existem clientes comecados pela letra %c\n", letra);
                }
                printf("==================================================== \n");
                printf("1 - CATALOGOS | 2 - Menu Principal | 3 - Sair        \n");
                printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
                printf("==================================================== \n");
                printf("Insira nº da opcao > ");
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
                        printf("Indique a pag para que quer ir > ");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    default:
                        estado = MENU_PRINCIPAL;
                        break;
                }
            }
        
        }
    }
    
    cat_free_lista_clientes(lista_cli);
    cat_cli_free_pag(paginador);
    return estado;
}

int _07_compras_intervalo_meses() {
    int i, leitura;
    char input[50];
    int mes_inf = 1, mes_sup = 1;
    clock_t ci, cf;
    int total_vendas_n = 0, total_vendas_p = 0;
    double total_fact_n = 0, total_fact_p = 0;
    int estado = QUERIE_7;

    while (estado == QUERIE_7) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 7               |\n");
        printf("|  Nº compras e facturação mensal produto               |\n");
        printf("| ----------------------------------------------------- |\n");
        printf("| 1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair     |\n");
        printf(" ======================================================  \n");
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
            if(mes_inf < 1 || mes_inf > 12 || mes_sup < 1 || mes_sup > 12){
                mes_inf = 1;
                mes_sup = 12;
            }
        }

        if (estado == QUERIE_7) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> CONTABILIDADE >> QUERIE 7                 \n");
            printf("Nº compras e facturação mensal global              \n");
            printf("================================================================================== \n");

            ci = clock();
            printf("---------------------------------------------------------------------------------- \n");
            printf("|       |      Compras      |         ||        Facturacao         |              |\n");
            printf("|  Mes  |    N    |    P    |  Total  ||      N      |      P      |    Total     |\n");
            printf("---------------------------------------------------------------------------------- \n");
            for (i = mes_inf; i <= mes_sup; i++) {
                printf("| %5d | %7d | %7d | %7d || %11.2f | %11.2f | %12.2f |\n",
                        i,
                        cont_total_compras_normais_mes(contabilidade, i),
                        cont_total_compras_promo_mes(contabilidade, i),
                        cont_total_compras_mes(contabilidade, i),
                        cont_total_facturacao_normal_mes(contabilidade, i),
                        cont_total_facturacao_promo_mes(contabilidade, i),
                        cont_total_facturacao_mes(contabilidade, i));


                total_vendas_n += cont_total_compras_normais_mes(contabilidade, i);
                total_vendas_p += cont_total_compras_promo_mes(contabilidade, i);

                total_fact_n += cont_total_facturacao_normal_mes(contabilidade, i);
                total_fact_p += cont_total_facturacao_promo_mes(contabilidade, i);

            }
            cf = clock();
            printf("---------------------------------------------------------------------------------- \n");
            printf("| TOTAL | %7d | %7d | %7d || %11.2f | %11.2f | %12.2f |\n",
                    total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                    total_fact_n, total_fact_p, total_fact_n + total_fact_p);
            printf("---------------------------------------------------------------------------------- \n");
            printf("Tempo: %5.5f =================================================================== \n", (float) (cf - ci) / CLOCKS_PER_SEC);


            printf("1 - CONTABILIDADE | 2 - Menu Principal | 3 - Sair        \n");
            printf("================================================================================== \n");
            printf("Insira nº da opcao > ");
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
                default:
                    estado = MENU_PRINCIPAL;
                    break;
            }

        }

    }/* END WHILE*/


    return estado;
}

int _08_clientes_compraram_prod() {
    int i = 0, leitura = 0;
    int estado = QUERIE_8;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    COMPRAS_ASSOC_PROD_CLIENTES lista_assoc = NULL;
    COMPRAS_CLIENTE_TIPO_COMPRA cliente_t_compra = NULL;
    COMPRAS_PAG_ASSOC_PROD_CLIENTES paginador = NULL;
    char *cod_produto= NULL;
    char input[50];
    
    while (estado == QUERIE_8) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> COMPRAS >> QUERIE 8                   |\n");
        printf("|  Lista de clientes que compraram produto            |\n");
        printf("| --------------------------------------------------- |\n");
        printf("| 1 - COMPRAS | 2 - Menu Principal | 3 - Sair         |\n");
        printf(" ====================================================  \n");
        printf("Insira codigo de produto > ");
        leitura = scanf("%s", input);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    estado = FACE_COMPRAS;
                    break;
                case '2':
                    estado = MENU_PRINCIPAL;
                    break;
                case '3':
                    estado = SAIR_PROGRAMA;
                    break;
                default:
                    estado = QUERIE_8;
                    cod_produto = (char *) malloc(sizeof(char)*(strlen(input)+1));
                    strcpy(cod_produto, input);
                    break;
            }
        }
        
        if (leitura > 0 && estado == QUERIE_8) {
            ci = clock();
            lista_assoc = compras_get_associacao_produto_clientes_tipo_compra(mod_compras, cod_produto);
            paginador = compras_assoc_inicializa_paginador_pag(lista_assoc, 1, TAM_PAGINA);
            resultados = compras_lista_assoc_prod_clis_get_num_elems(lista_assoc);
            total_pags = compras_assoc_get_num_pags(paginador);
            cf = clock();
            while (estado == QUERIE_8) {
                inicio_pag = compras_assoc_goto_pag(paginador, n_pagina);
                elems_pag = compras_assoc_prod_cli_pag_get_num_elems_pag(paginador);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("GESTHIPER >> COMPRAS >> QUERIE 8                 \n");
                printf("Lista de clientes que compraram produto\n");
                printf("================================================= \n");
                printf("Codigo Produto: %s\n", cod_produto);
                printf("================================================= \n");
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("------------------------------- \n");
                printf("|   #   | Codigo | Tipo Compra |\n");
                printf("------------------------------- \n");
                for (i = 0; i < elems_pag; i++){
                    cliente_t_compra = compras_get_cli_compra_from_prod_cli(lista_assoc, inicio_pag + i);
                    
                    printf("| %5d | %6s | %11c |\n",
                    inicio_pag + i +1,
                    compras_assoc_cli_compra_get_cod_cli(cliente_t_compra),
                    compras_assoc_cli_compra_get_tipo_compra(cliente_t_compra));
                }
                printf("------------------------------- \n");
                printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
                printf("==================================================== \n");
                printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
                printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
                printf("==================================================== \n");
                printf("Insira nº da opcao > ");
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
                        printf("Indique a pag para que quer ir > ");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    default:
                        estado = SAIR_PROGRAMA;
                        break;
                }
            }
        }
    }
    compras_free_assoc_prod_clientes(lista_assoc);
    compras_assoc_free_pag(paginador);
    free(cod_produto);
    return estado;
}

int _09_produtos_mais_comprados_cliente_mes() {
    int i = 0, leitura = 0, mes = 0;
    int estado = QUERIE_9;
    clock_t ci, cf;
    char *cod_cliente = NULL;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    COMPRAS_LISTA_PRODUTOS lista_prod = NULL;
    COMPRAS_FICHA_PRODUTO ficha_prod = NULL;
    COMPRAS_PAG_LISTA_PRODUTOS paginador = NULL;
    char input[50];


    printf("\033[2J\033[1;1H");
    printf(" ======================================================  \n");
    printf("|  GESTHIPER >> COMPRAS >> QUERIE 9                     |\n");
    printf("|  Produtos mais comprados por cliente                  |\n");
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
                estado = QUERIE_9;
                cod_cliente = (char *) malloc(sizeof (char)*(strlen(input) + 1));
                strcpy(cod_cliente, input);
                while(mes < 1 || mes > 12){
                printf("Indique o mes > ");
                leitura = scanf("%d", &mes);
                if(mes < 1 || mes > 12) printf("Mes invalido\n");
                }
                break;
        }
    }

    if (estado == QUERIE_9 && cat_existe_cliente(catalogo_clientes, cod_cliente)) {

        ci = clock();

        lista_prod = compras_produtos_mais_comprados_cliente_mes(mod_compras, cod_cliente, mes);
        paginador = compras_prod_inicializa_paginador_pag(lista_prod,1, TAM_PAGINA);
        resultados = compras_lista_prod_get_num_elems(lista_prod);
        total_pags = compras_prod_get_num_pags(paginador);

        cf = clock();

        while (estado == QUERIE_9) {
            inicio_pag = compras_prod_goto_pag(paginador, n_pagina);
            elems_pag = compras_prod_pag_get_num_elems_pag(paginador);
            fim_pag = inicio_pag + elems_pag;
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> COMPRAS >> QUERIE 9                 \n");
            printf("Produtos mais comprados por cliente\n");
            printf("================================================= \n");
            printf("Codigo Cliente: %s | Mes: %d\n", cod_cliente, mes);
            printf("================================================= \n");

            if (resultados > 0) {
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("------------------------------- \n");
                printf("|   #   |  Codigo  | Nº vendas |\n");
                printf("------------------------------- \n");
                for (i = 0; i < elems_pag; i++) {
                    ficha_prod = compras_lista_get_fich_prod(lista_prod, inicio_pag + i);
                    printf("| %5d | %8s | %9d |\n", inicio_pag + i + 1,
                                                    compras_get_cod_prod_ficha(ficha_prod), 
                                                    compras_produtos_comprados_ficha_produto_mes(ficha_prod, mes));
                }
                printf("------------------------------- \n");
                printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float) (cf - ci) / CLOCKS_PER_SEC);
            } else {
                printf("Nao ha resultados a mostrar.\n");
            }

            printf("==================================================== \n");
            printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair    \n");
            printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
            printf("==================================================== \n");
            printf("Insira nº da opcao > ");
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
                    printf("Indique a pag para que quer ir > ");
                    leitura = scanf("%d", &escolha_pag);
                    if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                        n_pagina = escolha_pag;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }
        }
    } else {
        printf("Cliente nao existe\n");
    }
    
    compras_free_lista_produtos(lista_prod);
    compras_prod_free_pag(paginador);
    free(cod_cliente);
    return estado;
}

int _10_clientes_regulares() {
    int i = 0, leitura = 0;
    int estado = QUERIE_10;
    clock_t ci, cf;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    COMPRAS_LISTA_CLIENTES lista_cli = NULL;
    COMPRAS_FICHA_CLIENTE ficha_cli = NULL;
    COMPRAS_PAG_LISTA_CLIENTES paginador = NULL;
    char input[50];
    
    ci = clock();
    
    lista_cli   = compras_lista_clientes_regulares(mod_compras);
    paginador   = compras_cli_inicializa_paginador_pag(lista_cli, 1 , TAM_PAGINA);
    resultados  = compras_lista_cli_get_num_elems(lista_cli);
    total_pags  = compras_cli_get_num_pags(paginador);

    cf= clock();
    while (estado == QUERIE_10) {
        inicio_pag = compras_cli_goto_pag(paginador, n_pagina);
        elems_pag = compras_cli_pag_get_num_elems_pag(paginador);
        fim_pag = inicio_pag + elems_pag;
        printf("\033[2J\033[1;1H");
        printf("GESTHIPER >> COMPRAS >> QUERIE 10                 \n");
        printf("Clientes que compraram todos os meses \n");
        printf("================================================= \n");

        if (resultados > 0) {
            printf("Pagina %2d/%d \n", n_pagina, total_pags);
            printf("------------------- \n");
            printf("|   #   |  Codigo  |\n");
            printf("------------------- \n");

            for (i = 0; i < elems_pag; i++) {
                ficha_cli = compras_lista_get_fich_cli(lista_cli, inicio_pag + i);
                printf("| %5d | %8s |\n",inicio_pag + i + 1 ,compras_get_cod_cli_ficha(ficha_cli));
            }

            printf("------------------- \n");
            printf("A mostrar %d-%d de %d resultados (em %.5f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float)(cf-ci)/CLOCKS_PER_SEC);
        } else {
            printf("Nao ha resultados a mostrar.\n");
        }

        printf("==================================================== \n");
        printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair    \n");
        printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
        printf("==================================================== \n");
        printf("Insira nº da opcao > ");
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
                printf("Indique a pag para que quer ir > ");
                leitura = scanf("%d", &escolha_pag);
                if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                    n_pagina = escolha_pag;
                break;
            default:
                estado = SAIR_PROGRAMA;
                break;
        }

    }
    
    compras_cli_free_pag(paginador);
    compras_free_lista_clientes(lista_cli);
    return estado;
}

int _11_compras_CSV() {
    int i, leitura;
    int total_compras=0, total_clientes=0;
    int n_compras[12];
    COMPRAS_NUM_CLIENTES_MENSAIS n_clientes_meses= NULL;
    char input[50];
    clock_t ci, cf;
    int estado = QUERIE_11;

    while (estado == QUERIE_11) {
        ci = clock();
        for(i=0;i<12;i++) 
            n_compras[i] = cont_total_compras_mes(contabilidade, i+1);
        n_clientes_meses = compras_num_clientes_por_mes(mod_compras);
        cf = clock();
        
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> COMPRAS >> QUERIE 11                    |\n");
        printf("|  Nº Compras e clientes mensais                        |\n");
        printf(" ======================================================  \n");
        printf("----------------------------------- \n");
        printf("|  Mes  | Nº Compras | Nº Clientes |\n");
        printf("----------------------------------- \n");
        for (i = JANEIRO; i <= DEZEMBRO; i++) {
            printf("| %5d | %10d | %11d |\n", i, n_compras[i-1], compras_get_num_clientes_mes(n_clientes_meses, i));
                    
            total_compras   += n_compras[i-1];
            total_clientes  += compras_get_num_clientes_mes(n_clientes_meses, i);
        }
        
        printf("----------------------------------- \n");
        printf("| TOTAL | %10d | %11d |\n", total_compras, total_clientes);
        printf("----------------------------------- \n");
        printf("Tempo: %5.5f ============================================= \n", (float) (cf - ci) / CLOCKS_PER_SEC);
        printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair       \n");
        printf("4 - Guardar em ficheiro                           \n");
        printf("========================================================== \n");
        printf("Insira nº da opcao > ");
        leitura = scanf("%s", input);
        
        if(leitura>0){
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
                estado = _11_aux_cria_CSV(n_clientes_meses,n_compras);
                break;
            default:
                estado = SAIR_PROGRAMA;
                break;
        }
        }else{
            estado = MENU_PRINCIPAL;
        }

    }/* END WHILE*/
    compras_free_num_clientes_mensais(n_clientes_meses);
    return estado;
}

int _12_prods_mais_vendidos() {
    int i = 0, leitura = 0;
    int estado = QUERIE_12;
    clock_t ci, cf;
    int n_top = 0;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    int *n_clientes = NULL;
    CONT_LISTA_PRODUTOS lista_prod = NULL;
    CONT_FICHA_PRODUTO ficha_prod = NULL;
    CONT_PAG_LISTA_PRODUTOS paginador = NULL;
    char input[50];


    printf("\033[2J\033[1;1H");
    printf(" ======================================================  \n");
    printf("|  GESTHIPER >> COMPRAS >> QUERIE 12                    |\n");
    printf("|  Lista de produtos mais comprados                     |\n");
    printf("| ----------------------------------------------------- |\n");
    printf("| w - COMPRAS | e - Menu Principal | q - Sair           |\n");
    printf(" ======================================================  \n");
    printf("Indique quantos produtos quer ver > ");
    leitura = scanf("%s", input);

    if (leitura > 0) {
        switch (toupper(input[0])) {
            case 'W':
                estado = FACE_COMPRAS;
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
                            "Serão mostrados os 100 de produtos mais comprados\n"
                            "Carregue em qualquer tecla para continuar > ");
                    leitura = scanf("%s", input);
                }
                /*END SWITCH*/
                break;
        }
    }


    if (estado == QUERIE_12) {
        ci = clock();
        printf("A obter lista ordenada produtos...");
        lista_prod = cont_top_produtos_comprados(contabilidade, n_top);
        printf("concluido.\n");
        paginador = cont_inicializa_paginador_pag(lista_prod, 1, TAM_PAGINA);
        resultados = cont_lista_get_num_elems(lista_prod);
        total_pags = cont_get_num_pags(paginador);
        n_clientes = (int *) malloc(sizeof(int)*resultados);
        printf("A calcular nº clientes para cada produto...");
        printf("\033[s");
        for(i=0;i<resultados;i++){
            /*
            if(i==10 || i==resultados-1){
                printf("\033[u");
                printf("A processar nº clientes de cada produto %d/%d (%.2f %%)\n", 
                            i+1,resultados, (double) ((double)(i+1)/(double)resultados)*100);
            }
             */
            ficha_prod = cont_lista_get_fich_prod(lista_prod, i);
            n_clientes[i] = compras_num_clientes_compraram_prod(mod_compras,cont_get_cod_prod_ficha(ficha_prod));
        }
        printf("concluido.\n");
        cf = clock();


        while (estado == QUERIE_12) {
            inicio_pag = cont_goto_pag(paginador, n_pagina);
            elems_pag = cont_pag_get_num_elems_pag(paginador);
            fim_pag = inicio_pag + elems_pag;
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> COMPRAS >> QUERIE 12                 \n");
            printf("Lista de produtos mais comprados\n");
            printf("================================================= \n");
            if (resultados > 0) {
                printf("Pagina %2d/%d \n", n_pagina, total_pags);
                printf("------------------------------------ \n");
                printf("|   #   | Produto | Vendas | Nº Cli |\n");
                printf("------------------------------------ \n");
                for (i = 0; i < elems_pag; i++) {
                    ficha_prod = cont_lista_get_fich_prod(lista_prod, inicio_pag + i);
                    printf("| %5d | %7s | %6d | %6d |\n", inicio_pag + i + 1,
                            cont_get_cod_prod_ficha(ficha_prod),
                            cont_total_vendas_fich_produto(ficha_prod),
                            n_clientes[inicio_pag+i]);
                }

                printf("------------------------------------ \n");
                printf("A mostrar %d-%d de %d resultados (em %.2f segundos)\n", inicio_pag + 1, fim_pag, resultados, (float) (cf - ci) / CLOCKS_PER_SEC);
            } else {
                printf("Nao ha resultados a mostrar.\n");
            }

            printf("==================================================== \n");
            printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair          \n");
            printf("[<<] 4   [<] 5  ###  6 [>]   7 [>>]  |   9 - Pag...  \n");
            printf("==================================================== \n");
            printf("Insira nº da opcao > ");
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
                    printf("Indique a pag para que quer ir > ");
                    leitura = scanf("%d", &escolha_pag);
                    if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                        n_pagina = escolha_pag;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }
        }
    }
    
    cont_free_lista_produtos(lista_prod);
    free(n_clientes);
    cont_free_pag(paginador);
    return estado;
}

int _13_tres_prods_mais_comprados() {
    int i, leitura;
    char input[50];
    char *cod_cliente=NULL;
    clock_t ci, cf;
    int estado = QUERIE_13;
    COMPRAS_LISTA_PRODUTOS lista = NULL;
    COMPRAS_FICHA_PRODUTO produto = NULL;

    while (estado == QUERIE_13) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> COMPRAS >> QUERIE 13                    |\n");
        printf("|  Tres produtos mais comprados por cliente             |\n");
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

        
        if (estado == QUERIE_13) {
            printf("\033[2J\033[1;1H");
            printf("GESTHIPER >> COMPRAS >> QUERIE 13                 \n");
            printf("Tres produtos mais comprados por cliente         \n");
            printf("========================================================== \n");

            if (cat_existe_cliente(catalogo_clientes, input)) {
                ci=clock();
                cod_cliente = (char *) malloc(sizeof(char)*(strlen(input)+1));
                strcpy(cod_cliente, input);
                lista = compras_top_n_produtos_mais_comprados_cliente(mod_compras, cod_cliente, 3);
                cf=clock();
                
                printf("Codigo de cliente: %s\n", cod_cliente);
                printf("========================================================== \n");
                printf("-------------------------- \n");
                printf("|  #  |  Codigo  | Vendas |\n");
                printf("-------------------------- \n");
                for (i = 0; i < 3; i++) {
                    produto = compras_lista_get_fich_prod(lista,i);
                    printf("| %3d | %8s | %6d |\n", i+1, compras_get_cod_prod_ficha(produto),
                                                        compras_total_produtos_comprados_ficha(produto));
                }
                printf("-------------------------- \n");
                printf("Tempo: %5.5f =========================================== \n",(float)(cf-ci)/CLOCKS_PER_SEC);
                compras_free_lista_produtos(lista);
            } else {
                printf("O cliente nao existe\n");
                printf("========================================================== \n");
            }

            printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
            printf("4 - Ver outro cliente \n");
            printf("========================================================== \n");
            printf("Insira nº da opcao > ");
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
                    estado = QUERIE_13;
                    break;
                default:
                    estado = MENU_PRINCIPAL;
                    break;
            }

        }

    }/* END WHILE*/
    free(cod_cliente);
    return estado;
}

int _14_clientes_prods_fantasma() {
    int n_produtos, n_clientes, leitura;
    char input[TAM_INPUT];
    clock_t ci, cf;
    int estado = QUERIE_14;

    while (estado == QUERIE_14) {
        ci = clock();
        n_produtos = cont_num_prod_sem_compras(contabilidade);
        n_clientes = compras_num_cliente_sem_compras(mod_compras);
        cf = clock();
        printf("\033[2J\033[1;1H");
        printf(" ======================================================= \n");
        printf("|  GESTHIPER >> COMPRAS >> QUERIE 14                    |\n");
        printf("|  Produtos nao vendidos e clientes sem compras         |\n");
        printf(" ======================================================= \n");
        printf(" ------------------------------------------------------- \n");
        printf(" Nº produtos sem compras: %8d                            \n", n_produtos);
        printf(" Nº clientes sem compras: %8d                            \n", n_clientes);
        printf(" ------------------------------------------------------- \n");
        printf(" Tempo: %6.5f segundos                                   \n", (float) (cf - ci) / CLOCKS_PER_SEC);
        printf(" ======================================================= \n");
        printf("| 1 - COMPRAS | 2 - Menu Principal | 3 - Sair           |\n");
        printf(" ======================================================= \n");

        printf("Insira nº da opcao > ");
        leitura = scanf("%s", input);
        if (leitura > 0) {
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
                    estado = QUERIE_14;
                    break;
                default:
                    estado = SAIR_PROGRAMA;
                    break;
            }
        }

    }/* END WHILE*/


    return estado;
}

/*
 * FUNCOES AUXILIARES
 */

static int _05_aux_guarda_ficheiro(Compras mod_compras, char *cod_cli) {
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

static int _11_aux_cria_CSV(COMPRAS_NUM_CLIENTES_MENSAIS n_clientes_meses, int n_compras[]) {
    int estado = QUERIE_11;
    int i = 0, escolha = 0;
    FILE *fich = NULL;
    char input[TAM_INPUT];
    int leitura = -1;

    printf("\033[2J\033[1;1H");
    printf("GESTHIPER >> COMPRAS >> QUERIE 11 >> Guardar ficheiro \n");
    printf("\n");
    printf("Nº Compras e clientes mensais                         \n");
    printf("------------------------------------------------------- \n");
    printf("1 - COMPRAS | 2 - Menu Principal | 3 - Sair        \n");
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

    if (estado == QUERIE_11 && leitura > 0) {
        fich = fopen(input, "w");

        if (fich != NULL) {
            fprintf(fich, "\"Mes\",\"#Compras\",\"#Clientes\"\n");
            for (i = JANEIRO; i <= DEZEMBRO; i++) {
                fprintf(fich, "\"%d\",\"%d\",\"%d\"\n",
                        i,
                        n_compras[i - 1],
                        compras_get_num_clientes_mes(n_clientes_meses, i));
            }

            fclose(fich);
            printf("Ficheiro guardado com sucesso.\n");
        } else {
            printf("Houve um erro ao abrir o ficheiro.\n");
        }

        printf("Insira o nº da opcao > ");
        leitura = scanf("%d", &escolha);

        switch (escolha) {
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
                estado = QUERIE_11;
                break;
        }
    }

    return estado;
}

