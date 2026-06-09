#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvb.h"
#include "funcoes.h"

int main() {
    NoHash *tabela[TAM_TABELA];
    ArquivoAberto *lista_arquivos = NULL;

    int operacao;
    char nome_csv[100];
    char nome_bin[100];
    char nome_index[100];

    int n, i, j, m, p; 
    cabecalho cab = cria_cabecalho();
    FILE *bin;
    inicializar_tabela(tabela);

    while (scanf("%d", &operacao) != EOF) {
        switch (operacao){
            case 1:
                scanf("%s %s", nome_csv, nome_bin);

                bin = abrir_para_escrita_binário(nome_bin);
                FILE *csv = fopen(nome_csv, "r");
                if (csv == NULL || bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                        
                    if (csv != NULL) fclose(csv);
                    if (bin != NULL) fclose(bin);
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }
                //abre o csv dentro da função
                adicionar_csv_no_binario(csv, bin, tabela);

                if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                    adicionar_arquivo_processado(&lista_arquivos, nome_bin, NULL);
                }
                fclose(bin);
                fclose(csv);
                BinarioNaTela(nome_bin);
            break;
        
            case 2:
                scanf("%s", nome_bin);
                
               bin = ler_binario(nome_bin);
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }
                if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                    adicionar_arquivo_processado(&lista_arquivos, nome_bin, NULL);
                    carregar_nomes_no_hash(bin, tabela);
                }
                mostrar_binario_sequencial(bin);
                fclose(bin);
                break;

            case 3: 
                
                scanf("%s", nome_bin);

                bin = ler_binario(nome_bin);

                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

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
                bin = abrir_para_escrita_binário(nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

    
                ler_cabecalho(bin, &cab);
                // começa o arquivo com inoperando
                cab.status = '0';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab);

                scanf("%d", &n);

                for (i = 0; i < n; i++) {
                    char nomesCampos[8][50];
                    char valoresCampos[8][200];

                    scanf("%d", &m);

                    for (j = 0; j < m; j++) {
                        ler_par_campo_valor(nomesCampos[j], valoresCampos[j]);
                    }

                    remover_registros_dinamico(bin, tabela, m, nomesCampos, valoresCampos);
                }
                    
                fclose(bin);
                BinarioNaTela(nome_bin);
                break;
                    


            case 5:

                scanf("%s %d", nome_bin, &n);
                // obtém um ponteiro para o arquivo, abrindo um novo se necessário
                bin = abrir_para_escrita_binário(nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

                
                cabecalho cab_insercao;
                
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
                cab.status = '1';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &cab);

                fclose(bin);
                BinarioNaTela(nome_bin);

                break;




            case 6:

                scanf("%s", nome_bin);
                bin = abrir_para_escrita_binário(nome_bin);
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

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
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    break;
                }

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
                if (bin == NULL || index== NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

                if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                    adicionar_arquivo_processado(&lista_arquivos, nome_bin, NULL);
                    carregar_nomes_no_hash(bin, tabela);
                }
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

        }

    }
    liberar_tabela(tabela);
    liberar_lista_arquivos(lista_arquivos);
    return 0;
}

