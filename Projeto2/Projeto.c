#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/* Forma de chamada do algoritmo: */
/* ./lattes -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos> */

void main (int argc, char *argv[]) 
{
    /* Testa o numero de entradas para que seja compativel com o programa */
    if (argc != 7) {
        printf("ERRO: número de argumentos inválido.\n");
        printf("Forma de chamada: \n./lattes -d </dir com os CVs> -c <.txt Lista Qualis Conf> -p <.txt Lista Qualis Periódicos>");
        exit(1);
    }
    /* Analisa a entrada para relacionar os arquivos com suas variaveis
     * Obedecendo a ordem "-comando" seguido do "arquivo correspondente" */
    char *diretorio, *arquivoConf, *arquivoPeriodicos;
    int i;
    for (i = 1; i < argc; i++) {
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
    if (diretorio && arquivoConf && arquivoPeriodicos == NULL) {
        printf("ERRO: arquivo não encontrado.\n");
        exit(1);
    }


}