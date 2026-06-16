#ifndef SWITCH_H
#define SWITCH_H
#include "arvb.h"
#include "funcoes.h"


/*---------------------------------------------------------------------------
    Funções que representam as funcionalidades do trabalho
    usadas na main/ switch case
------------------------------------------------------------------------------
*/

    /*Dado o nome de um arquivo csv e o nome do arquivo binário.
    O csv precisa existir na pasta, senão imprime erro.
    O binário é criado, se existir reescreve ele.
    A função faz o parse do csv e transforma os dados em registros de tamanho fixo no binário.
    lista encadeada HASH é usada para manter o registro de estações únicas.
    ->é assumido que os dados estão na forma padrão esperada.
    */
void create_table(NoHash *tabela[], char * csv, char*bin);



    /*Dado o nome do arquivo binário. Ele precisa existir na pasta, senão dá erro.
    tabela hash deve ser INICIALIZADA.
    Mostra de forma sequencial todos os registros armazenados no binário.
    Com a formatação especificada.
    */
void select_everything_from(NoHash *tabela[], char* nomebin);



    /*Parametros - arquivo binario de dados, tabela INICIALIZADA hash.
    O arquivo bin deve existir - senão msg de erro.
    Busca os registros de acordo com campos e valores dados.
    Imprime cada registro também.
    */
void select_from_where(NoHash *tabela[], char* nome_bin);



    /*Parametros - arquivo binario de dados, tabela INICIALIZADA hash.
     Busca os n registros de m campos, e marca como logicamente removidos.
     Cabeçalho de dados é reescrito no inicio (inconsistente) e no fim(consistente) da função 
    */ 
void delete_from_where(NoHash *tabela[], char* nome_bin);



    /*Parametros - arquivo binario de dados, tabela INICIALIZADA hash, N quantos registro a inserir
    
    */ 

void insert_into(NoHash *tabela[], char* nome_bin, int n);


void update_table(NoHash *tabela[], char* nome_bin);


void create_index(NoHash *tabela[], char* nome_bin, char* nome_index);


void select_from_where_index(NoHash *tabela[], char* nome_bin,char*nome_index);


    /*Dados os arquivos de dados e arquivos de indece e o numero de registro p/inserir.
    A tabela hash deve estar INICIALIZADA!.
    tenta inserir usando arvore B. Nao insere se já existe aquele códigoEstacao.
    Senao, insere tanto na arvore B e no arquivo de dados, usando a pilha de logicamente removidos de ambos
    */
void insert_into_index(NoHash *tabela[], char* nome_bin,char*nome_index , int n);

    /*Dados os arquivos de dados e arquivos de indece e o numero de registro p/remover.
    A tabela hash deve estar INICIALIZADA!.
    marca como removido primeiro no arquivo de indices- Se foi possivel, então marca a remoção (delete_from_where) 
    no arquivo de dados.
    */
void delete_from_where_index(NoHash *tabela[], char* nome_bin, char*nome_index, int n);


#endif 

