#ifndef ERROS_H
#define	ERROS_H

typedef enum erro {
    ERRO_ARGS,
    ERRO_ABIR_FICH,
    ERRO_FLAG_DESCONHECIDA
} ERRO;

void error_msg(ERRO, void *);

#endif	/* NEWFILE_H */

