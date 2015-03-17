#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "headers/interface.h"
#include "headers/erros.h"
#include "headers/compra.h"
#include "headers/cat_clientes.h"
#include "headers/cat_produtos.h"

#define LINHA_CLIENTE_MAX 20
#define LINHA_PRODUTO_MAX 20
#define LINHA_COMPRA_MAX 40

int le_ficheiros(int, char **);
void le_clientes(FILE *, char *);
void le_produtos(FILE *, char *);
void le_compras(FILE *, char *);
int compra_valida(COMPRA);
void mostra_compra(COMPRA);
void mostra_numero_codigos();
void testes();
void testes2();

CatClientes catalogo_clientes;
CatProdutos catalogo_produtos;

int main(int argc, char** argv) {
    catalogo_clientes = inicializa_catalogo_clientes();
    catalogo_produtos = inicializa_catalogo_produtos();
    
    /*
    testes2();
    */
    
    le_ficheiros(argc, argv);
    mostra_numero_codigos();
    interface();
    
    return (EXIT_SUCCESS);
}

int le_ficheiros(int argc, char **argv) {
    FILE *f_clientes = NULL;
    FILE *f_produtos = NULL;
    FILE *f_compras = NULL;
    char *nfclientes, *nfprodutos, *nfcompras;

    switch (argc) {
        case 1: nfclientes = "datasets/FichClientes.txt";
            nfprodutos = "datasets/FichProdutos.txt";
            nfcompras = "datasets/Compras.txt";
            break;

        case 2: if (strcmp(argv[1], "--mini") == 0) {
                nfclientes = "datasets/miniClientes.txt";
                nfprodutos = "datasets/miniProdutos.txt";
                nfcompras = "datasets/miniCompras.txt";
            } else {
                error_msg(ERRO_FLAG_DESCONHECIDA, NULL);
                return EXIT_FAILURE;
            }
            break;
        case 4: nfclientes = argv[1];
            nfprodutos = argv[2];
            nfcompras = argv[3];
            break;
        default: error_msg(ERRO_ARGS, NULL);
            return EXIT_FAILURE;
    }

    f_clientes = fopen(nfclientes, "r");
    f_produtos = fopen(nfprodutos, "r");
    f_compras = fopen(nfcompras, "r");

    if (f_clientes == NULL || f_produtos == NULL || f_compras == NULL) {
        error_msg(ERRO_ABIR_FICH, NULL);
        return EXIT_FAILURE;
    }

    le_clientes(f_clientes, nfclientes);
    le_produtos(f_produtos, nfprodutos);
    le_compras(f_compras, nfcompras);


    fclose(f_clientes);
    fclose(f_produtos);
    fclose(f_compras);

    return EXIT_SUCCESS;
}

void le_clientes(FILE *f_cli, char *nf) {
    int clientes_validos = 0, total_linhas_clientes = 0;
    clock_t ci, cf;
    char *cliente, *linha_cliente;
    char *delim = " \n\r";

    ci = clock();
    linha_cliente = (char *) malloc(sizeof(char)*LINHA_CLIENTE_MAX);

    while (fgets(linha_cliente, LINHA_CLIENTE_MAX, f_cli) != NULL) {
        cliente = strtok(linha_cliente, delim);
        if(cliente!=NULL){
            insere_cliente(catalogo_clientes, cliente);
            clientes_validos++;
        }
        total_linhas_clientes++;
    }

    cf = clock();

    printf("-----CLIENTES-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", clientes_validos);
    printf("Linhas lidas: %d\n", total_linhas_clientes);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);

    free(linha_cliente);
}

void le_produtos(FILE *f_prod, char *nf) {
    int produtos_validos = 0, total_linhas_produtos = 0;
    clock_t ci, cf;
    char *produto, *linha_produto;
    char *delim = " \n\r";

    ci = clock();

    linha_produto = (char *) malloc(sizeof(char)*LINHA_PRODUTO_MAX);

    while (fgets(linha_produto, LINHA_PRODUTO_MAX, f_prod) != NULL) {
        produto = strtok(linha_produto, delim);
        insere_produto(catalogo_produtos, produto);
        produtos_validos++;
        total_linhas_produtos++;
    }

    cf = clock();

    printf("-----PRODUTOS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", produtos_validos);
    printf("Linhas lidas: %d\n", total_linhas_produtos);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);
    free(linha_produto);
}

