#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcoes.h"
#include "fornecidas.h"
#include "estruturas.h"

int main() {
    /* cria a tabela de hashing e a lista de arquivos. O uso da tabela de hashing
    depende do contexto, enquanto a lista de arquivos é usada para evitar que o 
    arquivo fique lendo toda hora, veficando se ele já foi aberto, isso permite 
    o uso de múltiplos arquivos binários, embora seja necessário não existe esse 
    caso no run.codes
    */
    NoHash *tabela[TAM_TABELA];
    ArquivoAberto *lista_arquivos = NULL;

    // declaração de variáveis auto-explicatórias básicas
    int operacao;
    char nome_csv[100];
    char nome_bin[100];
    inicializar_tabela(tabela);

    // lê enquanto houver algo para ser lido
    while (scanf("%d", &operacao) != EOF) {

        if (operacao == 1) {
            /* função de ler o arquivo csv e passar para o binário,
            como o arquivo deve ser criado, se supôe que estes não serão repetidos
            e logo, não é usado a lógica da lista encadeada
            */
            scanf("%s %s", nome_csv, nome_bin);

            FILE *csv = fopen(nome_csv, "r");
            FILE *bin = fopen(nome_bin, "w+b");
            // caso algum dos arquivo for inexistente, mostra a saída pedida e limpa as estruturas
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
                adicionar_arquivo_processado(&lista_arquivos, nome_bin, bin);
            }

            // precisamos do ffluse para lidar com a funçao binário na tela
            fclose(csv);
            fflush(bin);

            BinarioNaTela(nome_bin);
        }

        else if (operacao == 2) {
            /* o objetivo dessa função é mostrar o conteúdo do binário de forma sequencial
            de acordo com as especificações do arquivo
            */
            FILE *bin;

            scanf("%s", nome_bin);
            // obtém um ponteiro para o arquivo, abrindo um novo se necessário
            bin = obter_arquivo_binario(&lista_arquivos, nome_bin, "rb", tabela);
            // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            mostrar_binario_sequencial(bin);
        }

        // o objetivo dessa função é realizar um busca de acordo com os campos dados pelo usuário
        else if (operacao == 3) {
            int n, i, j, m;
            FILE *bin;

            scanf("%s", nome_bin);
            // obtém um ponteiro para o arquivo, abrindo um novo se necessário
            bin = obter_arquivo_binario(&lista_arquivos, nome_bin, "rb", tabela);
            // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            scanf("%d", &n);
            // lê cada um dos campos dados n vezes e busca cada um deles
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
        }

        /* 
        O objetivo dessa função dessa função é remover de forma dinâmica um registro,
        seguindo o ensinado em aula, isto é, apenas indicar logicamente que ele está
        removido, criando uma pilha em cada cada registro aponta para o registro removido anteior
        */
        else if (operacao == 4) {
            int n, i, j, m;
            char status;
            int topo, proxRRN, nroEstacoes, nroParesEstacoes;
            FILE *bin;

            scanf("%s", nome_bin);
            // obtém um ponteiro para o arquivo, abrindo um novo se necessário
            bin = obter_arquivo_binario(&lista_arquivos, nome_bin, "rb+", tabela);
            // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            // lê o cabeçalho
            fseek(bin, 0, SEEK_SET);
            ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

            // começa o arquivo com inoperando
            status = '0';
            fseek(bin, 0, SEEK_SET);
            escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

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

                remover_registros_dinamico(bin, tabela, m, nomesCampos, valoresCampos, &topo, ultimo);
            }

            // não nos preocupamos em voltar o primeiro byte(arquivo consistente) para um pois a função já faz isso
            fflush(bin);
            BinarioNaTela(nome_bin);
        }

        /*
        O objetivo dessa função é inserir um novo registro, para isso
        vamos também usar a lógica de removação, já que caso já tivermos
        um removido, vamos inserir nele logicamente, senão temos que inserir
        no final do registro
        */
        else if (operacao == 5) {
            int n, i;
            FILE *bin;

            scanf("%s %d", nome_bin, &n);
            // obtém um ponteiro para o arquivo, abrindo um novo se necessário
            bin = obter_arquivo_binario(&lista_arquivos, nome_bin, "rb+", tabela);
            // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            for (i = 0; i < n; i++) {
                ler_e_inserir_registro(bin, tabela);
            }

            fflush(bin);
            BinarioNaTela(nome_bin);
        }

        // o objetivo dessa função e atualizar um registro de acordo com o pedido pelo usuário
        else if (operacao == 6) {
            int n, i, j, m, p;
            char status;
            int topo, proxRRN, nroEstacoes, nroParesEstacoes;
            FILE *bin;

            scanf("%s", nome_bin);
            // obtém um ponteiro para o arquivo, abrindo um novo se necessário
            bin = obter_arquivo_binario(&lista_arquivos, nome_bin, "rb+", tabela);
            // caso o arquivo for inexistente, mostra a saída pedida e limpa as estruturas
            if (bin == NULL) {
                printf("Falha no processamento do arquivo.\n");
                liberar_tabela(tabela);
                liberar_lista_arquivos(lista_arquivos);
                return 0;
            }

            // lê o cabeçalho inicialmente
            fseek(bin, 0, SEEK_SET);
            ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

            // marca o arquivo como inconsistente
            status = '0';
            fseek(bin, 0, SEEK_SET);
            escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

            scanf("%d", &n);

            /* para cada uma daz n vezes lê os campos pedidos e 
            atualiza os registros que atendem os critérios*/
            for (i = 0; i < n; i++) {
                char nomesBusca[8][50];
                char valoresBusca[8][200];
                char nomesAtualiza[8][50];
                char valoresAtualiza[8][200];
                char ultimo = '0';

                scanf("%d", &m);

                for (j = 0; j < m; j++) {
                    ler_par_campo_valor(nomesBusca[j], valoresBusca[j]);
                }

                scanf("%d", &p);

                for (j = 0; j < p; j++) {
                    ler_par_campo_valor(nomesAtualiza[j], valoresAtualiza[j]);
                }

                if (i == n - 1) {
                    ultimo = '1';
                }

                atualizar_registros_dinamico(bin, tabela,
                                             m, nomesBusca, valoresBusca,
                                             p, nomesAtualiza, valoresAtualiza,
                                             ultimo);
            }

            // não nos preocupamos em voltar o primeiro byte(arquivo consistente) para um pois a função já faz isso
            fflush(bin);
            BinarioNaTela(nome_bin);
        }
    }

    // libera a tebela e a lista de arquivos, já que estão armezanados sequencialmente
    liberar_tabela(tabela);
    liberar_lista_arquivos(lista_arquivos);
    return 0;
}
