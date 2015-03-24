typedef struct mod_contabilidade *Contabilidade;

Contabilidade inicializa_contabilidade();
void free_nodo_produto();
void free_contabilidade();

int existeNodo();
int existeCodigo();

void inserir_produto();
int total_fact_mes();
int total_vendas_mes();