void le_compras(FILE *f_comp, char *nf) {
    COMPRA compra;
    int compras_validas = 0, total_linhas_compras = 0;
    clock_t ci, cf;
    char *linha_compra, *token;
    char *delim = " \n\r";

    ci = clock();
    
    linha_compra = (char *) malloc(sizeof (char)*LINHA_COMPRA_MAX);
    compra = inicializa_compra();

    while (fgets(linha_compra, LINHA_COMPRA_MAX, f_comp) != NULL) {
        
        /*Produto*/
        token = strtok(linha_compra, delim);
        set_cod_produto(compra, token);
        /*Preco*/
        token = strtok(NULL, delim);
        set_preco_unit(compra, atof(token));
        /*Quantidade*/
        token = strtok(NULL, delim);
        set_quantidade(compra, atoi(token));
        /*Promo*/
        token = strtok(NULL, delim);
        set_promo(compra, token[0]);
        /*Cliente*/
        token = strtok(NULL, delim);
        set_cod_cliente(compra, token);
        /*Mes*/
        token = strtok(NULL, delim);
        set_mes(compra, atoi(token));

        if (compra_valida(compra)) {
            compras_validas++;
        }
        total_linhas_compras++;
    }
    
    cf = clock();
    
    printf("------COMPRAS-----\n");
    printf("Nome do ficheiro: %s\n", nf);
    printf("Linhas validas: %d\n", compras_validas);
    printf("Linhas lidas: %d\n", total_linhas_compras);
    printf("Leitura em %f segundos.\n", ((float) cf - ci) / CLOCKS_PER_SEC);

    free_compra(compra);
    free(linha_compra);
}

void mostra_numero_codigos(){
    char letra = 'A';
    int tot_prod=0, tot_cli=0;
    printf("----------------------------\n");
    printf("CLIENTES\tPRODUTOS\n");
    for(letra = 'A'; letra <= 'Z';letra++){
        printf("%c: %5d\t%c: %5d\n", 
                letra, total_clientes_letra(catalogo_clientes, letra),
                letra, total_produtos_letra(catalogo_produtos, letra));
        tot_cli += total_clientes_letra(catalogo_clientes, letra);
        tot_prod+= total_produtos_letra(catalogo_produtos, letra);
        
    }
    printf("Tot: %d\tTot:%d\n", tot_cli, tot_prod);
    printf("----------------------------\n");
}


/*
 * TODO: Verificar se produto e cliente existem, depois de ter catalogos prontos.
 */
int compra_valida(COMPRA compra) {
    return get_mes(compra) >= 1 && get_mes(compra) <= 12
            && get_preco_unit(compra) >= 0
            && (get_promo(compra) == 'N' || get_promo(compra) == 'P');
}

void mostra_compra(COMPRA compra) {
    printf("Produto: %s | ", get_cod_produto(compra));
    printf("Preco: %5.2f | ", get_preco_unit(compra));
    printf("Quantidade: %2d | ", get_quantidade(compra));
    printf("Promo: %c | ", get_promo(compra));
    printf("Cliente: %s | ", get_cod_cliente(compra));
    printf("Mes: %2d\n", get_mes(compra));

}

