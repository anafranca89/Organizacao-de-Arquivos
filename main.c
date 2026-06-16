#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvb.h"
#include "funcoes.h"

int main() {
    NoHash *tabela[TAM_TABELA];

    int operacao;
    char nome_csv[100];
    char nome_bin[100];
    char nome_index[100];

    int n, i, j, m, p; 
    cabecalho cab;
    inicializar_tabela(tabela);

    while (scanf("%d", &operacao) != EOF) {
        switch (operacao){
            case 1:
                scanf("%s %s", nome_csv, nome_bin);
                //mesmo que o arquivo já exista - reescreve
                FILE *bin = cria_escreve_binario(nome_bin);
                FILE *csv = fopen(nome_csv, "r");
                if (verifica_se_foi_aberto(csv, bin, 1)){
                    break;
                }
                liberar_tabela(tabela);
                
                //abre o csv dentro da função
                adicionar_csv_no_binario(csv, bin, tabela);
                fclose(bin);
                fclose(csv);
                BinarioNaTela(nome_bin);
                break;  
                    
            case 2:
                scanf("%s", nome_bin);
                
               bin = ler_binario(nome_bin);
                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                mostrar_binario_sequencial(bin);
                fclose(bin);
                break;

            case 3: 
                
                scanf("%s", nome_bin);

                bin = ler_binario(nome_bin);

                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                scanf("%d", &n);

                for (i = 0; i < n; i++) {
                    char nomesCampos[8][50];
                    char valoresCampos[8][200];

                    scanf("%d", &m);
                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
                    }
                    buscar_registros(bin, tabela, m, nomesCampos, valoresCampos);
                    printf("\n");
                }
                fclose(bin);

                break;
            case 4:
                
                scanf("%s", nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                bin = abrir_para_escrita_binário(nome_bin);

                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                ler_cabecalho(bin, &cab);
                // começa o arquivo com inconsistente - inicia a escrita
                cab.status = '0';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab);
                
                scanf("%d", &n);



                for (i = 0; i < n; i++) {
                    char nomesCampos[8][50];
                    char valoresCampos[8][200];
                    char ultimo = '0';
                    scanf("%d", &m);

                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
                    }
                    if (i == n - 1) {
                        ultimo = '1';
                    }
                    remover_registros_dinamico(bin, tabela,&cab , m, nomesCampos, valoresCampos, ultimo);
                }
                
                
                fclose(bin);
                BinarioNaTela(nome_bin);
                break;
                    


            case 5:
                int n,i;
                scanf("%s %d", nome_bin, &n);
                // obtém um ponteiro para o arquivo, abrindo um novo se necessário
                bin = abrir_para_escrita_binário(nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);
                
                cabecalho cab_insercao = cria_cabecalho();
                
                fseek(bin, 0, SEEK_SET);
                ler_cabecalho(bin, &cab_insercao);
                if (cab_insercao.status == '0') {
                    printf("Falha no processamento do arquivo.\n");
                    fclose(bin);
                    break;
                }
                cab_insercao.status = '0';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab_insercao);

                for (i = 0; i < n; i++) {
                    ler_e_inserir_registro(bin, tabela, &cab_insercao);
                }
                // volta a colocar o arquivo como funcional
                cab_insercao.status = '1'; 
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab_insercao);

                fclose(bin);
                BinarioNaTela(nome_bin);

                break;




            case 6:

                scanf("%s", nome_bin);
                bin = abrir_para_escrita_binário(nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                ler_cabecalho(bin, &cab);
                // marca o arquivo como inconsistente
                cab.status = '0';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin,&cab);

                scanf("%d", &n);
                /* para cada uma daz n vezes lê os campos pedidos e 
                atualiza os registros que atendem os critérios*/
                for (i = 0; i < n; i++) {
                    char nomesBusca[8][50];
                    char valoresBusca[8][200];
                    char nomesAtualiza[8][50];
                    char valoresAtualiza[8][200];

                    scanf("%d", &m);

                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(nomesBusca[j], valoresBusca[j]);
                    }

                    scanf("%d", &p);

                    for (j = 0; j < p; j++) {
                        ler_par_campo_valor(nomesAtualiza[j], valoresAtualiza[j]);
                    }

                    atualizar_registros_dinamico(bin, tabela,
                                                m, nomesBusca, valoresBusca,
                                                p, nomesAtualiza, valoresAtualiza);
                }
                // volta o arquivo como consistente 
                cab.status = '1';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab);
                fclose(bin);
                BinarioNaTela(nome_bin);
                break;


            case 7:
                //recebe o nome dos arquivos de dados e de index, respectivamente

                scanf("%s %s", nome_bin, nome_index);
                bin = ler_binario(nome_bin);
                if(verifica_se_foi_aberto(bin, NULL, 0)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                liberar_tabela(tabela);
                inicializar_tabela(tabela);
                
                cria_arvore(bin, nome_index);
                fclose(bin);
                BinarioNaTela(nome_index);
                break;
            case 8:

                scanf("%s %s", nome_bin, nome_index);

                bin = ler_binario(nome_bin);
                FILE* index = ler_binario(nome_index);
                if(verifica_se_foi_aberto(index, bin, 1)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);

                cab_indice ci;
                ler_ind_cabecalho(index, &ci);

                scanf("%d", &n);
                for (i = 0; i < n; i++) {
                    char nomesCampos[8][50];
                    char valoresCampos[8][200];

                    scanf("%d", &m);

                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
                    }

                    int possui_cod_estacao = 0;
                    int chave_buscada;

                    for (j = 0; j < m; j++) {
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

                            long byte_dados = -1;
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
                break;

            case 9:
                scanf("%s %s", nome_bin, nome_index);

                bin = escrever_binario(nome_bin);
                index = escrever_binario(nome_index);
                if(verifica_se_foi_aberto(bin, index, 1)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);
                

                scanf("%d", &n);

                cabecalho cab_insercao9;
                cab_indice cab_index;

                fseek(bin, 0, SEEK_SET);
                ler_cabecalho(bin, &cab_insercao9);

                fseek(index, 0, SEEK_SET);
                ler_ind_cabecalho(index, &cab_index);

                if (cab_insercao9.status == '0' || cab_index.status == '0') {
                    printf("Falha no processamento do arquivo.\n");
                    fclose(bin);
                    fclose(index);
                    break;
                }

                int houve_insercao9 = 0;

                for (i = 0; i < n; i++) {
                    dados reg_inserido9;

                    ler_registro_entrada(&reg_inserido9);

                    int pos_chave_no9 = NEGATIVO;

                    if (reg_inserido9.codEstacao != -1) {
                        busca_arvore(
                            index,
                            cab_index.noRaiz,
                            reg_inserido9.codEstacao,
                            &pos_chave_no9
                        );
                    }

                    if (pos_chave_no9 != NEGATIVO) {
                        continue;
                    }

                    if (!houve_insercao9) {
                        cab_insercao9.status = '0';
                        fseek(bin, 0, SEEK_SET);
                        escreve_cabecalho(bin, &cab_insercao9);

                        cab_index.status = '0';
                        fseek(index, 0, SEEK_SET);
                        escreve_ind_cabecalho(index, &cab_index);

                        houve_insercao9 = 1;
                    }

                    long byteoffset9 = inserir_registro_dinamico(
                        bin,
                        tabela,
                        &cab_insercao9,
                        &reg_inserido9
                    );

                    if (byteoffset9 != NEGATIVO && reg_inserido9.codEstacao != NEGATIVO) {
                        insere_recebendo_chave_e_byteoffset(
                            index,
                            &cab_index,
                            reg_inserido9.codEstacao,
                            byteoffset9
                        );
                    }
                }

                if (houve_insercao9) {
                    cab_insercao9.status = '1';
                    fseek(bin, 0, SEEK_SET);
                    escreve_cabecalho(bin, &cab_insercao9);

                    cab_index.status = '1';
                    fseek(index, 0, SEEK_SET);
                    escreve_ind_cabecalho(index, &cab_index);
                }

                fclose(bin);
                fclose(index);

                BinarioNaTela(nome_bin);
                BinarioNaTela(nome_index);

                break;
            case 10:
                scanf("%s %s", nome_bin, nome_index);

                bin = escrever_binario(nome_bin);
                FILE *index10 = escrever_binario(nome_index);
                if(verifica_se_foi_aberto(bin, index, 1)){
                    break;
                }
                liberar_tabela(tabela);
                carregar_nomes_no_hash(bin, tabela);
                
                cabecalho cab_remocao10;
                cab_indice cab_index10;

                fseek(bin, 0, SEEK_SET);
                ler_cabecalho(bin, &cab_remocao10);

                fseek(index10, 0, SEEK_SET);
                ler_ind_cabecalho(index10, &cab_index10);

                if (cab_remocao10.status == '0' || cab_index10.status == '0') {
                    printf("Falha no processamento do arquivo.\n");
                    fclose(bin);
                    fclose(index10);
                    break;
                }

                cab_remocao10.status = '0';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab_remocao10);

                cab_index10.status = '0';
                fseek(index10, 0, SEEK_SET);
                escreve_ind_cabecalho(index10, &cab_index10);

                scanf("%d", &n);

                for (i = 0; i < n; i++) {
                    char nomesCampos10[8][50];
                    char valoresCampos10[8][200];

                    scanf("%d", &m);

                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(
                            nomesCampos10[j],
                            valoresCampos10[j]
                        );
                    }

                    remover_registros_dinamico_com_arvore(
                        bin,
                        index10,
                        tabela,
                        &cab_remocao10,
                        &cab_index10,
                        m,
                        nomesCampos10,
                        valoresCampos10
                    );
                }

                cab_remocao10.status = '1';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab_remocao10);

                cab_index10.status = '1';
                fseek(index10, 0, SEEK_SET);
                escreve_ind_cabecalho(index10, &cab_index10);

                fclose(bin);
                fclose(index10);

                BinarioNaTela(nome_bin);
                BinarioNaTela(nome_index);

                break;
    }
    liberar_tabela(tabela);
    return 0;
}
}

