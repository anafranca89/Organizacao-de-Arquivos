#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "switchfunctions.h"
#include "join_functions.h"

/**
 * Ana Julia Lopez Franqueira de França - 16838230
 * Leonardo Biondo Bertho - 16881900
 */


int main() {
    NoHash *tabela[TAM_TABELA];

    int operacao;
    char nome_csv[50];
    char nome_bin[50];
    char nome_bin2[50];
    char nome_index[50];
    
    char campo1[50];
    char campo2[50];
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
                delete_from_where(tabela, nome_bin);
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

            case 11:
                scanf("%s %s %s %s", nome_bin, campo1, nome_bin2 , campo2);
                nested_join(nome_bin,campo1, nome_bin2 , campo2 );
                break;
            case 12:
                scanf("%s %s %s %s %s", nome_bin, campo1, nome_bin2 , campo2, nome_index);
                junction_join(nome_bin,campo1, nome_bin2 , campo2, nome_index);
                break;
            case 13:
                scanf("%s %s %s", nome_bin, campo1, nome_bin2);
                order_join(nome_bin,campo1, nome_bin2);
                break;
            case 14:
                scanf("%s %s %s %s", nome_bin, campo1, nome_bin2, campo2);
                merge_sort_join(nome_bin, campo1, nome_bin2, campo2);
                break;

        }
        liberar_tabela(tabela);
        return 0;
    }
}

