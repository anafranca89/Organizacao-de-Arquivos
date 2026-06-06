#ifndef CSV_H
#define CSV_H
#define MAX_LINHA_CSV 512

/* Funções que trabalham com o arquivo csv
*/

int ler_registro_csv(FILE *csv, dados *reg_dados);
void adicionar_csv_no_binario(char *arq_csv, char *arq_bin, NoHash *tabela[]);

void ler_par_campo_valor(char *campo, char *valor);


static void pular_virgula(char **p);
static int ler_inteiro_csv(char **p);
static int ler_string_csv(char **p, char *destino);


#endif