#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "fornecidas.h"
#include "funcoes.h"
#include "estruturas.h"

/*
as três primeiras funções são simples e autoexplicatórias,
apenas imprimem os campos do registro de acordo com o que foi
pedido, geralmente, envolvendo mostrar todos os campos com
um espaço e nulo ao invés de -1 que é como nulo é representado do campo
*/

void imprime_inteiro_ou_nulo(int valor){
    if(valor == -1) printf("NULO");
    else printf("%d", valor);
}

void imprime_texto_ou_nulo(char *texto, int tamanho){
    if(tamanho == 0) printf("NULO");
    else printf("%.*s", tamanho, texto);
}

void printar_registro_inteiro(int codEstacao, int tamNomeEstacao, char *nomeEstacao,
                              int codLinha, int tamNomeLinha, char *nomeLinha,
                              int codProxEstacao, int distProxEstacao,
                              int codLinhaIntegra, int codEstIntegra) {
    
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
}

/* função usada principalmente na main para ler campos entregues como input, 
usando inclusive a função dada
*/
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

/* essa função é usada várias vezes durante o código para verificar se 
os campos dados batem com o que estamos querendo comparar, retornando
0 ou 1 de acordo com a veficação
*/

int verificar_criterios(int m, char nomesCampos[][50], char valoresCampos[][200],
                       int codEstacao, int tamNomeEstacao, char *nomeEstacao,
                       int codLinha, int tamNomeLinha, char *nomeLinha,
                       int codProxEstacao, int distProxEstacao,
                       int codLinhaIntegra, int codEstIntegra) {
    
    for (int i = 0; i < m; i++) {
        if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (tamNomeEstacao != 0) return 0;
            } else if (tamNomeEstacao == 0 || 
                       (int)strlen(valoresCampos[i]) != tamNomeEstacao ||
                       strncmp(nomeEstacao, valoresCampos[i], tamNomeEstacao) != 0) {
                return 0;
            }
        }
        else if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) {
                if (tamNomeLinha != 0) return 0;
            } else if (tamNomeLinha == 0 || 
                       (int)strlen(valoresCampos[i]) != tamNomeLinha ||
                       strncmp(nomeLinha, valoresCampos[i], tamNomeLinha) != 0) {
                return 0;
            }
        }
        // Campos Inteiros
        else if (strcmp(nomesCampos[i], "codEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (codEstacao != -1) return 0; }
            else if (codEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinha") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (codLinha != -1) return 0; }
            else if (codLinha != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (codProxEstacao != -1) return 0; }
            else if (codProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "distProxEstacao") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (distProxEstacao != -1) return 0; }
            else if (distProxEstacao != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codLinhaIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (codLinhaIntegra != -1) return 0; }
            else if (codLinhaIntegra != atoi(valoresCampos[i])) return 0;
        }
        else if (strcmp(nomesCampos[i], "codEstIntegra") == 0) {
            if (strlen(valoresCampos[i]) == 0) { if (codEstIntegra != -1) return 0; }
            else if (codEstIntegra != atoi(valoresCampos[i])) return 0;
        }
    }
    
    return 1;
}

int busca_por_id(int m, char nomesCampos[][50]) {
    int i;

    for (i = 0; i < m; i++) {
        if (strcmp(nomesCampos[i], "codEstacao") == 0) {
            return 1;
        }
    }

    return 0;
}

// AS DUAS FUNÇÕES SEGUINTES SÃO USADAS PARA A ATUALIZAÇÃO DE REGISTROS