void testes() {
    IT_CLIENTES it_fim, it_inicio, it_inicio_letra, it_fim_letra, it_elem, it_iteran, it_iteran_fim;
    PagClientes paginacao;
    char *res_it;
    int it_count, i=0;
    char *cods[50], *cods_f[50];
    
    /* Testa inserções */
    printf("A colocar items no catalogo...");
    insere_cliente(catalogo_clientes, "A1");
    insere_cliente(catalogo_clientes, "A2");
    insere_cliente(catalogo_clientes, "A3");
    insere_cliente(catalogo_clientes, "A4");
    insere_cliente(catalogo_clientes, "A5");
    insere_cliente(catalogo_clientes, "A6");
    insere_cliente(catalogo_clientes, "B1");
    insere_cliente(catalogo_clientes, "B2");
    insere_cliente(catalogo_clientes, "C1");
    insere_cliente(catalogo_clientes, "C2");
    insere_cliente(catalogo_clientes, "C3");
    insere_cliente(catalogo_clientes, "C4");
    insere_cliente(catalogo_clientes, "C5");
    insere_cliente(catalogo_clientes, "D1");
    printf("done!\n");
    
    paginacao = inicializa_pag_clientes(catalogo_clientes,2);
    
    printf("Vai para pagina 1:\n");
    it_count = pag_clientes_goto_pag(paginacao ,1, cods);
    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Vai para pagina 2:\n");
    it_count = pag_clientes_goto_pag(paginacao ,2, cods);
    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Volta para pagina 1:\n");
    it_count = pag_clientes_goto_pag(paginacao ,1, cods);
    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Vai para pagina 5:\n");
    it_count = pag_clientes_goto_pag(paginacao ,5, cods);
    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("A inicializar iteradores...");
    it_fim = inicializa_it_clientes_fim(catalogo_clientes);
    it_inicio = inicializa_it_clientes_inicio(catalogo_clientes);
    it_inicio_letra = inicializa_it_clientes_inicio_letra(catalogo_clientes,'c');
    it_fim_letra = inicializa_it_clientes_fim_letra(catalogo_clientes,'c');
    it_elem = inicializa_it_clientes_elem(catalogo_clientes, "CL");
    it_iteran = inicializa_it_clientes_inicio(catalogo_clientes);
    it_iteran_fim = inicializa_it_clientes_fim(catalogo_clientes);
    printf("done!\n");
    
    printf("Itera n:\n");
    it_count = itera_n_clientes_proximos(it_iteran,cods,2);

    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Itera n outra vez:\n");
    it_count = itera_n_clientes_proximos(it_iteran,cods,2);

    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Volta para a 1ª pag:\n");
    it_count = itera_n_clientes_anteriores(it_iteran,cods,2);

    for(i=0;i<it_count;i++){
        printf("%s\n",cods[i]);
    }
    
    printf("Itera n:\n");
    it_count = itera_n_clientes_anteriores(it_iteran_fim,cods_f,2);

    for(i=0;i<it_count;i++){
        printf("%s\n",cods_f[i]);
    }
    /* 
     * Testa se a funcao iterador_proximo() consegue pegar num
     * iterador a começar no inicio da arvore e percorrer todos os elementos.
     */
    printf("Imprimir o catalogo do inicio para o fim (iterador inicializado):\n");
    it_cliente_anterior(it_inicio);
    while((res_it = it_cliente_proximo(it_inicio)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    /*
     * Testa se o mesmo iterador que foi inicializado no inicio do catalogo
     * e levado até ao fim no ciclo anterior agora consegue fazer o caminho inverso,
     * de volta ao inicio.
     */
    printf("Imprimir o catalogo do fim para o inicio (iterador anterior):\n");
    while ((res_it = it_cliente_anterior(it_inicio)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    /*
     * Testa se um iterador inicializado no fim do catalogo consegue ser levado até
     * ao principio.
     */
    printf("Imprimir o catalogo do fim para o inicio (iterador inicializado):\n");
    while ((res_it = it_cliente_anterior(it_fim)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");

    printf("[Antes da remoção] O elemento existe?:\n");
    existe_cliente(catalogo_clientes, "AA") ? 
        printf("O elemento existe.\n") : printf("O elemento nao existe.\n");
    /*
     * Testa a remoção de um elemento.
     */
    remove_cliente(catalogo_clientes, "AA");

    printf("[Depois da remoção] O elemento existe?:\n");
    existe_cliente(catalogo_clientes, "AA") ? 
        printf("O elemento existe.\n") : printf("O elemento nao existe.\n");

    printf("Imprimir o catalogo do inicio para o fim:\n");
    while ((res_it = it_cliente_proximo(it_inicio)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    /*
     * Testa função total_codigos.
     */
    printf("Total codigos: %d\n", total_clientes(catalogo_clientes));
    
    
    printf("Letra do inicio ao fim (iterador inicializado):\n");
    while ((res_it = it_cliente_proximo_letra(it_inicio_letra)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    
    printf("Letra do fim para o inicio (iterador anterior):\n");
    while ((res_it = it_cliente_anterior_letra(it_inicio_letra)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    
    printf("Letra do fim para o inicio (iterador inicializado):\n");
    while ((res_it = it_cliente_anterior_letra(it_fim_letra)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    
    printf("Imprimir o catalogo a partir de elemento até ao fim (iterador inicializado):\n");
    while ((res_it = it_cliente_proximo(it_elem)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    
}

void testes2(){
    IT_CLIENTES it;
    char *res_it;
    
    /* Testa inserções */
    printf("A colocar items no catalogo...");
    insere_cliente(catalogo_clientes, "A1");
    insere_cliente(catalogo_clientes, "A2");
    insere_cliente(catalogo_clientes, "A3");
    insere_cliente(catalogo_clientes, "A4");
    insere_cliente(catalogo_clientes, "A5");
    insere_cliente(catalogo_clientes, "A6");
    insere_cliente(catalogo_clientes, "B1");
    insere_cliente(catalogo_clientes, "B2");
    insere_cliente(catalogo_clientes, "C1");
    insere_cliente(catalogo_clientes, "C2");
    insere_cliente(catalogo_clientes, "C3");
    insere_cliente(catalogo_clientes, "C4");
    insere_cliente(catalogo_clientes, "C5");
    insere_cliente(catalogo_clientes, "D1");
    printf("done!\n");
    
    it = inicializa_it_clientes_inicio(catalogo_clientes);
    
    printf("Imprimir o catalogo do inicio para o fim (iterador inicializado):\n");
    it_cliente_anterior(it);
    while((res_it = it_cliente_proximo(it)) != NULL) {
        printf("%s ", res_it);
    }
    printf("\n");
    
    res_it = it_cliente_anterior(it);
    res_it = it_cliente_anterior(it);
    res_it = it_cliente_anterior(it);
    
}

