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
    fwrite(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&cab->noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->nroNos, sizeof(int), 1, ponteiro_arquivo);
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
    fread(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fread(&cab->.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.topo, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.nroNos, sizeof(int), 1, ponteiro_arquivo);
}




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


/*Construtor Base de Um índice--> Valores são criados com o valor  padrão*/
indice new_indice(){
    indice aux; 
    aux.removido = 0;
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








/*Assumindo q existe os registro de índice.
Busca uma chave única int - codEstacao-  num nó da árvore.
Caso 1: Arvore com elementos --> raiz >=0
Caso 2: Arvore vazia --> raiz == -1

    -Chama a função no no raiz
    - o No está cheio?  nrochaves == 3
        -percorre as chaves do no, a chave buscada está lá?
            -byte_found recebe o ponteiro para o reg_dados
        - Senão, busca(subarvore correspondente) --> recursao
    - O no tem espaço ? -- nrochaves<3
        - 

*/
void busca_chave(FILE *arquivo, int rrn_no, int chave_unica, int *byte_found){
    if(rrn_no==NEGATIVO){
        *byte_found = NEGATIVO;
        return;
    } 
    indice aux; //colocar p/fora da função
    int bytecorrente = rrn_no*TAM_REG_IND + TAM_CAB_IND;
    //byte_found = bytecorrente;
    //posiciona no lugar de leitura dos dados
    fseek(arquivo, bytecorrente, SEEK_SET);
    ler_indice(arquivo, &aux);
    if(aux.removido == '1'){
        *byte_found = NEGATIVO;
        return;
    }
    // no não removido 
    // adicionar a leitura e analise do nro de chaves!!!!!!!!!1
    if(chave_unica< aux.C1){
        //busca na subarvore 1 a esquerda.
        busca_chave(aux.arv1, chave_unica, byte_found);
    }else if(chave_unica== aux.C1){
        //achou a chave
        byte_found = aux.Pr1;
        return;
    }else if(chave_unica< aux.C2){
        busca_chave(aux.arv2, chave_unica, byte_found);
        
    }else if(chave_unica == aux.C2 ){
        byte_found = aux.Pr2;
        return;
        
    }else if(chave_unica < aux.C3){
        busca_chave(aux.arv3, chave_unica, byte_found);
    }
    else if(chave_unica==aux.C3){
        byte_found = aux.Pr3;
        return;
    }else{
        busca_chave(aux.arv4, chave_unica, byte_found);
    }
      
}


/*Algoritmo Driver: Cria a lista de nós da árvore.
Caso1: Arquivo/arvore n existe--> cria um arquivo p/leitura e escrita
Caso2: Arquivo existe --> apenas abre arquivo p/escrita 
    - Enquanto existe registros --> count é menor que proxRRN
        - verifica se n é logicamente removido, senão vai pro proximo registro
        - removido == 0, então tenta inserir na raiz
        - segue a função de inserção

*/
void cria_arvore(FILE* arq_dados, char*arq_index ,){

    FILE* indexes = escrever_binario( arq_index);
    cab_indice index_cab = new_cab_indice();
    if(indexes == NULL){
        indexes = cria_escreve_binario(arq_index);
        escreve_ind_cabecalho(indexes, &index_cab);
    }
    ler_ind_cabecalho(indexes, &index_cab);


    //ler o arquivo de dados
    cabecalho reg_cab_dados = cria_cabecalho();
    ler_cabecalho(arq_dados, reg_cab_dados);
    if(reg_cab_dados.status == 0){
        //status inconsistente ? --> Erro 
        printf("Falha no processamento do arquivo.");
        return;
    }
    fseek(arq_dados, TAM_CABECALHO,SEEK_SET);

    dados reg_dados = cria_dados();
    //leitura dos registros e inserção na arvore
    for(int i=0; i<reg_cab_dados.proxRRN; i++){
        // é removido?
        ler_regdados(arq_dados, &reg_dados);
        if (reg_dados.removido =='1') continue; // o ponteiro do fseek já está no começo do registro i+1

        int PROMOTION =0, filho_promovido, chave_promovida;

        insere_arvore(arq_dados, index_cab.noRaiz,  reg_dados.codEstacao,     
                     filho_promovido,  chave_promovida,
                     PROMOTION);
            if (PROMOTION == 1){
                cria_novoNo();
            }
    }


}