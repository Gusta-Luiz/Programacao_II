#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "filelib.h"

/* Chamada: ./teste -d <dir lattes> -c <txt conferencia> -p <txt periodicos>*/

int main(int argc, char *argv[])
{
    /* Preenche as variaveis de acordo com sua entrada, e as testam */
    char *inp_dir, *inp_periodicos, *inp_conferencias = NULL;
    if (testa_entrada(argc, argv, &inp_dir, &inp_conferencias, &inp_periodicos) == 0) {
        printf("Forma de chamada: ./lattes -d <dir lattes> -c <txt conferencia> -p <txt periodico>\n");
        exit(1);
    }

    /* Processa o conteudo do arquivo de conferencias e o separa em uma lista */
    char *arq_conferencias = leitura_arq(inp_conferencias);
    char *conteudo_conferencias = arq_conferencias;
    maiusculo_str(conteudo_conferencias);
    lista_qualis *lista_conferencias = listagem_str(conteudo_conferencias);
    free(arq_conferencias);

    /* Processa o conteudo do arquivo de periodicos e o separa em uma lista */
    char *arq_periodicos = leitura_arq(inp_periodicos);
    char *conteudo_periodicos = arq_periodicos;
    lista_qualis *lista_periodicos = listagem_str(conteudo_periodicos);
    free(arq_periodicos);

    /* Abre o diretorio de entrada */
    struct dirent *files;
    DIR *dir = opendir(inp_dir);
    if (dir == NULL){
        printf("ERRO: não foi possivel abrir diretorio\n" );
        exit(1);
    }
    /* Salva o numero total de arquivos .xml */
    int num_arquivos = 0;
    while ((files = readdir(dir)) != NULL) {
        if (strstr(files->d_name, ".xml") != NULL) {
            num_arquivos++;
        }
    }
    rewinddir(dir);

    /* Inicia o laco de repeticao para todos os curriculos do diretorio */
    while ((files = readdir(dir)) != NULL) {
    /* Ignora os arquivos . e .. */
    if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0)
        continue;
    /* Concatena o nome do arquivo com o diretorio de entrada */
    char *nome_arquivo = malloc(strlen(inp_dir) + strlen(files->d_name) + 2);   
    strcpy(nome_arquivo, inp_dir);
    strcat(nome_arquivo, files->d_name);


    printf("Numero de arquivos: %d\n", num_arquivos);
    printf("Arquivo atual %s: \n", nome_arquivo);

    /* Processa o conteudo do lattes e cria uma nova struct para o pesquisador */
    char *arq_lattes = leitura_arq(nome_arquivo);
    char *conteudo_lattes = arq_lattes;
    pesquisador *autor = cria_pesquisador(encontra_str(&conteudo_lattes, "NOME-COMPLETO"));

    /*--------------------------------------------------------------------*/
    /* Realiza a busca de todos os periodicos e seus anos no conteudo do lattes */
    char *ano, *titulo, *classe;
    while ((ano = encontra_str(&conteudo_lattes, "ANO-DO-ARTIGO")) != NULL) {
        titulo = encontra_str(&conteudo_lattes, "REVISTA");
        /* Verifica na lista de periodicos se existe um correspondente */
        maiusculo_str(titulo);
        checa_artigo(&titulo, &classe, lista_periodicos);
        cria_artigo(autor, titulo, ano, classe, "periodico");
    }
    conteudo_lattes = arq_lattes;
    /*--------------------------------------------------------------------*/
    /* Aponta o ponteiro para o inicio do primeiro evento */
    while ((conteudo_lattes = strstr(conteudo_lattes, "DADOS-BASICOS-DA-ORGANIZACAO")) != NULL) {
        titulo = encontra_str(&conteudo_lattes, "TITULO");
        ano = encontra_str(&conteudo_lattes, "ANO");
        /* Verifica na lista de periodicos se existe um correspondente */
        maiusculo_str(titulo);
        checa_artigo(&titulo, &classe, lista_conferencias);
        cria_artigo(autor, titulo, ano, classe, "conferencia");
    }
    conteudo_lattes = arq_lattes;
    /*--------------------------------------------------------------------*/

    free(arq_lattes);
    free(nome_arquivo);
    }
    closedir(dir);


    /* Imprime o resultado */
    imprime_pesquisador(autor);

    limpa_lista(lista_conferencias);
    limpa_lista(lista_periodicos);
    return 0;
}