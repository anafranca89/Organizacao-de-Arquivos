#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "switchfunctions.h"


void create_table(NoHash *tabela[], char* nome_csv, char * nome_bin){
    FILE *bin = cria_escreve_binario(nome_bin);
    FILE *csv = fopen(nome_csv, "r");
    if (csv == NULL || bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    //abre o csv dentro da função
    adicionar_csv_no_binario(csv, bin, tabela);
    fclose(bin);
    fclose(csv);
    BinarioNaTela(nome_bin);
}





void select_everything_from(NoHash *tabela[], char* nome_bin){
    FILE *bin = ler_binario(nome_bin);
    if (bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    carregar_nomes_no_hash(bin, tabela);
    mostrar_binario_sequencial(bin);
    fclose(bin);
}






void select_from_where(NoHash *tabela[], char* nome_bin){
    FILE *bin = ler_binario(nome_bin);
    if (bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //tabela inicializada, então armazena as estações unicas 
    // rapidez na busca
    carregar_nomes_no_hash(bin, tabela);
    int n, m;
    scanf("%d", &n);
    //são n buscas 
    for (int i = 0; i < n; i++) {
        char nomesCampos[8][50];
        char valoresCampos[8][200];
    
        scanf("%d", &m);
        // em cada busca - existe m campos a buscar
        for (int j = 0; j < m; j++) {
            ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
        }
        //faz a busca no registro de dados 
        buscar_registros(bin, tabela, m, nomesCampos, valoresCampos);
        // o registro é impresso na função de busca
        printf("\n"); // pula linha p/formatação da saída
    }
    fclose(bin);
}







void delete_from_where(NoHash *tabela[], char* nome_bin){
    //abre ou cria o arquivo p/esrita
    FILE *bin = abrir_para_escrita_binário(nome_bin);
    if (bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //LIMPA a tabela hash se o arquivo foi reescrito. 
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);

    cabecalho cab = cria_cabecalho();
    ler_cabecalho(bin, &cab);

    // começa o arquivo com inconsistente - inicia a escrita
    cab.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab);
    
    int n,m;
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        char nomesCampos[8][50];
        char valoresCampos[8][200];
        char ultimo = '0';
        scanf("%d", &m);

        for (int j = 0; j < m; j++) {
            ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
        }
        if (i == n - 1) {
            ultimo = '1';
        }
        remover_registros_dinamico(bin, tabela,&cab , m, nomesCampos, valoresCampos, ultimo);
    }
    
    fclose(bin);
    BinarioNaTela(nome_bin);
}

                







void insert_into(NoHash *tabela[], char* nome_bin , int n){
    // obtém um ponteiro para o arquivo, abrindo um novo se necessário
    FILE *bin = abrir_para_escrita_binário(nome_bin);
    // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
    if (bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    // se um novo arquivo foi criado, limpa os dados HASH
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);
    
    // sempre limpar o novo registro com todos os valores padrão
    cabecalho cab_insercao = cria_cabecalho();
    ler_cabecalho(bin, &cab_insercao);

    //arquivo inconsistente - não muda nada
    if (cab_insercao.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        return;
    }

    cab_insercao.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_insercao);

    for (int i = 0; i < n; i++) {
        dados reg_dados = cria_dados();
        // lê o registro do teclado, armazenando apropriadamente
        ler_registro_entrada(&reg_dados);
        long int byteofset;
        inserir_registro_dinamico(bin,tabela, &cab_insercao, &reg_dados, &byteofset);
    }
    // volta a colocar o arquivo como funcional
    cab_insercao.status = '1'; 
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_insercao);

    fclose(bin);
    BinarioNaTela(nome_bin);
}








void update_table(NoHash *tabela[], char* nome_bin){
    scanf("%s", nome_bin);
    FILE *bin = abrir_para_escrita_binário(nome_bin);
    // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
    if (bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);

    cabecalho  cab = cria_cabecalho();
    ler_cabecalho(bin, &cab);
    // marca o arquivo como inconsistente
    cab.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin,&cab);
    int n, m, p;
    scanf("%d", &n);
    /* para cada uma daz n vezes lê os campos pedidos e 
    atualiza os registros que atendem os critérios*/
    for (int i = 0; i < n; i++) {
        char nomesBusca[8][50];
        char valoresBusca[8][200];
        char nomesAtualiza[8][50];
        char valoresAtualiza[8][200];

        scanf("%d", &m);

        for (int j = 0; j < m; j++) {
            ler_par_campo_valor(nomesBusca[j], valoresBusca[j]);
        }

        scanf("%d", &p);

        for (int j = 0; j < p; j++) {
            ler_par_campo_valor(nomesAtualiza[j], valoresAtualiza[j]);
        }

        atualizar_registros_dinamico(bin, tabela,m, nomesBusca, valoresBusca, 
                                        p, nomesAtualiza, valoresAtualiza);
    }
    // volta o arquivo como consistente 
    cab.status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab);
    fclose(bin);
    BinarioNaTela(nome_bin);

}






