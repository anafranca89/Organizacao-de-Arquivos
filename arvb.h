#ifndef ARVOREB_H
#define ARVOREB_H
#include "registros.h"
#define TAM_CAB_IND 17
#define TAM_REG_IND 53
#define PROMOTION 1
#define NO_PROMOTION 0

//Funções que definem a arvore B, como busca, inserção e remoção
//Para isso precisamos do arquivo de indices e struct de indices.


typedef struct reg_cab_indice{
    char status;
    int noRaiz;
    int topo;
    int proxRRN;
    int nroNos;
}cab_indice;


typedef struct reg_indice{
    char removido;
    int proximo;
    int tipoNo; //raiz 0, intermediario 1, folha -1
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



int calculo_byteoffset_indice(int RRN);

int busca_arvore(FILE *arquivo, int rrn_no, int chave_unica, int *posicao_chave_no);

void cria_arvore(FILE* arq_dados, char*arq_index );


int insere_arvore(FILE* arq_index, cab_indice *cabecalho, int rrn_no, int chave, 
                  int byte_dados_chave, int *filho_promovido, int *chave_promovida, int *byte_dados_promovido);
                    
void cria_novoNo();
void insere_ordenado_no(indice *no, int chave, int filho_promovido,  int byte_ponteiro);
void split (FILE *arq_index, cab_indice *cab_ind, int rrn_no_ant, indice *no_ant, 
           int chave, int byte_chave, int filho_chave,
           int *chave_promovida, int *byte_promovido, int *filho_promovido );
#endif