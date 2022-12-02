#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "filelib.h"

/* Recebe a entrada de "main" e analisa seus argumentos para que correspondam a chamada 
 * Retorna 1 em caso de sucesso, ou 0 em erro */
int testa_entrada(int argc, char *argv[], char **curriculo, char **conferencias, char **artigos)
{
    /* Testa o numero de argumentos inseridos */
    if (argc != 7) {
        printf("Erro: Numero de argumentos invalido.\n");
        return 0;
    }
    /* Envia os argumentos da chamada as suas respectivas variaveis */
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            *curriculo = argv[i+1];

        if (strcmp(argv[i], "-c") == 0)
            *conferencias = argv[i+1];
        
        if (strcmp(argv[i], "-p") == 0)
            *artigos = argv[i+1];
    }
    /* Testa se os arquivos nao sao vazios */
    if (*curriculo && *conferencias && *artigos == NULL) {
        printf("Erro: Argumentos invalidos.\n");
        return 0;
    }
    return 1;
}

/* Recebe o nome ou diretorio do arquivo, entao aloca e armazena seu conteudo na memoria 
 * Retorna um ponteiro para o conteudo do arquivo */
char *leitura_arq(char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO: Arquivo %s nao encontrado.\n", nome_arquivo);
        exit(1);
    }
    /* Chega ao final do arquivo, resgata o numero total de bytes e retorna ao inicio */
    fseek(arquivo, 0, SEEK_END);
    int tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    /* Aloca um espaco na memoria para o conteudo do arquivo e o preenche */
    char *conteudo_arquivo = malloc((tamanho_arquivo + 1) * sizeof(char));
    conteudo_arquivo[tamanho_arquivo] = '\0';
    if (fread(conteudo_arquivo, sizeof(char), tamanho_arquivo, arquivo) != tamanho_arquivo) {
        printf("ERRO na leitura do arquivo.\n");
        exit(1);
    } 
    fclose(arquivo);
    return conteudo_arquivo;
}

/* Recebe uma string separada por "\n", remove repeticoes e detalhes desnecessarios
 * Aloca uma nova lista de artigos, armazena cada linha nesta lista e a retorna */
lista_qualis *listagem_str(char *str)
{

    /* Aloca o espaco da struct */
    lista_qualis *lista = malloc(sizeof(lista_qualis));
    lista->num_total = 0;

    /* Cria uma copia da str */
    char *str_tmp = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(str_tmp, str);
    /* Conta o numero de linhas */
    char *linha = strtok(str_tmp, "\n");
    while (linha != NULL) {
        (lista->num_total)++;
        linha = strtok(NULL, "\n");
    }
    free(str_tmp);

    /* Aloca o vetor de strings */
    lista->strings = malloc((lista->num_total) * sizeof(char *));
    /* Separa a string linha por linha */
    str_tmp = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(str_tmp, str);
    linha = strtok(str_tmp, "\n");
    int i = 0;
    while (linha != NULL) {
        lista->strings[i] = malloc((strlen(linha) + 1) * sizeof(char));
        strcpy(lista->strings[i], linha);
        i++;
        linha = strtok(NULL, "\n");
    }
    free(str_tmp);
    /* Remove as repetidas */
    for (int i = 0; i < lista->num_total; i++) {
        for (int j = i + 1; j < lista->num_total; j++) {
            if (strcmp(lista->strings[i], lista->strings[j]) == 0) {
                free(lista->strings[j]);
                lista->strings[j] = lista->strings[lista->num_total - 1];
                (lista->num_total)--;
                j--;
            }
        }
    }
    return lista;
}

/* Limpa todos os dados alocados na lista de artigos 
 * Em seguida limpa a propria struct, retorna 1 em sucesso, 0 o contrario */
int limpa_lista(lista_qualis *lista)
{
    if (lista == NULL)
        return 0;
    for (int i = 0; i < lista->num_total; i++)
        free(lista->strings[i]);
    free(lista->strings);
    free(lista);
    return 1;
}

/* Aloca espaco para um nova struct de pesquisador e a preenche com alguns dados 
 * Retorna um ponteiro para esta struct */
pesquisador *cria_pesquisador(char *nome)
{
    pesquisador *novo = malloc(sizeof(pesquisador));
    if (novo == NULL) {
        printf("ERRO na criacao da struct de pesquisador.\n");
        exit(1);
    }
    novo->nome = nome;
    novo->num_periodicos = 0;
    novo->num_conferencias = 0;
    novo->periodicos = NULL;
    novo->conferencias = NULL;
    return novo;
}

/* Limpa os dados alocados na lista de artigos de um certo pesquisador 
 * E em seguida limpa a propria struct, retorna 1 em sucesso, ou 0 o contrario */
int limpa_pesquisador(pesquisador *pesquisador)
{
    if (pesquisador == NULL)
        return 0;
    /* Limpa a lista */
    printf("Liberando memoria...\n");
    for (int i = 0; i < pesquisador->num_periodicos; i++){
        free(pesquisador->periodicos[i].titulo);
        free(pesquisador->periodicos[i].classe);
        free(pesquisador->periodicos[i].ano);
    }
    for (int i = 0; i < pesquisador->num_conferencias; i++){
        free(pesquisador->conferencias[i].titulo);
        free(pesquisador->conferencias[i].classe);
        free(pesquisador->conferencias[i].ano);
    }
    free(pesquisador->nome);
    free(pesquisador->periodicos);
    free(pesquisador->conferencias);
    free(pesquisador);
    return 1;
}

