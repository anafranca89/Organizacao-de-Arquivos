#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REG 80
#define TAM_CABECALHO 17
#define NEGATIVO -1
#include "fornecidas.h"


/*------------------------------------
    Definição das structs de Registro do arquivo de dados.
---------------------------------------
*/
typedef struct reg_cabecalho{
    char status;  // 0 ou 1
    int topo;     // >= -1
    int proxRRN; // >=-1
    int nroEstacoes; // >=0
    int nroParesEstacoes; //>=0
}cabecalho;


typedef struct reg_dados{
    char removido;  // 0 ou 1
    int proximo;   // -1 ou >=0
    int codEstacao;  // inteiro não nulo
    int codLinha;   // inteiro nao nulo
    int codProxEstacao;// inteiro nao nulo
    int distProxEstacao;// inteiro nao nulo
    int codLinhaIntegra;// inteiro nao nulo
    int codEstIntegra;// inteiro nao nulo
    int tamNomeEstacao; // >=0
    /*espaço de 50 - suficiente para o pior caso, onde apenas uma das strings ocupa os 
    80 - 37 = 43 bytes restantes */
    char nomeEstacao[50];
    int tamNomeLinha;// >=0
    char nomeLinha[50]; 
}dados;


// Estrutura auxiliar para atualização
typedef struct {
    int rrn;
    dados reg_dados;
} AtualizacaoPendente;


/*-----------------------------------------
construtores - Retornam a struct com os valores padrão p/ registro vazio/novo.
-------------------------------------------
*/
cabecalho cria_cabecalho();
dados cria_dados();


/*---------------------------------------------------------
Funções auxiliares p/ escrita ou leitura do arquivo binário
---------------------------------------------------------
*/ 
        //retorna o ponteiro no modo rb
FILE *ler_binario(char *arqbin);  
        //retorna o ponteiro no modo wb+
FILE * cria_escreve_binario(char *arqbin); 
        //retorna o ponteiro no modo rb+
FILE* escrever_binario(char *arqbin); 
        //retorna o ponteiro no modo rb+ ou wb+
FILE* abrir_para_escrita_binário(char *arq);  

/*---------------------------------
funções auxiliares que lidam com Registros de dados.
------------------------------------------
*/
    /*Calcula o byteoffset usando o rrn como parametro.*/
int calculo_byteoffset_dados(int RRN);

    /*Ponteiro do fseek no COMEÇO do arquivo!
    Parametros: arquivo  binario, ponteiro p/ -> ler ou inserir o reg.cabeçalho
    Ler - o registro vai ser reescrito pelo valor no arquivo.
    Escreve - o registro precisa ter os valores corretos.
    */
void ler_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab); 
void escreve_cabecalho(FILE* ponteiro_arquivo, cabecalho *reg_cab);

    /*Ponteiro do fseek no BYTEOFFSET do RRN!
    Parametros: arquivo  binario, ponteiro p/ -> ler ou inserir o reg.dados
    Ler - o registro vai ser reescrito pelo valor no arquivo.
    Escreve - o registro precisa ter os valores corretos.*/
void ler_regdados(FILE* ponteiro_arquivo, dados* reg_dados);
void escreve_regdados(FILE *ponteiro_arquivo, dados* reg_dados);

    /*Parametros: qtd de campos P, nome dos p campos, valor dos p campos e o ponteiro p/ o registro a ser escrito.
    reescreve os campos em nomesAtualiza com os valores em valoresAtualiza. 
    */
void atualizar_campos_registro(int p, char nomesAtualiza[][50], char valoresAtualiza[][200], dados *reg_dados);

    //Retorna a string lida usando o SCANQUOTESTRING
void ler_par_campo_valor(char *campo, char *valor);

    /*---------------------------
    Funções q lidam com impressão do campo NULO.
    ---------------------------------
    - se o campo for inteiro, usa o valor do int.
    - se o campo for string,  precisa da string e o tamanho dela.
    */
void imprime_inteiro_ou_nulo(int valor);
void imprime_texto_ou_nulo(char *texto, int tamanho);

    /*imprime o registro de dados dado como parametro, usando a formatação necessária.
    */
void imprime_registro_dados(dados* reg_dados);

#endif