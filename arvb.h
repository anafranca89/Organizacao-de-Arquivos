#ifndef ARVOREB_H
#define ARVOREB_H
#include "funcoes.h"
#include "regindices.h"
#define PROMOTION 1
#define NO_PROMOTION 0

/*------------------------------------------------------
Funções que lidam com a estrutura de ARVORE B. 
Funções estruturais e auxiliares.
------------------------------------------------------
*/

/*------------------------------------------------------
  Algoritmo Driver: Cria a lista de nós da árvore.
    Caso1: Arquivo/arvore n existe--> cria um arquivo p/leitura e escrita
    Caso2: Arquivo existe --> apenas abre arquivo p/escrita 
        - Enquanto existe registros --> count é menor que proxRRN
            - verifica se n é logicamente removido, senão vai pro proximo registro
            - removido == 0, então tenta inserir na raiz
            - segue a função de inserção
                - retornou PROMOTION => raiz sofreu split
                - cria novo nó
            - Atualiza cabeçalho da arvore
            - fecha arquivo
  */
void cria_arvore(FILE* arq_dados, FILE *arq_index);


/*------------------------------------------------------
    Assumindo q existe o arquivo de indice existe e é != NULL.
      Busca uma chave única int - codEstacao-  num nó da árvore.
      Caso 1: Arvore com elementos --> rrn da raiz >=0
      Caso 2: Arvore vazia --> rrn da raiz == -1
      Parametros: Arquivo de indices, rrn do no onde faz a busca, a chave para busca, 
              e a posicao (1, 2, 3) da chave. posicao == -1 se a chave n existe
              ------util para inserção
      Retorna : RRN do nó onde DEVERIA ESTAR ou ESTÁ.

      ->Chama a função no nó raiz
          - arvore existe? 
              - Se não - retorna -1
              - Se sim - continua
          - No atual foi removido?
              - retorna -1, ou seja, não encontrou a chave
          - Senão - Lê nó (posiciona o ponteiro para ler o no exato)
              - chave está no nó ? 
                  - se sim, acaba a busca, retorna o rrn e atualiza a pos_chava_no
                  - se não, verifica se é nó intermediário
                      - se sim, continua a busca recursivamente
                      - se não, termina a busca. E no folha - logo sem descendentes p/buscar.

      */
int busca_arvore(FILE *arquivo, int rrn_no, int chave_unica, int *posicao_chave_no);


    /*------------------------------------------------------
    Parametros: arquivo index arvore, rrn do nó atual a ser inserido, 
                chave a ser inserida,ponteiro da chave,  chave da promoção,
                 nó filho promovido.
      Retorna INT : 1 se houve PROMOTION
                    -1 se houve ERRO
                    0 Padrão - No PROMOTION
      Caso base : rrn-atual = -1, promove a chave
          OBS:Inserção apenas em No Folha  E começa por uma busca(na raiz!:))
          - busca a posição esperada da chave
              - encontrou a chave? -> retorna erro de chaveduplicada -> quando pos_chave_no != de -1 
              - SENÃO- temos o rrn esperado daquela chave.
                  - lê o nó
          - encontra a posicao esperada da chave no nó - 1, 2, 3
          - ret_value = insere recursivo
              -  NO_PROMOTION ou ERRO na pilha de recursão  ?
                  - retorna NO PROMOTION  OU NEGATIVO
              - Senão tenta inserção no nó
                  - nó tem espaço?
                      - insere ordenado
                  - nó não tem espaço?
                      - split-> cria um novo no, ordena as chaves, escreve o no_esq
                          escreve o no_dir, promove a chave mais a esquerda do no_dir, 
                          com filho nó_dir.
                      retorna PROMOTION  */
int insere_arvore(FILE* arq_index, cab_indice *cabecalho, int rrn_no, int chave, int byteoffset_dados,
                 int *filho_promovido, int *chave_promovida, int *byte_dados_promovido);


