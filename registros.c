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
FILE * cria_escreve_binario(char *arqbin){
    FILE *file = fopen(arqbin, "wb+");
    if(file == NULL){
        return NULL;
    }
    return file;
}
FILE * escrever_binario(char *arqbin){
    FILE *file = fopen(arqbin, "rb+");
    if(file == NULL){
        return NULL;
    }
    return file;

}



/*Parametros : arquivo inário para escrita e a struct em RAM do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Se não, salva campo a campo.
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/
void escreve_cabecalho(FILE* ponteiro_arquivo,cabecalho *reg_cab){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(reg_cab.status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(reg_cab.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cab.topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cab.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cab.nroNos, sizeof(int), 1, ponteiro_arquivo);
}

/*Parametros : arquivo binário para escrita e a struct para escrita do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Senão,  salva campo a campo na struct fornecida.
*/
void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(reg_cab.status, sizeof(char), 1, ponteiro_arquivo);
    fread(reg_cab.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cab.topo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cab.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cab.nroNos, sizeof(int), 1, ponteiro_arquivo);
}


void ler_indice(FILE* ponteiro_arquivo, indice* reg_indice ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fread(reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}
void escreve_indice(FILE *ponteiro_arquivo, indice* reg_indice){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}




/*Leitura e Escrita de Registros. 
Dado o arquivo binário, retorna as informções naquele registro
ATENÇÃO */
void ler_regdados(FILE* ponteiro_arquivo, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char *nomeEstacao, int *tamNomeLinha, char *nomeLinha){
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
    fread(nomeEstacao, sizeof(char), *tamNomeEstacao, ponteiro_arquivo);
    fread(tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(nomeLinha, sizeof(char), *tamNomeLinha, ponteiro_arquivo);
    int bytes_escritos = 37 + *tamNomeEstacao + *tamNomeLinha;
    char lixo = '$';
    while (bytes_escritos < 80) {
        fread(&lixo, sizeof(char), 1, ponteiro_arquivo);
        bytes_escritos++;
    }
}

void escreve_regdados(FILE* ponteiro_arquivo, char *removido,int *proximo, int *codEstacao, int *codLinha, int *codProxEstacao,int *distProxEstacao, 
int *codLinhaIntegra, int *codEstIntegra, int *tamNomeEstacao, char *nomeEstacao, int *tamNomeLinha, char *nomeLinha){
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
    fwrite(nomeEstacao, sizeof(char), *tamNomeEstacao, ponteiro_arquivo);
    fwrite(tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(nomeLinha, sizeof(char), *tamNomeLinha, ponteiro_arquivo);
    int bytes_escritos = 37 + *tamNomeEstacao + *tamNomeLinha;
    char lixo = '$';
    while (bytes_escritos < 80) {
        fwrite(&lixo, sizeof(char), 1, ponteiro_arquivo);
        bytes_escritos++;
    }
}
