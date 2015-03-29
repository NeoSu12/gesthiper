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
    int sair_programa = 0, leitura;
    
    printf("Pressione qualquer tecla para continuar (q para sair): ");
    leitura = scanf("%s", r);
    if(tolower(r[0])=='q') sair_programa = 1;
    
    while (sair_programa == 0) {
        printf("\033[2J\033[1;1H"
                " ========================================== \n"
                "| GESTHIPER                                |\n"
                "|                                          |\n"
                "|   1 - Catalogos                          |\n"
                "|   2 - Contabilidade                      |\n"
                "|   3 - Compras                            |\n"
                "| ---------------------------------------- |\n"
                "| BEM-VINDO                   Q - Sair     |\n"
                " ==========================================\n");


        printf("Insira o nº da opcao > ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
            case '1': sair_programa = face_catalogos();
                break;
            case '2': sair_programa = face_contabilidade();
                break;
            case '3': sair_programa = face_compras();
                break;
            case 'Q': sair_programa = 1;
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
    int sair_menu=0, leitura;
    int sair_programa=0;


    while (sair_menu == 0 && sair_programa == 0) {
        printf("\033[2J\033[1;1H"
                " ================================================= \n"
                "| GESTHIPER >> CATALOGOS                          |\n"
                "|                                                 |\n"
                "|   1 = Listar produtos começados por letra [2]   |\n");
        printf( "|   2 = Listar clientes comecados por letra [6]   |\n"
                "| ----------------------------------------------- |\n"
                "|  0 = Voltar                            Q = Sair |\n"
                " =================================================\n");


        printf("Insira o nº da opcao > ");
        leitura = scanf("%s", r);
        switch (toupper(r[0])) {
            case '\\':
            case '0': 
                sair_menu = 1;
                sair_programa = 0;
                break;
            case '1': 
                sair_programa = _02_codigo_produtos_letra();
                break;
            case '2': 
                sair_programa = _06_codigos_clientes_letra();
                break;
            case 'Q': 
                sair_menu = 1;
                sair_programa = 1;
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
    return sair_programa;
}

int face_contabilidade() {
    char r[50];
    int sair_menu = 0, sair_programa=0;
    int leitura;
    
        while (sair_menu == 0 && sair_programa ==0) {
        printf("\033[2J\033[1;1H"
                " ========================================================= \n"
                "| GESTHIPER >> CONTABILIDADE                              |\n"
                "|                                                         |\n"
                "|   1 - Nº vendas e facturação mensal produto [3]         |\n"
                "|   2 - Produtos nao comprados [4]                        |\n"
                "|   3 - Nº compras e total facturado [7]                  |\n");
        printf( "|   4 - Criar ficheiro CSV [11]                           |\n"
                "|   5 - Produtos nao vendidos e clientes sem compras [14] |\n"
                "| ------------------------------------------------------- |\n"
                "|  0 - Voltar                                 Q - Sair    |\n"
                " ========================================================= \n");
        
        printf("//O que procura(?): ");
        leitura = scanf("%s", r);
        
        switch (toupper(r[0])) {
        	case '\\':
            case '0': sair_menu=1;
                      sair_programa=0;
                      break;
            case '1': sair_programa = _03_compras_e_fact_mensal_prod(); break;
            case '2': sair_programa = _04_prods_nao_comprados(); break;
            case '3': sair_programa = _07_compras_intervalo_meses(); break;
            case '4': sair_programa = _11_compras_CSV(); break;
            case '5': sair_programa = _14_clientes_prods_fantasma(); break;
            case 'Q': sair_menu = 1;
                      sair_programa=1;
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
    return sair_programa;
}

int face_compras() {
    char r[50];
    int sair_menu = 0,sair_programa=0;
    int leitura;


    while (sair_menu == 0 && sair_programa ==0) {
        printf("\033[2J\033[1;1H"
                " ========================================================  \n"
                "| GESTHIPER >> COMPRAS                                    |\n"
                "|                                                         |\n"
                "|   1 - Nº produtos comprados por cliente [5]             |\n"
                "|   2 - Lista de clientes que compraram produto [8]       |\n"
                "|   3 - Produtos mais comprados por cliente [9]           |\n"
                "|   4 - Cliente que compraram todos os meses [10]         |\n");
        printf( "|   5 - Ficheiro CSV [11]                                 |\n"
                "|   6 - Tres produtos mais comprados por cliente [13]     |\n"
                "|   7 - Produtos nao vendidos e clientes sem compras [14] |\n"
                "| ------------------------------------------------------- |\n"
                "|  0 - Voltar                           Q - Sair          |\n"
                " ========================================================  \n");
        printf("//O que procura(?): ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
        	case '\\':
            case '0': sair_menu=1;
                      sair_programa=0;
                      break;
            case '1': sair_programa = _05_tabela_cliente();break;
            case '2': sair_programa = _08_clientes_compraram_prod();break;
            case '3': sair_programa = _09_produtos_mais_comprados_cliente();break;
            case '4': sair_programa = _10_clientes_regulares();break;
            case '5': sair_programa = _11_compras_CSV();break;
            case '6': sair_programa = _13_tres_prods_mais_comprados();break;
            case '7': sair_programa = _14_clientes_prods_fantasma();break;
            case 'Q': sair_menu = 1;
                      sair_programa = 1;
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
    return sair_programa;
}