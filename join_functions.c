#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "join_functions.h"



void nested_join(char *nome_bin, char *campo1, char *nome_bin2, char *campo2)
{
    FILE *bin = ler_binario(nome_bin);
    FILE *bin2 = ler_binario(nome_bin2);
    if (bin == NULL || bin2 == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //verifica se o campo de juncao é codEstacao - senao é erro
    if(strcmp(campo1, "codProxEstacao") || strcmp(campo2, "codEstacao")  ){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //quantos registro existem nos aruivos?
    cabecalho cab, cab2;
    dados dados1, dados2;
    cab = cria_cabecalho();    cab2= cria_cabecalho();
    ler_cabecalho(bin, &cab);
    ler_cabecalho(bin2, &cab2);
    int count_registros=0; // se nao teve junção

    //LOOP aninhado - para cada reg no arq1 tenta encontra o par no arquivo 2
    for(int i =0; i <cab.proxRRN; i++){
        //fseek(bin, calculo_byteoffset_dados(i), SEEK_SET);
        ler_regdados(bin, &dados1);

        //volta p/inicio dos registros
        fseek(bin2, TAM_CABECALHO, SEEK_SET);
        for (int j =0; j <cab2.proxRRN; j++){
            ler_regdados(bin2, &dados2);
            //encontrou par
            if(dados1.codProxEstacao == dados2.codEstacao){
                imprime_join_regdados(&dados1, &dados2);
                count_registros++;
            }
        }
    }

    //nao teve juncao
    if(count_registros ==0){
        printf("Registro Inexistente");
    }
}
