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
    char *inp_conferencias = NULL;
    if (testa_entrada(argc, argv, &inp_lattes, &inp_conferencias, &inp_periodicos) == 0) {
        exit(1);
    }

    /* Le o conteudo da lista de periodicos e separa cada linha em uma pos do vetor*/
    char *arq_periodicos = arq_leitura(inp_periodicos);
    char *conteudo_periodicos = arq_periodicos;
    lista_qualis *lista_periodicos = separa_str(conteudo_periodicos);
    free(arq_periodicos);

    /* Le o conteudo da lista de conferencias e realiza a mesma operação */
    char *arq_conferencias = arq_leitura(inp_lattes);
    char *conteudo_conferencias = arq_conferencias;
    lista_qualis *lista_conferencias = separa_str(conteudo_conferencias);
    free(arq_conferencias);

    /* Le o conteudo do lattes e cria uma nova struct para o pesquisador */
    char *arq_lattes = arq_leitura(inp_lattes);
    char *conteudo_lattes = arq_lattes;
    pesquisador *nome1 = cria_pesquisador(busca_str(&conteudo_lattes, "NOME-COMPLETO"));

    /* Realiza a busca de todos os periodicos e seus anos no conteudo do lattes */
    char *ano;
    char *titulo;
    char *classe;
    while ((ano = busca_str(&conteudo_lattes, "ANO-DO-ARTIGO")) != NULL) {
        titulo = busca_str(&conteudo_lattes, "REVISTA");
        /* Transforma a string em maiusculo, compara com a lista e retorna seu titulo e classe*/
        maiusculo_str(titulo);
        verifica_artigo(&titulo, &classe, lista_periodicos);
        /* Cria uma nova struct para o artigo e a preenche */
        cria_artigo(nome1, titulo, classe, ano);
    }

    /* Imprime lista de periodicos, com seus respectivos anos */
    printf("%s numero de periodicos: %d\n", nome1->nome, nome1->num_artigos);
    for (int i = 0; i < nome1->num_artigos; i++) {
        printf("FIM: %s -> %s -->> %s n%d\n",  nome1->artigos[i].ano, nome1->artigos[i].titulo, nome1->artigos[i].classe, i+1);
    }

    /* limpa todos os allocs */
    limpa_pesquisador(nome1);
    limpa_lista(lista_periodicos);
    free(arq_lattes);
    return 0;
}