void insere_recebendo_chave_e_byteoffset(FILE *indexes, cab_indice *index_cab, int chave, int byteoffset_dados);
             

  /*------------------------------------------------------
  Sobreescreve o no de indice, para ordenar as chaves dentro dele.
  Como ele insere no nó, é assumido que nroChaves<3.
  Parametros: no p/sobrescerver, chave, filho dessa chave, e o ponteiro p/arquivo de dados.
    - Se qtdChaves == 0 - insere no primeiro espaço 
    - se qtdchaves == 1 --> 2 casos     
                        - insere no espaço 2 
                        - ou shifta e insere no inicio
    - se qtdchaves ==2 --> 3 casos
                        - insere no espaço 3
                        - shifta tudo e insere no inicio
                        - shifta o espaço 2 e insere no meio.
*/
void insere_ordenado_no(indice *no, int chave, int filho_promovido,  int byte_ponteiro);



    /*------------------------------------------------------
    Cria um novo no, promove uma chave e a sub arvore, copiando os valores anteriores.
Parametros: arquivo de indezx, cabeçalho do index, rrn do no, o proprio no, 
            chave q deve ser inserida, o rrn filho, e o byte_ponteiro.
            a chave q é realmente promovida, o seu byte ponteiro e seu filho
    - Cria uma estrutura de comparação, com espaço para m+1 ->4 chaves, 4 ponteiro, 5 subarvores.
    - copia as chaves da pag atual e compara com a chave.
    - ordena as chaves
    - aloca um novo no no index
    - atualiza a chave a ser promovida e o filho a ser promovido 
    - organiza as chaves nos 2 nós e retorna a novapagina.
*/
void split (FILE *arq_index, cab_indice *cab_ind, int rrn_no_ant, indice *no_ant, int chave,
           int byte_chave, int filho_chave,int *chave_promovida, int *byte_promovido, int *filho_promovido );






void remove_chave_arvore(FILE *index,cab_indice *cab_index,int chave);

void remover_registros_dinamico_com_arvore(FILE *bin, FILE *index, NoHash *tabela[],cabecalho *reg_cab,
                                            cab_indice *cab_index,int m,char nomesCampos[][50],char valoresCampos[][200]);

int alocar_rrn_no_arvore(FILE *arq_index, cab_indice *cab_ind);



    /*------------------------------------------------------
função para corrigir underflow no nó filho-quando tem menos chaves que o mínimo
    Primeiro caso: pega a chave mais a esquerda do filho dir
                    -insere no pai
                    - insere no filho do meio
    Segundo caso: pega a chave mais a direita do filho esq
                    -insere no pai
                    - insere a chave q estava no pai,  filho do meio

    Terceiro caso: pega a chave mais a esquerda do pai 
                    -insere no filho a esquerda
    Quarto caso: pega a chave mais a esquerda do pai 
                    -insere no filho a esquerda + o a chave do meio
    Mas e se ter underflow do nó de cima? aí entra a lógica recursiva!
                    [1]
                []  [2]  [4]

                vai para:
                        [5, 6]
                    []          [7, 8]
                [1, 2] [4]

                vai para:
                        [6, 7]
                    [5]          [8]
                [1, 2] [4]
*/
void corrigir_underflow_filho(FILE *index, cab_indice *cab_index, int rrn_pai,int pos_filho);

int get_pr(indice *no, int pos);
int get_chave(indice *no, int pos);
int obter_filho(indice *no, int pos);

void setar_chave_no_pai(indice *pai, int pos, int chave, int pr);
void remover_chave_e_filho_direito(indice *pai, int pos_chave);
void remover_chave_da_pagina(indice *no, int pos);
int posicao_filho_para_chave(indice *no, int chave);
int posicao_chave_no(indice *no, int chave);
void atualizar_tipo_no(indice *no, cab_indice *cab_index, int rrn);
void vetores_para_no(indice *no, int nroChaves, int chaves[], int prs[], int filhos[]);
void no_para_vetores(indice *no, int chaves[], int prs[], int filhos[]);
void inicializar_vetor_com_negativo(int *vet, int tam);
int no_eh_folha(indice *no);
void escrever_no_arvore(FILE *index, int rrn, indice *no);
void ler_no_arvore(FILE *index, int rrn, indice *no);
int remover_recursivo_arvore(FILE *index, cab_indice *cab_index, int rrn_atual, int chave,int eh_raiz);
void remove_chave_arvore(FILE *index,cab_indice *cab_index,int chave);
void ajustar_raiz_apos_remocao(FILE *index, cab_indice *cab_index);
void corrigir_underflow_filho(FILE *index, cab_indice *cab_index, int rrn_pai,int pos_filho);
void concatenar_nos(FILE *index,cab_indice *cab_index,int rrn_esq,int rrn_pai,int pos_chave_pai,int rrn_dir);
void redistribuir_entre_nos(FILE *index,cab_indice *cab_index,int rrn_esq,int rrn_pai,int pos_chave_pai,int rrn_dir);
void empilhar_no_removido_arvore(FILE *index, cab_indice *cab_index, int rrn_removido);
void encontrar_sucessor(FILE *index, int rrn, int *chave_sucessora, int *pr_sucessor);


#endif