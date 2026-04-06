// PONTO_H SIMPLES, CASO QUISER COMENTÁRIOS DO QUE CADA FUNÇÃO FAZ OLHE NO .C

#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17

FILE* ler_binario(char *arqbin);
FILE* escrever_binario(char *arqbin);

void ler_cabecalho(FILE* ponteiro_arquivo, char *status, int *topo, int *proxRRN, int *nroEstacoes, int *nroParesEstacoes);

void escreve_cabecalho(FILE* ponteiro_arquivo, char *status, int *topo, int *proxRRN, int *nroEstacoes, int *nroParesEstacoes);

void ler_regdados(FILE* ponteiro_arquivo, 
                  char *removido,
                  int *proximo,
                  int *codEstacao,
                  int *codLinha,
                  int *codProxEstacao,
                  int *distProxEstacao,
                  int *codLinhaIntegra,
                  int *codEstIntegra,
                  int *tamNomeEstacao,
                  char *nomeEstacao,
                  int *tamNomeLinha,
                  char *nomeLinha);

void escreve_regdados(FILE* ponteiro_arquivo, 
                      char *removido,
                      int *proximo,
                      int *codEstacao,
                      int *codLinha,
                      int *codProxEstacao,
                      int *distProxEstacao,
                      int *codLinhaIntegra,
                      int *codEstIntegra,
                      int *tamNomeEstacao,
                      char *nomeEstacao,
                      int *tamNomeLinha,
                      char *nomeLinha);



#endif
