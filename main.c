#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main() {
    NoHash *tabela[TAM_TABELA];
    ArquivoAberto *lista_arquivos = NULL;

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
                // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
                bin = abrir_para_escrita_binário(nome_bin);

                if (!arquivo_ja_processado(lista_arquivos, nome_bin)) {
                    adicionar_arquivo_processado(&lista_arquivos, nome_bin, NULL);
                    carregar_nomes_no_hash(bin, tabela);
                }
                if (bin == NULL) {
                    printf("Falha no processamento do arquivo.\n");
                    liberar_tabela(tabela);
                    liberar_lista_arquivos(lista_arquivos);
                    return 0;
                }

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


            
        }

    }
    liberar_tabela(tabela);
    liberar_lista_arquivos(lista_arquivos);
    return 0;
}

