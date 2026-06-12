#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registros.h"

/*
Retornam o ponteiro de arquivo  fopen!
    Apenas lê ->rb
*/
FILE * ler_binario(char *arqbin){
    FILE *file = fopen(arqbin, "rb");
    if(file == NULL){
        return NULL;
    }
    return file;
}

/* Retorna o ponteiro FILE.
    Cria do zero no modo escrita. ->wb+
    Nao use se o arquivo existir!
*/
FILE * cria_escreve_binario(char *arqbin){
    FILE *file = fopen(arqbin, "wb+");
    if(file == NULL){
        return NULL;
    }
    return file;
}

/*Abre no modo escrita binário.
modo -> rb+
Apenas se o arquivo já existe!.
*/
FILE * escrever_binario(char *arqbin){
    FILE *file = fopen(arqbin, "rb+");
    if(file == NULL){
        return NULL;
    }
    return file;

}


/*Função que une abrir p/ escrita.
Se o arquivo existe retorna ele.
Senao, cria e retorna, ponteiro posicionado no inicio.
*/
FILE* abrir_para_escrita_binário(char *nome_arquivo) {
    if (nome_arquivo == NULL) return NULL;

    FILE *arq = escrever_binario(nome_arquivo);
    if (arq == NULL) {
        arq = cria_escreve_binario(nome_arquivo);
        /* cabecalho cab = cria_cabecalho();
        escreve_cabecalho(arq,&cab); */
    }

    return arq;
}



/* Construtor do Reg.Cabeçalho de Dados.
    Retorna a struct cabeçalho com os valores padrão de  NULOS.
*/
cabecalho cria_cabecalho(){
    cabecalho aux;
    aux.status = 0;
    aux.topo =NEGATIVO;
    aux.proxRRN =0;
    aux.nroEstacoes =0;
    aux.nroParesEstacoes =0;
    return aux;
}

/*  
    Construtor do Reg.Dados de Dados.
    Retorna a struct dados com os valores padrão p/Nulo.
*/
dados cria_dados(){
    dados aux;
    aux.removido = '0';
    aux.proximo = NEGATIVO;
    aux.nomeEstacao[0] = '\0';
    aux.nomeLinha[0] = '\0';
    aux.tamNomeEstacao =0;
    aux.tamNomeLinha =0;
    return aux;
}

/*  
    Função auxiliar p/ modulsrizar o
    calculo do byteoffset do registro de dados.
    Retorna o int dele.
*/
int calculo_byteoffset_dados(int RRN){
    return RRN*TAM_REG +TAM_CABECALHO;
}



/*Ponteiro do fseek no COMEÇO do arquivo!
Lê os dados do cabeçalho e coloca na struct.
Parametros: arquivo de dados, ponteiro p/ a struct p/ reescrever.
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


/*Ponteiro do fseek no COMEÇO do arquivo!
Escreve os dados do cabeçalho que foi dado na struct de parametro.
Parametros: arquivo de dados, ponteiro p/ a struct p/salvar.
*/
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






/*
Ponteiro do fseek no byteoffset do RRN!
Lê os dados reg.dados e passa para struct de parametro.
Parametros: arquivo de dados, ponteiro p/ a struct p/salvar.
*/
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




/*
Ponteiro do fseek no byteoffset do RRN!
Escreve os dados  da struct de parametro.
Parametros: arquivo de dados, ponteiro p/ a struct.
*/
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






void atualizar_campos_registro(int p, char nomesAtualiza[][50], char valoresAtualiza[][200], dados *reg_dados) {
    
    for (int i = 0; i < p; i++) {
        
        if (strcmp(nomesAtualiza[i], "codEstacao") == 0) {
            reg_dados->codEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codLinha") == 0) {
            reg_dados->codLinha = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codProxEstacao") == 0) {
            reg_dados->codProxEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "distProxEstacao") == 0) {
            reg_dados->distProxEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codLinhaIntegra") == 0) {
            reg_dados->codLinhaIntegra = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codEstIntegra") == 0) {
            reg_dados->codEstIntegra = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        
        else if (strcmp(nomesAtualiza[i], "nomeEstacao") == 0) {
            if (strlen(valoresAtualiza[i]) == 0) {
                reg_dados->tamNomeEstacao = 0;
                reg_dados->nomeEstacao[0] = '\0';
            } else {
                reg_dados->tamNomeEstacao = strlen(valoresAtualiza[i]);
                memcpy(reg_dados->nomeEstacao, valoresAtualiza[i], reg_dados->tamNomeEstacao);
                reg_dados->nomeEstacao[reg_dados->tamNomeEstacao] = '\0';
            }
        }
        else if (strcmp(nomesAtualiza[i], "nomeLinha") == 0) {
            if (strlen(valoresAtualiza[i]) == 0) {
                reg_dados->tamNomeLinha = 0;
                reg_dados->nomeLinha[0] = '\0';
            } else {
                reg_dados->tamNomeLinha = strlen(valoresAtualiza[i]);
                memcpy(reg_dados->nomeLinha, valoresAtualiza[i], reg_dados->tamNomeLinha);
                reg_dados->nomeLinha[reg_dados->tamNomeLinha] = '\0';
            }
        }
    }
}













/*Funções auxiliares para imprimir informações ao usuário.
-> Ajudam na modularização
-> Imprime os campos NULOs de int  com a formatação pedida.
*/
void imprime_inteiro_ou_nulo(int valor){
    if(valor == -1) printf("NULO");
    else printf("%d", valor);
    printf(" ");
}

/*
-> Imprime os camposde strings NULOs  com a formatação pedida.
*/
void imprime_texto_ou_nulo(char *texto, int tamanho){
    if(tamanho == 0) printf("NULO");
    else printf("%.*s", tamanho, texto);
    printf(" ");
}


/*Imprimi o Registro de dados com a formatação pedida.
Util para imprimir como se fosse a função printf()
Parametros: Ponteiro p/struct de dados
*/
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

