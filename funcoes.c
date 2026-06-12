#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
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

=======
#include "funcoes.h"



void ler_par_campo_valor(char *campo, char *valor) {
    if (scanf("%s", campo) == EOF) return;

    if (strcmp(campo, "nomeEstacao") == 0 || strcmp(campo, "nomeLinha") == 0) {
        ScanQuoteString(valor);
    } else {
        scanf("%s", valor);

        if (strcmp(valor, "NULO") == 0 || strcmp(valor, "nulo") == 0) {
            strcpy(valor, "");
        }
    }
}

/*  
Verifica se o registro de dados lido atende a todos os critérios 
de busca informados pelo usuário na main.
Parametros: qtd de campos p/ comparar; vetor com o nomes dos campos, o valor dos campos procurados e o registro p/comparação
Retorna:  1 - registro compativel. 0 se não é comaptivel
    -Para cada valor m:
    - verifica qual o campo 
        - compara o valor buscado com o valor do campo no reg.dados
 */
int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200], dados *reg_dados) {
    
    for (int i = 0; i < m; i++) {
        
        //os campos de texto 
        if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (reg_dados->tamNomeEstacao != 0) return 0;
            } else if (reg_dados->tamNomeEstacao == 0 || 
                       (int)strlen(valoresCampos[i]) != reg_dados->tamNomeEstacao ||
                       strncmp(reg_dados->nomeEstacao, valoresCampos[i], reg_dados->tamNomeEstacao) != 0) {
                return 0;
            }
        }
        else if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (reg_dados->tamNomeLinha != 0) return 0;
            } else if (reg_dados->tamNomeLinha == 0 || 
                       (int)strlen(valoresCampos[i]) != reg_dados->tamNomeLinha ||
                       strncmp(reg_dados->nomeLinha, valoresCampos[i], reg_dados->tamNomeLinha) != 0) {
                return 0;
            }
        }
        
        
        // os inteiros
        else if (strcmp(nomesCampos[i], "codEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codEstacao != -1) return 0; 
            }
            else if (reg_dados->codEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codLinha != -1) return 0; 
            }
            else if (reg_dados->codLinha != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codProxEstacao != -1) return 0; 
            }
            else if (reg_dados->codProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "distProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->distProxEstacao != -1) return 0; 
            }
            else if (reg_dados->distProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinhaIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codLinhaIntegra != -1) return 0; 
            }
            else if (reg_dados->codLinhaIntegra != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codEstIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { 
                if (reg_dados->codEstIntegra != -1) return 0; 
            }
            else if (reg_dados->codEstIntegra != atoi(valoresCampos[i])) return 0;
        }
    }
    
    return 1; 
}







/*
Função SELECT WHERE :  dados os campos e os valores procurados,
 imprimir os registros validos
 Parametros: arquivo de dados, tabela Hash, qtd de campos na busca, 
            o vetor com nomesCampos, e o vetor com valoresCampos
    - Para manter o registro de Estaçoes Unicas, existe a tabela Hash.
    Caso1: A busca foi feita com o campo nomeEstacao e ele n existe?
        - Busca na tabela Hash, que retorna o ponteiro p/ a posição na tabela dinamica
        - Existe na tabela Hash?
        - Se sim - continua. Se não -> termina a busca com Registro Inexistente
    Caso2: A busca n tem o nomeEstacao, ou aquele nome existe nos registros.
        - busca sequencial em cada rrn.
            - pula os rrns logicamente removidos.
        - verifica cada campo, comparando com os valores procurados
        - Achou? - Imprime
        - Nao achou? - Registro Inexistente
*/
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                      char nomesCampos[][50], char valoresCampos[][200]) {
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
    if (bin == NULL) {
        return;
    }

<<<<<<< HEAD
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
=======
    int i, rrn;
    int achou = 0;

    cabecalho reg_cab;
    dados reg_dados;

    int temEstacao = 0;
    char valorEstacao[200];
    int temValorEstacao = 0;

    for (i = 0; i < m; i++) {
        // verifica se um dos campos é Estação,
        // Se sim, podemos procurar no hash e facilitar a busca.
        if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
            temEstacao = 1;
            strcpy(valorEstacao, valoresCampos[i]);
            temValorEstacao = strlen(valorEstacao);
        }
    }


    //Se  tem nomeLinha - usa o hash para buscar usando o nome
    if (temEstacao && strlen(valorEstacao) != 0) {
        NoHash *h = buscar_hash(tabela, valorEstacao, temValorEstacao);
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b

        if (h == NULL) {
            printf("Registro inexistente.\n");
            return;
        }
    }
