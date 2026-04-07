#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "fornecidas.h"
#include "funcoes.h"

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

//-----------------
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

int ler_registro_csv(FILE *csv,
                     int *codEstacao,
                     char *nomeEstacao,
                     int *tamNomeEstacao,
                     int *codLinha,
                     char *nomeLinha,
                     int *tamNomeLinha,
                     int *codProxEstacao,
                     int *distProxEstacao,
                     int *codLinhaIntegra,
                     int *codEstIntegra) {
    char linha[MAX_LINHA_CSV];
    char *p;
    int valor, tem_digito, i;

    if (csv == NULL) return 0;
    if (fgets(linha, MAX_LINHA_CSV, csv) == NULL) return 0;

    p = linha;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    i = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        nomeEstacao[i++] = *p;
        p++;
    }
    *tamNomeEstacao = i;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codLinha = tem_digito ? valor : -1;
    if (*p == ',') p++;

    i = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        nomeLinha[i++] = *p;
        p++;
    }
    *tamNomeLinha = i;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *distProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codLinhaIntegra = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codEstIntegra = tem_digito ? valor : -1;
    return 1;
}

void adicionar_csv_no_binario(FILE *csv, FILE *bin, NoHash *tabela[]) {
    char linha_cabecalho[MAX_LINHA_CSV];

    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, tamNomeEstacao;
    int codLinha, tamNomeLinha;
    int codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;

    char nomeEstacao[200];
    char nomeLinha[200];

    int rrn_atual;
    long byteoffset;
    int debug_count = 0;
    int i;

    if (csv == NULL || bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    if (status != '0' && status != '1') {
        status = '0';
        topo = -1;
        proxRRN = 0;
        nroEstacoes = 0;
        nroParesEstacoes = 0;

        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    }

    if (fgets(linha_cabecalho, MAX_LINHA_CSV, csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    rrn_atual = proxRRN;
    byteoffset = TAM_CABECALHO + rrn_atual * TAM_REG;

    fseek(bin, byteoffset, SEEK_SET);

    while (ler_registro_csv(csv,
                            &codEstacao,
                            nomeEstacao,
                            &tamNomeEstacao,
                            &codLinha,
                            nomeLinha,
                            &tamNomeLinha,
                            &codProxEstacao,
                            &distProxEstacao,
                            &codLinhaIntegra,
                            &codEstIntegra))
        {

        removido = '0';
        proximo = -1;

        escreve_regdados(bin,
                         &removido,
                         &proximo,
                         &codEstacao,
                         &codLinha,
                         &codProxEstacao,
                         &distProxEstacao,
                         &codLinhaIntegra,
                         &codEstIntegra,
                         &tamNomeEstacao,
                         nomeEstacao,
                         &tamNomeLinha,
                         nomeLinha);

        {
            NoHash *busca = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

            if (busca == NULL) {
                inserir_hash(tabela, nomeEstacao, tamNomeEstacao);
                nroEstacoes++;
            } else {
                busca->repeticoes++;
            }
        }

        if (codProxEstacao != -1) {
            nroParesEstacoes++;
        }

        if (debug_count < 10) {
            printf("REGISTRO %d\n", debug_count + 1);
            printf("codEstacao = %d\n", codEstacao);
            printf("nomeEstacao = ");
            for (i = 0; i < tamNomeEstacao; i++) {
                printf("%c", nomeEstacao[i]);
            }
            printf("\n");
            printf("-------------------------\n");
            debug_count++;
        }

        rrn_atual++;
        byteoffset += TAM_REG;
    }

    proxRRN = rrn_atual;
    status = '1';

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
}

void imprime_inteiro_ou_nulo(int valor){
    if(valor == -1) printf("NULO");
    else printf("%d", valor);
}

void imprime_texto_ou_nulo(char *texto, int tamanho){
    if(tamanho == 0) printf("NULO");
    else printf("%.*s", tamanho, texto);
}

void mostrar_binario_sequencial(FILE *bin){
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;

    char nomeEstacao[200];
    char nomeLinha[200];

    int i;

    if(bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    if (proxRRN == 0 || nroEstacoes == 0) {
            printf("Registro inexistente.\n");
    return;
}
    if(status != '1'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for(i = 0; i < proxRRN; i++){
        ler_regdados(bin,
                     &removido,
                     &proximo,
                     &codEstacao,
                     &codLinha,
                     &codProxEstacao,
                     &distProxEstacao,
                     &codLinhaIntegra,
                     &codEstIntegra,
                     &tamNomeEstacao,
                     nomeEstacao,
                     &tamNomeLinha,
                     nomeLinha);

        if(removido == '1') continue;
        imprime_inteiro_ou_nulo(codEstacao);
        printf(" ");

        imprime_texto_ou_nulo(nomeEstacao, tamNomeEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(codLinha);
        printf(" ");

        imprime_texto_ou_nulo(nomeLinha, tamNomeLinha);
        printf(" ");

        imprime_inteiro_ou_nulo(codProxEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(distProxEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(codLinhaIntegra);
        printf(" ");

        imprime_inteiro_ou_nulo(codEstIntegra);
        printf("\n");
    }
}


