#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.c"

#define MAX_TAM_REG 80

// faz a busca no arquivo .csv de acordo com o codigo primeiro campo
void busca_codigo(int CodEstacao, FILE* ponteiro_arquivo) {

    if (ponteiro_arquivo == NULL) {
        // seguindo as especificações, essa eh a mensagem caso der algum erro
        printf("Falha no processamento do arquivo.");
        return;
    }

    char registro[MAX_TAM_REG];
    int encontrado = 0;

    // guarda a posição atual do ponteiro
    long posicao_inicial = ftell(ponteiro_arquivo);

    // se o arquivo estiver no começo, pula o cabeçalho
    if (posicao_inicial == 0) {
        if (fgets(registro, MAX_TAM_REG, ponteiro_arquivo) == NULL) {
            printf("Falha no processamento do arquivo.");
            return;
        }
        posicao_inicial = ftell(ponteiro_arquivo);
    }

    // primeiro busca da posição atual até o final do arquivo
    while (fgets(registro, MAX_TAM_REG, ponteiro_arquivo) != NULL) {

        registro[strcspn(registro, "\n")] = '\0';

        char registro_original[MAX_TAM_REG];
        strcpy(registro_original, registro);

        char *token = strtok(registro, ",");
        int codEstacao_lido = (token != NULL) ? atoi(token) : -1;

        if (codEstacao_lido == CodEstacao) {
            printf("%s\n", registro_original);
            encontrado = 1;
        }
    }

    // depois se nao encontrou, volta ao início e busca até a posição inicial
    if (!encontrado) {
        fseek(ponteiro_arquivo, 0, SEEK_SET);
        clearerr(ponteiro_arquivo);
        // pula cabeçalho
        if (fgets(registro, MAX_TAM_REG, ponteiro_arquivo) == NULL) {
            printf("Falha no processamento do arquivo.");
            return;
        }

        while (ftell(ponteiro_arquivo) < posicao_inicial &&
               fgets(registro, MAX_TAM_REG, ponteiro_arquivo) != NULL) {

            registro[strcspn(registro, "\n")] = '\0';

            char registro_original[MAX_TAM_REG];
            strcpy(registro_original, registro);

            char *token = strtok(registro, ",");
            int codEstacao_lido = (token != NULL) ? atoi(token) : -1;

            if (codEstacao_lido == CodEstacao) {
                printf("%s\n", registro_original);
                encontrado = 1;
            }
        }
    }

    if (!encontrado) {
        printf("COD %d nao encontrado.\n", CodEstacao);
    }
}

/*
    busca um registro de acordo com o campo
    a mascara é simplesmente os campos que queremos buscar, por exemplo, caso queremos 
    buscar o codigo e nome, colocamos a mascara 11000000
    o unsigned char eh pq tem tamanho de exatamente um byte
*/

