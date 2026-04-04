#include <stdio.h>
#include "registros.h"

/*Funções para o ARQUIVO BINÁRIO DE SAÍDA. Retornam o ponteiro de arquivo  fopen!*/
FILE * ler_binario(char *arqbin){
    FILE *file = fopen(arqbin, "rb");
    if(file == NULL){
        return NULL;
    }
    return file;
}

FILE * escrever_binario(char *arqbin){
    FILE *file = fopen(arqbin, "r+b");
    if(file == NULL){
        return NULL;
    }
    return file;

}


/*Lê os dados do cabeçalho e coloca nas variáveis dadas
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro
*/
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

/* Dado o arquivo binário, atualiza os valores no cabeçalho
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/
void escreve_cabecalho(FILE* ponteiro_arquivo, char *status, int *topo, int *proxRRN, int *nroEstacoes, int *nroParesEstacoes){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fwrite(nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
}

/*Leitura e Escrita de Registros. 
Dado o arquivo binário, retorna as informções naquele registro
ATENÇÃO */
void ler_regdados(FILE* ponteiro_arquivo, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char **nomeEstacao, int *tamNomeLinha, char **nomeLinha){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(removido, sizeof(char), 1, ponteiro_arquivo);
    fread(proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(codLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fread(codEstIntegra, sizeof(int), 1, ponteiro_arquivo);
    fread(tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(*nomeEstacao, sizeof(char), *tamNomeEstacao, ponteiro_arquivo);
    fread(tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(*nomeLinha, sizeof(char), *tamNomeLinha, ponteiro_arquivo);
    int bytes_escritos = 37 + *tamNomeEstacao + *tamNomeLinha;
    char lixo = '$';
    while (bytes_escritos < 80) {
        fread(&lixo, sizeof(char), 1, ponteiro_arquivo);
        bytes_escritos++;
    }
}

void escreve_regdados(FILE* ponteiro_arquivo, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char **nomeEstacao, int *tamNomeLinha, char **nomeLinha){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codEstIntegra, sizeof(int), 1, ponteiro_arquivo);
    fwrite(tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(*nomeEstacao, sizeof(char), *tamNomeEstacao, ponteiro_arquivo);
    fwrite(tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(*nomeLinha, sizeof(char), *tamNomeLinha, ponteiro_arquivo);
    int bytes_escritos = 37 + *tamNomeEstacao + *tamNomeLinha;
    char lixo = '$';
    while (bytes_escritos < 80) {
        fwrite(&lixo, sizeof(char), 1, ponteiro_arquivo);
        bytes_escritos++;
    }
}







