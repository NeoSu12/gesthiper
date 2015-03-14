#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "interface.h"
#include "queries.h"
#include "erros.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define WHITE  "\x1B[37m"

int faceprodutos();
int faceclientes();
int facecompras();

void interface() {
    char r[50];
    int sair_programa = 0, leitura;
    
    printf("Pressione qualquer tecla para continuar: ");
    leitura = scanf("%s", r);
    
    while (sair_programa == 0) {
        printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
                "%sGESTHIPER%s                                 |\n|"
                "                                          |\n|"
                "   1 - Produtos                           |\n|"
                "   2 - Clientes                           |\n|"
                "   3 - Compras/Contabilidade              |\n|"
                "                                          |\n| "
                "BEM-VINDO                       Q - %sSair%s |\n "
                "------------------------------------------\n%s",
                WHITE, BLUE, WHITE, RED, WHITE, NORMAL);


        printf("//O que procura(?): ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
            case '1': sair_programa = faceprodutos();
                break;
            case '2': sair_programa = faceclientes();
                break;
            case '3': sair_programa = facecompras();
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

int faceprodutos() {
    char r[50];
    int sair_menu=0, leitura;
    int sair_programa=0;
    
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
            "%s*1* %sPRODUTOS%s                              |\n|"
            "                                                    |\n|"
            "   1 - Listar produtos começados por letra [2]      |\n|"
            "   2 - Produtos nao comprados [4]                   |\n|"
            "   3 - Clientes que compraram o produto [8]         |\n|"
            "                                                    |\n|"
            "  0 - Voltar                     Q - %sSair%s |\n"
            " ------------------------------------------\n%s", 
            WHITE, YELLOW, BLUE, WHITE, RED, WHITE, NORMAL);
    
    
    while (sair_menu == 0) {
        printf("//O que procura(?): ");
        leitura = scanf("%s", r);
        switch (toupper(r[0])) {
        	case '\\':
            case '0': sair_menu=1;
                      sair_programa=0;
                      break;
            case '1': _02_codigo_produtos_letra(); break;
            case '2': _04_prods_nao_comprados(); break;
            case '3': _08_clientes_compraram_prod(); break;
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

int faceclientes() {
    char r[50];
    int sair_menu = 0, sair_programa=0;
    int leitura;
    
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
            "%s*2* %sCLIENTES%s                              |\n|"
            "                                                      |\n|"
            "   1 - Número de produtos comprados por cliente [5]   |\n|"
            "   2 - Codigos de clientes começados por letra [6]    |\n|"
            "   3 - Produtos mais comprados por cliente [9]        |\n|"
            "   4 - Clientes regulares [10]                        |\n|"
            "   5 - Top 3 produtos mais comprados por cliente [13] |\n", WHITE, YELLOW, BLUE, WHITE);
    
    printf("|                                          |\n|"
            "  0 - Voltar                     Q - %sSair%s |\n"
            " ------------------------------------------\n%s", RED, WHITE, NORMAL);
        while (sair_menu == 0) {

        printf("//O que procura(?): ");
        leitura = scanf("%s", r);
        
        switch (toupper(r[0])) {
        	case '\\':
            case '0': sair_menu=1;
                      sair_programa=0;
                      break;
            case '1': _05_tabela_cliente(); break;
            case '2': _06_codigos_clientes_letra(); break;
            case '3': _09_produtos_mais_comprados_cliente(); break;
            case '4': _10_clientes_regulares(); break;
            case '5': _13_tres_prods_mais_comprados(); break;
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

int facecompras() {
    char r[50];
    int sair_menu = 0,sair_programa=0;
    int leitura;
    printf("\033[2J\033[1;1H%s ------------------------------------------\n|"
            "%s*3* %sCONTABILIDADE%s                         |\n|"
            "                                                |\n|"
            "   1 - Numero vendas num mes de um produto[3]   |\n|"
            "   2 - Numero vendas num intervalo de meses[7]  |\n|"
            "   3 - Criar ficheiro CSV [11]                  |\n|"
            "   4 - N produtos mais vendidos [12]            |\n|"
            "   5 - Clientes e produtos fantasma [14]        |\n", WHITE, YELLOW, BLUE, WHITE);

    printf("|                                          |\n|"
            "  0 - Voltar                     Q - %sSair%s |\n"
            " ------------------------------------------\n%s", RED, WHITE, NORMAL);

    while (sair_menu == 0) {

        printf("//O que procura(?): ");
        leitura = scanf("%s", r);

        switch (toupper(r[0])) {
        	case '\\':
            case '0': sair_menu=1;
                      sair_programa=0;
                      break;
            case '1': _03_compras_mensais_prod();break;
            case '2': _07_compras_intervalo_meses();break;
            case '3': _11_compras_CSV();break;
            case '4': _12_prods_mais_vendidos();break;
            case '5': _14_clientes_prods_fantasma();break;
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