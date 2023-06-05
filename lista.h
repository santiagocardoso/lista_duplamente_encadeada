typedef struct no { // no de palavra
    char *palavra;
    int linha;
    int coluna;
    struct no *proximo;
    struct no *antes;
} No;

typedef struct multilista { // multilista de linhas
    No *linha;
    int num_palavras;
    struct multilista *cima;
    struct multilista *abaixo;
} Multilista;

typedef struct descritor { // descritor
    int num_linhas;
    Multilista *multilista;
} Desc;

void menu(); // exibe o menu para o usuario

No* inicializa_no(int linha, int coluna, char *palavra);
Multilista* inicializa_multilista();
Desc* inicializa_desc();

void le_arquivo(FILE *arquivo, Desc **desc);

int verifica(Desc *desc); // verifica se a estrutura foi inicializada

void imprime_ldde(Desc *desc); // imprime no formato da ldde
void imprime_original(Desc *desc); // imprime no fomato do texto original
void buscar_palavra(Desc *desc, char *palavra, int *lin, int *col); // busca a palavra fornecida e retorna a linha e coluna
void corrige_num_coluna(Multilista *multilista); // apos ocorrer uma insercao ou remocao de uma palavra, corrige a posicao da coluna dos seguintes nos da estrutura
int remover_palavra(Desc *desc, char *palavra); // remove uma palavra
int remover_palavra_lin_col(Desc *desc, int lin, int col); // remove uma palavra na linha e coluna indicadas
int ocorrencias(Desc *desc, char *palavra); // retorna o numero de ocorrencias de uma palavra
int total_palavras(Desc *desc); // retorna o total de palavras
int editar(Desc *desc, int lin, int col); // edita uma palavra na linha e coluna indicadas
int inserir(Desc *desc, char *palavra, int lin, int col); // insere uma nova palavra na linha e coluna indicadas
int inserir_no_fim(Desc *desc, char *palavra, int lin); // insere uma nova palavra no fim da linha indicada
void substring(Desc *desc, char *sub); // busca o total de ocorrencias e as linhas e colunas da substring indicada

void copiar_multilista_para_arquivo(Desc *desc, char *nome_arquivo); // realiza a copia da estrutura para um arquivo .txt
