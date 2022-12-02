#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/* Recebe a entrada do terminal e analisa os argumentos para que correspondam a chamada 
 * Retorna 1 em caso de sucesso ou 0 em caso de erro */
int testa_entrada(int argc, char *argv[], char **curriculo, char **conferencias, char **artigos)
{
    /* Testa o numero de entradas para que seja compativel com o programa */
    if (argc != 5) {
        printf("ERRO: número de argumentos inválido.\n");
        printf("Forma de chamada: \n ./lattes -d <dir com os lattes> -c <txt conferencias> -p <txt artigos>\n");
        return 0;
    }
    /* Adiciona os argumentos da chamada as suas respectivas variaveis */
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            *curriculo = argv[i+1];
        }
        if (strcmp(argv[i], "-c") == 0) {
            *conferencias = argv[i+1]; 
        }
        if (strcmp(argv[i], "-p") == 0) {
            *artigos = argv[i+1];
        }
    }
    /* Testa se os arquivos estao corretos */
    if (*curriculo && *artigos && *conferencias == NULL) {
        printf("ERRO: argumentos inválidos.\n");
        printf("Forma de chamada: \n ./lattes -d <dir com os lattes> -c <txt conferencias> -p <txt artigos>\n");
        return 0;
    }
    return 1;
}

/* Recebe o nome ou diretorio do arquivo e armazena seu conteudo na memoria
 * Retorna um ponteiro para este conteudo em caso de sucesso */
char *arq_leitura(char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO: arquivo %s não encontrado\n", nome_arquivo);
        exit(1);
    }
    /* Chega ao final do arquivo, recupera o numero total de bytes 
     * E retorna o ponteiro ao inicio */
    fseek(arquivo, 0, SEEK_END);
    int tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    /* Aloca memoria para o conteudo do arquivo */
    char *conteudo_arquivo = malloc((tamanho_arquivo + 1)  * sizeof(char));
    conteudo_arquivo[tamanho_arquivo] = '\0';

    /* Le o conteudo do arquivo na memoria e verifica se bate com o tamanho recebido */
    if (fread(conteudo_arquivo, 1, tamanho_arquivo, arquivo) != tamanho_arquivo) {
        printf("ERRO ao ler o conteudo do arquivo %s\n", nome_arquivo);
        exit(1);
    }
    /* Fecha o arquivo e retorna seu conteudo */
    fclose(arquivo);
    return conteudo_arquivo;
}

/* Cria uma nova struct para um pesquisador e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
pesquisador *cria_pesquisador(char *nome)
{
    /* Aloca espaco para a struct e a testa */
    pesquisador *novo_pesquisador = malloc(sizeof(pesquisador));
    if (novo_pesquisador == NULL ) {
        printf("ERRO: Problema na alocação da struct pesquisador\n");
        exit(1);
    }
    /* Preenche os dados e retorna */
    novo_pesquisador->nome = nome;
    novo_pesquisador->num_artigos = 0;
    novo_pesquisador->artigos = NULL;
    return novo_pesquisador;
}

/* Cria uma nova struct para um artigo e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
void cria_artigo(pesquisador *autor, char *titulo, char *classe, char *ano)
{
    /* Abre espaco no vetor do pesquisador para uma nova insercao */
    autor->num_artigos++;
    autor->artigos = realloc(autor->artigos, autor->num_artigos * sizeof(artigos));
    if (autor->artigos == NULL) {
        printf("ERRO: Problema na realocação do vetor de artigos\n");
        exit(1);
    }
    /* Preenche os dados e retorna */
    autor->artigos[autor->num_artigos - 1].titulo = titulo;
    autor->artigos[autor->num_artigos - 1].classe = classe;
    autor->artigos[autor->num_artigos - 1].ano = ano;
    return;
}

/* Limpa os dados alocados de todos os artigos de um certo pesquisador
 * E em seguida limpa a propria struct deste pesquisador */
void limpa_pesquisador(pesquisador *pesquisador)
{
    for (int i = 0; i < pesquisador->num_artigos; i++) {
        free(pesquisador->artigos[i].titulo);
        free(pesquisador->artigos[i].ano);
        free(pesquisador->artigos[i].classe);
    }
    free(pesquisador->artigos);
    free(pesquisador->nome);
    free(pesquisador);
    return;
}

/* Busca uma string dentro do arquivo
 * Retorna a proxima str que estiver entre aspas duplas */
char *busca_str(char **lattes, char *str)
{
    /* Salva lattes em uma outra variavel temporaria */
    char *inicio= *lattes;
    /* Busca a string */
    inicio = strstr(inicio, str);
    if (inicio == NULL) 
        return NULL;
    /* Encontra o inicio e o fim da nossa string resposta, a copia e retorna */
    inicio += (strlen(str) + 2);
    char *fim = strchr(inicio, '"');
    int tamanho = fim - inicio;

    char *resposta = malloc((tamanho + 1) * sizeof(char));
    strncpy(resposta, inicio, tamanho);
    resposta[tamanho] = '\0';

    /* Altera o valor de lattes para a proxima busca */
    *lattes = fim;
    return resposta;
}

/* Recebe uma string qualquer e a retorna totalmente me maiusculo 
 * Funcao retirada do site https://www.codevscolor.com/c-convert-string-to-uppercase */
void maiusculo_str(char *Str)
{
    int i;
    for (i = 0; Str[i] != '\0'; i++) {
        if (Str[i] >= 'a' && Str[i] <= 'z') {
            Str[i] = Str[i] - 32;
        }
    }
}

/* Funcao de distancia de edicao para comparar os artigos do pesquisador
 * com os artigos do arquivo de artigos */
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

/* Compara o titulo do artigo do pesquisador com todos os titulos do arquivo
 * Retorna o titulo que mais se assemelha ao do pesquisador */
void verifica_artigo(char **titulo, char **classe, lista_qualis *lista)
{
    int distancia = 0;
    int menor_distancia = 1000;
    char *titulo_mais_proximo = NULL;

    /* Compara o titulo do artigo com todas as strings do vetor */
    for (int i = 0; i < lista->num_total; i++) {
        distancia = compara_str(*titulo, lista->strings[i]);
        if (distancia < menor_distancia) {
            menor_distancia = distancia;
            titulo_mais_proximo = lista->strings[i];
        }
    }

    printf("Verificando %s ->-> %s -> Dif: %d\n",*titulo ,titulo_mais_proximo, menor_distancia);
    /* Separa o titulo do arquivo de artigos em titulo e classe */
    char *tmp2 = strrchr(titulo_mais_proximo, ' ');

    /* Pega a string antes do ultimo espaco */
    int tamanho = tmp2 - titulo_mais_proximo;
    *titulo = realloc(*titulo, (tamanho + 1) * sizeof(char));
    strncpy(*titulo, titulo_mais_proximo, tamanho);
    (*titulo)[tamanho] = '\0';

    /* Pega a string depois do ultimo espaco */
    tamanho = strlen(titulo_mais_proximo) - tamanho;
    *classe = malloc((tamanho + 1) * sizeof(char));
    strncpy(*classe, tmp2 + 1, tamanho);
    (*classe)[tamanho] = '\0';
    
    return;
}

/* Separa a string linha por linha, remove as repetidas, aloca uma struct de lista
 * E preenche esta lista com todas as linhas da string */
lista_qualis *separa_str(char *str)
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

/* Limpa os dados alocados da lista de artigos
 * E em seguida limpa a propria struct da lista */
void limpa_lista(lista_qualis *lista)
{
    for (int i = 0; i < lista->num_total; i++) {
        free(lista->strings[i]);
    }
    free(lista->strings);
    free(lista);
}