// função que substitui cada um dos campos com aquele desejado pela atualização
void atualizar_campos_registro(int p, char nomesAtualiza[][50], char valoresAtualiza[][200],
                               int *codEstacao, int *tamNomeEstacao, char *nomeEstacao,
                               int *codLinha, int *tamNomeLinha, char *nomeLinha,
                               int *codProxEstacao, int *distProxEstacao,
                               int *codLinhaIntegra, int *codEstIntegra) {
    for (int i = 0; i < p; i++) {
        // Campos Inteiros
        if (strcmp(nomesAtualiza[i], "codEstacao") == 0) {
            *codEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codLinha") == 0) {
            *codLinha = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codProxEstacao") == 0) {
            *codProxEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "distProxEstacao") == 0) {
            *distProxEstacao = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codLinhaIntegra") == 0) {
            *codLinhaIntegra = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        else if (strcmp(nomesAtualiza[i], "codEstIntegra") == 0) {
            *codEstIntegra = (strlen(valoresAtualiza[i]) == 0) ? -1 : atoi(valoresAtualiza[i]);
        }
        // Campos de String Dinâmica
        else if (strcmp(nomesAtualiza[i], "nomeEstacao") == 0) {
            if (strlen(valoresAtualiza[i]) == 0) {
                *tamNomeEstacao = 0;
                nomeEstacao[0] = '\0';
            } else {
                *tamNomeEstacao = strlen(valoresAtualiza[i]);
                memcpy(nomeEstacao, valoresAtualiza[i], *tamNomeEstacao);
                nomeEstacao[*tamNomeEstacao] = '\0';
            }
        }
        else if (strcmp(nomesAtualiza[i], "nomeLinha") == 0) {
            if (strlen(valoresAtualiza[i]) == 0) {
                *tamNomeLinha = 0;
                nomeLinha[0] = '\0';
            } else {
                *tamNomeLinha = strlen(valoresAtualiza[i]);
                memcpy(nomeLinha, valoresAtualiza[i], *tamNomeLinha);
                nomeLinha[*tamNomeLinha] = '\0';
            }
        }
    }
}

// função que atualiza o registro (se aproveitando também da lógica dinâmica de remoção)
void atualizar_registros_dinamico(FILE *bin, NoHash *tabela[],
                                  int m, char nomesBusca[][50], char valoresBusca[][200],
                                  int p, char nomesAtualiza[][50], char valoresAtualiza[][200],
                                  char ultimo) {
    int i, rrn;
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;
    int tem_id = busca_por_id(m, nomesBusca);

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200];
    char nomeLinha[200];

    int temNomeEstacaoBusca = 0;
    char valorNomeEstacaoBusca[200];
    int tamValorNomeEstacaoBusca = 0;

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    for (i = 0; i < m; i++) {
        if (strcmp(nomesBusca[i], "nomeEstacao") == 0) {
            temNomeEstacaoBusca = 1;
            strcpy(valorNomeEstacaoBusca, valoresBusca[i]);
            tamValorNomeEstacaoBusca = strlen(valorNomeEstacaoBusca);
        }
    }

    if (temNomeEstacaoBusca && tamValorNomeEstacaoBusca != 0) {
        NoHash *h = buscar_hash(tabela, valorNomeEstacaoBusca, tamValorNomeEstacaoBusca);

        if (h == NULL) {
            if (ultimo == '1') {
                status = '1';
                fseek(bin, 0, SEEK_SET);
                escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
            }

            return;
        }
    }

    for (rrn = 0; rrn < proxRRN; rrn++) {
        int ok;

        int codProxEstacao_antigo;
        int tamNomeEstacao_antigo, tamNomeLinha_antigo;
        char nomeEstacao_antigo[200];
        char nomeLinha_antigo[200];

        removido = ler_regdados(bin, &removido, &proximo, &codEstacao, &codLinha,
                     &codProxEstacao, &distProxEstacao, &codLinhaIntegra,
                     &codEstIntegra, &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

        if (removido == '1') {
            continue;
        }

        ok = verificar_criterios(m, nomesBusca, valoresBusca,
                                 codEstacao, tamNomeEstacao, nomeEstacao,
                                 codLinha, tamNomeLinha, nomeLinha,
                                 codProxEstacao, distProxEstacao,
                                 codLinhaIntegra, codEstIntegra);

        if (ok) {
            codProxEstacao_antigo = codProxEstacao;
            tamNomeEstacao_antigo = tamNomeEstacao;
            tamNomeLinha_antigo = tamNomeLinha;

            if (tamNomeEstacao_antigo > 0) {
                memcpy(nomeEstacao_antigo, nomeEstacao, tamNomeEstacao_antigo);
            }
            nomeEstacao_antigo[tamNomeEstacao_antigo] = '\0';

            if (tamNomeLinha_antigo > 0) {
                memcpy(nomeLinha_antigo, nomeLinha, tamNomeLinha_antigo);
            }
            nomeLinha_antigo[tamNomeLinha_antigo] = '\0';

            atualizar_campos_registro(p, nomesAtualiza, valoresAtualiza,
                                      &codEstacao, &tamNomeEstacao, nomeEstacao,
                                      &codLinha, &tamNomeLinha, nomeLinha,
                                      &codProxEstacao, &distProxEstacao,
                                      &codLinhaIntegra, &codEstIntegra);

            if (codProxEstacao_antigo == -1 && codProxEstacao != -1) {
                nroParesEstacoes++;
            } else if (codProxEstacao_antigo != -1 && codProxEstacao == -1) {
                nroParesEstacoes--;
            }

            if (!(tamNomeEstacao_antigo == tamNomeEstacao &&
                  strncmp(nomeEstacao_antigo, nomeEstacao, tamNomeEstacao_antigo) == 0)) {

                if (tamNomeEstacao_antigo > 0) {
                    NoHash *h_antigo = buscar_hash(tabela, nomeEstacao_antigo, tamNomeEstacao_antigo);

                    if (h_antigo != NULL && h_antigo->repeticoes == 1) {
                        nroEstacoes--;
                    }

                    decrementar_hash(tabela, nomeEstacao_antigo, tamNomeEstacao_antigo);
                }

                if (tamNomeEstacao > 0) {
                    NoHash *h_novo = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

                    if (h_novo == NULL) {
                        nroEstacoes++;
                    }

                    inserir_hash(tabela, nomeEstacao, tamNomeEstacao);
                }
            }

            fseek(bin, -80, SEEK_CUR);

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

            if (tem_id) {
                break;
            }
        }
    }

    if (ultimo == '1') {
        status = '1';
        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    }
}

