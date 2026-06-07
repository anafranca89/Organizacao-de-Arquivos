#ifndef CSV_H
#define CSV_H
#define MAX_LINHA_CSV 512

#include "registros.h"
#include "hash.h"

/* Funções que trabalham com o arquivo csv
*/

int ler_registro_csv(FILE *csv, dados *reg_dados);
void adicionar_csv_no_binario(FILE *arq_csv, FILE *arq_bin, NoHash *tabela[]);

void ler_par_campo_valor(char *campo, char *valor);


void pular_virgula(char **p);
int ler_inteiro_csv(char **p);
int ler_string_csv(char **p, char *destino);


#endif