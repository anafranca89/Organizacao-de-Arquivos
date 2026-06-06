#ifndef HASH_H
#define HASH_H

#define TAM_TABELA 997  // número primo

typedef struct no_hash {
    char nomeLinha[100];
    int repeticoes;
    struct no_hash *prox;
} NoHash;





void inicializar_tabela(NoHash *tabela[]);
void liberar_tabela(NoHash *tabela[]);
void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);

