/* Programa criado em 11/2022 por Luiz Gustavo Dalmaz Paquete*/

struct periodicos_t {
    char *titulo;               /* Titulo da revista ou periodico */
    char *ano;                  /* Ano de publicacao */
    char *classe;               /* Classe de periodico, entre A1 a C */
} typedef periodicos;

struct pesquisador_t {
    char *nome;                 /* Nome do pesquisador */
 /* char *grupo; */             /* Grupo de pesquisa do pesquisador NAO USEI*/
    int num_periodicos;         /* Numero de periodicos publicados */
    periodicos *periodicos;     /* Vetor de periodicos */
} typedef pesquisador;


/* Recebe a entrada do terminal e analisa os argumentos para que correspondam a chamada 
 * Retorna 1 em caso de sucesso ou 0 em caso de erro */
int testa_entrada(int argc, char *argv[], char **curriculo, char **periodicos);

/* Recebe o nome ou diretorio do arquivo e armazena seu conteudo na memoria
 * Retorna um ponteiro para este conteudo em caso de sucesso */
char *arq_leitura(char *nome_arquivo);

/* Cria uma nova struct para um pesquisador e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
pesquisador *cria_pesquisador(char *nome);

/* Cria uma nova struct para um periodico e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
void cria_periodico(pesquisador *autor, char *titulo, char *classe, char *ano);

/* Limpa os dados alocados de todos os periodicos de um certo pesquisador
 * E em seguida limpa a propria struct deste pesquisador */
void limpa_pesquisador(pesquisador *pesquisador);

/* Busca uma string dentro do arquivo (Chamar po & altera o valor de inicio)
 * Retorna a proxima str que estiver entre aspas duplas */
char *busca_str(char **lattes, char *str);

/* Recebe uma string qualquer e a retorna totalmente me maiusculo 
 * Funcao retirada do site https://www.codevscolor.com/c-convert-string-to-uppercase */
void maiusculo_str(char *Str);

/* Funcao de distancia de edicao para comparar os periodicos do pesquisador
 * com os periodicos do arquivo de periodicos */
int compara_str(char *str1, char *str2);

/* Compara o titulo do periodico do pesquisador com todos os titulos do arquivo
 * Retorna o periodico do arquivo que mais se assemelha ao do pesquisador */
void verifica_periodico(char **titulo, char **classe, char *str);

/* Armazena cada linha de uma string em uma posicao de um vetor de strings
 * Retorna o vetor de strings e o numero de linhas */
char **separa_str(char *str, int *linhas);