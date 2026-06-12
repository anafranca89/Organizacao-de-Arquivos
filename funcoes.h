#ifndef FUNCOES_H
#define FUNCOES_H

<<<<<<< HEAD
#define MAX_LINHA_CSV 512
#define TAM_REG 80
#define TAM_CABECALHO 17
#define TAM_TABELA 997  // número primo

typedef struct ArquivoAberto {
    char nome[100];
    struct ArquivoAberto *prox;
} ArquivoAberto;


typedef struct no_hash {
    char nomeLinha[100];
    int repeticoes;
    struct no_hash *prox;
} NoHash;

void inicializar_tabela(NoHash *tabela[]);
void liberar_tabela(NoHash *tabela[]);
void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);
NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha);


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
int arquivo_ja_processado(ArquivoAberto *lista, char *nome_bin);
void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin);
void liberar_lista_arquivos(ArquivoAberto *lista);
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]);
=======
#include "hash.h"
#include "csv.h"




void mostrar_binario_sequencial(FILE *bin);


int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200], dados* reg_dados);
void buscar_registros(FILE *bin, NoHash *tabela[], int m,char nomesCampos[][50], char valoresCampos[][200]);

void ler_par_campo_valor(char *campo, char *valor);


void remover_registros_dinamico(FILE *bin, NoHash *tabela[], cabecalho* reg, int  m,
                                char nomesCampos[][50], char valoresCampos[][200], char ultimo);
void inserir_registro_dinamico(FILE *bin, NoHash *tabela[], cabecalho *cab, dados *reg_dados); 
void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[], int m, char nomesBusca[][50], char valoresBusca[][200],
                                    int p, char nomesAtualiza[][50], char valoresAtualiza[][200]);


void ler_e_inserir_registro(FILE *arq_dados, NoHash *tabela[], cabecalho *cab);



>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b

#endif

