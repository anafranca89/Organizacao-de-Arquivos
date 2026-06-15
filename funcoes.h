#ifndef FUNCOES_H
#define FUNCOES_H
#include "hash.h"
#include "csv.h"

/*------------------------------
Funções relacionadas ao trabalho - utilizadas na main.
--------------------------------
*/


void ler_registro_entrada(dados *reg_dados);
void ler_par_campo_valor(char *campo, char *valor);


void mostrar_binario_sequencial(FILE *bin);



int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200], dados* reg_dados);
void buscar_registros(FILE *bin, NoHash *tabela[], int m,char nomesCampos[][50], char valoresCampos[][200]);



void remover_registros_dinamico(FILE *bin, NoHash *tabela[], cabecalho* reg, int  m,
                                char nomesCampos[][50], char valoresCampos[][200], char ultimo);

long inserir_registro_dinamico(FILE *bin, NoHash *tabela[], cabecalho *cab, dados *reg_dados);

void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[], int m, char nomesBusca[][50], char valoresBusca[][200],
                                    int p, char nomesAtualiza[][50], char valoresAtualiza[][200]);


long ler_e_inserir_registro(FILE *bin, NoHash *tabela[], cabecalho *reg_cabecalho);


#endif