// insere um registro usando o conceito de pilha de rns dos removidos ensinado em aula
void inserir_registro_dinamico(FILE *bin, NoHash *tabela[],
                               int codEstacao, int codLinha, int codProxEstacao, int distProxEstacao,
                               int codLinhaIntegra, int codEstIntegra,
                               int tamNomeEstacao, char *nomeEstacao,
                               int tamNomeLinha, char *nomeLinha) {
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido = '0';
    int proximo = -1;

    long offset_insercao;
    int rrn_insercao;

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    status = '0';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    if (tamNomeEstacao > 0) {
        NoHash *h = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

        if (h == NULL) {
            nroEstacoes++;
        }

        inserir_hash(tabela, nomeEstacao, tamNomeEstacao);
    }

    if (codProxEstacao != -1) {
        nroParesEstacoes++;
    }

    if (topo != -1) {
        int proximo_rrn_topo;

        rrn_insercao = topo;
        offset_insercao = 17 + rrn_insercao * 80;

        fseek(bin, offset_insercao + 1, SEEK_SET);
        fread(&proximo_rrn_topo, sizeof(int), 1, bin);

        topo = proximo_rrn_topo;
    } else {
        rrn_insercao = proxRRN;
        offset_insercao = 17 + rrn_insercao * 80;
        proxRRN++;
    }

    fseek(bin, offset_insercao, SEEK_SET);
    escreve_regdados(bin, &removido, &proximo,
                     &codEstacao, &codLinha, &codProxEstacao, &distProxEstacao,
                     &codLinhaIntegra, &codEstIntegra,
                     &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

    status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
}
/* 
remover um registro usando o conceito de pilha de rns dos removidos ensinado em aula,
isto é, apenas marca como logicamente removido.
*/
void remover_registros_dinamico(FILE *bin, NoHash *tabela[], int m,
                                char nomesCampos[][50], char valoresCampos[][200],
                                int *topo, char ultimo) {
    int rrn;
    char status;
    int proxRRN, nroEstacoes, nroParesEstacoes;
    int tem_id = busca_por_id(m, nomesCampos);

    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200];
    char nomeLinha[200];

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    for (rrn = 0; rrn < proxRRN; rrn++) {
        int ok = 1;

        removido = ler_regdados(bin, &removido, &proximo, &codEstacao, &codLinha,
                     &codProxEstacao, &distProxEstacao, &codLinhaIntegra,
                     &codEstIntegra, &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

        if (removido == '1') {
            continue;
        }

        ok = verificar_criterios(m, nomesCampos, valoresCampos,
                                 codEstacao, tamNomeEstacao, nomeEstacao,
                                 codLinha, tamNomeLinha, nomeLinha,
                                 codProxEstacao, distProxEstacao,
                                 codLinhaIntegra, codEstIntegra);

        if (ok) {
            int topo_antigo = *topo;

            removido = '1';
            proximo = topo_antigo;

            fseek(bin, -80, SEEK_CUR);

            fwrite(&removido, sizeof(char), 1, bin);
            fwrite(&proximo, sizeof(int), 1, bin);

            fseek(bin, 75, SEEK_CUR);

            *topo = rrn;

            if (tamNomeEstacao > 0) {
                NoHash *h = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

                if (h != NULL && h->repeticoes == 1) {
                    nroEstacoes--;
                }

                decrementar_hash(tabela, nomeEstacao, tamNomeEstacao);
            }

            if (codProxEstacao != -1) {
                nroParesEstacoes--;
            }

            if (tem_id) {
                break;
            }
        }
    }

    if (ultimo == '1') status = '1';
    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
}

// busca os registro de acordo com os critérios dados pelo usuário 
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                      char nomesCampos[][50], char valoresCampos[][200]) {
    // variáveis de controle do laço e indicação se algum registro foi encontrado
    int i, rrn;
    int achou = 0;
    int tem_id = busca_por_id(m, nomesCampos);

    // campos do cabeçalho
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    // campos de um registro de dados
    char removido;
    int proximo;
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200];
    char nomeLinha[200];

    // variáveis auxiliares para verificar se a busca envolve nomeEstacao
    int temNomeEstacao = 0;
    char valorNomeEstacao[200];
    int tamValorNomeEstacao = 0;

    // percorre os campos de busca tentando descobrir se um deles é nomeEstacao
    for (i = 0; i < m; i++) {
        if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
            temNomeEstacao = 1;
            strcpy(valorNomeEstacao, valoresCampos[i]);
            tamValorNomeEstacao = strlen(valorNomeEstacao);
        }
    }

    // se a busca contém nomeEstacao e esse nome não está na hash,
    // então já sabemos que o registro não existe e podemos encerrar antes
    if (temNomeEstacao && strlen(valorNomeEstacao) != 0) {
        NoHash *h = buscar_hash(tabela, valorNomeEstacao, tamValorNomeEstacao);

        if (h == NULL) {
            printf("Registro inexistente.\n");
            return;
        }
    }

    // volta ao início do arquivo para ler o cabeçalho
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    // percorre todos os registros válidos do arquivo usando proxRRN como limite
    for (rrn = 0; rrn < proxRRN; rrn++) {
        int ok = 1;

        // lê sequencialmente um registro do arquivo
        removido = ler_regdados(bin, &removido, &proximo, &codEstacao, &codLinha,
                     &codProxEstacao, &distProxEstacao, &codLinhaIntegra,
                     &codEstIntegra, &tamNomeEstacao, nomeEstacao,
                     &tamNomeLinha, nomeLinha);

        // ignora registros logicamente removidos
        if (removido == '1')
            continue;

        // verifica se o registro lido atende a todos os critérios pedidos
        ok = verificar_criterios(m, nomesCampos, valoresCampos,
                                 codEstacao, tamNomeEstacao, nomeEstacao,
                                 codLinha, tamNomeLinha, nomeLinha,
                                 codProxEstacao, distProxEstacao,
                                 codLinhaIntegra, codEstIntegra);

        // se atender aos critérios, imprime o registro completo
        if (ok) {
            printar_registro_inteiro(codEstacao, tamNomeEstacao, nomeEstacao,
                                     codLinha, tamNomeLinha, nomeLinha,
                                     codProxEstacao, distProxEstacao,
                                     codLinhaIntegra, codEstIntegra);
            printf("\n");
            achou = 1;

            if (tem_id) {
                break;
            }
        }
    }

    // se nenhum registro foi encontrado ao final da busca, imprime a mensagem pedida
    if (!achou)
        printf("Registro inexistente.\n");
}

