#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forma de chamada do algoritmo: */
/* ./lattes -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos> */

void main (int argc, char *argv[]) 
{
    /* Testa o numero de entradas e retorna Erro caso argc != 7 */
    if (argc != 7) {
        printf("ERRO: número de argumentos inválido.\n");
        printf("Forma de chamada: \n./lattes -d </dir com os CVs> -c <.txt Lista Qualis Conf> -p <.txt Lista Qualis Periódicos>");
        exit(1);
    }
    
    /* Analisa a entrada para relacionar os arquivos com suas variaveis */
    /* Obedecendo a ordem "-comando" seguido do "arquivo correspondente" */
    char *diretorio, *arquivoConf, *arquivoPeriodicos;
    int i;
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            diretorio = argv[i+1];
        }
        if (strcmp(argv[i], "-c") == 0) {
            arquivoConf = argv[i+1];
        }
        if (strcmp(argv[i], "-p") == 0) {
            arquivoPeriodicos = argv[i+1];
        }
    }
    /* Abre o arquivo de Qualis de Conferencias */
    FILE *arqConf = fopen(arquivoConf, "r");
    if (arqConf == NULL) {
        printf("ERRO: Arquivo de Lista de Conferencias '%s' não encontrado.\n", arquivoConf);
        exit(1);
    }
    /* Abre o arquivo de Qualis de Periodicos */
    FILE *arqPeriodicos = fopen(arquivoPeriodicos, "r");
    if (arqPeriodicos == NULL) {
        printf("ERRO: Arquivo de Lista de Periodicos '%s' não encontrado.\n", arquivoPeriodicos);
        exit(1);
    }
    /* Abre o arquivo de Qualis de Periodicos */
    FILE *arqDiretorio = fopen(diretorio, "r");
    if (arqDiretorio == NULL) {
        printf("ERRO: Diretorio '%s' não encontrado.\n", diretorio);
        exit(1);
    }
    /* Le o arquivo indicado e retorna um ponteiro para o stream */
    FILE *arquivo = fopen("curriculo.xml", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo");
        exit(1);
    }

}