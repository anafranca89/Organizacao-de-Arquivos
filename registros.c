#include <stdio.h>
#include <string.h>
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


cabecalho cria_cabecalho(){
    cabecalho aux;
    aux.status = 0
    aux.topo =-1
    aux.proxRRN =0
    aux.nroEstacoes =
    aux.nroParesEstacoes =0;
    return aux;
}


dados cria_dados(){
    dados aux;
    aux.removido = 0
    aux.proximo = -1;
    return aux;
}


/*Lê os dados do cabeçalho e coloca nas variáveis dadas
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro
*/
void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cabecalho){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(reg_cabecalho->status, sizeof(char), 1, ponteiro_arquivo);
    fread(reg_cabecalho->topo, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cabecalho->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cabecalho->nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_cabecalho->nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
}

/* Dado o arquivo binário, atualiza os valores no cabeçalho
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cabecalho){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(reg_cabecalho->status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(reg_cabecalho->topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cabecalho->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cabecalho->nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_cabecalho->nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
}






/*Leitura e Escrita de Registros. 
Dado o arquivo binário, retorna as informções naquele registro
ATENÇÃO */
void ler_regdados(FILE* ponteiro_arquivo, dados* reg_dados){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&reg_dados->removido, sizeof(char), 1, ponteiro_arquivo);
    fread(&reg_dados->proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->codLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_dados->codEstIntegra, sizeof(int), 1, ponteiro_arquivo);


    fread(&reg_dados->tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_dados->nomeEstacao, sizeof(char), reg_dados->tamNomeEstacao, ponteiro_arquivo);
   

    fread(&reg_dados->tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_dados->nomeLinha, sizeof(char), reg_dados->tamNomeLinha, ponteiro_arquivo);
   

    int bytes_escritos = 37 + reg_dados->tamNomeEstacao + reg_dados->tamNomeLinha;
    int lixo_a_pular = TAM_REG - bytes_escritos;
    if (lixo_a_pular > 0) {
        fseek(ponteiro_arquivo, lixo_a_pular, SEEK_CUR);
    }
}







void escreve_regdados(FILE* ponteiro_arquivo, dados *reg_dados){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    //atualiza os tamanhos das strings
    reg_dados->tamNomeEstacao = strlen(reg_dados->nomeEstacao);
    reg_dados->tamNomeLinha = strlen(reg_dados->nomeLinha);

    fwrite(&reg_dados->removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&reg_dados->proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->codEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->codLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->codProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->distProxEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->codLinhaIntegra, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_dados->codEstIntegra, sizeof(int), 1, ponteiro_arquivo);



    fwrite(&reg_dados->tamNomeEstacao, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_dados->nomeEstacao, sizeof(char), reg_dados->tamNomeEstacao, ponteiro_arquivo);

    fwrite(&reg_dados->tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fwrite(reg_dados->nomeLinha, sizeof(char), reg_dados->tamNomeLinha, ponteiro_arquivo);

    int bytes_escritos = 37 + reg_dados->tamNomeEstacao + reg_dados->tamNomeLinha;
    int lixo_escrever = TAM_REG - bytes_escritos;

    if (lixo_a_escrever > 0) {
        //Cria um buffer temporário de '$'
        char lixo[50];
        memset(lixo, '$', lixo_a_escrever);
        fwrite(lixo, sizeof(char), lixo_a_escrever, ponteiro_arquivo);
    }
}
