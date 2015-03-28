#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/queries.h"
#include "headers/cat_produtos.h"
#include "headers/cat_clientes.h"
#include "headers/arrays_dinamicos.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

#define TAM_PAGINA 20
#define ARRAY_PAGINA_IMPOSSIVEL -1

extern CatClientes catalogo_clientes;
extern CatProdutos catalogo_produtos;

ARRAY_DINAMICO _06_clientes_to_ad(char);
void free_str(void *);

int _02_codigo_produtos_letra() {
    return 1;
}

int _03_compras_mensais_prod(){
    return 1;
}

int _04_prods_nao_comprados() {
    return 1;
}

int _05_tabela_cliente() {
    return 1;
}

int _06_codigos_clientes_letra() {
    int i=0, leitura = 0;
    int sair_menu = 0, sair_programa = 0;
    int n_pagina = 1, elems_pag=0, inicio_pag = 1, fim_pag = 0;
    int resultados=0, total_pags=0, escolha_pag=0;
    LISTA_CLIENTES lista_cli = NULL;
    char letra;
    char input[50];
    
    while (sair_menu == 0) {
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);
        
        if (isalpha(letra) && leitura > 0) {
            lista_cli = lista_clientes_letra(catalogo_clientes,letra, TAM_PAGINA);
            resultados = lista_cli_get_num_elems(lista_cli);
            total_pags = lista_cli_get_num_pags(lista_cli);
            
            while (sair_menu == 0) {
                elems_pag = lista_cli_get_pos_and_num_elems_pag(lista_cli, &inicio_pag, n_pagina);
                fim_pag = inicio_pag + elems_pag;
                
                printf("\033[2J\033[1;1H");
                printf("-----------------\n");
                printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
                printf("-----------------\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n",(char *) lista_cli_get_elemento(lista_cli,inicio_pag+i));

                printf("\n");

                printf("A mostrar %d-%d de %d resultados\n", inicio_pag+1, fim_pag, resultados);
                printf("---------------------------------------------------------\n");
                printf("1- Pag. Anterior | 2 - Proxima Pag | 3 - Ir Para pag... | 0- Voltar | Q - Sair\n");
                printf("Escolha >");
                leitura = scanf("%s", input);

                switch (toupper(input[0])) {
                    case '0':
                        sair_menu = 1;
                        break;
                    case '1':
                        if (n_pagina > 1)
                            n_pagina--;
                        break;
                    case '2':
                        if(n_pagina < total_pags)
                            n_pagina++;
                        break;
                    case '3':
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
        } else {
            sair_menu = 1;
        }
    }
    free_lista_clientes(lista_cli);
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
