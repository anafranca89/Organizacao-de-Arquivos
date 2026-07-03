#ifndef JOIN_H
#define JOIN_H
#include "arvb.h"
#include "funcoes.h"

/*------------------------------
Funções sobre JOIN - trabalho 3
--------------------------------
*/

/// @brief
/**Faz uma auto junção, utilizando dois arquivos que são iguais.
 * é certo que os campos sempre sao codProxestacao e codEstacap
 * Imprime o registro modificado - usando alguns campos do arquivo 1 e alguns campos do arq2
*/
/// @param nome_bin  arquivo de dados 1 
/// @param campo1   campo deve ser codestacao
/// @param nome_bin2  arquivo de dados 2 é o mesmo que o arquivo 1
/// @param campo2   campo deve ser codestacao
void nested_join(char *nome_bin,char *campo1, char *nome_bin2 , char *campo2 );

/// @brief 
/** Faz a auto junção de 2 arquivos iguais - usando arvore B. O primeiro arquivo é percorrido
 * iterativamente. Usando o campo1 busca no arquivo2 usando um indice ArvB.
 * Os campos buscados são necessariamente codProxEstacao e codEstacao.
 * Usa a função de busca que retorna a posicao da chave no nó e o rrn do arquivo de index. 
*/
/// @param arq1 
/// @param campo1 
/// @param arq2 
/// @param campo2 
/// @param indice 
void junction_join(char *arq1, char* campo1, char * arq2, char*campo2,char *indice);

/// @brief 
/**
 * Função geral p/ ordenar um arquivo de registro desordenado. Usada em order_join.
 */
/// @param bin  arquivo binario de dados
/// @param arq_ordenado arquivo de dados q/vai ser ordenado
void ordena_arquivo_geral(FILE *bin, FILE *arq_ordenado);

/// @brief
/** Como precisamos comparar registros, a função qsort precisa como comparar esses elementos
 * logo, essa função, ordena ou pelo codEstacao ou codProxestacao.
 * Para os casos onde cod é NEGATIVO, eles vem depois de qualquer valor válido de cod.
 */
/// @param a 
/// @param b 
/// @return 0 SE são iguais; 1 se A > B e -1 se A<B
int compara_dados(const void *a, const void *b);

/// @brief 
/// @param nome_bin 
/// @param campo1 
/// @param nome_bin2 
void order_join(char* nome_bin,char *campo1, char *nome_bin2);


/// @brief 
/// @param bin 
/// @param reg 
/// @param RRN 
/// @param totalRRN 
/// @return 
int avanca_A(FILE *bin, dados *reg, int *RRN, int totalRRN);


/// @brief 
/// @param bin 
/// @param reg 
/// @param RRN 
/// @param totalRRN 
/// @return 
int avanca_B(FILE *bin, dados *reg, int *RRN, int totalRRN);

/// @brief 
/**
 * Ordenar os arquivos 1 e 2 usando, respectivamente CodProxEstacao e CodEstacao. Os campos sãonecessáriamente
 * codProxestacao e codEstacao.
 */
/// @param arq1 arquivo de registros desordenado
/// @param campo1 = CodProxEstacao
/// @param arq2 arquivo de registros desordenado
/// @param campo2 = CodEstacao
void merge_sort_join(char *arq1, char *campo1, char* arq2,char* campo2);


#endif

