#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "erros.h"
#include "compra.h"

void ler_ficheiros(FILE *, FILE *, FILE *);
void le_clientes(FILE *);
void le_produtos(FILE *);
void le_compras(FILE *);
int cliente_valido(char *);
int produto_valido(char *);
int compra_valida(char *);

int main(int argc, char** argv) {

    FILE *f_clientes = NULL;
    FILE *f_produtos = NULL;
    FILE *f_compras = NULL;

    switch (argc) {
        case 1: f_clientes = fopen("datasets/FichClientes.txt", "r");
            f_produtos = fopen("datasets/FichProdutos.txt", "r");
            f_compras = fopen("datasets/Compras.txt", "r");
            break;
            
        case 2: if (strcmp(argv[1], "--mini") == 0) {
                f_clientes = fopen("datasets/miniClientes.txt", "r");
                f_produtos = fopen("datasets/miniProdutos.txt", "r");
                f_compras = fopen("datasets/miniCompras.txt", "r");
            } else {
                error_msg(ERRO_FLAG_DESCONHECIDA,NULL);
                return EXIT_FAILURE;
            }
            break;
            
        case 4: f_clientes = fopen(argv[1], "r");
            f_produtos = fopen(argv[2], "r");
            f_compras = fopen(argv[3], "r");
            break;
            
        default: error_msg(ERRO_ARGS, NULL);
            return EXIT_FAILURE;
    }

    if (f_clientes == NULL || f_produtos == NULL || f_compras == NULL) {
        error_msg(ERRO_ABIR_FICH, NULL);
        return EXIT_FAILURE;
    }
    
    ler_ficheiros(f_clientes, f_produtos, f_compras);
    
    close(f_clientes);
    close(f_produtos);
    close(f_compras);
    return (EXIT_SUCCESS);
}

void ler_ficheiros(FILE *f_cli, FILE *f_prod, FILE *f_comp){
    le_clientes(f_cli);
    le_produtos(f_prod);
    le_compras(f_comp);
}

void le_clientes(FILE *f_cli){
    char *cliente = (char *) malloc(sizeof(char)*10);
    int clientes_validos = 0,total_linhas_clientes=0;
    while(!feof(f_cli) && !ferror(f_cli)){
        fscanf(f_cli, "%s\n",cliente);
        if(cliente_valido(cliente)!=-1) clientes_validos++;
        printf("%s\n",cliente);
        total_linhas_clientes++;
    }
    printf("Leitura Clientes: %d\\%d\n",clientes_validos,total_linhas_clientes);
    free(cliente);
}

void le_produtos(FILE *f_prod){
    char *produto = (char *) malloc(sizeof(char)*10);
    int produtos_validos = 0,total_linhas_produtos=0;
    while(!feof(f_prod) && !ferror(f_prod)){
        fscanf(f_prod, "%s\n",produto);
        if(produto_valido(produto)!=-1) produtos_validos++;
        printf("%s\n",produto);
        total_linhas_produtos++;
    }
    printf("Leitura Produto: %d\\%d\n",produtos_validos,total_linhas_produtos);
    free(produto);
}

void le_compras(FILE *f_comp){
    char *compra = (char *) malloc(sizeof(char)*30);
    int compras_validas = 0,total_linhas_compras=0;
    
    while(!feof(f_comp) && !ferror(f_comp)){
        fscanf(f_comp, "%s\n",compra);
        if(compra_valida(compra)!=-1) compras_validas++;
        printf("%s\n",compra);
        total_linhas_compras++;
    }
    
    printf("Leitura Compras: %d\\%d\n",compras_validas,total_linhas_compras);
    free(compra);
}


int cliente_valido(char *cl){
    int res=0;
    int tam=strlen(cl);
    
    if(tam!=5){
        res = -1;
    }else{
        res = isupper(cl[0]) && isupper(cl[1]) && 
                isdigit(cl[2]) && isdigit(cl[3]) && isdigit(cl[4]);
    }
    
    return res;
}

int produto_valido(char *prod){
    int res=0;
    int tam=strlen(prod);
    
    if(tam!=6){
        res = -1;
    }else{
        res = isupper(prod[0]) && isupper(prod[1]) && 
                isdigit(prod[2]) && isdigit(prod[3]) && 
                isdigit(prod[4]) && isdigit(prod[5]);
    }
    
    return res;
}

int compra_valida(char* compra){
    return 0;
}
