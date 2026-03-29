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


/*Dado um arquivo binário, a função precisa ler o registro de cabeçalho */
cabecalho ler_cabecalho(char * arqbin){
    cabecalho regcab;
    FILE *bin = ler_binario(arqbin);
    fread(&regcab.status, sizeof(char), 1, bin);
    fread(&regcab.topo, sizeof(int), 1, bin);
    fread(&regcab.proxRRN, sizeof(int), 1, bin);
    fread(&regcab.nroEstacoes, sizeof(int), 1, bin);
    fread(&regcab.nroParesEstacoes, sizeof(int), 1, bin);
    fclose(bin);
    return regcab;
}

/* Dado o arquivo binário, precisa atualizar os valores no cabeçalho*/
void escreve_cabecalho(char *arqbin, cabecalho regcab){
    FILE *bin = escrever_binario(arqbin);
    fseek(bin, 0, SEEK_SET);
    fwrite(&regcab.status, sizeof(char), 1, bin);
    fwrite(&regcab.topo, sizeof(int), 1, bin);
    fwrite(&regcab.proxRRN, sizeof(int), 1, bin);
    fwrite(&regcab.nroEstacoes, sizeof(int), 1, bin);
    fwrite(&regcab.nroParesEstacoes, sizeof(int), 1, bin);
    fclose(bin);
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







