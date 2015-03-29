#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/queries.h"
#include "headers/cat_produtos.h"
#include "headers/cat_clientes.h"
#include "headers/arrays_dinamicos.h"
#include "headers/contabilidade.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

#define TAM_PAGINA 20

typedef enum mes {
    JANEIRO = 1, FEVEREIRO, MARCO, ABRIL, MAIO, JUNHO, JULHO, AGOSTO, SETEMBRO, OUTUBRO, NOVEMBRO, DEZEMBRO
} Mes;

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;
extern Contabilidade contabilidade;

ARRAY_DINAMICO _06_clientes_to_ad(char);
void free_str(void *);

int _02_codigo_produtos_letra() {
    int i = 0, leitura = 0;
    int sair_menu = 0, sair_programa = 0;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CAT_LISTA_PRODUTOS lista_prod = NULL;
    char letra;
    char input[50];

    while (sair_menu == 0) {
        printf("\033[2J\033[1;1H");
        printf(" ====================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 2                 |\n");
        printf("|                                                     |\n");
        printf("|  Listar produtos começados por letra                |\n");
        printf("| --------------------------------------------------- |\n");
        printf("| 1- Voltar para CONTABILIDADE | 2 - Sair programa    |\n");
        printf(" ====================================================  \n");
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);
        
        if(leitura>0){
        switch (toupper(input[0])) {
                case '1':
                    sair_menu = 1;
                    sair_programa = 0;
                    break;
                case'2':
                    sair_menu = 1;
                    sair_programa = 1;
                    break;
                default:
                    break;
            }
        }
        if (isalpha(letra) && leitura > 0 && !sair_menu) {
            lista_prod = cat_lista_produtos_letra(catalogo_produtos, letra, TAM_PAGINA);
            resultados = cat_lista_prod_get_num_elems(lista_prod);
            total_pags = cat_lista_prod_get_num_pags(lista_prod);

            while (sair_menu == 0) {
                elems_pag = cat_lista_prod_get_pos_and_num_elems_pag(lista_prod, &inicio_pag, n_pagina);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("-----------------\n");
                printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
                printf("-----------------\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n", cat_lista_prod_get_elemento(lista_prod, inicio_pag + i));

                printf("\n");

                printf("A mostrar %d-%d de %d resultados\n", inicio_pag + 1, fim_pag, resultados);
                printf("---------------------------------------------------------\n");
                printf("1- << | 2- < | 3 - > | 4 - >> | 5 - Ir Para pag... | 0- Voltar | Q - Sair\n");
                printf("Escolha >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '0':
                        sair_menu = 1;
                        break;
                    case '1':
                        n_pagina = 1;
                        break;
                    case '2':
                        if (n_pagina > 1)
                            n_pagina--;
                        break;
                    case '3':
                        if (n_pagina < total_pags)
                            n_pagina++;
                        break;
                    case'4':
                        n_pagina = total_pags;
                        break;
                    case '5':
                        printf("Indique a pag para que quer ir >");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    case 'Q':
                        sair_menu = 1;
                        sair_programa = 1;
                        break;
                    default:
                        sair_menu = 1;
                        break;
                }
            }
            cat_free_lista_produtos(lista_prod);
            
        } else {
            sair_menu = 1;
        }
    }
    return sair_programa;
}

int _03_compras_e_fact_mensal_prod() {
    int i, leitura;
    char input[50];
    int total_vendas_n = 0, total_vendas_p = 0;
    double total_fact_n = 0, total_fact_p = 0;
    int sair_menu = 0, sair_programa = 0;


    while (sair_menu == 0) {
        printf("\033[2J\033[1;1H");
        printf(" ======================================================  \n");
        printf("|  GESTHIPER >> CONTABILIDADE >> QUERIE 3               |\n");
        printf("|                                                       |\n");
        printf("|  Nº vendas e facturação mensal produto                |\n");
        printf("| ----------------------------------------------------- |\n");
        printf("| 1- Voltar para CONTABILIDADE | 2 - Sair programa      |\n");
        printf(" ======================================================  \n");
        printf("Insira o codigo de produto a procurar > ");
        leitura = scanf("%s", input);

        if (leitura > 0) {
            switch (toupper(input[0])) {
                case '1':
                    sair_menu = 1;
                    sair_programa = 0;
                    break;
                case'2':
                    sair_menu = 1;
                    sair_programa = 1;
                    break;
                default:
                    break;
            }

            if (cat_existe_produto(catalogo_produtos, input) && !sair_menu) {
                printf("\033[2J\033[1;1H");
                printf("Codigo de produto: %s\n", input);
                printf("========================================================= \n");
                printf("     |  Vendas   |       ||    Facturacao     |           |\n");
                printf(" Mes |  P  |  N  | Total ||    P    |    N    |   Total   |\n");
                printf("========================================================== \n");
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
                printf("========================================================== \n");
                printf("TOTAL| %3d | %3d | %5d || %7.2f | %7.2f | %9.2f |\n",
                        total_vendas_n, total_vendas_p, total_vendas_n + total_vendas_p,
                        total_fact_n, total_fact_p, total_fact_n + total_fact_p);
            } else {
                if (!sair_menu) printf("Produto nao encontrado\n");
            }
        }

    }

    return sair_programa;
}

