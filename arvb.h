#ifndef ARVOREB_H
#define ARVOREB_H
#include "registros.h"
#define NEGATIVO -1
#define TAM_CAB_IND 17
#define TAM_REG_IND 53
//Funções que definem a arvore B, como busca, inserção e remoção
//Para isso precisamos do arquivo de indices e struct de indices.


typedef struct reg_cabecalho{
    char status;
    int noRaiz;
    int topo;
    int proxRRN;
    int nroNos;
}cab_indice;


typedef struct reg_indice{
    char removido;
    int proximo;
    int tipoNo;
    int nroChaves;
    int C1;
    int Pr1;
    int C2;
    int Pr2;
    int C3;
    int Pr3;
    //arvx = Px -- é o ponteiro(byteoffset) para subarvore correspondente 
    int arv1; 
    int arv2; 
    int arv3; 
    int arv4; 
}indice;

indice new_indice();
void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab );
void escreve_ind_cabecalho(FILE *ponteiro_arquivo, cab_indice* cab);

void ler_indice(FILE* ponteiro_arquivo, indice* ind );
void escreve_indice(FILE *ponteiro_arquivo, indice* ind);





void busca_chave(FILE *arquivo, int rrn_no, int chave_unica, int *byte_found);

void cria_arvore(FILE* arq_dados, char*arq_index ,);


void insere_arvore(FILE* arq_dados,
                     int rrn_no , int chave,    
                     int filho_promovido, int chave_promovida,
                    int PROMOTION);
                    
void cria_novoNo();

#endif