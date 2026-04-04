#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17

FILE* ler_binario(char *arqbin);
FILE * escrever_binario(char *arqbin);

cabecalho ler_cabecalho(char *arqbin);
void escreve_cabecalho(char *arqbin, cabecalho regcab);
dados ler_regdados(char *arqbin, int byteoffset);
void escreve_regdados(char *arqbin, dados registro, int byteoffset);



#endif
