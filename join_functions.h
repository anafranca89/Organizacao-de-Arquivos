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
/// @param arq1 
/// @param campo1 
/// @param arq2 
/// @param campo2 
/// @param indice 
void junction_join(char *arq1, char* campo1, char * arq2, char*campo2,char *indice);

#endif

