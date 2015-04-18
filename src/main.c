#include <stdio.h>
#include <stdlib.h>
#include "headers/leitura_ficheiros.h"
#include "headers/interface.h"
#include "headers/cat_clientes.h"
#include "headers/cat_produtos.h"
#include "headers/compras.h"
#include "headers/contabilidade.h"


CatClientes catalogo_clientes;
CatProdutos catalogo_produtos;
Contabilidade contabilidade;
Compras mod_compras;

void main_inicializa_modulos();
void main_free_modulos();

int main(int argc, char** argv) {
    main_inicializa_modulos();
    leitura_ficheiros(argc, argv);
    interface();
    main_free_modulos();
    return (EXIT_SUCCESS);
}

void main_inicializa_modulos(){
    catalogo_clientes = inicializa_catalogo_clientes();
    catalogo_produtos = inicializa_catalogo_produtos();
    contabilidade = inicializa_contabilidade();
    mod_compras = inicializa_compras();
}

void main_free_modulos(){
    free_catalogo_clientes(catalogo_clientes);
    free_catalogo_produtos(catalogo_produtos);
    free_contabilidade(contabilidade);
    free_compras(mod_compras);
}