void busca_mascara(FILE *ponteiro_arquivo, unsigned char mascara, char valores[8][25]) {
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char linha[MAX_TAM_REG];
    int encontrados = 0;

    long posicao_inicial = ftell(ponteiro_arquivo);

    // se começou no início do arquivo, pula o cabeçalho
    if (posicao_inicial == 0) {
        if (fgets(linha, MAX_TAM_REG, ponteiro_arquivo) == NULL) {
            printf("Falha no processamento do arquivo.\n");
            return;
        }
        posicao_inicial = ftell(ponteiro_arquivo);
    }

    // primeiro buscamos da posição atual até o final
    while (fgets(linha, MAX_TAM_REG, ponteiro_arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        char linha_original[MAX_TAM_REG];
        strcpy(linha_original, linha);

        char *campos[8];
        int i = 0;

        char *token = strtok(linha, ",");
        while (token != NULL && i < 8) {
            campos[i++] = token;
            token = strtok(NULL, ",");
        }

        while (i < 8) {
            campos[i++] = "";
        }

        int ok = 1;

        for (int j = 0; j < 8; j++) {
            if (mascara & (1 << j)) {
                if (strcmp(campos[j], valores[j]) != 0) {
                    ok = 0;
                    break;
                }
            }
        }

        if (ok) {
            printf("%s\n", linha_original);
            encontrados++;
        }
    }

    // chegou no final, entao limpa EOF e volta ao início
    clearerr(ponteiro_arquivo);
    fseek(ponteiro_arquivo, 0, SEEK_SET);

    // pula cabeçalho
    if (fgets(linha, MAX_TAM_REG, ponteiro_arquivo) == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    } 

    // depois busca do início até a posição inicial
    while (ftell(ponteiro_arquivo) < posicao_inicial && fgets(linha, MAX_TAM_REG, ponteiro_arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        char linha_original[MAX_TAM_REG];
        strcpy(linha_original, linha);

        char *campos[8];
        int i = 0;

        char *token = strtok(linha, ",");
        while (token != NULL && i < 8) {
            campos[i++] = token;
            token = strtok(NULL, ",");
        }

        while (i < 8) {
            campos[i++] = "";
        }

        int ok = 1;

        for (int j = 0; j < 8; j++) {
            if (mascara & (1 << j)) {
                if (strcmp(campos[j], valores[j]) != 0) {
                    ok = 0;
                    break;
                }
            }
        }

        if (ok) {
            printf("%s\n", linha_original);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum registro encontrado.\n");
    }
}




/* Função que lê os m nomeCampo e dadoCampo. Busca no arquivo binário os registros que tem esses campos e valores.
Retorna a lista de rrn com esses valores .*/

int* busca_registro(int m){
    int *rrns = (int*)malloc(m * sizeof(int));
    for (int i=0; i<m; i++){
        char nomeCampo[50];
        char valorCampo[50];
        scanf("%s %s", nomeCampo, valorCampo);
        //busca o campo e o valor no arquivo binário. Se encontrar, retorna o RRN do registro.
        rrns = //rrn da busca;
    }
    

}







/* Função para ler o arquivo binário, e remover lógicamente os registros de dados
Delete from --- where*/

void delete (char* binario, int n){
    //chama função pra ler o arquivo binário.
    cabecalho regcab = ler_cabecalho(binario);

    for(int i=0; i<n; i++){
        //ler os n reg a serem removidos
        //m  nomeCampo1 valorCampo1 ... nomeCampom valorCampom
        // ler a qtd m de campos a buscar
        /*Usa uma função pra buscar os registros com esses campos.
        Retorna o rrn dos registros encontrados.
        */
        
        //se o registro é encontrado, 
        if(RRN< regcab.proxRRN){
            int byteoffset = RRN *80;
            fseek(binario,  byteoffset, SEEK_SET);
            dados registro = ler_regdados(binario, byteoffset);
            if(registro.removido == '0'){
                /*registro encontrado e n está removido
                Então: marca como removido == 1; atualiza o topo e o proximo registro removido*/
                registro.removido = '1';
                registro.proximo = regcab.topo;
                regcab.topo = RRN;
                escreve_cabecalho(binario, regcab);
                escreve_regdados(binario, registro, byteoffset);

            }
        } else{
            //registro nao encontrado
            printf("Registro nao encontrado\n");
        }
    
    }

    BinarionaTela(binario);
}




/* Função Atualização UPDATE.

*/


void update (char* binario, int n){
    //chama função pra ler o arquivo binário.
    cabecalho regcab = ler_cabecalho(binario);

    for(int i=0; i<n; i++){
        int byteoffset;
        //ler os n reg a serem atualizados
        //m  nomeCampo1 valorCampo1 ... nomeCampom valorCampom
        // ler a qtd m de campos a buscar
        /*Usa uma função pra buscar os registros com esses campos.
        Retorna o rrn dos registros encontrados.
        */
       // ler o p nomecampo valor campo a ser atualizado.
        
        //se o registro é encontrado, 
        if(regcab.topo == -1){
            byteoffset = regcab.proxRRN *80;
            fseek(binario,  byteoffset, SEEK_SET);
            dados registro = ler_regdados(binario, byteoffset);
            /*
            atualiza os campos necessários
            */
            escreve_regdados(binario, registro, byteoffset);
        } else{
            byteoffset = regcab.topo * 80;
            fseek(binario,  byteoffset, SEEK_SET);
            regcab.topo = RRN;
            escreve_cabecalho(binario, regcab);
            dados registro = ler_regdados(binario, byteoffset);
            /*
            atualiza os campos necessários
            */
            escreve_regdados(binario, registro, byteoffset);
        }

    
    }

    BinarionaTela(binario);
}