void create_index(NoHash *tabela[], char* nome_bin, char*nome_index){
    FILE* bin = ler_binario(nome_bin);
    liberar_tabela(tabela);
    FILE * index = abrir_para_escrita_binário(nome_index);
    if (index == NULL|| bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    //carregar_nomes_no_hash(bin, tabela);
    cria_arvore(bin, index);
    fclose(bin);
    fclose(index);
    BinarioNaTela(nome_index);
}




void select_from_where_index(NoHash *tabela[], char* nome_bin, char*nome_index){

    FILE *bin = ler_binario(nome_bin);
    FILE* index = ler_binario(nome_index);
    if (index == NULL|| bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);

    cab_indice ci = new_cab_indice();
    ler_ind_cabecalho(index, &ci);
    int n, m;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        char nomesCampos[8][50];
        char valoresCampos[8][200];

        scanf("%d", &m);

        for (int j = 0; j < m; j++) {
            ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
        }

        int possui_cod_estacao = 0;
        int chave_buscada;

        for (int j = 0; j < m; j++) {
            if (strcmp(nomesCampos[j], "codEstacao") == 0) {
                possui_cod_estacao = 1;
                chave_buscada = atoi(valoresCampos[j]);
                break;
            }
        }

        fseek(index, 0, SEEK_SET);


        if(possui_cod_estacao){
            //retorna o rrn da busca em arvoreB
            int posicao_na_pagina=-1;
            int rrn_encontrado = busca_arvore(index, ci.noRaiz, chave_buscada, &posicao_na_pagina);
            
            //temos Um rrn e a chave existe dentro desse nó?
            if (rrn_encontrado != -1 && posicao_na_pagina != -1) {
                //então vamos ler o conteudo da pagina
                indice no_atual;
                fseek(index, calculo_byteoffset_indice(rrn_encontrado), SEEK_SET);
                ler_indice(index, &no_atual);

                long byte_dados = NEGATIVO;
                if (posicao_na_pagina == 1) byte_dados = no_atual.Pr1;
                else if (posicao_na_pagina == 2) byte_dados = no_atual.Pr2;
                else if (posicao_na_pagina == 3) byte_dados = no_atual.Pr3;

                if (byte_dados != -1) {
                    dados reg_dados;
                    memset(&reg_dados, 0, sizeof(dados));
                    
                    fseek(bin, byte_dados, SEEK_SET);
                    ler_regdados(bin, &reg_dados);
                    
                    if (reg_dados.removido != '1' && verificar_criterios(m, nomesCampos, valoresCampos, &reg_dados)) {
                        imprime_registro_dados(&reg_dados);
                    } else {
                        printf("Registro inexistente.\n");
                    }
                } else {
                    printf("Registro inexistente.\n");
                }
            } else {
                // Se a árvore retornou -1, o código de estação simplesmente não existe
                printf("Registro inexistente.\n");
            }
        }else{
            buscar_registros(bin,tabela,m,nomesCampos,valoresCampos);
        }
        printf("\n");
    }
    fclose(index);
    fclose(bin);
}
 


