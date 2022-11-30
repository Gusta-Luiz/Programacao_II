#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *arq_leitura(char *nome_arquivo)
{
    /* Abre o arquivo para leitura indicado e retorna um ponteiro para o stream */    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO: arquivo %s não encontrado\n", nome_arquivo);
        exit(1);
    }
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
    char *conteudo_arquivo = (char *) malloc(tamanho_arquivo+1);
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

/* <DETALHAMENTO-DO-ARTIGO TITULO-DO-PERIODICO-OU-REVISTA="Random Structures &amp; Algorithms" ISSN="10429832" */
/* Chamada: ./teste -c <curriculo> -p <txt periodicos>*/

int main (int argc, char *argv[])
{
    /* Testa o numero de entradas para que seja compativel com o programa */
    if (argc != 5) {
        printf("ERRO: número de argumentos inválido.\n");
        printf("Forma de chamada: \n ./lattes -c <curriculo> -p <txt periodicos>\n");
        exit(1);
    }
    /* Adiciona os argumentos da chamada as suas respectivas variaveis */
    char *curriculo, *periodicos;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            curriculo = argv[i+1]; 
        }
        if (strcmp(argv[i], "-p") == 0) {
            periodicos = argv[i+1];
        }
    }
    if (curriculo && periodicos == NULL) {
        printf("ERRO: argumentos inválidos.\n");
        printf("Forma de chamada: \n ./lattes -c <curriculo> -p <txt periodicos>");
        exit(1);
    }
    /* Le o conteudo dos arquivos */
    char *conteudo_curriculo = arq_leitura(curriculo);
    char *conteudo_periodicos = arq_leitura(periodicos);
    char *lista = malloc(sizeof(char));

    /* Encontra todas as ocorrencias de "Periodicos" no curriculo e os salva em outra string */
    char *periodicos_curriculo = conteudo_curriculo;
    int cont = 0;
    while ((periodicos_curriculo = strstr(periodicos_curriculo, "REVISTA")) != NULL) {
        cont++;
        periodicos_curriculo += strlen("REVISTA='");
        /* junta a string finalizada em " na lista */
        char *p = strchr(periodicos_curriculo, '"');
        /* Copia a string ao final da lista */
        lista = realloc(lista, strlen(lista) + strlen(periodicos_curriculo) + 2);
        strncat(lista, periodicos_curriculo, p - periodicos_curriculo);
        /* Adiciona um espaço ao final da string */
        strcat(lista, "\n");
        

    }
    printf("Total de periodicos: %d \n", cont);
    printf("Lista de periodicos: %s \n", lista);

    free(lista);
    free(conteudo_curriculo);
    free(conteudo_periodicos);
    return 1;
}