#include <stdio.h>
#include <stdlib.h>
#include "registros.h"
#include "funcoes.h"
#include "fornecidas.h"

int main() {
    NoHash *tabela[TAM_TABELA];
    int operacao;
    char nome_csv[100];
    char nome_bin[100];

    inicializar_tabela(tabela);

    while (scanf("%d", &operacao) != EOF) {

        if (operacao == 1) {
            /*Primeira operação: CREATE TABLE -> Dado o arquivo csv, lê as informações, armazenada apropriadamente como registro no arquivo 
            binário*/
            scanf("%s %s", nome_csv, nome_bin);

            FILE *csv = fopen(nome_csv, "r");
            FILE *bin = fopen(nome_bin, "w+b");
            
            if (csv == NULL || bin == NULL) {
                // Mensagems de erro. Fecha os arquivos que foram abertos e libera a memória
                printf("Falha no processamento do arquivo.\n");
                if (csv != NULL) fclose(csv);
                if (bin != NULL) fclose(bin);
                liberar_tabela(tabela);
                return 0;
            }

            adicionar_csv_no_binario(csv, bin, tabela);

            fclose(csv);
            fclose(bin);

            BinarioNaTela(nome_bin);
        }

        else if (operacao == 2) {
            /*Operação 2: SELECT
            Mostra os registros guardados no arquivo binário, de forma sequencial.
            */
            scanf("%s", nome_bin);

            FILE *bin = fopen(nome_bin, "rb");

            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                return 0;
            }

            mostrar_binario_sequencial(bin);
            fclose(bin);
        }
    
    else if (operacao == 3) {
        /*Operação de SELECT - WHERE 
        Imprime todos os registros que têm os campos mencionados
        */
        int n, i, j, m;

        scanf("%s", nome_bin);

        FILE *bin = fopen(nome_bin, "rb");

        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            liberar_tabela(tabela);
            return 0;
        }

        // N é a quantidade de buscas a serem feitas
        scanf("%d", &n);

        for (i = 0; i < n; i++) {
            char nomesCampos[10][50];
            char valoresCampos[10][200];
            // Para cada busca - m campos
            scanf("%d", &m);

            for (j = 0; j < m; j++) {
                //ler o nome do campo procurado - o valor do campo pode estar entre aspas ou nao
                //a função ScanQuoteString cuida disso
                scanf("%s", nomesCampos[j]);
                ScanQuoteString(valoresCampos[j]);
            }

            buscar_registros(bin, tabela, m, nomesCampos, valoresCampos);
    }

    fclose(bin);
}
    }

    liberar_tabela(tabela);
    return 0;
}