/* Recebe uma string onde ira ser procurado a string "alvo", que antecede nossa resposta,
 * entao encontra esta resposta (que esta entre " ") e a retorna */
char *encontra_str(char **str, char *alvo)
{
    /* Encontra nosso alvo e entao encontra o fim da resposta */
    char *inicio = strstr(*str, alvo);
    if (inicio == NULL)
        return NULL;
    inicio += (strlen(alvo) + 2);
    char *fim = strchr(inicio, '"');
    int tamanho = fim - inicio;

    /* Aloca espaco para a resposta e a preenche */
    char *resposta = malloc((tamanho + 1) * sizeof(char));
    strncpy(resposta, inicio, tamanho);
    resposta[tamanho] = '\0';
    /* Atualiza o ponteiro da string */
    *str = fim;
    return resposta;
}

/* Recebe uma string qualquer e a retorna totalmente em maiusculo */
int maiusculo_str(char *Str)
{
    int i;
    for (i = 0; Str[i] != '\0'; i++) {
        if (Str[i] >= 'a' && Str[i] <= 'z') {
            Str[i] = Str[i] - 32;
        }
    }
    return 1;
}

/* Recebe duas strings e as compara, utilizando o metodo de edit distance
 * Fortemente inspirado no algoritmo de https://www.geeksforgeeks.org/edit-distance-dp-5/ */
int min(int x, int y, int z)
{
    if (x <= y && x <= z) 
        return x;
    if (y <= x && y <= z) 
        return y;
    else return z;
}
int compara_str(char *str1, char *str2)    
{  
    int m = strlen(str1);  
    int n = strlen(str2);  
    int matriz[m+1][n+1];  
    for (int i=0; i<=m; i++)  
    {  
        for (int j=0; j<=n; j++)  
        {  
            if (i==0)  
                matriz[i][j] = j;  
            else if (j==0)  
                matriz[i][j] = i;  
            else if (str1[i-1] == str2[j-1])  
                matriz[i][j] = matriz[i-1][j-1];  
            else
                matriz[i][j] = 1 + min(matriz[i][j-1], matriz[i-1][j], matriz[i-1][j-1]);  
        }  
    }  
    return matriz[m][n];  
}

/* Recebe o titulo de um artigo e o compara com uma entrada do vetor de qualis 
 * Retorna o titulo que mais se assemelha ao original, ou o original caso a distancia > 11 */
int checa_artigo(char **titulo, char **classe, lista_qualis *lista)
{
    int distancia = 0;
    int menor_distancia = 100;
    char *titulo_menor = NULL;

    printf("Checando artigo: %s\n", *titulo);
    /* Compara o titulo do artigo com todas as strings do vetor */
    for (int i = 0; i < lista->num_total; i++) {
        distancia = compara_str(*titulo, lista->strings[i]);
        if (distancia < menor_distancia) {
            menor_distancia = distancia;
            titulo_menor = lista->strings[i];
        }
        if (menor_distancia == 3)
            break;
    }
    
    printf("Menor: %d -> ", menor_distancia);
    printf("Titulo: %s\n", titulo_menor);
    if (menor_distancia < 16) {
        /* Separa o titulo do arquivo de artigos em titulo e classe */
        char *tmp = strrchr(titulo_menor, ' ');

        /* Pega a string antes do ultimo espaco */
        int tamanho = tmp - titulo_menor;
        *titulo = realloc(*titulo, (tamanho + 1) * sizeof(char));
        strncpy(*titulo, titulo_menor, tamanho);
        (*titulo)[tamanho] = '\0';

        /* Pega a string depois do ultimo espaco */
        tamanho = strlen(titulo_menor) - tamanho;
        *classe = malloc((tamanho + 1) * sizeof(char));
        strncpy(*classe, tmp + 1, tamanho);
        (*classe)[tamanho] = '\0';
    } else {
        /* Se a distancia for maior que 11, o titulo original é mantido */
        *classe = malloc(2 * sizeof(char));
        strcpy(*classe, "C");
    }
    return 1;
}

/* Recebe os dados para a criacao de um novo artigo e o autor do mesmo,
 * Aloca o espaco necessario e retorna 1 em sucesso, 0 o contrario */
int cria_artigo(pesquisador *autor, char *titulo, char *ano, char *classe, char *tipo)
{
    if (strcmp(tipo, "periodico") == 0) {
        /* Em caso do artigo ser do tipo periodico */
        autor->periodicos = realloc(autor->periodicos, (autor->num_periodicos + 1) * sizeof(artigos));
        autor->periodicos[autor->num_periodicos].titulo = titulo;
        autor->periodicos[autor->num_periodicos].ano = ano;
        autor->periodicos[autor->num_periodicos].classe = classe;
        autor->num_periodicos++;
    } else if (strcmp(tipo, "conferencia") == 0) {
        /* Em caso do artigo ser do tipo conferencia */
        autor->conferencias = realloc(autor->conferencias, (autor->num_conferencias + 1) * sizeof(artigos));
        autor->conferencias[autor->num_conferencias].titulo = titulo;
        autor->conferencias[autor->num_conferencias].ano = ano;
        autor->conferencias[autor->num_conferencias].classe = classe;
        autor->num_conferencias++;
    } else {
        return 0;
    }
    return 1;
}
