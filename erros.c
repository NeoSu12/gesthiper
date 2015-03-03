#include <stdio.h>
#include <stdlib.h>
#include "erros.h"

void msg_erro_args(ERRO, char *);
void msg_erro_abrir_fich(ERRO, void *);
void msg_erro_flag_desconhecida(ERRO, void *);

void error_msg(ERRO erro, void *param) {
    fprintf(stderr, "ERRO[%d]: ",erro);
    switch (erro) {
        case ERRO_ARGS: msg_erro_args(erro, param);
        case ERRO_ABIR_FICH: msg_erro_abrir_fich(erro,param);
        case ERRO_FLAG_DESCONHECIDA: msg_erro_flag_desconhecida(erro,param);
        default:;
    }
}

void msg_erro_args(ERRO erro, char *nome_prog) {
    if(nome_prog==NULL) nome_prog="./gesthiper";
    
    fprintf(stderr, "Nao foram especificados ficheiros de input.\n");
    fprintf(stderr, "\tModo de executar:\n");
    fprintf(stderr, "\t%s\n", nome_prog);
    fprintf(stderr, "\tOU\n");
    fprintf(stderr, "\t%s <ficheiro_clientes> <ficheiro_produtos> <ficheiro_compras>\n", nome_prog);
}

void msg_erro_abrir_fich(ERRO erro, void *param){
    fprintf(stderr, "Erro ao abrir um dos ficheiros.\n");
}

void msg_erro_flag_desconhecida(ERRO erro, void *param){
    fprintf(stderr, "O 1º parametro nao corresponde a nenhuma tag conhecida.\n");
    fprintf(stderr, "FLAGS Válidas:\n");
    fprintf(stderr, "\t--mini : Abre ficheiros mais pequenos de dados.\n");
}