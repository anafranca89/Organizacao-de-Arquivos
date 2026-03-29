#ifndef REGISTROS_H
#define REGISTROS_H


/*A representação é de 17 bytes. Como saber se tem essa qts de bytes....
pela ordem que foi chamado as variáveis*/
typedef struct regcabecalho {
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacoes;
}cabecalho;


/*Valores nulos são representados por -1 para campos de tam.fixo.
Valores nulos nos Campos de tam.variável tem 0 no indicador de tamanho
80 bytes!!!
*/

typedef struct regdados{
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char  nomeEstacao[tamNomeEstacao];
    int tamNomeLinha;
    char nomeLinha[tamNomeLinha];
} dados;



FILE* ler_binario(char *arqbin);
FILE * escrever_binario(char *arqbin);

cabecalho ler_cabecalho(char *arqbin);
void escreve_cabecalho(char *arqbin, cabecalho regcab);
dados ler_regdados(char *arqbin, int byteoffset);
void escreve_regdados(char *arqbin, dados registro, int byteoffset);



#endif