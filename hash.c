#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

ArquivoAberto* buscar_arquivo(ArquivoAberto *lista, char *nome_bin) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome_bin) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}


int arquivo_ja_processado(ArquivoAberto *lista, char *nome_bin) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome_bin) == 0) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin, FILE *arquivo) {
    ArquivoAberto *novo = (ArquivoAberto *) malloc(sizeof(ArquivoAberto));
    strcpy(novo->nome, nome_bin);
    novo->arquivo = arquivo;
    novo->prox = *lista;
    *lista = novo;
}

void liberar_lista_arquivos(ArquivoAberto *lista) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        ArquivoAberto *temp = atual;
        atual = atual->prox;

        if (temp->arquivo != NULL) {
            fclose(temp->arquivo);
        }

        free(temp);
    }
}



// Formula para criação da chave hash 
int hash_string(char *str, int tam, int primo) {
    int soma = 0;

    for (int i = 0; i < tam; i++) {
        soma += (unsigned char) str[i];
    }

    return soma % primo;
}





/*Estrutura escolhida para auxiliar na busca de string unicas foi tabela Hash.

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
        reg_dados = cria_dados();
        fseek(bin, calculo_byteoffset_dados(rrn), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1') {
            continue;
        }
        if (reg_dados.tamNomeEstacao > 0) {
            inserir_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
        }
    }

    fseek(bin, 0, SEEK_SET);
}


/* Inicialização da tabela Hash 
- Setar os ponteiros como NULL
*/
void inicializar_tabela(NoHash *tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        tabela[i] = NULL;
    }
}


/* Tabela alocada dinamicamente.
-libera cada ponteiro 
*/
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



void inserir_hash(NoHash *tabela[], char *nomeEstacao, int tamnomeEstacao) {
    int pos = hash_string(nomeEstacao, tamnomeEstacao, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nome) == tamnomeEstacao &&
            strncmp(atual->nome, nomeEstacao, tamnomeEstacao) == 0) {
            atual->repeticoes++;
            return;
        }
        atual = atual->prox;
    }

    NoHash *novo = (NoHash *) malloc(sizeof(NoHash));
    if (novo == NULL) return;
    memcpy(novo->nome, nomeEstacao, tamnomeEstacao);
    novo->nome[tamnomeEstacao] = '\0';
    novo->repeticoes = 1;
    novo->prox = tabela[pos];

    tabela[pos] = novo;
}



NoHash* buscar_hash(NoHash *tabela[], char *nomeEstacao, int tamnomeEstacao) {
    int pos = hash_string(nomeEstacao, tamnomeEstacao, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nomeEstacao) == tamnomeEstacao &&
            strncmp(atual->nomeEstacao, nomeEstacao, tamnomeEstacao) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}



// função que integra todas as outras para obter o ponteiro de um arquivo binário
FILE *obter_arquivo_binario(ArquivoAberto **lista_arquivos,
                            char *nome_bin,
                            NoHash *tabela[]) {
    ArquivoAberto *atual;
    FILE *bin;

    atual = buscar_arquivo(*lista_arquivos, nome_bin);

    if (atual != NULL) {
        return atual->arquivo;
    } else {
        bin = fopen(nome_bin, "r+b");

        if (bin == NULL) {
            return NULL;
        }

        adicionar_arquivo_processado(lista_arquivos, nome_bin, bin);
        carregar_nomes_no_hash(bin, tabela);

        return bin;
    }
}



// caso for encontrado uma repetição, decrementa o hashing
void decrementar_hash(NoHash *tabela[], char *nomeEstacao, int tamNomeEstacao) {
    int pos = hash_string(nomeEstacao, tamNomeEstacao, TAM_TABELA);
    NoHash *atual = tabela[pos];
    NoHash *anterior = NULL;

    while (atual != NULL) {
        if ((int)strlen(atual->nomeEstacao) == tamNomeEstacao &&
            strncmp(atual->nomeEstacao, nomeEstacao, tamNomeEstacao) == 0) {

            atual->repeticoes--;

            if (atual->repeticoes <= 0) {
                if (anterior == NULL) {
                    tabela[pos] = atual->prox;
                } else {
                    anterior->prox = atual->prox;
                }
                free(atual);
            }
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }
}






