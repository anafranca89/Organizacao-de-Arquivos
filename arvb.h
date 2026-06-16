#ifndef ARVOREB_H
#define ARVOREB_H
#include "funcoes.h"
#include "regindices.h"

#define PROMOTION 1
#define NO_PROMOTION 0



int busca_arvore(FILE *arquivo, int rrn_no, int chave_unica, int *posicao_chave_no);

void cria_arvore(FILE* arq_dados, FILE *arq_index);


int insere_arvore(FILE* arq_index, cab_indice *cabecalho, int rrn_no, int chave, int byteoffset_dados,
                 int *filho_promovido, int *chave_promovida, int *byte_dados_promovido);
  void insere_recebendo_chave_e_byteoffset(FILE *indexes, cab_indice *index_cab, int chave, int byteoffset_dados);
                  
void insere_ordenado_no(indice *no, int chave, int filho_promovido,  int byte_ponteiro);

void split (FILE *arq_index, cab_indice *cab_ind, int rrn_no_ant, indice *no_ant, 
           int chave, int byte_chave, int filho_chave,
           int *chave_promovida, int *byte_promovido, int *filho_promovido );






void remove_chave_arvore(FILE *index,cab_indice *cab_index,int chave);

void remover_registros_dinamico_com_arvore(FILE *bin, FILE *index, NoHash *tabela[],cabecalho *reg_cab,
                                            cab_indice *cab_index,int m,char nomesCampos[][50],char valoresCampos[][200]);

int alocar_rrn_no_arvore(FILE *arq_index, cab_indice *cab_ind);

#endif