#ifndef HASH_H
#define HASH_H
#define DEBUG 0
#define TAM_TABELA 997  // número primo

#include "registros.h"


// estruturas da lista encadeada e do hashing aberto
typedef struct ArquivoAberto {
    char nome[100];
    FILE *arquivo;
    struct ArquivoAberto *prox;
} ArquivoAberto;

typedef struct no_hash {
    char nome[100];
    int repeticoes;
    struct no_hash *prox;
} NoHash;



ArquivoAberto* buscar_arquivo(ArquivoAberto *lista, char *nome_bin);
int arquivo_ja_processado(ArquivoAberto *lista, char *nome_bin);
void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin, FILE *arquivo);
void liberar_lista_arquivos(ArquivoAberto *lista);


void inicializar_tabela(NoHash *tabela[]);
void liberar_tabela(NoHash *tabela[]);
int hash_string(char *str, int tam, int primo);


void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]);
void decrementar_hash(NoHash *tabela[], char *nomeEstacao, int tamNomeEstacao);

NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
FILE *obter_arquivo_binario(ArquivoAberto **lista_arquivos,
                            char *nome_bin,
                            NoHash *tabela[]);

#endif