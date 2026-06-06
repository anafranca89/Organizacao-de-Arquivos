#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "fornecidas.h"
#include "hash.h"
#include "funcoes.h"







/*Função auxiliar: usando a estrutura criada, para saber se o arquivo já foi aberto anteriormente */
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
//Adiciona o nome do arquivo aberto na estrutura de arquivos abertos anteriormente
void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin) {
    ArquivoAberto *novo = (ArquivoAberto *) malloc(sizeof(ArquivoAberto));
    strcpy(novo->nome, nome_bin);
    novo->prox = *lista;
    *lista = novo;
}

// Liberar a memoria da estrutura de arquivos utilizada
void liberar_lista_arquivos(ArquivoAberto *lista) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        ArquivoAberto *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}


/*  Verifica se o registro de dados lido atende a todos os critérios 
  de busca informados pelo usuário na main.
 Retorna 1 se o registro for compatível ou 0 caso falhe em algum critério.
 */
int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200], dados *reg_dados) {
    
    for (int i = 0; i < m; i++) {
        
        //os campos de texto 
        if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (reg_dados->tamNomeEstacao != 0) return 0;
            } else if (reg_dados->tamNomeEstacao == 0 || 
                       (int)strlen(valoresCampos[i]) != reg_dados->tamNomeEstacao ||
                       strncmp(reg_dados->nomeEstacao, valoresCampos[i], reg_dados->tamNomeEstacao) != 0) {
                return 0;
            }
        }
        else if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (reg_dados->tamNomeLinha != 0) return 0;
            } else if (reg_dados->tamNomeLinha == 0 || 
                       (int)strlen(valoresCampos[i]) != reg_dados->tamNomeLinha ||
                       strncmp(reg_dados->nomeLinha, valoresCampos[i], reg_dados->tamNomeLinha) != 0) {
                return 0;
            }
        }
        
        // os inteiros
        else if (strcmp(nomesCampos[i], "codEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codEstacao != -1) return 0; 
            }
            else if (reg_dados->codEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codLinha != -1) return 0; 
            }
            else if (reg_dados->codLinha != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codProxEstacao != -1) return 0; 
            }
            else if (reg_dados->codProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "distProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->distProxEstacao != -1) return 0; 
            }
            else if (reg_dados->distProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinhaIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codLinhaIntegra != -1) return 0; 
            }
            else if (reg_dados->codLinhaIntegra != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codEstIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codEstIntegra != -1) return 0; 
            }
            else if (reg_dados->codEstIntegra != atoi(valoresCampos[i])) return 0;
        }
    }
    
    return 1; 
}







/*Função SELECT WHERE :  dados os campos e os valores procurados, imprimir os registros validos */
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                      char nomesCampos[][50], char valoresCampos[][200]) {
    if (bin == NULL) {
        return;
    }

    int i, rrn;
    int achou = 0;

    cabecalho reg_cab;
    dados reg_dados;

    int temNomeLinha = 0;
    char valorNomeLinha[200];
    int tamValorNomeLinha = 0;
    for (i = 0; i < m; i++) {
        // verifica se um dos campos é nomeLinha
        if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
            temNomeLinha = 1;
            strcpy(valorNomeLinha, valoresCampos[i]);
            tamValorNomeLinha = strlen(valorNomeLinha);
        }
    }

    //Se  tem nomeLinha - usa o hash para buscar usando o nome
    if (temNomeLinha && strlen(valorNomeLinha) != 0) {
        NoHash *h = buscar_hash(tabela, valorNomeLinha, tamValorNomeLinha);

        if (h == NULL) {
            printf("Registro inexistente.\n");
            return;
        }
    }

    ler_cabecalho(bin, &reg_cab);
    
    if (reg_cab.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    /*ler os registros de forma sequencial
    Se o registro foi removido, ignora na busca
    Se não, verifica qual campo foi digitado para busca
    */
    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        int ok = 1;

        memset(&reg_dados, 0, sizeof(dados));
        // Reposiciona o ponteiro de leitura
        fseek(bin, calculo_byteoffset_dados(rrn), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1')
            continue;

        ok = verificar_criterios(m, nomesCampos, valoresCampos, &reg_dados);
            

        // Se o registro atendeu a todas as m condições concomitantes
        if (ok) {
            imprime_registro_dados(&reg_dados);
            achou = 1;
        }
    }
    
    if (!achou)
        printf("Registro inexistente.\n");
}








/*Função que lê todos os registros do arquivo binário e retorna essas informações de forma sequencial.
*/
void mostrar_binario_sequencial(FILE *bin){
    // Cria um registro temporário para ler os dados do arquivo
    cabecalho reg_cab ;
    dados reg_dados;
    int i;

    if(bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Posiciona a leitura no inicio do arquivo e le o registro de cabeçalho
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);
    if (reg_cab.proxRRN == 0 || reg_cab.nroEstacoes == 0) {
        printf("Registro inexistente.\n");
        return;
    }
    if(reg_cab.status != '1'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for(i = 0; i < reg_cab.proxRRN; i++){
        fseek(bin, calculo_byteoffset_dados(i), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if(reg_dados.removido == '1') continue;
        imprime_registro_dados(&reg_dados);
    }
}