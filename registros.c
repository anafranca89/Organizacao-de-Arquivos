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
void ler_regdados(FILE* ponteiro_arquivo, int byteoffset, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char **nomeEstacao, int *tamNomeLinha, char **nomeLinha){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&registro.removido, sizeof(char), 1, ponteiro_arquivo);
    fread(&registro.proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.codLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.codEstIntegra, sizeof(int), 1, ponteiro_arquivo);
    fread(&registro.tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(registro.nomeEstacao, sizeof(char), registro.tamNomeEstacao, ponteiro_arquivo);
    fread(&registro.tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(registro.nomeLinha, sizeof(char), registro.tamNomeLinha, ponteiro_arquivo);
}

void escreve_regdados(FILE* ponteiro_arquivo, int byteoffset, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char **nomeEstacao, int *tamNomeLinha, char **nomeLinha){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fseek(bin, byteoffset, SEEK_SET);
    fwrite(removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fwrite(codEstIntegra, sizeof(int), 1, ponteiro_arquivo);
    fwrite(tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(nomeEstacao, sizeof(char), registro.tamNomeEstacao, ponteiro_arquivo);
    fwrite(tamNomeLinha, sizeof(int), 1, ponterio_arquivo);
    fwrite(nomeLinha, sizeof(char), registro.tamNomeLinha, ponteiro_arquivo);
}