void insert_into_index(NoHash *tabela[], char* nome_bin,char*nome_index, int n){

    FILE *bin = escrever_binario(nome_bin);
    FILE *index = escrever_binario(nome_index);
    if (index == NULL|| bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);
    

    cabecalho cab_insercao= cria_cabecalho();
    cab_indice cab_index = new_cab_indice();

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &cab_insercao);

    fseek(index, 0, SEEK_SET);
    ler_ind_cabecalho(index, &cab_index);

    if (cab_insercao.status == '0' || cab_index.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        fclose(index);
        return;
    }

    //iniciou a inserção - cabeçalho inconsistente
    cab_insercao.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_insercao);

    cab_index.status = '0';
    fseek(index, 0, SEEK_SET);
    escreve_ind_cabecalho(index, &cab_index);

    // int teve_insercao = 0;
    for (int i = 0; i < n; i++) {
        dados reg_inserido = cria_dados();
        ler_registro_entrada(&reg_inserido);

        int pos_chave_no9 = NEGATIVO;

        if (reg_inserido.codEstacao != NEGATIVO) {
            busca_arvore(index, cab_index.noRaiz, reg_inserido.codEstacao, &pos_chave_no9);
        }

        if (pos_chave_no9 != NEGATIVO) {
            //chave duplicada
            continue;
        }
        long int byteoffset9; 
        inserir_registro_dinamico(bin, tabela, &cab_insercao, &reg_inserido, &byteoffset9);
        //teve_insercao = 1;

        // CORREÇÃO 1: Declarando as variáveis de promoção ANTES de chamar a função
        int filho_promovido, chave_promovida, byte_dados_promovido;

        // Agora sim, passando 'byteoffset9' corretamente
        int retorno_prom = insere_arvore(index, &cab_index, cab_index.noRaiz, reg_inserido.codEstacao, byteoffset9,      
                                        &filho_promovido, &chave_promovida, &byte_dados_promovido);
            
        // Trata o split da raiz se a promoção subir até o topo
        if (retorno_prom == PROMOTION) {
            indice nova_raiz = new_indice();
            
            if (cab_index.noRaiz == NEGATIVO) {
                nova_raiz.tipoNo = NEGATIVO; // Primeira raiz da árvore (folha)
            } else {
                nova_raiz.tipoNo = 0; // Nó raiz intermediário
            }
        
            nova_raiz.nroChaves = 1;
            nova_raiz.C1 = chave_promovida;
            nova_raiz.Pr1 = byte_dados_promovido;
            
            nova_raiz.arv1 = cab_index.noRaiz; 
            nova_raiz.arv2 = filho_promovido;
            nova_raiz.arv3 = NEGATIVO;
            nova_raiz.arv4 = NEGATIVO;

            nova_raiz.C2 = NEGATIVO;
            nova_raiz.C3 = NEGATIVO;
            nova_raiz.Pr2 = NEGATIVO;
            nova_raiz.Pr3 = NEGATIVO;
            
            int rrn_nova_raiz = cab_index.proxRRN;
            int byte_new_raiz = calculo_byteoffset_indice(rrn_nova_raiz);
            
            fseek(index, byte_new_raiz, SEEK_SET);
            escreve_indice(index, &nova_raiz);
            
            cab_index.noRaiz = rrn_nova_raiz;
            cab_index.proxRRN++;
            cab_index.nroNos++;
        }
    }


    
    cab_insercao.status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_insercao);

    cab_index.status = '1';
    fseek(index, 0, SEEK_SET);
    escreve_ind_cabecalho(index, &cab_index);


    fclose(bin);
    fclose(index);

    BinarioNaTela(nome_bin);
    BinarioNaTela(nome_index);
}








void delete_from_where_index(NoHash *tabela[], char* nome_bin, char*nome_index, int n){
    FILE*bin = escrever_binario(nome_bin);
    FILE *index = escrever_binario(nome_index);
    if (index == NULL|| bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    liberar_tabela(tabela);
    carregar_nomes_no_hash(bin, tabela);
    
    cabecalho cab_dados=cria_cabecalho();
    cab_indice cab_arv;

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &cab_dados);

    fseek(index, 0, SEEK_SET);
    ler_ind_cabecalho(index, &cab_arv);

    if (cab_dados.status == '0' || cab_arv.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        fclose(index);
        return;
    }

    cab_dados.status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_dados);

    cab_arv.status = '0';
    fseek(index, 0, SEEK_SET);
    escreve_ind_cabecalho(index, &cab_arv);
    

    for (int i = 0; i < n; i++) {
        char nomesCampos10[8][50];
        char valoresCampos10[8][200];
        int m;
        scanf("%d", &m);

        for (int j = 0; j < m; j++) {
            ler_par_campo_valor(
                nomesCampos10[j],
                valoresCampos10[j]
            );
        }

        remover_registros_dinamico_com_arvore(bin,index,tabela,&cab_dados,
                                                &cab_arv,m,nomesCampos10,valoresCampos10);
    }

    cab_dados.status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &cab_dados);

    cab_arv.status = '1';
    fseek(index, 0, SEEK_SET);
    escreve_ind_cabecalho(index, &cab_arv);

    fclose(bin);
    fclose(index);

    BinarioNaTela(nome_bin);
    BinarioNaTela(nome_index);

}