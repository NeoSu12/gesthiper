#ifndef ERROS_H
#define	ERROS_H

typedef enum erro {
            ERRO_ARGS,
            ERRO_ABIR_FICH
}ERRO;

void error_msg(ERRO, void *);

#endif	/* NEWFILE_H */