int _04_prods_nao_comprados() {
    return 1;
}

int _05_tabela_cliente() {
    return 1;
}

int _06_codigos_clientes_letra() {
    int i = 0, leitura = 0;
    int sair_menu = 0, sair_programa = 0;
    int n_pagina = 1, elems_pag = 0, inicio_pag = 1, fim_pag = 0;
    int resultados = 0, total_pags = 0, escolha_pag = 0;
    CAT_LISTA_CLIENTES lista_cli = NULL;
    char letra;
    char input[50];

    while (sair_menu == 0) {
        printf("\033[2J\033[1;1H");
        printf(" =========================================================  \n");
        printf("|  GESTHIPER >> CATALOGOS >> QUERIE 6                      |\n");
        printf("|                                                          |\n");
        printf("|  Listar clientes comecados por letra                     |\n");
        printf("| -------------------------------------------------------- |\n");
        printf("| 1- Voltar para CONTABILIDADE | 2 - Sair programa         |\n");
        printf(" =========================================================  \n");
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);
        
        if(leitura>0){
        switch (toupper(input[0])) {
                case '1':
                    sair_menu = 1;
                    sair_programa = 0;
                    break;
                case'2':
                    sair_menu = 1;
                    sair_programa = 1;
                    break;
                default:
                    break;
            }
        }

        if (isalpha(letra) && leitura > 0 && !sair_menu) {
            lista_cli = cat_lista_clientes_letra(catalogo_clientes, letra, TAM_PAGINA);
            resultados = cat_lista_cli_get_num_elems(lista_cli);
            total_pags = cat_lista_cli_get_num_pags(lista_cli);

            while (sair_menu == 0) {
                elems_pag = cat_lista_cli_get_pos_and_num_elems_pag(lista_cli, &inicio_pag, n_pagina);
                fim_pag = inicio_pag + elems_pag;

                printf("\033[2J\033[1;1H");
                printf("-----------------\n");
                printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
                printf("-----------------\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n", cat_lista_cli_get_elemento(lista_cli, inicio_pag + i));

                printf("\n");

                printf("A mostrar %d-%d de %d resultados\n", inicio_pag + 1, fim_pag, resultados);
                printf("---------------------------------------------------------\n");
                printf("1- << | 2- < | 3 - > | 4 - >> | 5 - Ir Para pag... | 0- Voltar | Q - Sair\n");
                printf("Escolha >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '0':
                        sair_menu = 1;
                        break;
                    case '1':
                        n_pagina = 1;
                        break;
                    case '2':
                        if (n_pagina > 1)
                            n_pagina--;
                        break;
                    case '3':
                        if (n_pagina < total_pags)
                            n_pagina++;
                        break;
                    case'4':
                        n_pagina = total_pags;
                        break;
                    case '5':
                        printf("Indique a pag para que quer ir >");
                        leitura = scanf("%d", &escolha_pag);
                        if (escolha_pag > 0 && escolha_pag <= total_pags && leitura != 0)
                            n_pagina = escolha_pag;
                        break;
                    case 'Q':
                        sair_menu = 1;
                        sair_programa = 1;
                        break;
                    default:
                        sair_menu = 1;
                        break;
                }
            }
            cat_free_lista_clientes(lista_cli);
        } else {
            sair_menu = 1;
        }
    }
    return sair_programa;
}

int _07_compras_intervalo_meses() {
    return 1;
}

int _08_clientes_compraram_prod() {
    return 1;
}

int _09_produtos_mais_comprados_cliente() {
    return 1;
}

int _10_clientes_regulares() {
    return 1;
}

int _11_compras_CSV() {
    return 1;
}

int _12_prods_mais_vendidos() {
    return 1;
}

int _13_tres_prods_mais_comprados() {
    return 1;
}

int _14_clientes_prods_fantasma() {
    return 1;
}
