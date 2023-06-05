typedef struct no {
    char *palavra;
    int linha;
    int coluna;
    struct no *proximo;
    struct no *antes;
} No;

typedef struct multilista {
    No *linha;
    int num_palavras;
    struct multilista *cima;
    struct multilista *abaixo;
} Multilista;

typedef struct descritor {
    int num_linhas;
    Multilista *multilista;
} Desc;

void menu();

No* inicializa_no(int linha, int coluna, char *palavra);
Multilista* inicializa_multilista();
Desc* inicializa_desc();

void le_arquivo(FILE *arquivo, Desc **desc);

int verifica(Desc *desc);

void imprime_ldde(Desc *desc);
void imprime_original(Desc *desc);
void buscar_palavra(Desc *desc, char *palavra, int *lin, int *col);
void corrige_num_coluna(Multilista *multilista);
int remover_palavra(Desc *desc, char *palavra);
int remover_palavra_lin_col(Desc *desc, int lin, int col);
int ocorrencias(Desc *desc, char *palavra);
int total_palavras(Desc *desc);
int editar(Desc *desc, int lin, int col);
int inserir(Desc *desc, char *palavra, int lin, int col);
int inserir_no_fim(Desc *desc, char *palavra, int lin);
void substring(Desc *desc, char *palavra);

void copiar_multilista_para_arquivo(Desc *desc, char *nome_arquivo);
