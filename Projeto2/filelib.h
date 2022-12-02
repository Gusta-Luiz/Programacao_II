/* Programa criado em 11/2022 por Luiz Gustavo Dalmaz Paquete*/

struct artigos_t {
    char *titulo;               /* Titulo da revista ou artigo */
    char *ano;                  /* Ano de publicacao */
    char *classe;               /* Classe de artigo, entre A1 a C */
} typedef artigos;

struct pesquisador_t {
    char *nome;                 /* Nome do pesquisador */
 /* char *grupo; */             /* Grupo de pesquisa do pesquisador NAO USEI*/
    int num_artigos;         /* Numero de artigos publicados */
    artigos *artigos;     /* Vetor de artigos */
} typedef pesquisador;

struct lista_qualis_t {
    char **strings;              /* Vetor de qualis */
    int num_total;            /* Numero de qualis */
} typedef lista_qualis;


/* Recebe a entrada do terminal e analisa os argumentos para que correspondam a chamada 
 * Retorna 1 em caso de sucesso ou 0 em caso de erro */
int testa_entrada(int argc, char *argv[], char **curriculo, char **conferencias, char **artigos);

/* Recebe o nome ou diretorio do arquivo e armazena seu conteudo na memoria
 * Retorna um ponteiro para este conteudo em caso de sucesso */
char *arq_leitura(char *nome_arquivo);

/* Cria uma nova struct para um pesquisador e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
pesquisador *cria_pesquisador(char *nome);

/* Cria uma nova struct para um artigo e preenche alguns dados 
 * Retorna a struct em caso de sucesso */
void cria_artigo(pesquisador *autor, char *titulo, char *classe, char *ano);

/* Limpa os dados alocados de todos os artigos de um certo pesquisador
 * E em seguida limpa a propria struct deste pesquisador */
void limpa_pesquisador(pesquisador *pesquisador);

/* Busca uma string dentro do arquivo (Chamar po & altera o valor de inicio)
 * Retorna a proxima str que estiver entre aspas duplas */
char *busca_str(char **lattes, char *str);

/* Recebe uma string qualquer e a retorna totalmente me maiusculo 
 * Funcao retirada do site https://www.codevscolor.com/c-convert-string-to-uppercase */
void maiusculo_str(char *Str);

/* Funcao de distancia de edicao para comparar os artigos do pesquisador
 * com os artigos do arquivo de artigos */
int compara_str(char *str1, char *str2);

/* Compara o titulo do artigo do pesquisador com todos os titulos do arquivo
 * Retorna o artigo do arquivo que mais se assemelha ao do pesquisador */
void verifica_artigo(char **titulo, char **classe, lista_qualis *lista);

/* Separa a string linha por linha, remove as repetidas, aloca uma struct de lista
 * E preenche esta lista com todas as linhas da string */
lista_qualis *separa_str(char *str);

/* Limpa os dados alocados da lista de artigos
 * E em seguida limpa a propria struct da lista */
void limpa_lista(lista_qualis *lista);