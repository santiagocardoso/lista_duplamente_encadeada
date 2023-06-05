#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

void menu() {
    printf("-=-=-=-=-=-=- OPCOES -=-=-=-=-=-=-\n");
    printf("[01] Mostrar LDDE\n");
    printf("[02] Mostra texto original\n");
    printf("[03] Buscar palavra\n");
    printf("[04] Remover palavra\n");
    printf("[05] Remover palavra (lin/col)\n");
    printf("[06] Ocorrencias de uma palavra\n");
    printf("[07] Quantidade de palavras\n");
    printf("[08] Edicao de palavra (lin/col)\n");
    printf("[09] Inserir palavra\n");
    printf("[10] Inserir no fim\n");
    printf("[11] Palavras com uma substring\n");
    printf("[00] Sair\n");
    printf("-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-\n");
}

No* inicializa_no(int linha, int coluna, char *palavra) {
    No *novo = (No*) malloc(sizeof(No));
    novo->antes = NULL;
    novo->coluna = coluna;
    novo->linha = linha;
    novo->palavra = (char*) malloc(strlen(palavra) + 1);
    strcpy(novo->palavra, palavra);
    novo->proximo = NULL;
    return novo;
}

Multilista* inicializa_multilista() {
    Multilista *nova = (Multilista*) malloc(sizeof(Multilista));
    nova->abaixo = NULL;
    nova->cima = NULL;
    nova->linha = NULL;
    nova->num_palavras = 0;
    return nova;
}

Desc* inicializa_desc() {
    Desc* desc = (Desc*) malloc(sizeof(Desc));
    desc->multilista = NULL;
    desc->num_linhas = 0;
    return desc;
}

void le_arquivo(FILE *arquivo, Desc **desc) {
    *desc = inicializa_desc();
    Multilista *multaux = inicializa_multilista();
    (*desc)->multilista = multaux;

    char linha[50];
    int i = 0;
    int num = 0;
    int col = 0;

    No *aux = NULL;

    while (fgets(linha, sizeof(linha), arquivo)) { // acessa todas as linhas do arquivo
        linha[strcspn(linha, "\n")] = '\0'; // troca o caractere '\n' por '\0' no final de cada linha

        char* token = strtok(linha, " "); // separa a linha em tokens
        while (token) {
            No* no = inicializa_no(i, col, token);
            if (aux) {
                no->antes = aux;
                aux->proximo = no;
            }
            else // caso aux seja nulo, inicia a estrutura
                multaux->linha = no;

            col += strlen(token) + 1; // insere a posicao correta da coluna de cada palavra
            num++;

            token = strtok(NULL, " ");

            aux = no;
        }
        multaux->num_palavras = num; // insere o numero de palavras da linha
        num = 0;
        col = 0;
        i++;

        Multilista* multilista = inicializa_multilista(); // inicializa uma nova multilista
        multaux->abaixo = multilista; // faz o encadeamento das multilistas
        multilista->cima = multaux;
        multaux = multilista;
        aux = NULL;
    }

    (*desc)->num_linhas = i; // insere o numero de linhas do arquivo no descritor
}

int verifica(Desc *desc) { // verifica se a estrutura foi inicializada
    if (!desc->multilista || !desc->multilista->linha) {
        printf("LDDE nao iniciada!\n");
        return 0;
    }
    return 1;
}

void imprime_ldde(Desc *desc) { // imprime no formato da ldde
    if (!verifica(desc))
        return;

    printf("[Desc|%d]->", desc->num_linhas);
    Multilista *multilista = desc->multilista;
    while (multilista) {
        printf("[%d]->", multilista->num_palavras);
        No *aux = multilista->linha;
        while (aux) {
            printf("[%s|%d|%d]->", aux->palavra, aux->linha, aux->coluna);
            aux = aux->proximo;
        }
        printf("//\n          ");
        multilista = multilista->abaixo;
    }
    printf("\n");
}

void imprime_original(Desc *desc) { // imprime no formato do texto original
    if (!verifica(desc))
        return;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            printf("%s ", aux->palavra);
            aux = aux->proximo;
        }
        printf("\n");
        multilista = multilista->abaixo;
    }
    printf("\n");
}

void buscar_palavra(Desc *desc, char* palavra, int *lin, int *col) {
    if (!verifica(desc))
        return;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strcmp(palavra, aux->palavra) == 0) { // se encontrar a palavra retorna sua linha e coluna
                *lin = aux->linha;
                *col = aux->coluna;
                return;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }
}

void corrige_num_coluna(Multilista *multilista) { // apos ocorrer uma insercao ou remocao de uma palavra, corrige a posicao da coluna dos seguintes nos da estrutura
    No *no = multilista->linha;
    int tamanho = 0;
    int coluna = 0;
    while (no) {
        if (no->coluna != coluna) // caso a coluna esteja errada faz a correcao
            no->coluna = coluna;
        
        tamanho = strlen(no->palavra); // tamanho da palavra
        coluna += strlen(no->palavra) + 1; // posicao da coluna
        no = no->proximo;
    }
}

