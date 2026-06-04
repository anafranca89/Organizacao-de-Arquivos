#include <stdio.h>

#include "arvb.h"  


/*Parametros : arquivo inário para escrita e a struct em RAM do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Se não, salva campo a campo.
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/

void escreve_ind_cabecalho(FILE *ponteiro_arquivo, cab_indice* cab){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(cab.status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(cab.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fwrite(cab.topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(cab.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(cab.nroNos, sizeof(int), 1, ponteiro_arquivo);
}


/*Parametros : arquivo binário para escrita e a struct para escrita do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Senão,  salva campo a campo na struct fornecida.
*/
void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(cab.status, sizeof(char), 1, ponteiro_arquivo);
    fread(cab.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(cab.topo, sizeof(int), 1, ponteiro_arquivo);
    fread(cab.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(cab.nroNos, sizeof(int), 1, ponteiro_arquivo);
}




void ler_indice(FILE* ponteiro_arquivo, indice* reg_indice ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(reg_indice.removido, sizeof(char), 1, ponteiro_arquivo);
    fread(reg_indice.proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.C1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.Pr1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.C2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.Pr2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.C3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.Pr3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.arv1, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.arv2, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.arv3, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_indice.arv4, sizeof(int), 1, ponteiro_arquivo);
}


void escreve_indice(FILE *ponteiro_arquivo, indice* reg_indice){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(reg_indice.removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(reg_indice.proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.C1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.Pr1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.C2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.Pr2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.C3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.Pr3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.arv1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.arv2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.arv3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_indice.arv4, sizeof(int), 1, ponteiro_arquivo);
}




/*Assumindo q existe os registro de índice.
Busca uma chave única int - codEstacao-  num nó da árvore.
Caso 1: Arvore com elementos --> raiz >=0
Caso 2: Arvore vazia --> raiz == -1
*/
void busca_chave(FILE *arquivo, int rrn_no, int chave_unica, int *byte_chave_buscada){
    if(rrn_no==NEGATIVO){
        byte_chave_buscada = NEGATIVO;
        return;
    } 
    indice aux;
    int bytecorrente = rrn_no*TAM_REG_IND + TAM_CAB_IND;
    //posiciona no lugar de leitura dos dados
    fseek(arquivo, bytecorrente, SEEK_SET);
    ler_indice(arquivo, &aux);
    if(aux.removido == 1) return;

    // no não removido 

    if(chave_unica< aux.C1){
        //busca na subarvore 1 a esquerda.
        busca_chave(aux.arv1, chave_unica, byte_chave_buscada);
    }else if(chave_unica== aux.C1){
        //achou a chave
        byte_chave_buscada = aux.Pr1;
        return;
    }else if(chave_unica< aux.C2){
        busca_chave(aux.arv2, chave_unica, byte_chave_buscada);
        
    }else if(chave_unica == aux.C2 ){
        byte_chave_buscada = aux.Pr2;
        return;
        
    }else if(chave_unica < aux.C3){
        busca_chave(aux.arv3, chave_unica, byte_chave_buscada);
    }
    else if(chave_unica==aux.C3){
        byte_chave_buscada = aux.Pr3;
        return;
    }else{
        busca_chave(aux.arv4, chave_unica, byte_chave_buscada);
    }
      
}



