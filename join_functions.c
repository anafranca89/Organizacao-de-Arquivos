#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
    if(strcmp(campo1, "codProxEstacao")!= 0 || strcmp(campo2, "codEstacao")!=0  ){
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
        if (dados1.removido == '1') continue;
        //volta p/inicio dos registros
        fseek(bin2, TAM_CABECALHO, SEEK_SET);
        for (int j =0; j <cab2.proxRRN; j++){
            ler_regdados(bin2, &dados2);
            // é removido?
            if (dados2.removido == '1') continue;
            //n é removido e encontrou par
            if(dados1.codProxEstacao == dados2.codEstacao  ){
                imprime_join_regdados(&dados1, &dados2);
                count_registros++;
            }
        }
    }

    //nao teve juncao
    if(count_registros ==0){
        printf("Registro Inexistente");
    }


    fclose(bin);
    fclose(bin2);
}




void junction_join(char *arq1, char *campo1, char *arq2, char *campo2, char *arq_index){
    FILE *bin = ler_binario(arq1);
    FILE *bin2 = ler_binario(arq2);
    FILE *arq_ind= ler_binario(arq_index);
    if (bin == NULL || bin2 == NULL || arq_ind == NULL){
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
    cab_indice cabindex;
    dados dados1, dados2;
    cab = cria_cabecalho();    cab2= cria_cabecalho();
    ler_cabecalho(bin, &cab);
    ler_cabecalho(bin2, &cab2);
    ler_ind_cabecalho(arq_ind, &cabindex);

    //teste de consistência do INDEX
    if(cabindex.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    int count_registros=0; // se nao teve junção



    //para cada registro no arquivo 1 - busca usando arvore B no arquivo 2
    for(int i =0; i <cab.proxRRN; i++){
        ler_regdados(bin, &dados1);

        int posicao_na_pagina=-1;
        int rrn_encontrado = busca_arvore(arq_ind, cabindex.noRaiz, dados1.codProxEstacao, &posicao_na_pagina);
        
        //temos Um rrn e a chave existe dentro desse nó?
        if (rrn_encontrado != -1 && posicao_na_pagina != -1) {
            //então vamos ler o conteudo da pagina
            indice no_atual;
            fseek(arq_ind, calculo_byteoffset_indice(rrn_encontrado), SEEK_SET);
            ler_indice(arq_ind, &no_atual);

            long byte_dados = NEGATIVO;
            if (posicao_na_pagina == 1) byte_dados = no_atual.Pr1;
            else if (posicao_na_pagina == 2) byte_dados = no_atual.Pr2;
            else if (posicao_na_pagina == 3) byte_dados = no_atual.Pr3;

            //achou o registro então busca os dados no arquivo de dados
            fseek(bin2, byte_dados, SEEK_SET);
            ler_regdados(bin2, &dados2);
            if (byte_dados != -1) {
                //achou? imprime da forma padronizada JOIN
                imprime_join_regdados(&dados1, &dados2);
                count_registros++;
                //senao continua prox iteracao
            }
        }
    }

    //nao teve juncao
    if(count_registros ==0){
        printf("Registro Inexistente");
    }

    fclose(bin);
    fclose(bin2);
    fclose(arq_ind);

}



//ordenacao = 1-> CoEstacao; 2-> codProxEstacao
int ordenacao = 1;


//Função para comparar Registros
int compara_dados(const void *a, const void *b ) {
    dados *regA = (dados *)a;
    dados *regB = (dados *)b;
    
    int valA, valB;

    // Define qual campo extrair baseado na configuração genérica
    if (ordenacao == 1) {
        valA = regA->codEstacao;
        valB = regB->codEstacao;
    } else {
        valA = regA->codProxEstacao;
        valB = regB->codProxEstacao;
    }

    // comparação
    // Se ambos forem nulos, são iguais
    if (valA == NEGATIVO && valB == NEGATIVO) return 0;
    // Se A for nulo, A é 'maior' q B
    if (valA == NEGATIVO) return 1;
    // Se A for nulo, A é 'menor' q B
    if (valB == NEGATIVO) return -1;

    // ordenação crescente padrão para valores não nulos
    if (valA < valB) return -1;
    if (valA > valB) return 1;
    return 0;
}

void ordena_arquivo_geral(FILE *bin, FILE *arq_ordenado ){
    if (bin == NULL || arq_ordenado == NULL) return;
    //ponteiro deve estar no começo do arquivo!
    cabecalho cab;
    ler_cabecalho(bin, &cab);

    //LE  o arquivo 1 inteiro usando realloc de 50 em 50
    int qtd_registros = 0,capacidade= 50;
    dados *lista_registros = malloc(capacidade * sizeof(dados));
    // Loop de leitura até o fim do arquivo
    for(int i=0; i<cab.proxRRN; i++) {
        dados reg_aux;
        ler_regdados(bin, &reg_aux);
       
        //É removido - ignora
        if (reg_aux.removido == '1') continue;
        
        //nao é removido 
        lista_registros[qtd_registros] = reg_aux;
        qtd_registros++;

        // Se atingiu o limite atual, expande mais 50 na capacidade da lista
        if (qtd_registros == capacidade) {
            capacidade += 50;
            lista_registros = realloc(lista_registros, capacidade * sizeof(dados));
        }
    }

    //ordena com qsort usando ordenação 1 ou 2.- funcao comparadados
    qsort(lista_registros, qtd_registros, sizeof(dados), compara_dados);

    cab.status = '0'; 
    cab.proxRRN = qtd_registros; 
    escreve_cabecalho(arq_ordenado, &cab);

    //escreve todos os registros
    for (int i = 0; i < qtd_registros; i++) {
        escreve_regdados(arq_ordenado, &lista_registros[i]);
    }

    // arquivo consistente 
    cab.status = '1';
    fseek(arq_ordenado, 0, SEEK_SET);
    escreve_cabecalho(arq_ordenado, &cab);
    free(lista_registros);

}





void order_join(char *arq1, char *campo1, char *arq2){
    FILE *bin = ler_binario(arq1);
    FILE *arq_ordenado = abrir_para_escrita_binário(arq2);
    if(bin == NULL || arq_ordenado==NULL){
        printf("Falha no processamento do arquivo.\n");
        if(bin != NULL) fclose(bin);
        if(arq_ordenado != NULL) fclose(arq_ordenado);
        return;
    } 

    //determina qual é a ordenacao
    if(strcmp(campo1, "codEstacao")==0){
        ordenacao = 1;
    }else if(strcmp(campo1, "codProxEstacao")==0){
        ordenacao = 2;
    }else{
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        fclose(arq_ordenado);
        return;
    } 
    ordena_arquivo_geral(bin, arq_ordenado);
    fclose(bin);
    fclose(arq_ordenado);
    BinarioNaTela(arq2);
}








int avanca_A(FILE *bin, dados *reg, int *RRN, int totalRRN) {
    while (*RRN < totalRRN) {
        ler_regdados(bin, reg);
        (*RRN)++;
        if (reg->removido != '1') return 1; // nao foi removido
    }
    return 0;
}

int avanca_B(FILE *bin, dados *reg, int *RRN, int totalRRN) {
    while (*RRN < totalRRN) {
        ler_regdados(bin, reg);
        (*RRN)++;
        if (reg->removido != '1') return 1; // registro válido
    }
    return 0; 
}






void merge_sort_join(char *arq1, char *campo1, char *arq2, char *campo2){
    FILE *bin = ler_binario(arq1);
    FILE *bin2 = ler_binario(arq2);
    if(bin == NULL || bin2==NULL ){
        printf("Falha no processamento do arquivo.\n");
        if(bin != NULL) fclose(bin);
        if(bin2 != NULL) fclose(bin2);
        return;
    } 

    //cria arq temporatios que serao os ordenados
    FILE *temp1 = cria_escreve_binario("temp1.bin");
    FILE *temp2 = cria_escreve_binario("temp2.bin");
    
    // Ordenar os arquivos 1 e 2
    //Pelo codestacao ou proxestacao?
    if (strcmp(campo1, "codProxEstacao") == 0) {
        ordenacao = 2;
    }else {
        return;
    }
    ordena_arquivo_geral(bin, temp1);

    if (strcmp(campo2, "codEstacao") == 0) ordenacao = 1;
    else return;
    ordena_arquivo_geral(bin2, temp2);
    fclose(bin); fclose(bin2);
    fclose(temp1); fclose(temp2);

    //le ambos os arquivos ao mesmo tempo
    FILE *f1_ordenado = ler_binario("temp1.bin");
    FILE *f2_ordenado = ler_binario("temp2.bin");


    cabecalho cab1, cab2;
    ler_cabecalho(f1_ordenado, &cab1);
    ler_cabecalho(f2_ordenado, &cab2);

    int rrn1 = 0, rrn2 = 0;
    dados reg1, reg2;

    int tem1 = avanca_A(f1_ordenado, &reg1, &rrn1, cab1.proxRRN);
    int tem2 = avanca_B(f2_ordenado, &reg2, &rrn2, cab2.proxRRN);

    int encontrou_match = 0;

    while (tem1 && tem2) {
        int key1, key2;

        // Extrai a chave do arquivo 1 dependendo do campo1
        if (strcmp(campo1, "codProxEstacao") == 0) key1 = reg1.codProxEstacao;
        else key1 = reg1.codEstacao;

        // Extrai a chave do arquivo 2 dependendo do campo2
        if (strcmp(campo2, "codEstacao") == 0) key2 = reg2.codEstacao;
        else key2 = reg2.codProxEstacao;

        // Se chegar nos valores nulos/vazios que o qsort jogou para o fim, encerra
        if (key1 == NEGATIVO || key2 == NEGATIVO) {
            break;
        }

        if (key1 < key2) {
            // Chave do arquivo 1 é menor, avança arquivo 1
            tem1 = avanca_A(f1_ordenado, &reg1, &rrn1, cab1.proxRRN);
        } 
        else if (key1 > key2) {
            // Chave do arquivo 2 é menor, avança arquivo 2
            tem2 = avanca_B(f2_ordenado, &reg2, &rrn2, cab2.proxRRN);
        } 
        else {
            // achou o par - printa o registro
            imprime_join_regdados(&reg1, &reg2);
            encontrou_match = 1;

            // Avança ambos para continuar a busca
            tem1 = avanca_A(f1_ordenado, &reg1, &rrn1, cab1.proxRRN);
            tem2 = avanca_B(f2_ordenado, &reg2, &rrn2, cab2.proxRRN);
        }
    }

    if (!encontrou_match) {
        printf("Registro inexistente.\n");
    }

    fclose(f1_ordenado);
    fclose(f2_ordenado);

    //remover os arquivo temporarios
    remove("temp1.bin");
    remove("temp2.bin");


}