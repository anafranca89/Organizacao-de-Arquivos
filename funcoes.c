#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "fornecidas.h"
#include "funcoes.h"

/*Função auxiliar: usando a estrutura criada, para saber se o arquivo já foi aberto anteriormente */
int arquivo_ja_processado(ArquivoAberto *lista, char *nome_bin) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome_bin) == 0) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}
//Adiciona o nome do arquivo aberto na estrutura de arquivos abertos anteriormente
void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin) {
    ArquivoAberto *novo = (ArquivoAberto *) malloc(sizeof(ArquivoAberto));
    strcpy(novo->nome, nome_bin);
    novo->prox = *lista;
    *lista = novo;
}

// Liberar a memoria da estrutura de arquivos utilizada
void liberar_lista_arquivos(ArquivoAberto *lista) {
    ArquivoAberto *atual = lista;

    while (atual != NULL) {
        ArquivoAberto *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}
/*Estrutura escolhida para auxiliar na busca de string foi por tabela Hash. 
Se o tamnaho das strings é >0 insere na tabela hash.*/
void carregar_nomes_no_hash(FILE *bin, NoHash *tabela[]) {
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200];
    char nomeLinha[200];

    int rrn;

    if (bin == NULL) {
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    for (rrn = 0; rrn < proxRRN; rrn++) {
        ler_regdados(bin, &removido, &proximo, &codEstacao, &codLinha,
                     &codProxEstacao, &distProxEstacao, &codLinhaIntegra,
                     &codEstIntegra, &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

        if (removido == '1')
            continue;

        if (tamNomeLinha > 0)
            inserir_hash(tabela, nomeLinha, tamNomeLinha);
    }

    fseek(bin, 0, SEEK_SET);
}

// Formula para criação da chave hash 
int hash_string(char *str, int tam, int primo) {
    int soma = 0;

    for (int i = 0; i < tam; i++) {
        soma += (unsigned char) str[i];
    }

    return soma % primo;
}

void liberar_tabela(NoHash *tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash *atual = tabela[i];

        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->prox;
            free(temp);
        }

        tabela[i] = NULL;
    }
}
//Inicialização da tabela Hash
void inicializar_tabela(NoHash *tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        tabela[i] = NULL;
    }
}

void inserir_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha) {
    int pos = hash_string(nomeLinha, tamNomeLinha, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nomeLinha) == tamNomeLinha &&
            strncmp(atual->nomeLinha, nomeLinha, tamNomeLinha) == 0) {
            atual->repeticoes++;
            return;
        }
        atual = atual->prox;
    }

    NoHash *novo = (NoHash *) malloc(sizeof(NoHash));
    memcpy(novo->nomeLinha, nomeLinha, tamNomeLinha);
    novo->nomeLinha[tamNomeLinha] = '\0';
    novo->repeticoes = 1;
    novo->prox = tabela[pos];

    tabela[pos] = novo;
}

