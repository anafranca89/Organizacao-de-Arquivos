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
Dado o arquivo binário,  e o byteoffset do registro escolhido, retorna ou escreve as informções naquele registro*/
dados ler_regdados(char *arqbin, int byteoffset){
    dados registro;
    FILE *bin = ler_binario(arqbin);
    fseek(bin, byteoffset, SEEK_SET);
    fread(&registro.removido, sizeof(char), 1, bin);
    fread(&registro.proximo, sizeof(int), 1, bin);
    fread(&registro.codEstacao, sizeof(int), 1, bin);
    fread(&registro.codLinha, sizeof(int), 1, bin);
    fread(&registro.codProxEstacao, sizeof(int), 1, bin);
    fread(&registro.distProxEstacao, sizeof(int), 1, bin);
    fread(&registro.codLinhaIntegra, sizeof(int), 1, bin);
    fread(&registro.codEstIntegra, sizeof(int), 1, bin);
    fread(&registro.tamNomeEstacao, sizeof(int), 1, bin);
    fread(registro.nomeEstacao, sizeof(char), registro.tamNomeEstacao, bin);
    fread(&registro.tamNomeLinha, sizeof(int), 1, bin);
    fread(registro.nomeLinha, sizeof(char), registro.tamNomeLinha, bin);
    fclose(bin);

    return registro;
}

void escreve_regdados(char *arqbin, dados registro, int byteoffset){

    FILE *bin = escrever_binario(arqbin);
    fseek(bin, byteoffset, SEEK_SET);
    fwrite(&registro.removido, sizeof(char), 1, bin);
    fwrite(&registro.proximo, sizeof(int), 1, bin);
    fwrite(&registro.codEstacao, sizeof(int), 1, bin);
    fwrite(&registro.codLinha, sizeof(int), 1, bin);
    fwrite(&registro.codProxEstacao, sizeof(int), 1, bin);
    fwrite(&registro.distProxEstacao, sizeof(int), 1, bin);
    fwrite(&registro.codLinhaIntegra, sizeof(int), 1, bin);
    fwrite(&registro.codEstIntegra, sizeof(int), 1, bin);
    fwrite(&registro.tamNomeEstacao, sizeof(int), 1, bin);
    fwrite(registro.nomeEstacao, sizeof(char), registro.tamNomeEstacao, bin);
    fwrite(&registro.tamNomeLinha, sizeof(int), 1, bin);
    fwrite(registro.nomeLinha, sizeof(char), registro.tamNomeLinha, bin);
    fclose(bin);
}







