#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcHash.h"

/*Estrutura escolhida para auxiliar na busca de string foi por tabela Hash. 
Se o tamnaho das strings é >0 insere na tabela hash.*/
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]) {
    
    if (bin == NULL) {
        return;
    }
    cabecalho reg_cab;
    dados reg_dados;
    int rrn;


    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);

    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        
        int byteoffset = calculo_byteoffset_dados(rrn);
        fseek(bin, byteoffset, SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1') {
            continue;
        }
        if (reg_dados.tamNomeLinha > 0) {
            inserir_hash(tabela, reg_dados.nomeLinha, reg_dados.tamNomeLinha);
        }
    }

    fseek(bin, 0, SEEK_SET);
}








// Formula para criação da chave hash 
int hash_string(char *str, int tam, int primo) {
    int soma = 0;

    for (int i = 0; i < tam; i++) {
        soma += (unsigned char) str[i];
    }

    return soma % primo;
}








void liberar_tabela(NoHash *tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash *atual = tabela[i];

        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->prox;
            free(temp);
        }

        tabela[i] = NULL;
    }
}






//Inicialização da tabela Hash
void inicializar_tabela(NoHash *tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        tabela[i] = NULL;
    }
}




void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha) {
    int pos = hash_string(nomeLinha, tamNomeLinha, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nomeLinha) == tamNomeLinha &&
            strncmp(atual->nomeLinha, nomeLinha, tamNomeLinha) == 0) {
            atual->repeticoes++;
            return;
        }
        atual = atual->prox;
    }

    NoHash *novo = (NoHash *) malloc(sizeof(NoHash));
    memcpy(novo->nomeLinha, nomeLinha, tamNomeLinha);
    novo->nomeLinha[tamNomeLinha] = '\0';
    novo->repeticoes = 1;
    novo->prox = tabela[pos];

    tabela[pos] = novo;
}



NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha) {
    int pos = hash_string(nomeLinha, tamNomeLinha, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nomeLinha) == tamNomeLinha &&
            strncmp(atual->nomeLinha, nomeLinha, tamNomeLinha) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}