NoHash* buscar_hash(NoHash *tabela[], char *nomeLinha, int tamNomeLinha) {
    int pos = hash_string(nomeLinha, tamNomeLinha, TAM_TABELA);
    NoHash *atual = tabela[pos];

    while (atual != NULL) {
        if ((int)strlen(atual->nomeLinha) == tamNomeLinha &&
            strncmp(atual->nomeLinha, nomeLinha, tamNomeLinha) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}



/*Funções auxiliares para imprimir informações ao usuário.
 Quando o campo de inteiro é nulo ou quando o campo de string é nulo
*/
void imprime_inteiro_ou_nulo(int valor){
    if(valor == -1) printf("NULO");
    else printf("%d", valor);
}

void imprime_texto_ou_nulo(char *texto, int tamanho){
    if(tamanho == 0) printf("NULO");
    else printf("%.*s", tamanho, texto);
}







/*Função SELECT WHERE :  dados os campos e os valores procurados, imprimir os registros validos */
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                      char nomesCampos[][50], char valoresCampos[][200]) {
    int i, rrn;
    int achou = 0;

    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200];
    char nomeLinha[200];

    int temNomeLinha = 0;
    char valorNomeLinha[200];
    int tamValorNomeLinha = 0;

    for (i = 0; i < m; i++) {
        // verifica se um dos campos é nomeLinha
        if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
            temNomeLinha = 1;
            strcpy(valorNomeLinha, valoresCampos[i]);
            tamValorNomeLinha = strlen(valorNomeLinha);
        }
    }

    //Se  tem nomeLinha - usa o hash para buscar usando o nome
    if (temNomeLinha && strlen(valorNomeLinha) != 0) {
        NoHash *h = buscar_hash(tabela, valorNomeLinha, tamValorNomeLinha);

        if (h == NULL) {
            printf("Registro inexistente.\n");
            return;
        }
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    /*ler os registros de forma sequencial
    Se o registro foi removido, ignora na busca
    Se não, verifica qual campo foi digitado para busca
    */
    for (rrn = 0; rrn < proxRRN; rrn++) {
        int ok = 1;
        
        ler_regdados(bin, &removido, &proximo, &codEstacao, &codLinha,
                     &codProxEstacao, &distProxEstacao, &codLinhaIntegra,
                     &codEstIntegra, &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

        if (removido == '1')
            continue;

        for (i = 0; i < m && ok; i++) {
            if (strcmp(nomesCampos[i], "codEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (codEstacao != -1)
                        ok = 0;
                } else if (codEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (tamNomeEstacao != 0)
                        ok = 0;
                } else if (tamNomeEstacao == 0 ||
                           (int)strlen(valoresCampos[i]) != tamNomeEstacao ||
                           strncmp(nomeEstacao, valoresCampos[i], tamNomeEstacao) != 0) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codLinha") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (codLinha != -1)
                        ok = 0;
                } else if (codLinha != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (tamNomeLinha != 0)
                        ok = 0;
                } else if (tamNomeLinha == 0 ||
                           (int)strlen(valoresCampos[i]) != tamNomeLinha ||
                           strncmp(nomeLinha, valoresCampos[i], tamNomeLinha) != 0) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codProxEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (codProxEstacao != -1)
                        ok = 0;
                } else if (codProxEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "distProxEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (distProxEstacao != -1)
                        ok = 0;
                } else if (distProxEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codLinhaIntegra") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (codLinhaIntegra != -1)
                        ok = 0;
                } else if (codLinhaIntegra != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codEstIntegra") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (codEstIntegra != -1)
                        ok = 0;
                } else if (codEstIntegra != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }
        }




        if (ok) {
            imprime_inteiro_ou_nulo(codEstacao);
            imprime_texto_ou_nulo(nomeEstacao, tamNomeEstacao);
            imprime_inteiro_ou_nulo(codLinha);
            imprime_texto_ou_nulo(nomeLinha, tamNomeLinha);
            imprime_inteiro_ou_nulo(codProxEstacao);
            imprime_inteiro_ou_nulo(distProxEstacao);
            imprime_inteiro_ou_nulo(codLinhaIntegra);
            imprime_inteiro_ou_nulo(codEstIntegra);

            achou = 1;
        }
    }

    if (!achou)
        printf("Registro inexistente.\n");
}


/* Função que recebe um arquivo csv, com as especificações do trabalho e 
retorna a informação nas variáveis correspondentes. 

*/
int ler_registro_csv(FILE *csv,
                     int *codEstacao,
                     char *nomeEstacao,
                     int *tamNomeEstacao,
                     int *codLinha,
                     char *nomeLinha,
                     int *tamNomeLinha,
                     int *codProxEstacao,
                     int *distProxEstacao,
                     int *codLinhaIntegra,
                     int *codEstIntegra) {
    char linha[MAX_LINHA_CSV];
    char *p;
    int valor, tem_digito, i;

    if (csv == NULL) return 0;
    if (fgets(linha, MAX_LINHA_CSV, csv) == NULL) return 0;

    p = linha;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    i = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        nomeEstacao[i++] = *p;
        p++;
    }
    *tamNomeEstacao = i;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codLinha = tem_digito ? valor : -1;
    if (*p == ',') p++;

    i = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        nomeLinha[i++] = *p;
        p++;
    }
    *tamNomeLinha = i;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *distProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codLinhaIntegra = tem_digito ? valor : -1;
    if (*p == ',') p++;

    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    *codEstIntegra = tem_digito ? valor : -1;
    return 1;
}

