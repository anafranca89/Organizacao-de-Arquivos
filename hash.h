#ifndef HASH_H
#define HASH_H
#define DEBUG 0
#define TAM_TABELA 997  // número primo
#include "registros.h"


/*---------------------------
Estrutura HASH para lógica de contar Estações únicas rapidamente

---------------------------
*/
typedef struct no_hash {
    char nome[100];
    int repeticoes;
    struct no_hash *prox;
} NoHash;


/*---------------------------
Construtor  e desalocação de memória da lista encadeada p/Hash
---------------------------
*/
    /* fórmula p/ criar chave única do nome estação baseado no tamanho 
    da string e a propria string.  */
int hash_string(char *str, int tam);
    //construtor da tabela - todos os ponteiros como NULL
void inicializar_tabela(NoHash *tabela[]);
    //free em cada nó da lista encadeada.
void liberar_tabela(NoHash *tabela[]);


/*---------------------------
Funções Base Hash:
    ->inserir: cria um novo nó no bucket[posicao dado por hash_string]
                e aumenta o numero de repetições daquela string
    -> Decrementa: remove a string do bucket[posicao] ou apenas diminue o numero de repetições
    -> Busca: Dado uma string, procura no bucket[posicao], retorna o ponteiro do no, ou NULL se não existe
    -> carregar_nomes: dado um arquivo binário de registro de dados, usa insere p/ armazenar as Estações únicas.
*/
        /*  Ponteiro do fseek no COMEÇO do arquivo antes de chamar a função!
        precisa dos ponteiros arqivo binario de dados ABERTO em leitura e tabela HASH inicializada*/
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]);
    //precisa da tabela HASH inicializada, e o tamanho da string e a propra string a inserir
void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
    //precisa da tabela HASH inicializada, e o tamanho da string e a propra string a remover
void decrementar_hash(NoHash *tabela[], char *nomeEstacao, int tamNomeEstacao);
    //precisa da tabela HASH inicializada, e o tamanho da string e a propra string a buscar
NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);


#endif