// AS QUATRO FUNÇÕES SEGUINTES SÃO AUXILIAR DA FUNÇÃO QUE LÊ O CSV

// pula a vírgula do csv usando o ponteiro
static void pular_virgula(char **p) {
    if (**p == ',') (*p)++;
}

// lê um inteiro no csv
static int ler_inteiro_csv(char **p) {
    int valor = 0;
    int tem_digito = 0;

    while (**p != ',' && **p != '\0' && **p != '\n' && **p != '\r') {
        // fórmula usada para converter string em inteiro(um casting, por assim dizer)
        valor = valor * 10 + (**p - '0');
        tem_digito = 1;
        (*p)++;
    }

    pular_virgula(p);
    return tem_digito ? valor : -1;
}

// lê uma string no csv
static int ler_string_csv(char **p, char *destino) {
    int i = 0;

    while (**p != ',' && **p != '\0' && **p != '\n' && **p != '\r') {
        destino[i++] = **p;
        (*p)++;
    }

    pular_virgula(p);
    return i;
}

// função que integra as anteriores para ler o registro inteiro
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

    if (csv == NULL) return 0;
    if (fgets(linha, MAX_LINHA_CSV, csv) == NULL) return 0;

    p = linha;

    *codEstacao       = ler_inteiro_csv(&p);
    *tamNomeEstacao   = ler_string_csv(&p, nomeEstacao);
    *codLinha         = ler_inteiro_csv(&p);
    *tamNomeLinha     = ler_string_csv(&p, nomeLinha);
    *codProxEstacao   = ler_inteiro_csv(&p);
    *distProxEstacao  = ler_inteiro_csv(&p);
    *codLinhaIntegra  = ler_inteiro_csv(&p);
    *codEstIntegra    = ler_inteiro_csv(&p);

    return 1;
}

