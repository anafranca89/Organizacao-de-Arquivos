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
    aux.status = 0;
    aux.topo =NEGATIVO;
    aux.proxRRN =0;
    aux.nroEstacoes =0;
    aux.nroParesEstacoes =0;
    return aux;
}


dados cria_dados(){
    dados aux;
    aux.removido = '0';
    aux.proximo = NEGATIVO;
    return aux;
}


int calculo_byteoffset_dados(int RRN){
    return RRN*TAM_REG +TAM_CABECALHO;
}

/*Lê os dados do cabeçalho e coloca nas variáveis dadas
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro
*/
void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cabecalho){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&reg_cabecalho->status, sizeof(char), 1, ponteiro_arquivo);
    fread(&reg_cabecalho->topo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_cabecalho->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_cabecalho->nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_cabecalho->nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
}

/* Dado o arquivo binário, atualiza os valores no cabeçalho
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cabecalho){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(&reg_cabecalho->status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&reg_cabecalho->topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_cabecalho->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_cabecalho->nroEstacoes, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_cabecalho->nroParesEstacoes, sizeof(int), 1, ponteiro_arquivo);
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
   
    reg_dados->nomeEstacao[reg_dados->tamNomeEstacao] = '\0';
    fread(&reg_dados->tamNomeLinha, sizeof(int), 1, ponteiro_arquivo);
    fread(reg_dados->nomeLinha, sizeof(char), reg_dados->tamNomeLinha, ponteiro_arquivo);
    reg_dados->nomeLinha[reg_dados->tamNomeLinha] = '\0';

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

    if (lixo_escrever > 0) {
        //Cria um buffer temporário de '$'
        char lixo[50];
        memset(lixo, '$', lixo_escrever);
        fwrite(lixo, sizeof(char), lixo_escrever, ponteiro_arquivo);
    }
}





/*Funções auxiliares para imprimir informações ao usuário.
-> Ajudam na modularização
-> Imprime os campos NULOs  com a formatação pedida.
*/
void imprime_inteiro_ou_nulo(int valor){
    if(valor == -1) printf("NULO");
    else printf("%d", valor);
    printf(" ");
}

void imprime_texto_ou_nulo(char *texto, int tamanho){
    if(tamanho == 0) printf("NULO");
    else printf("%.*s", tamanho, texto);
    printf(" ");
}

/*Imprimi o Registro de dados com a formatação pedida.
Util para imprimir como se fosse a função printf()*/


void imprime_registro_dados(dados * reg_dados) {
    
    imprime_inteiro_ou_nulo(reg_dados->codEstacao);
    imprime_texto_ou_nulo(reg_dados->nomeEstacao, reg_dados->tamNomeEstacao);
    imprime_inteiro_ou_nulo(reg_dados->codLinha);
    imprime_texto_ou_nulo(reg_dados->nomeLinha,reg_dados->tamNomeLinha);
    imprime_inteiro_ou_nulo(reg_dados->codProxEstacao);
    imprime_inteiro_ou_nulo(reg_dados->distProxEstacao);
    imprime_inteiro_ou_nulo(reg_dados->codLinhaIntegra);
    imprime_inteiro_ou_nulo(reg_dados->codEstIntegra);
    printf("\n");
}