void adicionar_csv_no_binario(FILE *csv, FILE *bin, NoHash *tabela[]) {
    char linha_cabecalho[MAX_LINHA_CSV];

    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, tamNomeEstacao;
    int codLinha, tamNomeLinha;
    int codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;

    char nomeEstacao[200];
    char nomeLinha[200];

    int rrn_atual;
    long byteoffset;

    if (csv == NULL || bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    if (status != '0' && status != '1') {
        status = '0';
        topo = -1;
        proxRRN = 0;
        nroEstacoes = 0;
        nroParesEstacoes = 0;

        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    }

    if (fgets(linha_cabecalho, MAX_LINHA_CSV, csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    rrn_atual = proxRRN;
    byteoffset = TAM_CABECALHO + rrn_atual * TAM_REG;

    fseek(bin, byteoffset, SEEK_SET);

    while (ler_registro_csv(csv,
                            &codEstacao,
                            nomeEstacao,
                            &tamNomeEstacao,
                            &codLinha,
                            nomeLinha,
                            &tamNomeLinha,
                            &codProxEstacao,
                            &distProxEstacao,
                            &codLinhaIntegra,
                            &codEstIntegra)) {

        removido = '0';
        proximo = -1;

        escreve_regdados(bin,
                         &removido,
                         &proximo,
                         &codEstacao,
                         &codLinha,
                         &codProxEstacao,
                         &distProxEstacao,
                         &codLinhaIntegra,
                         &codEstIntegra,
                         &tamNomeEstacao,
                         nomeEstacao,
                         &tamNomeLinha,
                         nomeLinha);

        
            NoHash *busca = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

            if (busca == NULL) {
                inserir_hash(tabela, nomeEstacao, tamNomeEstacao);
                nroEstacoes++;
            } else {
                busca->repeticoes++;
            }
        

        if (codProxEstacao != -1) 
            nroParesEstacoes++;
        
        rrn_atual++;
        byteoffset += TAM_REG;
    }

    proxRRN = rrn_atual;
    status = '1';

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
}





/*Função que lê todos os registros do arquivo binário e retorna essas informações de forma sequencial.
*/
void mostrar_binario_sequencial(FILE *bin){
    // Cria um registro temporário para ler os dados do arquivo
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;

    char nomeEstacao[200];
    char nomeLinha[200];

    int i;

    if(bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Posiciona a leitura no inicio do arquivo e le o registro de cabeçalho
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    if (proxRRN == 0 || nroEstacoes == 0) {
            //Nao existem registros
            printf("Registro inexistente.\n");
    return;
    }
    if(status != '1'){
        // O status é diferente de 1, indica inconsistencia de dados.
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for(i = 0; i < proxRRN; i++){
        ler_regdados(bin,
                     &removido,
                     &proximo,
                     &codEstacao,
                     &codLinha,
                     &codProxEstacao,
                     &distProxEstacao,
                     &codLinhaIntegra,
                     &codEstIntegra,
                     &tamNomeEstacao,
                     nomeEstacao,
                     &tamNomeLinha,
                     nomeLinha);
        // Se o registro está marcado como removido, não imprime
        if(removido == '1') continue;

        // Registro não removido - Usa as funções auxilares para imprimir inteiro ou string
        imprime_inteiro_ou_nulo(codEstacao);
        printf(" ");

        imprime_texto_ou_nulo(nomeEstacao, tamNomeEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(codLinha);
        printf(" ");

        imprime_texto_ou_nulo(nomeLinha, tamNomeLinha);
        printf(" ");

        imprime_inteiro_ou_nulo(codProxEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(distProxEstacao);
        printf(" ");

        imprime_inteiro_ou_nulo(codLinhaIntegra);
        printf(" ");

        imprime_inteiro_ou_nulo(codEstIntegra);
        printf("\n");
    }
}


