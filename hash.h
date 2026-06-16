#ifndef HASH_H
#define HASH_H
#define DEBUG 0
#define TAM_TABELA 997  // número primo
#include "registros.h"


/*Estrutura HASH para lógica de contar Estações únicas rapidamente
*/
typedef struct no_hash {
    char nome[100];
    int repeticoes;
    struct no_hash *prox;
} NoHash;



void inicializar_tabela(NoHash *tabela[]);
void liberar_tabela(NoHash *tabela[]);
int hash_string(char *str, int tam, int primo);


void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]);
void decrementar_hash(NoHash *tabela[], char *nomeEstacao, int tamNomeEstacao);

NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);


#endif