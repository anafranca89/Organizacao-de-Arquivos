#ifndef REGINDICES_H
#define REGINDICES_H
#include "funcoes.h"
#define TAM_CAB_IND 17
#define TAM_REG_IND 53

#define MIN_CHAVES_B 1
#define MAX_CHAVES_B 3

/*
Definição de estruturas para o arquivo de INDICES ARVORE B
- Registro de cabeçalho 
- registro de indice/ página/ nó da árvore
*/
typedef struct reg_cab_indice{
    char status; // 0 ou 1
    int noRaiz; // inicial -1
    int topo;   // inicia -1
    int proxRRN;   //pilha de remoção
    int nroNos;    //qtd de registros
}cab_indice;


typedef struct reg_indice{
    char removido; // 0 ou 1
    int proximo; // pilha de remoção
    int tipoNo; //raiz 0, intermediario 1, folha -1
    int nroChaves;  // 0, 1 , 2 ou 3
    int C1; // numero da chave única
    int Pr1; // byteoFfset no arq.de dados da chave
    int C2; 
    int Pr2;
    int C3;
    int Pr3;
    //arvx = Px -- é o ponteiro(RRN) para subarvore correspondente 
    int arv1; 
    int arv2; 
    int arv3; 
    int arv4; 
}indice;

/*---------------------
Construtores das structs
Retornam a struct com os valores padrão.
---------------------
*/
cab_indice  new_cab_indice();
indice new_indice();

void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab );
void escreve_ind_cabecalho(FILE *ponteiro_arquivo, cab_indice* cab);

void ler_indice(FILE* ponteiro_arquivo, indice* ind );
void escreve_indice(FILE *ponteiro_arquivo, indice* ind);



int calculo_byteoffset_indice(int RRN);

#endif