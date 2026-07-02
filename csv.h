#ifndef CSV_H
#define CSV_H
#define MAX_LINHA_CSV 512
#include "registros.h"
#include "hash.h"
/*---------------------------
Funções que trabalham com o arquivo csv + registro de dados
---------------------------
*/
        /*---------------------------
        Funções auxiliares em leitura csv
        ---------------------------
        */
        void pular_virgula(char **p);
        int ler_inteiro_csv(char **p);
        int ler_string_csv(char **p, char *destino);

    /* Parametros: arquivo csv na linha a ser lida, struct para guardar os dados lidos
    Faz o parse manual de uma linha do csv  = 1 registro, e escreve campo a campo, na RAM/struct.*/
int ler_registro_csv(FILE *csv, dados *reg_dados);

    /* Adiciona os dados que estão no arquivo .csv no formato de registro de dados.
         Os ponteiros não devem ser NULL!
        Parametros: arquivo csv, arquivo binário de dados, tabela Hash*/
void adicionar_csv_no_binario(FILE *arq_csv, FILE *arq_bin, NoHash *tabela[]);





#endif