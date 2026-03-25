#include <stdio.h>
#include <stdlib.h>
/*Nomes:
Ana Julia Lopez Franqueira de França - 16838230
Leonardo Biondo Bertho - 16881900            -
*/

FILE *binario = NULL;

/*A representação é de 17 bytes. Como saber se tem essa qts de bytes....
pela ordem que foi chamado as variáveis*/
typedef struct regcabecalho {
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacoes;
} cabecalho;



typedef struct regdados {
    char removido;
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