// função principal de leitura de csv, 
void adicionar_csv_no_binario(FILE *csv, FILE *bin, NoHash *tabela[]) {
    // declara variáveis necessárias para execução da função 
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
    // vefica se o arquivo existe, senão, só mostra mensagem de erro e acaba a função
    if (csv == NULL || bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    // vefica se o cebaçalho para ver se ele existe logicamente, senão cria um
    status = '0';
    topo = -1;
    proxRRN = 0;
    nroEstacoes = 0;
    nroParesEstacoes = 0;

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);

    if (fgets(linha_cabecalho, MAX_LINHA_CSV, csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    rrn_atual = proxRRN;
    byteoffset = TAM_CABECALHO + rrn_atual * TAM_REG;
    // coloca o ponteiro onde queremos e começa a fazer o processo de leitura no csv e inserção no arquivo binário
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

        {
            // vê se ele já existe e atualiza no hashing
            NoHash *busca = buscar_hash(tabela, nomeEstacao, tamNomeEstacao);

            if (busca == NULL) {
                inserir_hash(tabela, nomeEstacao, tamNomeEstacao);
                nroEstacoes++;
            } else {
                busca->repeticoes++;
            }
        }

        if (codProxEstacao != -1) {
            nroParesEstacoes++;
        }
        rrn_atual++;
        byteoffset += TAM_REG;
    }   
    // volta a marcar o arquivo com '1', significando que é possível acessá-lo
    proxRRN = rrn_atual;
    status = '1';

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
}

/*
percorre o arquivo aproveitando a limitação de 80 bytes e
mostra na tela de acordo com a função já explicadas
*/

void mostrar_binario_sequencial(FILE *bin){
    // declara variáveis necessárias para a função
    char status;
    int topo, proxRRN, nroEstacoes, nroParesEstacoes;

    char removido ='0';
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
    // lê o cabeçalho
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &status, &topo, &proxRRN, &nroEstacoes, &nroParesEstacoes);
    if (proxRRN == 0 || nroEstacoes == 0) {
            printf("Registro inexistente.\n");
        return;
    }
    // percorre os rrns, lendo e imprimindo
    for(i = 0; i < proxRRN; i++){
        removido = ler_regdados(bin,
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

        if(removido == '1') continue;
        printar_registro_inteiro(codEstacao, tamNomeEstacao, nomeEstacao,
                             codLinha, tamNomeLinha, nomeLinha,
                             codProxEstacao, distProxEstacao,
                             codLinhaIntegra, codEstIntegra);
        printf("\n");
    }
}

/* 
função usada uma vez durante a função 5 durante a main para atender as demandas específicas de quando um
registro é dada com todos os campos, pedindo para tratar o caso NULO paara -1 como é no arquivo
*/
void ler_e_inserir_registro(FILE *bin, NoHash *tabela[]) {
    // declara as varíaveis necessárias para o código
    int codEstacao, codLinha, codProxEstacao, distProxEstacao;
    int codLinhaIntegra, codEstIntegra;
    int tamNomeEstacao, tamNomeLinha;
    char nomeEstacao[200], nomeLinha[200];

    char strCodEstacao[50], strCodLinha[50], strCodProxEstacao[50], strDistProxEstacao[50];
    char strCodLinhaIntegra[50], strCodEstIntegra[50];
    // faz a leitura de cada um dos campos
    scanf("%s", strCodEstacao);
    ScanQuoteString(nomeEstacao);
    scanf("%s", strCodLinha);
    ScanQuoteString(nomeLinha);
    scanf("%s", strCodProxEstacao);
    scanf("%s", strDistProxEstacao);
    scanf("%s", strCodLinhaIntegra);
    scanf("%s", strCodEstIntegra);
    // converte os campos caso eles sejam nulos para -1
    codEstacao = (strcmp(strCodEstacao, "NULO") == 0 || strcmp(strCodEstacao, "nulo") == 0) ? -1 : atoi(strCodEstacao);
    codLinha = (strcmp(strCodLinha, "NULO") == 0 || strcmp(strCodLinha, "nulo") == 0) ? -1 : atoi(strCodLinha);
    codProxEstacao = (strcmp(strCodProxEstacao, "NULO") == 0 || strcmp(strCodProxEstacao, "nulo") == 0) ? -1 : atoi(strCodProxEstacao);
    distProxEstacao = (strcmp(strDistProxEstacao, "NULO") == 0 || strcmp(strDistProxEstacao, "nulo") == 0) ? -1 : atoi(strDistProxEstacao);
    codLinhaIntegra = (strcmp(strCodLinhaIntegra, "NULO") == 0 || strcmp(strCodLinhaIntegra, "nulo") == 0) ? -1 : atoi(strCodLinhaIntegra);
    codEstIntegra = (strcmp(strCodEstIntegra, "NULO") == 0 || strcmp(strCodEstIntegra, "nulo") == 0) ? -1 : atoi(strCodEstIntegra);

    tamNomeEstacao = strlen(nomeEstacao);
    tamNomeLinha = strlen(nomeLinha);
    // chama a função principal
    inserir_registro_dinamico(bin, tabela,
                              codEstacao, codLinha, codProxEstacao, distProxEstacao,
                              codLinhaIntegra, codEstIntegra,
                              tamNomeEstacao, nomeEstacao,
                              tamNomeLinha, nomeLinha);
}


