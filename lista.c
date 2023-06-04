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
    printf("[10] Palavras com uma substring\n");
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

   while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';

        char* token = strtok(linha, " ");
        while (token) {
            No* no = inicializa_no(i, col, token);
            if (aux) {
                no->antes = aux;
                aux->proximo = no;
            }
            else
                multaux->linha = no;

            col += strlen(token) + 1;
            num++;

            token = strtok(NULL, " ");

            aux = no;
        }
        multaux->num_palavras = num;
        num = 0;
        col = 0;
        i++;

        Multilista* multilista = inicializa_multilista();
        multaux->abaixo = multilista;
        multilista->cima = multaux;
        multaux = multilista;
    }

    (*desc)->num_linhas = i;
}

int verifica(Desc *desc) {
    if (!desc->multilista || !desc->multilista->linha) {
        printf("LDDE nao iniciada!\n");
        return 0;
    }
    return 1;
}

void imprime_ldde(Desc *desc) {
    if (!verifica(desc))
        return;

    printf("-=-=-=-=-=-=-  LDDE  -=-=-=-=-=-=-\n");
    Multilista *multilista = desc->multilista;
    while (multilista) {
        No *aux = multilista->linha;
        while (aux) {
            printf("\n%s\n", aux->palavra);
            printf("Linha: %d\n", aux->linha);
            printf("Coluna: %d\n", aux->coluna);
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }
    printf("-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-\n\n");
}

void imprime_original(Desc *desc) {
    if (!verifica(desc))
        return;

    printf("-=-=-=-=- TEXTO ORIGINAL -=-=-=-=-\n");
    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            printf("%s ", aux->palavra);
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }
    printf("\n-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-\n\n");
}

void buscar_palavra(Desc *desc, char* palavra, int *lin, int *col) {
    if (!verifica(desc))
        return;

    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strcmp(palavra, aux->palavra) == 0) {
                *lin = aux->linha;
                *col = aux->coluna;
                return;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
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
                aux->antes->proximo = aux->proximo;
                aux->proximo->antes = aux->antes;
                free(aux);
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
            if (aux->linha == lin && aux->coluna == col) {
                aux->antes->proximo = aux->proximo;
                aux->proximo->antes = aux->antes;
                free(aux);
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

    int cont = 0;
    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        No *aux = multilista->linha;
        while (aux) {
            if (strcmp(palavra, aux->palavra) == 0)
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

    int cont = 0;
    Multilista *multilista = desc->multilista;
    while (multilista->abaixo) {
        cont += multilista->num_palavras;
        multilista = multilista->abaixo;
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
            if (aux->linha == lin && aux->coluna == col) {
                char palavra[50];
                printf("Digite a nova palavra: ");
                scanf("%s", palavra);
                strcpy(aux->palavra, palavra);
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
            if (aux->linha == lin && aux->coluna == col) {
                No* no = inicializa_no(lin, (aux->coluna + strlen(palavra) + 1), palavra);
                no->proximo = aux;
                no->antes = aux->antes;
                aux->antes->proximo = no;
                aux->antes = no;

                return 1;
            }
            aux = aux->proximo;
        }
        multilista = multilista->abaixo;
    }

    return 0;
}
