#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"






int hash_string(char *str, int tam) {
    int soma = 0;
    // Formula para criação da chave hash 
    for (int i = 0; i < tam; i++) {
        soma += (unsigned char) str[i];
    }

    return soma % TAM_TABELA;
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





/*Estrutura escolhida para auxiliar na busca de string unicas foi tabela Hash.
Se o tamnaho das strings é >0 insere na tabela hash.*/
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]) {
     
    cabecalho reg_cab;
    dados reg_dados;
    int rrn;

    ler_cabecalho(bin, &reg_cab);
    
    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        reg_dados = cria_dados();
        //fseek(bin, calculo_byteoffset_dados(rrn), SEEK_SET);
        ler_regdados(bin, &reg_dados);


        //é removido ?
        if (reg_dados.removido == '1') {
            //o ponteiro já está no final desse rrn logo, vai p/ proxima iteração
            continue;
        }    
        if (reg_dados.tamNomeEstacao > 0) {
            // a string existe no hash?
            NoHash *h = buscar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
            if (h == NULL) {
                inserir_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
            } else {
                // apenas aumenta as repetições
                h->repeticoes++; 
            }    
        }    
    }   
    //atualiza o ponteiro do arquivo de dados p/ usar em outras funções. 
    fseek(bin, 0, SEEK_SET);

}    







void inserir_hash(NoHash *tabela[], char *nomeEstacao, int tamnomeEstacao) {
    int pos = hash_string(nomeEstacao, tamnomeEstacao);
    NoHash *atual = tabela[pos];
    //estamos no bucket correto da string 
    while (atual != NULL) {
        //houve colisões? verifica se a  string armazenada é a msm
        if ((int)strlen(atual->nome) == tamnomeEstacao &&
            strncmp(atual->nome, nomeEstacao, tamnomeEstacao) == 0) {
            atual->repeticoes++;
            return;
        }
        atual = atual->prox;
    }

    //a strin não existia - cria novo Nó
    NoHash *novo = (NoHash *) malloc(sizeof(NoHash));
    if (novo == NULL) return;
    memcpy(novo->nome, nomeEstacao, tamnomeEstacao);
    novo->nome[tamnomeEstacao] = '\0';
    novo->repeticoes = 1;
    novo->prox = tabela[pos];

    tabela[pos] = novo;
}






void decrementar_hash(NoHash *tabela[], char *nomeEstacao, int tamNomeEstacao) {
    int pos = hash_string(nomeEstacao, tamNomeEstacao);
    NoHash *atual = tabela[pos];
    NoHash *anterior = NULL;
    
    // caso for encontrado uma repetição, decrementa o hashing
    while (atual != NULL) {
        if ((int)strlen(atual->nome) == tamNomeEstacao &&
            strncmp(atual->nome, nomeEstacao, tamNomeEstacao) == 0) {

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



NoHash* buscar_hash(NoHash *tabela[], char *nomeEstacao, int tamnomeEstacao) {
    int pos = hash_string(nomeEstacao, tamnomeEstacao);
    NoHash *atual = tabela[pos];
    //retorna o ponteiro do nó da string encontrada
    while (atual != NULL) {
        if ((int)strlen(atual->nome) == tamnomeEstacao &&
            strncmp(atual->nome, nomeEstacao, tamnomeEstacao) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    //não encontrou a string
    return NULL;
}



