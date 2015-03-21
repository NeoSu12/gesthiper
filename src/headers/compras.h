#ifndef COMPRAS_H
#define	COMPRAS_H

#include "compra.h"

typedef struct modulo_compras* Compras;

Compras inicializa_compras();
void insere_compra(Compras, COMPRA);
void free_compras(Compras compras);

#endif	/* COMPRAS_H */

