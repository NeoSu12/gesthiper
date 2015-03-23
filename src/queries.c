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
    ARRAY_DINAMICO paginacao = NULL;
    char letra;
    char input[50];
    
    while (sair_menu == 0) {
        printf("Insira a letra a procurar > ");
        leitura = scanf("%s", input);
        letra = toupper(input[0]);
        
        if (isalpha(letra) && leitura > 0) {
            paginacao = _06_clientes_to_ad(letra);
            resultados = ad_get_tamanho(paginacao);
            total_pags = (resultados / TAM_PAGINA) + 1;
            
            while (sair_menu == 0) {
                elems_pag = ad_goto_pag(paginacao, &inicio_pag, n_pagina, TAM_PAGINA);
                fim_pag = inicio_pag + elems_pag;
                
                printf("\033[2J\033[1;1H");
                printf("-----------------\n");
                printf("--Pagina %2d/%d--\n", n_pagina, total_pags);
                printf("-----------------\n");

                for (i = 0; i < elems_pag; i++)
                    printf("%s\n",(char *) ad_get_elemento(paginacao,inicio_pag+i));

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
    ad_deep_free(paginacao,free_str);
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

ARRAY_DINAMICO _06_clientes_to_ad(char letra) {
    char *cliente;
    ARRAY_DINAMICO ad = ad_inicializa(8000);
    IT_CLIENTES it = inicializa_it_clientes_letra(catalogo_clientes, letra);

    while ((cliente = it_cliente_proximo_letra(it)) != NULL) {
        ad_insere_elemento(ad, cliente);
    }
    
    free_it_cliente(it);
    return ad;
}

void free_str(void *item) {
    free(item);
}