#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17

FILE* ler_binario(char *arqbin);
FILE * escrever_binario(char *arqbin);

void ler_cabecalho(char *arqbin,
                   char *status,
                   int *topo,
                   int *proxRRN,
                   int *nroEstacoes,
                   int *nroParesEstacoes);

void escreve_cabecalho(char *arqbin,
                       char status,
                       int topo,
                       int proxRRN,
                       int nroEstacoes,
                       int nroParesEstacoes);

void ler_regdados(char *arqbin, int byteoffset,
                  char *removido,
                  int *proximo,
                  int *codEstacao,
                  int *codLinha,
                  int *codProxEstacao,
                  int *distProxEstacao,
                  int *codLinhaIntegra,
                  int *codEstIntegra,
                  int *tamNomeEstacao,
                  char **nomeEstacao,
                  int *tamNomeLinha,
                  char **nomeLinha);

void escreve_regdados(char *arqbin, int byteoffset,
                      char removido,
                      int proximo,
                      int codEstacao,
                      int codLinha,
                      int codProxEstacao,
                      int distProxEstacao,
                      int codLinhaIntegra,
                      int codEstIntegra,
                      int tamNomeEstacao,
                      char *nomeEstacao,
                      int tamNomeLinha,
                      char *nomeLinha);



#endif
