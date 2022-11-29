#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/*  */

char* file_read(char *nome_arquivo, int *tamanho_arquivo)
{
    /* Abre o arquivo para leitura indicado e retorna um ponteiro para o stream */    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }
    /* Chega ao final do arquivo */
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        printf("Erro ao chegar ao final do arquivo\n");
        exit(1);
    }
    /* Retorna o numero de bytes do arquivo */
    *tamanho_arquivo = ftell(arquivo);
    if (*tamanho_arquivo == -1L) {
        printf("Erro ao retornar o tamanho do arquivo\n");
        exit(1);
    }
    /* Retorna o ponteiro para o inicio do arquivo */
    if (fseek(arquivo, 0, SEEK_SET) != 0) {
        printf("Erro ao retornar o ponteiro para o inicio do arquivo\n");
        exit(1);
    }
    /* Aloca memoria para o conteudo do arquivo */
    char *conteudo_arquivo = (char *) malloc(*tamanho_arquivo);
    if (conteudo_arquivo == NULL) {
        printf("Erro ao alocar memoria para o conteudo do arquivo\n");
        exit(1);
    }
    /* Le o conteudo do arquivo na memoria */
    if (fread(conteudo_arquivo, 1, *tamanho_arquivo, arquivo) != *tamanho_arquivo) {
        printf("Erro ao ler o conteudo do arquivo\n");
        exit(1);
    }
    /* Fecha o arquivo */
    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar o arquivo\n");
        exit(1);
    }
    /* Retorna o conteudo do arquivo */
    return conteudo_arquivo;
}

/* Realiza a busca de uma string e retorna o numero de ocorrencias da mesma 
 * Recebe a "Palavra" a ser procurada, o Local onde procurar e o tamanho deste local */
int str_grep(char *conteudo, int tamanho, char *palavra)
{
    int ocorrencia = 0;
    char *posicao_ocorrencia = conteudo;
    /* Conta a quantidade de ocorrencias da palavra no conteudo do arquivo */
    while ((posicao_ocorrencia = strstr(posicao_ocorrencia, palavra)) != NULL) {
        ocorrencia++;
        posicao_ocorrencia += strlen(palavra);
    }
}