// PONTO_H SIMPLES, CASO QUISER COMENTÁRIOS DO QUE CADA FUNÇÃO FAZ OLHE NO .C
#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17


typedef struct reg_cabecalho{
    char status;
    int noRaiz;
    int topo;
    int proxRRN;
    int nroNos;
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

    char nomeEstacao;// tamvariavel

    int tamNomeLinha;

    char nomeLinha; // tamanho variavel
}dados;


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
    int arv2; 
    int arv1; 
    int arv3; 
    int arv4; 
}indice;

FILE *ler_binario(char *arqbin);
FILE* escrever_binario(char *arqbin);

void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);

void ler_regdados(FILE* ponteiro_arquivo, dados* reg_dados);
void escreve_regdados(FILE *ponteiro_arquivo, dados* reg_dados);


void ler_indice(FILE* ponteiro_arquivo, indice* reg_indice );
void escreve_indice(FILE *ponteiro_arquivo, indice* reg_indice);

#endif