int remover_palavra(Desc *desc, char *palavra) {
    if (!verifica(desc))
        return 0;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strcmp(palavra, aux->palavra) == 0) {
                if (!aux->proximo) { // caso seja o fim da linha
                    aux->antes->proximo = NULL;
                    free(aux);
                }
                else if (aux->antes) { // caso seja no meio da linha
                    aux->antes->proximo = aux->proximo;
                    aux->proximo->antes = aux->antes;
                    free(aux);
                }
                else { // caso seja no comeco da linha
                    multilista->linha = aux->proximo;
                    aux->proximo->antes = NULL;
                    free(aux);
                }

                multilista->num_palavras--;
                corrige_num_coluna(multilista);

                return 1;
            }
            aux = aux->proximo;

            
        }
        multilista = multilista->abaixo;
    }

    return 0;
}

int remover_palavra_lin_col(Desc *desc, int lin, int col) {
    if (!verifica(desc))
        return 0;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (aux->linha == lin && aux->coluna == col && !aux->proximo) { // caso seja o fim da linha
                aux->antes->proximo = NULL;;
                free(aux);
                multilista->num_palavras--;
                corrige_num_coluna(multilista);
                return 1;
            }
            else if (aux->linha == lin && aux->coluna == col && aux->antes) { // caso seja o meio da linha
                aux->antes->proximo = aux->proximo;
                aux->proximo->antes = aux->antes;
                free(aux);
                multilista->num_palavras--;
                corrige_num_coluna(multilista);
                return 1;
            }
            else if (aux->linha == lin && aux->coluna == col && !aux->antes) { // caso seja o comeco da linha
                multilista->linha = aux->proximo;
                aux->proximo->antes = NULL;
                free(aux);
                multilista->num_palavras--;
                corrige_num_coluna(multilista);
                return 1;
            }

            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return 0;
}

int ocorrencias(Desc *desc, char *palavra) {
    if (!verifica(desc))
        return 0;

    int cont = 0; // contador de ocorrencias
    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strcmp(palavra, aux->palavra) == 0) // caso encontre a palavra aumenta o contador de ocorrencias
                cont++;
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return cont;
}

int total_palavras(Desc *desc) {
    if (!verifica(desc))
        return 0;

    int cont = 0; // contador de palavras
    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        cont += multilista->num_palavras; // incrementa o contador de palavras
        multilista = multilista->abaixo; // vai para a proxima multilista, ou seja, proxima linha
    }

    return cont;
}

int editar(Desc *desc, int lin, int col) {
    if (!verifica(desc))
        return 0;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (aux->linha == lin && aux->coluna == col) { // caso chegue na linha e coluna indicadas, realiza a alteracao
                char palavra[50];
                printf("Digite a nova palavra: ");
                scanf("%s", palavra);
                strcpy(aux->palavra, palavra); // troca a palavra para a nova palavra indicada

                corrige_num_coluna(multilista); // corrige as seguintes colunas com base no tamanho da nova palavra

                return 1;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return 0;
}

int inserir(Desc *desc, char *palavra, int lin, int col) {
    if (!verifica(desc))
        return 0;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (aux->linha == lin && aux->coluna == col) { // caso chegue na linha e coluna indicadas, realiza a insercao
                No* no = inicializa_no(lin, (aux->coluna + strlen(palavra) + 1), palavra); // cria um novo no com a nova palavra
                if (lin == 0 && col == 0) { // comeco da linha
                    no->proximo = aux;
                    multilista->linha = no;
                    aux->antes = no;

                    corrige_num_coluna(multilista);
                }
                else { // meio da linha
                    no->proximo = aux;
                    no->antes = aux->antes;
                    aux->antes->proximo = no;
                    aux->antes = no;

                    corrige_num_coluna(multilista);
                }
                multilista->num_palavras++;

                return 1;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return 0;
}

int inserir_no_fim(Desc *desc, char *palavra, int lin) {
    if (!verifica(desc))
        return 0;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (aux->linha == lin && aux->proximo == NULL) { // caso chegue no fim da linha, realiza a insercao
                No* no = inicializa_no(lin, (aux->coluna + strlen(palavra) + 1), palavra); // cria um novo no com a nova palavra
                no->antes = aux;
                aux->proximo = no;
                multilista->num_palavras++;

                corrige_num_coluna(multilista);
                
                return 1;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return 0;
}

void substring(Desc *desc, char *sub) {
    if (!verifica(desc))
        return;

    int quant = 0;
    Multilista *multilista = desc->multilista;
    printf("[L|C]\n"); // [ Linha | Coluna ]
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strstr(aux->palavra, sub)) { // caso encontre a substring
                printf("[%d|%d]\n", aux->linha, aux->coluna); // imprime a sua linha e coluna
                quant++;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }
    printf("Ocorrencias: %d\n\n", quant); // imprime o numero de ocorrencias da substring
}

void copiar_multilista_para_arquivo(Desc *desc, char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w"); // cria o novo arquivo editavel
    if (!arquivo) {
        printf("ERROR: Não foi possível criar o arquivo.\n");
        return;
    }

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) { // realiza a copia da estrutura para o arquivo
        No *no = multilista->linha;
        while (no) {
            fprintf(arquivo, "%s ", no->palavra);
            no = no->proximo;
        }
        fprintf(arquivo, "\n");
        multilista = multilista->abaixo;
    }

    fclose(arquivo);
}
