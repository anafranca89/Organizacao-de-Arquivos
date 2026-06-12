#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"



/*Avança o ponteiro, se encontrar uma virgula.
Util para parse do arquivo csv.
*/
void pular_virgula(char **p) {
    if (**p == ',') (*p)++;
}

/*Avança o ponteiro de leitura do csv, Retorna o inteiro lido, senão retorna -1.
*/
int ler_inteiro_csv(char **p) {
    int valor = 0;
    int tem_digito = 0;

    while (**p != ',' && **p != '\0' && **p != '\n' && **p != '\r') {
        // fórmula usada para converter string em inteiro(um casting, por assim dizer)
        valor = valor * 10 + (**p - '0');
        tem_digito = 1;
        (*p)++;
    }
    pular_virgula(p);
    return tem_digito ? valor : -1;
}


/*Avança o ponteiro do parser csv, lendo caracteres de uma string, e armazena no ponteiro de parametro.
Retorna o tamanho da string lida. A string no parametro tem o terminador '\0'!
*/
int ler_string_csv(char **p, char *destino) {
    int i = 0;

    while (**p != ',' && **p != '\0' && **p != '\n' && **p != '\r') {
        destino[i++] = **p;
        (*p)++;
    }
    //remove possiveis espaços e caracteres indesejados no final da string
    //TRIM
    while (i > 0 && (destino[i - 1] == ' ' || 
                     destino[i - 1] == '\r' || 
                     destino[i - 1] == '\n')) {
        i--;
    }
    destino[i] = '\0';
    pular_virgula(p);
    return i; // retorna o tam real da string -- SEM o \0 
}












/* Função que recebe um arquivo csv, realiza o parse manual tratando campos vazios
   e retorna as informações preenchidas  na struct 'dados' 
   Retorna: 1 se a leitura ocorreu.
            0 se os ponteiros são nulos ou a linha do csv está vazia*/
int ler_registro_csv(FILE *csv, dados *reg_dados) {
    char linha[MAX_LINHA_CSV];
    char *p;
    

    if (csv == NULL || reg_dados == NULL) return 0;
    if (fgets(linha, MAX_LINHA_CSV, csv) == NULL) return 0;

    //limpa a memoria 
    memset(reg_dados->nomeEstacao, 0, sizeof(reg_dados->nomeEstacao));
    memset(reg_dados->nomeLinha, 0, sizeof(reg_dados->nomeLinha));
    p = linha;

    reg_dados->codEstacao = ler_inteiro_csv(&p);
    reg_dados->tamNomeEstacao = ler_string_csv(&p, reg_dados->nomeEstacao);
    reg_dados->codLinha = ler_inteiro_csv(&p);
    reg_dados->tamNomeLinha = ler_string_csv(&p, reg_dados->nomeLinha);
    reg_dados->codProxEstacao = ler_inteiro_csv(&p);
    reg_dados->distProxEstacao = ler_inteiro_csv(&p);
    reg_dados->codLinhaIntegra = ler_inteiro_csv(&p);
    reg_dados->codEstIntegra = ler_inteiro_csv(&p);
    return 1; 
}




/* 
Adiciona os dados que estão no arquivo .csv no formato de registro de dados.
Os ponteiros não devem ser NULL!
Parametros: arquivo csv, arquivo binário de dados, tabela Hash
    - Verificação de segurança
        - os arquivos existem, mas existe inconsistencia de dados?
    - Se o arquivo e criado do zero.
        - escreve cabeçalho padrao
        - enquanto existe dados no  csv, escreve na struct de registro eno arq.binario
        - Para contar as Estaçoes unicas -- usa a tabela Hash, para contar as repetições
        - Verifica se precisa aumentar o parEstação
        - adiciona no arquivo cabeçalho

    - Se o arquivo já existe e já continha dados
        - verifica a consistencia dos dados e NAO sobreescreve.
        -começa a escrever no proxRRN.
        - enquanto existe dados no  csv, escreve na struct de registro eno arq.binario
        - Para contar as Estaçoes unicas -- usa a tabela Hash, para contar as repetições
        - Verifica se precisa aumentar o parEstação
        - adiciona no arquivo cabeçalho
*/

void adicionar_csv_no_binario(FILE *arq_csv, FILE *bin, NoHash *tabela[]) {
    char linha_cabecalho[MAX_LINHA_CSV];
    
    
    cabecalho reg_cab =cria_cabecalho();;
    // Garante que a struct comece limpa antes da primeira leitura
    dados reg_dados = cria_dados();
    
    
    // Arquivo já existe -> lê o cabeçalho existente
    ler_cabecalho(bin, &reg_cab);

    if (reg_cab.status != '0' && reg_cab.status != '1') {
        reg_cab = cria_cabecalho();

        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &reg_cab);
    }
    // Se o arquivo existe - mas está inconsistente
    if (reg_cab.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //volta o status p/ 0 pois estamos modificando o arquivo
    reg_cab.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &reg_cab);

    // Pula a linha de cabeçalho do CSV
    if (fgets(linha_cabecalho, MAX_LINHA_CSV, arq_csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    int rrn_atual = reg_cab.proxRRN;
    fseek(bin, calculo_byteoffset_dados(rrn_atual), SEEK_SET);

    while (ler_registro_csv(arq_csv, &reg_dados)) {

        reg_dados.removido = '0';
        reg_dados.proximo = NEGATIVO;

        // Escreve os dados estruturados no arquivo binário
        escreve_regdados(bin, &reg_dados);

        if (reg_dados.tamNomeEstacao > 0) {
            NoHash *busca = buscar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);

            if (busca == NULL) {
                inserir_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
                reg_cab.nroEstacoes++;
            } else {
                busca->repeticoes++;
            }
        }

        if (reg_dados.codProxEstacao != NEGATIVO) {
            reg_cab.nroParesEstacoes++;
        }
        
        rrn_atual++;

        reg_dados= cria_dados();
    }

    reg_cab.proxRRN = rrn_atual;
    reg_cab.status = '1'; // Arquivo fechado com sucesso e estável

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &reg_cab);
}


