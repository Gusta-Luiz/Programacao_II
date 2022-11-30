#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/* Abre o arquivo indicado e retorna o conteudo do mesmo em uma string 
 * Deve ser feito o free do conteudo posteriormente */
char* arq_leitura(char *nome_arquivo)
{
    /* Abre o arquivo para leitura indicado e retorna um ponteiro para o stream */    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO: arquivo %s não encontrado\n", nome_arquivo);
        exit(1);
    }

    /* Procura o final do arquivo para obter o tamanho total do conteudo 
     * retorna o ponteiro ao inicio e aloca o espaco necessario */

    /* Chega ao final do arquivo */
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        printf("ERRO ao chegar ao final do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Retorna o numero de bytes do arquivo */
    int tamanho_arquivo = ftell(arquivo);
    if (tamanho_arquivo == -1L) {
        printf("ERRO ao retornar o tamanho do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Retorna o ponteiro para o inicio do arquivo */
    if (fseek(arquivo, 0, SEEK_SET) != 0) {
        printf("ERRO ao retornar o ponteiro para o inicio do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Aloca memoria para o conteudo do arquivo */
    char *conteudo_arquivo = (char *) malloc(tamanho_arquivo);
    if (conteudo_arquivo == NULL) {
        printf("ERRO ao alocar memoria para o conteudo do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Le o conteudo do arquivo na memoria */
    if (fread(conteudo_arquivo, 1, tamanho_arquivo, arquivo) != tamanho_arquivo) {
        printf("ERRO ao ler o conteudo do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Fecha o arquivo */
    if (fclose(arquivo) != 0) {
        printf("ERRO ao fechar o arquivo\n");
        exit(1);
    }
    /* Retorna o conteudo do arquivo */
    return conteudo_arquivo;
}

/* Realiza a busca de uma string e retorna o numero de ocorrencias da mesma 
 * Recebe a "Palavra" a ser procurada, o Local onde procurar e o tamanho deste local */
int str_busca(char *conteudo, int tamanho, char *palavra)
{
    int ocorrencia = 0;
    char *posicao_ocorrencia = conteudo;
    /* Conta a quantidade de ocorrencias da palavra no conteudo do arquivo */
    while ((posicao_ocorrencia = strstr(posicao_ocorrencia, palavra)) != NULL) {
        ocorrencia++;
        posicao_ocorrencia += strlen(palavra);
    }
    return ocorrencia;
}

/* Separa todos os periodicos ou revistas do curriculo
 * Recebe o conteudo do arquivo e o tamanho do mesmo, e devolve a lista de periodicos */
char* separa_periodicos(char *conteudo, int tamanho)
{
    /* Localiza os periodicos e os escreve em uma nova string */
}