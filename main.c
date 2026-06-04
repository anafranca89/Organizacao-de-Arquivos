#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "arvb.h"
#include "funcoes.h"
#include "fornecidas.h"

int main() {
    NoHash *tabela[TAM_TABELA];
    ArquivoAberto *lista_arquivos = NULL;

    int operacao;
    char nome_csv[100];
    char nome_bin[100];

    inicializar_tabela(tabela);

    while (scanf("%d", &operacao) != EOF) {

        if (operacao == 1) {
            scanf("%s %s", nome_csv, nome_bin);

            FILE *csv = fopen(nome_csv, "r");
            FILE *bin = fopen(nome_bin, "w+b");

            if (csv == NULL || bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                if (csv != NULL) fclose(csv);
                if (bin != NULL) fclose(bin);
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            adicionar_csv_no_binario(csv, bin, tabela);

            if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                adicionar_arquivo_processado(&lista_arquivos, nome_bin);
            }

            fclose(csv);
            fclose(bin);

            BinarioNaTela(nome_bin);
        }

        else if (operacao == 2) {
            scanf("%s", nome_bin);

            FILE *bin = fopen(nome_bin, "rb");

            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                adicionar_arquivo_processado(&lista_arquivos, nome_bin);
                carregar_nomes_no_hash(bin, tabela);
            }

            mostrar_binario_sequencial(bin);
            fclose(bin);

            
        }

    else if (operacao == 3) {
    int n, i, j, m;

    scanf("%s", nome_bin);

    FILE *bin = fopen(nome_bin, "rb");

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        liberar_tabela(tabela);
        liberar_lista_arquivos(lista_arquivos);
        return 0;
    }

    if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
        adicionar_arquivo_processado(&lista_arquivos, nome_bin);
        carregar_nomes_no_hash(bin, tabela);
    }

    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        char nomesCampos[8][50];
        char valoresCampos[8][200];

        scanf("%d", &m);

        for (j = 0; j < m; j++) {
            scanf("%s", nomesCampos[j]);

            if (strcmp(nomesCampos[j], "nomeEstacao") == 0 ||
                strcmp(nomesCampos[j], "nomeLinha") == 0) {
                ScanQuoteString(valoresCampos[j]);
            } else {
                scanf("%s", valoresCampos[j]);

                if (strcmp(valoresCampos[j], "NULO") == 0 ||
                    strcmp(valoresCampos[j], "nulo") == 0) {
                    strcpy(valoresCampos[j], "");
                }
            }
        }

        buscar_registros(bin, tabela, m, nomesCampos, valoresCampos);
        printf("\n");
    }

    fclose(bin);
    } else if(operacao==4){
        /*DELETE FROM -
        Remoção lógica de registros - atualizando o topo da pilha e o status do registro*/
        //int n, i, j, m;
            /**/

    }

    liberar_tabela(tabela);
    liberar_lista_arquivos(lista_arquivos);
    return 0;
}

}