<<<<<<< HEAD

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
=======
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);
    
    if (reg_cab.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
    /*ler os registros de forma sequencial
    Se o registro foi removido, ignora na busca
    Se não, verifica qual campo foi digitado para busca
    */
<<<<<<< HEAD
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

=======
    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        int ok = 1;
        reg_dados = cria_dados();
        // Reposiciona o ponteiro de leitura
        fseek(bin, calculo_byteoffset_dados(rrn), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1')
            continue;

        ok = verificar_criterios(m, nomesCampos, valoresCampos, &reg_dados);
            

        // Se o registro atendeu a todas as m condições 
        if (ok) {
            imprime_registro_dados(&reg_dados);
            achou = 1;
        }
    }
    
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
    if (!achou)
        printf("Registro inexistente.\n");
}


<<<<<<< HEAD
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
=======

>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b





/*Função que lê todos os registros do arquivo binário e retorna essas informações de forma sequencial.
*/
void mostrar_binario_sequencial(FILE *bin){
    // Cria um registro temporário para ler os dados do arquivo
<<<<<<< HEAD
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;

    char nomeEstacao[200];
    char nomeLinha[200];

=======
    cabecalho reg_cab ;
    dados reg_dados;
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
    int i;

    if(bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Posiciona a leitura no inicio do arquivo e le o registro de cabeçalho
    fseek(bin, 0, SEEK_SET);
<<<<<<< HEAD
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
=======
    ler_cabecalho(bin, &reg_cab);
    if (reg_cab.proxRRN == 0 || reg_cab.nroEstacoes == 0) {
        printf("Registro inexistente.\n");
        return;
    }
    if(reg_cab.status != '1'){
        printf("Falha no processamento do arquivo. EEE\n");
        return;
    }

    for(i = 0; i < reg_cab.proxRRN; i++){
        fseek(bin, calculo_byteoffset_dados(i), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if(reg_dados.removido == '1') continue;
        imprime_registro_dados(&reg_dados);
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
    }
}


<<<<<<< HEAD
=======


 /* 
    insere um registro usando o conceito de pilha de rns dos removidos ensinado em aula,
    isto é, apenas marca como logicamente removido.
    Parâmetros:  arquivo de dados, Tabela Hash, 

*/

void ler_e_inserir_registro(FILE *bin, NoHash *tabela[], cabecalho *reg_cabecalho) {

    dados reg_dados= cria_dados();

    char strCodEstacao[50], strCodLinha[50], strCodProxEstacao[50], strDistProxEstacao[50];
    char strCodLinhaIntegra[50], strCodEstIntegra[50];

    // Faz a leitura sequencial de todos os campos 
    scanf("%s", strCodEstacao);
    ScanQuoteString(reg_dados.nomeEstacao);
    scanf("%s", strCodLinha);
    ScanQuoteString(reg_dados.nomeLinha);
    scanf("%s", strCodProxEstacao);
    scanf("%s", strDistProxEstacao);
    scanf("%s", strCodLinhaIntegra);
    scanf("%s", strCodEstIntegra);

    // converte os campos caso eles sejam nulos para -1
    
    reg_dados.codEstacao      = (strcasecmp(strCodEstacao, "NULO") == 0) ? -1 : atoi(strCodEstacao);
    reg_dados.codLinha        = (strcasecmp(strCodLinha, "NULO") == 0) ? -1 : atoi(strCodLinha);
    reg_dados.codProxEstacao  = (strcasecmp(strCodProxEstacao, "NULO") == 0) ? -1 : atoi(strCodProxEstacao);
    reg_dados.distProxEstacao = (strcasecmp(strDistProxEstacao, "NULO") == 0) ? -1 : atoi(strDistProxEstacao);
    reg_dados.codLinhaIntegra = (strcasecmp(strCodLinhaIntegra, "NULO") == 0) ? -1 : atoi(strCodLinhaIntegra);
    reg_dados.codEstIntegra   = (strcasecmp(strCodEstIntegra, "NULO") == 0) ? -1 : atoi(strCodEstIntegra);

    //definir o tamanho com o strlen - sem o \0
    reg_dados.tamNomeEstacao = strlen(reg_dados.nomeEstacao);
    reg_dados.tamNomeLinha   = strlen(reg_dados.nomeLinha);
    
   reg_dados.removido = '0';
    reg_dados.proximo = -1;

    inserir_registro_dinamico(bin, tabela, reg_cabecalho, &reg_dados);
}









 /* 
    remove um registro usando o conceito de pilha de rns dos removidos ensinado em aula,
    isto é, apenas marca como logicamente removido.

    Parâmetros:  arquivo de dados, Tabela Hash, qtd de campos a serem verificados,
                    nomedos campos, valor dos campos.

*/
void remover_registros_dinamico(FILE *bin, NoHash *tabela[],cabecalho *reg_cab, int m,
                                char nomesCampos[][50], char valoresCampos[][200],
                                char ultimo) {
    
    int rrn_atual = 0;
    
    dados reg_dados;

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }


	

  	for (rrn_atual = 0; rrn_atual < reg_cab->proxRRN; rrn_atual++) {
        int ok =1;
        reg_dados = cria_dados();
        //fseek(bin, calculo_byteoffset_dados(rrn_atual), SEEK_SET);
        ler_regdados(bin, &reg_dados);
        
        if (reg_dados.removido == '1') continue;

        ok = verificar_criterios(m, nomesCampos, valoresCampos, &reg_dados);
        if (ok) {
            int topo_antigo = reg_cab->topo;
            char removido = '1';
            int prox  = topo_antigo;

            reg_dados.removido = removido;
            reg_dados.proximo= prox;
            
            fseek(bin, -TAM_REG, SEEK_CUR);
            escreve_regdados(bin, &reg_dados);
            
            reg_cab->topo = rrn_atual;
            
            // Atualização estações únicas
            if (reg_dados.tamNomeEstacao > 0) {
                NoHash *h = buscar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);

                if (h != NULL && h->repeticoes == 1) {
                    reg_cab->nroEstacoes--;
                }
                decrementar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
            }
            
            // Atualização dos pares de estações
            if (reg_dados.codProxEstacao != -1) {
                reg_cab->nroParesEstacoes--;
            }
        }
    }
        
    if (ultimo == '1') reg_cab->status='1';
    
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, reg_cab);
    
}




