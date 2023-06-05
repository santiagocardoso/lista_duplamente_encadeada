#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

int main(int argc, char **argv) {
    FILE *arquivo = fopen("/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arquivo.txt", "r");
    if (!arquivo) {
        printf("ERROR: Nao foi possivel abrir o arquivo.\n");
        return 0;
    }

    if (remove("/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arquivoOLD.txt") // caso exista um arquivoOLD.txt, deleta ele para executar o programa novamente
                != 0) {
        printf("ERROR: Nao foi possivel deletar o arquivo antigo.\n");
    }

    Desc *desc;
    le_arquivo(arquivo, &desc); // realiza toda a inicializacao da estrutura

    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // cria o arqTemp.txt

    int lin = -1; // instanciacao de variaveis utilizadas futuramente
    int col = -1;
    char palavra[50];

    int opcao = -1;
    while (opcao != 0) {
        menu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 0: // finaliza o programa
                system("clear");
                fclose(arquivo);

                if (rename("/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arquivo.txt", 
                           "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arquivoOLD.txt") 
                           != 0) {
                    printf("ERROR: Nao foi possivel renomear o arquivo antigo.\n");
                    return 0;
                } // renomeia o arquivo.txt original para arquivoOLD.txt

                if (rename("/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt", 
                           "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arquivo.txt") 
                           != 0) {
                    printf("ERROR: Nao foi possivel renomear o arquivo temporario.\n");
                    return 0;
                } // renomeia o arquivo temporario arqTemp.txt para o arquivo permanente arquivo.txt

                printf("Saindo...\n");
                break;
            case 1: // imprime no formato da ldde
                system("clear");
                imprime_ldde(desc);
                break;
            case 2: // imprime no formato do texto original
                system("clear");
                imprime_original(desc);
                break;
            case 3: // busca a palavra fornecida e retorna a linha e coluna
                system("clear");
                printf("Digite a palavra que deseja buscar: ");
                scanf("%s", palavra);

                buscar_palavra(desc, palavra, &lin, &col); // altera as variaveis lin e col com base no resultado da busca
                if (lin == -1 && col == -1)
                    printf("Palavra nao encontrada!\n\n");
                else
                    printf("Linha: %d\nColuna: %d\n\n", lin, col);
                break;
            case 4: // remove uma palavra
                system("clear");
                printf("Digite a palavra que deseja remover: ");
                scanf("%s", palavra);
                if (remover_palavra(desc, palavra)) {
                    printf("Palavra removida!\n\n");
                    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // atualiza o arqTemp.txt
                }
                else
                    printf("Falha na remocao!\n\n");
                break;
            case 5: // remove uma palavra na linha e coluna indicadas
                system("clear");
                printf("Digite a linha e coluna da palavra que deseja remover:\nLinha: ");
                scanf("%d", &lin);
                printf("Coluna: ");
                scanf("%d", &col);
                if (remover_palavra_lin_col(desc, lin, col)) {
                    printf("Palavra removida!\n\n");
                    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // atualiza o arqTemp.txt
                }
                else
                    printf("Falha na remocao!\n\n");
                break;
            case 6: // retorna o numero de ocorrencias de uma palavra
                system("clear");
                printf("Digite a palavra que deseja verificar: ");
                scanf("%s", palavra);
                printf("Ocorrencias: %d\n\n", ocorrencias(desc, palavra));
                break;
            case 7: // retorna o total de palavras
                system("clear");
                printf("Total palavras: %d\n\n", total_palavras(desc));
                break;
            case 8: // edita uma palavra na linha e coluna indicadas
                system("clear");
                printf("Digite a linha e coluna para a edicao:\nLinha: ");
                scanf("%d", &lin);
                printf("Coluna: ");
                scanf("%d", &col);
                if (editar(desc, lin, col)) {
                    printf("Palavra editada!\n\n");
                    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // atualiza o arqTemp.txt
                }
                else
                    printf("Falha na edicao!\n\n");
                break;
            case 9: // insere uma nova palavra na linha e coluna indicadas
                system("clear");
                printf("Digite a palavra que deseja inserir e a linha:\nPalavra: ");
                scanf("%s", palavra);
                printf("Linha: ");
                scanf("%d", &lin);
                printf("Coluna: ");
                scanf("%d", &col);
                if (inserir(desc, palavra, lin, col)) {
                    printf("Palavra inserida!\n\n");
                    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // atualiza o arqTemp.txt
                }
                else
                    printf("Falha na insercao!\n\n");
                break;
            case 10: // insere uma nova palavra no fim da linha indicada
                system("clear");
                printf("Digite a palavra que deseja inserir e a linha:\nPalavra: ");
                scanf("%s", palavra);
                printf("Linha: ");
                scanf("%d", &lin);
                if (inserir_no_fim(desc, palavra, lin)) {
                    printf("Palavra inserida!\n\n");
                    copiar_multilista_para_arquivo(desc, "/home/santcar7/projects/dev/eda/multilista_duplamente_encadeada/arqTemp.txt"); // atualiza o arqTemp.txt
                }
                else
                    printf("Falha na insercao!\n\n");
                break;
            case 11: // busca o total de ocorrencias e as linhas e colunas da substring indicada
                system("clear");
                printf("Digite a palavra que deseja buscar: ");
                scanf("%s", palavra);
                substring(desc, palavra);
                break;
            default:
                system("clear");
                printf("Opcao invalida!\n\n");
                break;
        }
    }

    return 0;
}
