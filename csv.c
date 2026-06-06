#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "fornecidas.h"


// pula a vírgula do csv usando o ponteiro
static void pular_virgula(char **p) {
    if (**p == ',') (*p)++;
}

static int ler_inteiro_csv(char **p) {
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


// lê uma string no csv
static int ler_string_csv(char **p, char *destino) {
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
   e retorna as informações preenchidas diretamente na struct 'dados' */
int ler_registro_csv(FILE *csv, dados *reg_dados) {
    char linha[MAX_LINHA_CSV];
    char *p;
    int valor, tem_digito, i;

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








void adicionar_csv_no_binario(char *arq_csv, char *arq_bin, NoHash *tabela[]) {
    char linha_cabecalho[MAX_LINHA_CSV];
    
    FILE *csv = fopen(arq_csv, "r");
    if (csv == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    cabecalho reg_cab;
    dados reg_dados;

    FILE *bin = escrever_binario(arq_bin); 
    
    if (bin == NULL) {
        // Arquivo não existe -> cria e abre para escrita
        bin = cria_escreve_binario(arq_bin); 
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            fclose(csv); // CORREÇÃO 3: Evita vazamento do descritor do CSV
            return;
        }
        reg_cab = cria_cabecalho();
        escreve_cabecalho(bin, &reg_cab);
    } else {
        // Arquivo já existe -> lê o cabeçalho existente
        ler_cabecalho(bin, &reg_cab);
        
        // Se o arquivo existe - mas está inconsistente
        if (reg_cab.status == '0') {
            printf("Falha no processamento do arquivo (Arquivo corrompido).\n");
            fclose(bin);
            fclose(csv);
            return;
        }

        //volta o status p/ 0 pois estamos modificando o arquivo
        reg_cab.status = '0';
        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &reg_cab);
    }
    
    // Pula a linha de cabeçalho do CSV
    if (fgets(linha_cabecalho, MAX_LINHA_CSV, csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        fclose(csv);
        return;
    }
    
    int rrn_atual = reg_cab.proxRRN;
    int byteoffset = calculo_byteoffset_dados(rrn_atual);
    fseek(bin, byteoffset, SEEK_SET);
    // Garante que a struct comece limpa antes da primeira leitura
    memset(&reg_dados, 0, sizeof(dados));

    while (ler_registro_csv(csv, &reg_dados)) {

        reg_dados.removido = '0';
        reg_dados.proximo = NEGATIVO;

        // Escreve os dados estruturados no arquivo binário
        escreve_regdados(bin, &reg_dados);

        // Gerenciamento de metadados via tabela Hash usando as strings lidas
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

        memset(&reg_dados, 0, sizeof(dados));
    }

    reg_cab.proxRRN = rrn_atual;
    reg_cab.status = '1'; // Arquivo fechado com sucesso e estável

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &reg_cab);

    fclose(bin);
    fclose(csv);
}