void inserir_registro_dinamico(FILE *bin, NoHash *tabela[], cabecalho *cab, dados *reg_dados){
	
	
	long offset_insercao;
    int rrn_insercao;
    
    if (bin == NULL ) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }


    if (reg_dados->tamNomeEstacao> 0) {
        NoHash *h = buscar_hash(tabela, reg_dados->nomeEstacao, reg_dados->tamNomeEstacao);
        
        if (h == NULL) {
            cab->nroEstacoes++;
        }

        inserir_hash(tabela, reg_dados->nomeEstacao, reg_dados->tamNomeEstacao);
    }
    if (reg_dados->codProxEstacao != -1) {
        cab->nroParesEstacoes++;
    }

	 if (cab->topo != -1) {

        rrn_insercao = cab->topo;
        offset_insercao= calculo_byteoffset_dados(rrn_insercao);
        
        fseek(bin, offset_insercao + 1, SEEK_SET);
        
		int proximo_rrn_topo;
        fread(&proximo_rrn_topo, sizeof(int), 1, bin);
        
        cab->topo = proximo_rrn_topo;
    } else {
        rrn_insercao = cab->proxRRN;
        offset_insercao = calculo_byteoffset_dados(rrn_insercao);
        cab->proxRRN++;
    }

	
	fseek(bin, offset_insercao, SEEK_SET);
    escreve_regdados(bin, reg_dados);


}




