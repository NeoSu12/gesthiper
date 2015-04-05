#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "headers/interface.h"
#include "headers/queries.h"
#include "headers/erros.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

int face_catalogos();
int face_contabilidade();
int face_compras();

void interface() {
    char r[50];
    int estado=MENU_PRINCIPAL;
    int leitura=0;
    
    printf("Pressione qualquer tecla para continuar (q para sair): ");
    leitura = scanf("%s", r);
    
    if(tolower(r[0])=='q') 
        estado = SAIR_PROGRAMA;
    
    while (estado == MENU_PRINCIPAL) {
        printf("\033[2J\033[1;1H"
                " ========================================== \n"
                "| GESTHIPER >> MENU PRINCIPAL              |\n"
                "|                                          |\n"
                "|   1 - Catalogos                          |\n"
                "|   2 - Contabilidade                      |\n"
                "|   3 - Compras                            |\n"
                "| ---------------------------------------- |\n"
                "| BEM-VINDO                   Q - Sair     |\n"
                " ==========================================\n");


        printf("Escolha uma opcao > ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
            case '1': estado = face_catalogos();
                break;
            case '2': estado = face_contabilidade();
                break;
            case '3': estado = face_compras();
                break;
            case 'Q': estado = SAIR_PROGRAMA;
                break;
            default:
                /*TODO: Tratamento de erro temporario.
                 * Melhorar este tratamento depois de saber resposta do professor
                 sobre o uso do scanf.*/
                if (leitura == 0) error_msg(ERRO_NENHUMA_OPCAO, NULL);
                else error_msg(ERRO_OPCAO_INVALIDA, NULL);
                break;
        }
    }


}

int face_catalogos() {
    char r[50];
    int estado=FACE_CATALOGOS;
    int leitura=0;


    while (estado == FACE_CATALOGOS) {
        printf("\033[2J\033[1;1H"
                " ================================================= \n"
                "| GESTHIPER >> CATALOGOS                          |\n"
                "|                                                 |\n"
                "|   1 - Listar produtos começados por letra [2]   |\n");
        printf( "|   2 - Listar clientes comecados por letra [6]   |\n"
                "| ----------------------------------------------- |\n"
                "|  0 - Voltar                            Q - Sair |\n"
                " =================================================\n");


        printf("Escolha uma opcao > ");
        leitura = scanf("%s", r);
        switch (toupper(r[0])) {
            case '\\':
            case '0': 
                estado = MENU_PRINCIPAL;
                break;
            case '1': 
                estado = _02_codigo_produtos_letra();
                break;
            case '2': 
                estado = _06_codigos_clientes_letra();
                break;
            case 'Q': 
                estado = SAIR_PROGRAMA;
                break;
            default:
                /*TODO: Tratamento de erro temporario.
                 * Melhorar este tratamento depois de saber resposta do professor
                 sobre o uso do scanf.*/
                if (leitura == 0) error_msg(ERRO_NENHUMA_OPCAO, NULL);
                else error_msg(ERRO_OPCAO_INVALIDA, NULL);
                break;
        }
    }
    return estado;
}

int face_contabilidade() {
    char r[50];
    int estado=FACE_CONTABILIDADE;
    int leitura=0;
    
        while (estado ==FACE_CONTABILIDADE || estado == FACE_COMPRAS) {
        printf("\033[2J\033[1;1H"
                " ========================================================= \n"
                "| GESTHIPER >> CONTABILIDADE                              |\n"
                "|                                                         |\n"
                "|   1 - Nº vendas e facturação mensal produto         [3] |\n"
                "|   2 - Nº vendas e facturação produto todos os meses [3] |\n"
                "|   3 - Produtos nao comprados                        [4] |\n"
                "|   4 - Nº compras e total facturado intervalo meses  [7] |\n");
        printf( "| ------------------------------------------------------- |\n"
                "|  0 - Voltar                                 Q - Sair    |\n"
                " ========================================================= \n");
        
        printf("Escolha uma opcao > ");
        leitura = scanf("%s", r);
        
        switch (toupper(r[0])) {
        	case '\\':
            case '0': estado = MENU_PRINCIPAL;
                      break;
            case '1': estado = _03_compras_e_fact_mensal_prod(); break;
            case '2': estado = _03_compras_e_fact_mensal_prod_old(); break;
            case '3': estado = _04_prods_nao_comprados(); break;
            case '4': estado = _07_compras_intervalo_meses(); break;
            case 'Q': estado = SAIR_PROGRAMA;
                      break;
            default: 
                if(leitura==0) error_msg(ERRO_NENHUMA_OPCAO, NULL);
                else error_msg(ERRO_OPCAO_INVALIDA, NULL); 
                break;
        }
    }
    return estado;
}

int face_compras() {
    char r[50];
    int estado=FACE_COMPRAS;
    int leitura=0;

    
    while (estado ==FACE_COMPRAS || estado == FACE_CONTABILIDADE) {
        printf("\033[2J\033[1;1H"
                " ========================================================  \n"
                "| GESTHIPER >> COMPRAS                                    |\n"
                "|                                                         |\n"
                "|   1 - Nº produtos comprados por cliente             [5] |\n"
                "|   2 - Lista de clientes que compraram produto       [8] |\n"
                "|   3 - Produtos mais comprados por cliente           [9] |\n"
                "|   4 - Cliente que compraram todos os meses         [10] |\n");
        printf( "|   5 - Nº compras e clientes por mes (CSV)          [11] |\n"
                "|   6 - N produtos mais vendidos                     [12] |\n"
                "|   7 - Tres produtos mais comprados por cliente     [13] |\n"
                "|   8 - Produtos nao vendidos e clientes sem compras [14] |\n"
                "| ------------------------------------------------------- |\n"
                "|  0 - Voltar                           Q - Sair          |\n"
                " ========================================================  \n");
        printf("Escolha uma opcao > ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
        	case '\\':
            case '0': estado = MENU_PRINCIPAL;
                      break;
            case '1': estado = _05_tabela_cliente();break;
            case '2': estado = _08_clientes_compraram_prod();break;
            case '3': estado = _09_produtos_mais_comprados_cliente_mes();break;
            case '4': estado = _10_clientes_regulares();break;
            case '5': estado = _11_compras_CSV();break;
            case '6': estado = _12_prods_mais_vendidos();break;
            case '7': estado = _13_tres_prods_mais_comprados();break;
            case '8': estado = _14_clientes_prods_fantasma();break;
            case 'Q': estado = SAIR_PROGRAMA;
                      break;
            default:
                /*TODO: Tratamento de erro temporario.
                 * Melhorar este tratamento depois de saber resposta do professor
                 sobre o uso do scanf.*/
                if(leitura==0) error_msg(ERRO_NENHUMA_OPCAO, NULL);
                else error_msg(ERRO_OPCAO_INVALIDA, NULL);
                break;
        }
    }
    return estado;
}