#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/* Chamada: ./teste -c <lattes> -p <txt periodicos>*/

int main (int argc, char *argv[])
{
    /* Preenche e testa as entradas do programa */
    char *inp_lattes = NULL;
    char *inp_periodicos = NULL;
    if (testa_entrada(argc, argv, &inp_lattes, &inp_periodicos) == 0) {
        exit(1);
    }

    /* Le o conteudo do lattes */
    char *arq_lattes = arq_leitura(inp_lattes);
    char *conteudo_lattes = arq_lattes;

    /* Le o conteudo da lista de periodicos e o separa em linhas */
    char *arq_periodicos = arq_leitura(inp_periodicos);
    char *conteudo_periodicos = arq_periodicos;

    /* Cria um novo espaço para pesquisador e o preenche com seu nome */
    pesquisador *jose = cria_pesquisador(busca_str(&conteudo_lattes, "NOME-COMPLETO"));

    /* Realiza a busca de todos os periodicos e seus anos do lattes */
    char *ano;
    char *titulo;
    char *classe;
    while ((ano = busca_str(&conteudo_lattes, "ANO-DO-ARTIGO")) != NULL) {
        titulo = busca_str(&conteudo_lattes, "REVISTA");
        /* Transforma a string em maiusculo, compara com a lista e retorna seu titulo e classe*/
        maiusculo_str(titulo);
        verifica_periodico(&titulo, &classe, conteudo_periodicos);

        /* Cria uma nova struct e a preenche */
        cria_periodico(jose, titulo, classe, ano);
    }

    /* Imprime lista de periodicos, com seus respectivos anos */
    printf("%s numero de periodicos: %d\n", jose->nome, jose->num_periodicos);
    for (int i = 0; i < jose->num_periodicos; i++) {
        printf("FIM: %s -> %s -->> %s n%d\n",  jose->periodicos[i].ano, jose->periodicos[i].titulo, jose->periodicos[i].classe, i+1);
    }

    /* limpa todos os allocs */
    limpa_pesquisador(jose);
    free(arq_periodicos);
    free(arq_lattes);
    return 0;
}