void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[],
                                  int m, char nomesBusca[][50], char valoresBusca[][200],
                                  int p, char nomesAtualiza[][50], char valoresAtualiza[][200]) {
	int i;
    int qtd_atualizacoes = 0;
    int rrn_atual = 0;
    int indice_atualizacao = 0;
    
    cabecalho reg_cab;
    dados reg_dados;
    
    int temNomeEstacaoBusca = 0;
    char valorNomeEstacaoBusca[200];
    int tamValorNomeEstacaoBusca = 0;
    
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

	for (i = 0; i < m; i++) {
        if (strcmp(nomesBusca[i], "nomeEstacao") == 0) {
            temNomeEstacaoBusca = 1;
            strcpy(valorNomeEstacaoBusca, valoresBusca[i]);
            tamValorNomeEstacaoBusca = strlen(valorNomeEstacaoBusca);
            break;
        }
    }
    
    if (temNomeEstacaoBusca && tamValorNomeEstacaoBusca != 0) {
        NoHash *h = buscar_hash(tabela, valorNomeEstacaoBusca, tamValorNomeEstacaoBusca);
        if (h == NULL) {
            return; // Nome não indexado, nenhum registro sofrerá alteração
        }
    }

	fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);

    // Alocação dinâmica segura para o vetor de atualizações pendentes
    AtualizacaoPendente *atualizacoes = (AtualizacaoPendente *) malloc(sizeof(AtualizacaoPendente) * (reg_cab.proxRRN + 10));
    if (atualizacoes == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

	while (rrn_atual <reg_cab.proxRRN && !feof(bin)) {

        ler_regdados(bin, &reg_dados);
        
        if (feof(bin)) break;
        if (reg_dados.removido == '1') {
            memset(&reg_dados, 0, sizeof(dados));
            rrn_atual++;
            continue;
        }
        
        if (verificar_criterios(m, nomesBusca, valoresBusca, &reg_dados)) {
            
            int codProxEstacao_antigo = reg_dados.codProxEstacao;
            int tamNomeEstacao_antigo = reg_dados.tamNomeEstacao;
            char nomeEstacao_antigo[200];
            
            if (tamNomeEstacao_antigo > 0) {
                memcpy(nomeEstacao_antigo, reg_dados.nomeEstacao, tamNomeEstacao_antigo);
            }
            nomeEstacao_antigo[tamNomeEstacao_antigo] = '\0';
            
            atualizar_campos_registro(p, nomesAtualiza, valoresAtualiza, &reg_dados);
            
            if (codProxEstacao_antigo == -1 && reg_dados.codProxEstacao != -1) {
                reg_cab.nroParesEstacoes++;
            } else if (codProxEstacao_antigo != -1 && reg_dados.codProxEstacao == -1) {
                reg_cab.nroParesEstacoes--;
            }
            
            if (strcmp(nomeEstacao_antigo, reg_dados.nomeEstacao) != 0) {
                if (tamNomeEstacao_antigo > 0) {
                    NoHash *h_antigo = buscar_hash(tabela, nomeEstacao_antigo, tamNomeEstacao_antigo);
                    if (h_antigo != NULL && h_antigo->repeticoes == 1) {
                        reg_cab.nroEstacoes--;
                    }
                    decrementar_hash(tabela, nomeEstacao_antigo, tamNomeEstacao_antigo);
                }
                
                if (reg_dados.tamNomeEstacao > 0) {
                    NoHash *h_novo = buscar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
                    if (h_novo == NULL) {
                        reg_cab.nroEstacoes++;
                    }
                    inserir_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
                }
            }
            //guarda o registro atualizado
            atualizacoes[qtd_atualizacoes].rrn = rrn_atual;
            atualizacoes[qtd_atualizacoes].reg_dados = reg_dados;
            qtd_atualizacoes++;
        }
        
        memset(&reg_dados, 0, sizeof(dados));
        rrn_atual++;
    }
    
    if (qtd_atualizacoes > 0) {
        //estamos modificando o cabeçalho, logoo seta como inconsistente
		reg_cab.status = '0';
        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &reg_cab);
        
        char buffer_descarte[80];
        rrn_atual = 0;
        
        fseek(bin, 17, SEEK_SET);
        
        while (rrn_atual < reg_cab.proxRRN) {
            if (indice_atualizacao < qtd_atualizacoes && 
                rrn_atual == atualizacoes[indice_atualizacao].rrn) {
				//grava o registro atualizado
                escreve_regdados(bin, &atualizacoes[indice_atualizacao].reg_dados);
                indice_atualizacao++;
            } else {
				// reg nao modificado, apenas pula esses bytes
                fread(buffer_descarte, TAM_REG, 1, bin);
            }
            rrn_atual++;
        }
    }
    
	//cabecalho consistente
    reg_cab.status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &reg_cab);
    
    free(atualizacoes);
}
>>>>>>> ad50ee39c61771b70c2701147fcd4298c8c1bb9b
