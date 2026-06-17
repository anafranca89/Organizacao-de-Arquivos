#ifndef FUNCOES_H
#define FUNCOES_H
#include "hash.h"
#include "csv.h"

/*------------------------------
Funções relacionadas ao trabalho 1 - lidam apenas com manipulação de  registro de dados
--------------------------------
*/


void ler_registro_entrada(dados *reg_dados);
void ler_par_campo_valor(char *campo, char *valor);


void mostrar_binario_sequencial(FILE *bin);



int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200], dados* reg_dados);
void buscar_registros(FILE *bin, NoHash *tabela[], int m,char nomesCampos[][50], char valoresCampos[][200]);



void remover_registros_dinamico(FILE *bin, NoHash *tabela[], cabecalho* reg, int  m,
                                char nomesCampos[][50], char valoresCampos[][200], char ultimo);

void inserir_registro_dinamico(FILE *bin, NoHash *tabela[], cabecalho *cab, dados *reg_dados, long int *offset);

void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[], int m, char nomesBusca[][50], char valoresBusca[][200],
                                    int p, char nomesAtualiza[][50], char valoresAtualiza[][200]);


int verifica_se_foi_aberto(FILE *ARQ1, FILE *ARQ2, int tem2arquivos);
#endif

