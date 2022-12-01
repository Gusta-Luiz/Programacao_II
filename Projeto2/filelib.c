#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filelib.h"

/* Recebe a entrada do terminal e analisa os argumentos para que correspondam a chamada 
 * Retorna 1 em caso de sucesso ou 0 em caso de erro */
int testa_entrada(int argc, char *argv[], char **curriculo, char **periodicos)
{
    /* Testa o numero de entradas para que seja compativel com o programa */
    if (argc != 5) {
        printf("ERRO: número de argumentos inválido.\n");
        printf("Forma de chamada: \n ./lattes -c <curriculo> -p <txt periodicos>\n");
        return 0;
    }
    /* Adiciona os argumentos da chamada as suas respectivas variaveis */
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            *curriculo = argv[i+1]; 
        }
        if (strcmp(argv[i], "-p") == 0) {
            *periodicos = argv[i+1];
        }
    }
    /* Testa se os arquivos estao corretos */
    if (*curriculo && *periodicos == NULL) {
        printf("ERRO: argumentos inválidos.\n");
        printf("Forma de chamada: \n ./lattes -c <curriculo> -p <txt periodicos>");
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
    novo_pesquisador->num_periodicos = 0;
    novo_pesquisador->periodicos = NULL;
    return novo_pesquisador;
}

/* Cria uma nova struct para um periodico e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
void cria_periodico(pesquisador *autor, char *titulo, char *classe, char *ano)
{
    /* Abre espaco no vetor do pesquisador para uma nova insercao */
    autor->num_periodicos++;
    autor->periodicos = realloc(autor->periodicos, autor->num_periodicos * sizeof(periodicos));
    if (autor->periodicos == NULL) {
        printf("ERRO: Problema na realocação do vetor de periodicos\n");
        exit(1);
    }
    /* Preenche os dados e retorna */
    autor->periodicos[autor->num_periodicos - 1].titulo = titulo;
    autor->periodicos[autor->num_periodicos - 1].classe = classe;
    autor->periodicos[autor->num_periodicos - 1].ano = ano;
    return;
}

/* Limpa os dados alocados de todos os periodicos de um certo pesquisador
 * E em seguida limpa a propria struct deste pesquisador */
void limpa_pesquisador(pesquisador *pesquisador)
{
    for (int i = 0; i < pesquisador->num_periodicos; i++) {
        free(pesquisador->periodicos[i].titulo);
        free(pesquisador->periodicos[i].ano);
        free(pesquisador->periodicos[i].classe);
    }
    free(pesquisador->periodicos);
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

/* Funcao de distancia de edicao para comparar os periodicos do pesquisador
 * com os periodicos do arquivo de periodicos */
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

/* Compara o titulo do periodico do pesquisador com todos os titulos do arquivo
 * Retorna o periodico do arquivo que mais se assemelha ao do pesquisador */
void verifica_periodico(char **titulo, char **classe, char *str)
{
    int distancia = 0;
    int menor_distancia = 1000;
    char *titulo_mais_proximo = NULL;
    /* Cria uma copia da str */
    char *str_tmp = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(str_tmp, str);

    /* Procura o final de cada linha e compara com o titulo do pesquisador */
    char *linha = strtok(str_tmp, "\n");
    while (linha != NULL) {
        distancia = compara_str(linha, *titulo);
        if (distancia < menor_distancia) {
            menor_distancia = distancia;
            titulo_mais_proximo = linha;
        }
        linha = strtok(NULL, "\n");
    }
    printf("Verificando %s ->-> %s -> Dif: %d\n",*titulo ,titulo_mais_proximo, menor_distancia);
    /* Separa o titulo do arquivo de periodicos em titulo e classe */
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
    
    free(str_tmp);
    return;
}