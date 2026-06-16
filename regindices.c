#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "regindices.h"  



/*Construtor Base de Um índice--> Valores são criados com o valor  padrão
removido =0, proximo = -1, chaves e ponteiros -1, nrChaves =0*/
indice new_indice(){
    indice aux; 
    aux.removido = '0';
    aux.proximo= NEGATIVO;
    aux.tipoNo= NEGATIVO;
    aux.nroChaves= 0;
    aux.C1= NEGATIVO;
    aux.Pr1= NEGATIVO;
    aux.C2= NEGATIVO;
    aux.Pr2= NEGATIVO;
    aux.C3= NEGATIVO;
    aux.Pr3= NEGATIVO;
    aux.arv1= NEGATIVO; 
    aux.arv2= NEGATIVO; 
    aux.arv3= NEGATIVO; 
    aux.arv4= NEGATIVO; 
    return aux;
}

/*Função Auxiliar:
Retorna o byteoffset de um RRN do arquivo de indices da arvore B
*/
int calculo_byteoffset_indice(int RRN){
    return RRN*TAM_REG_IND +TAM_CAB_IND;
}


/* lê o cabeçalho no arquivo de indices e retorna p/ a struct de parametro
Ponteiro deve estar no começo do arquivo !
Parametros : arquivo binário para escrita e a struct para escrita do cabecalho
*/
void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fread(&cab->noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->topo, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->nroNos, sizeof(int), 1, ponteiro_arquivo);
}




/* Escreve o cabeçalho no arquivo de indice, com as informações da struct de parametro.
Parametros : arquivo inário para escrita e a struct em RAM do cabecalho
*/
void escreve_ind_cabecalho(FILE *ponteiro_arquivo, cab_indice* cab){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&cab->noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->nroNos, sizeof(int), 1, ponteiro_arquivo);
}

/* Lê a struct de indices no arquivo. 
Ponteiro deve estar no byteoffset correto!
Parametros: ponteiro p/arquivo de indice, ponteiro p/ struct onde serão salvas as informações
*/

void ler_indice(FILE* ponteiro_arquivo, indice* reg_indice ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fread(&reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}



/* Escreve a struct de indices no arquivo. 
Ponteiro deve estar no byteoffset correto!
Parametros: ponteiro p/arquivo de indice, a struct a ser salva.
*/
void escreve_indice(FILE *ponteiro_arquivo, indice* reg_indice){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(&reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}



/*Construtor de um Cabeçalho --> Arvore Vazia*/
cab_indice new_cab_indice(){
    cab_indice aux;
    aux.status ='0';
    aux.noRaiz =NEGATIVO;
    aux.topo =NEGATIVO;
    aux.proxRRN = 0;
    aux.nroNos =0;
    return aux;
}
