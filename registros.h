// PONTO_H SIMPLES, CASO QUISER COMENTÁRIOS DO QUE CADA FUNÇÃO FAZ OLHE NO .C
#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17
#define NEGATIVO -1
#include "fornecidas.h"



typedef struct reg_cabecalho{
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacoes;
}cabecalho;



typedef struct reg_dados{
    char removido; 
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao; 
    /*espaço de 50 - suficiente para o pior caso, onde apenas uma das strings ocupa os 
    80 - 37 = 43 bytes restantes */
    char nomeEstacao[200];
    int tamNomeLinha;
    char nomeLinha[200]; 
}dados;


// Estrutura auxiliar para atualização
typedef struct {
    int rrn;
    dados reg_dados;
} AtualizacaoPendente;



//construtores
cabecalho cria_cabecalho();
dados cria_dados();

FILE *ler_binario(char *arqbin);
FILE * cria_escreve_binario(char *arqbin);
FILE* escrever_binario(char *arqbin);
FILE* abrir_para_escrita_binário(char *arq);


void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);

void ler_regdados(FILE* ponteiro_arquivo, dados* reg_dados);
void escreve_regdados(FILE *ponteiro_arquivo, dados* reg_dados);
void atualizar_campos_registro(int p, char nomesAtualiza[][50], char valoresAtualiza[][200], dados *reg_dados);

int calculo_byteoffset_dados(int RRN);

void imprime_inteiro_ou_nulo(int valor);
void imprime_texto_ou_nulo(char *texto, int tamanho);
void imprime_registro_dados(dados* reg_dados);

#endif