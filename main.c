#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvb.h"
#include "funcoes.h"
#include "switchfunctions.h"

int main() {
    NoHash *tabela[TAM_TABELA];

    int operacao;
    char nome_csv[100];
    char nome_bin[100];
    char nome_index[100];
    int n;
    inicializar_tabela(tabela);

    while (scanf("%d", &operacao) != EOF) {
        switch (operacao){
            case 1:
                scanf("%s %s", nome_csv, nome_bin);
                create_table(tabela,nome_csv, nome_bin);
                break;  
                    
            case 2:
                scanf("%s", nome_bin);
                select_everything_from(tabela,nome_bin);
                break;

            case 3: 
                scanf("%s", nome_bin);
                select_from_where(tabela, nome_bin);
                break;
            case 4:
                scanf("%s", nome_bin);
                delete_from_where(tabela, nome_bin);
                break;
            

            case 5:
                scanf("%s %d", nome_bin, &n);
                insert_into(tabela, nome_bin, n);
                break;


            case 6:
                scanf("%s", nome_bin);
                update_table(tabela, nome_bin);
                break;

            case 7:
                scanf("%s %s", nome_bin, nome_index);
                create_index(tabela,nome_bin, nome_index);
                break;
            case 8:
                scanf("%s %s", nome_bin, nome_index);
                select_from_where_index(tabela, nome_bin, nome_index);
                break;

            case 9:
                scanf("%s %s %d", nome_bin, nome_index, &n);
                insert_into_index(tabela, nome_bin, nome_index, n);
                break;

            case 10:
                scanf("%s %s %d", nome_bin, nome_index , &n);
                delete_from_where_index(tabela, nome_bin, nome_index, n);
                break;
        }
        liberar_tabela(tabela);
        return 0;
    }
}

