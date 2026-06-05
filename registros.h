// PONTO_H SIMPLES, CASO QUISER COMENTÁRIOS DO QUE CADA FUNÇÃO FAZ OLHE NO .C
#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17

void ler_cabecalho(FILE* ponteiro_arquivo, char *status, int *topo, int *proxRRN, int *nroEstacoes, int *nroParesEstacoes){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(status, sizeof(char), 1, ponteiro_arquivo);
    fread(topo, sizeof(int), 1, ponteiro_arquivo);
    fread(proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fread(nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
}
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
    char nomeEstacao[50];
    int tamNomeLinha;
    char nomeLinha[50]; 
}dados;


//construtores
cabecalho cria_cabecalho();
dados cria_dados();

FILE *ler_binario(char *arqbin);
FILE * cria_escreve_binario(char *arqbin);
FILE* escrever_binario(char *arqbin);


void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);

void ler_regdados(FILE* ponteiro_arquivo, dados* reg_dados);
void escreve_regdados(FILE *ponteiro_arquivo, dados* reg_dados);

int calculo_byteoffset_dados(int RRN);


#endif