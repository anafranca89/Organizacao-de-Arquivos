#include <stdio.h>
#include "arvb.h"  


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



