#include <stdio.h>
#include <stdlib.h>
#include "erros.h"

void msg_erro_args(ERRO, char *);
void msg_erro_abrir_fich(ERRO, void *);

void error_msg(ERRO err, void *param) {
    fprintf(stderr, "ERRO[%d]: ",err);
    switch (err) {
        case ERRO_ARGS: msg_erro_args(err, param);
        case ERRO_ABIR_FICH: msg_erro_abrir_fich(err,param);
        default:;
    }
}

void msg_erro_args(ERRO err, char *nome_prog) {
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