#ifndef FUNCOES_H
#define FUNCOES_H

#define DEBUG 0
#define MAX_LINHA_CSV 512
#define TAM_REG 80
#define TAM_CABECALHO 17

#include "estruturas.h"
#define TAM_TABELA 997  

// .h normal, veja .c para comentários mais detalhados e implementações
void ler_par_campo_valor(char *campo, char *valor);

int verificar_criterios(int m, char nomesCampos[][50],
     char valoresCampos[][200], int codEstacao,
      int tamNomeEstacao, char *nomeEstacao, int codLinha, int tamNomeLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstIntegra);

void atualizar_campos_registro(int p, char nomesAtualiza[][50],
     char valoresAtualiza[][200], int *codEstacao, int *tamNomeEstacao,
      char *nomeEstacao, int *codLinha, int *tamNomeLinha, char *nomeLinha, int *codProxEstacao, int *distProxEstacao, int *codLinhaIntegra, int *codEstIntegra);

int ler_registro_csv(FILE *csv,
                     int *codEstacao,
                     char *nomeEstacao,
                     int *tamNomeEstacao,
                     int *codLinha,
                     char *nomeLinha,
                     int *tamNomeLinha,
                     int *codProxEstacao,
                     int *distProxEstacao,
                     int *codLinhaIntegra,
                     int *codEstIntegra);
                     
void adicionar_csv_no_binario(FILE *csv, FILE *bin, NoHash *tabela[]);

void imprime_inteiro_ou_nulo(int valor);
void imprime_texto_ou_nulo(char *texto, int tamanho);
void mostrar_binario_sequencial(FILE *bin);
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                                char nomesCampos[][50],
                                char valoresCampos[][200]);

void remover_registros_dinamico(FILE *bin,
                                NoHash *tabela[],
                                int m,
                                char nomesCampos[][50],
                                char valoresCampos[][200],
                                int *topo, char ultimo);

void inserir_registro_dinamico(FILE *bin, NoHash *tabela[],
                               int codEstacao, int codLinha, int codProxEstacao, int distProxEstacao,
                               int codLinhaIntegra, int codEstIntegra,
                               int tamNomeEstacao, char *nomeEstacao,
                               int tamNomeLinha, char *nomeLinha);

void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[],
                                  int m, char nomesBusca[][50], char valoresBusca[][200],
                                  int p, char nomesAtualiza[][50], char valoresAtualiza[][200], char status);

void printar_registro_inteiro(int codEstacao, int tamNomeEstacao, char *nomeEstacao, int codLinha, int tamNomeLinha,
     char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstIntegra);                                  
void ler_e_inserir_registro(FILE *bin, NoHash *tabela[]);              

#endif
