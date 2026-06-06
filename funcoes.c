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
    
    if (bin == NULL) {
        return;
    }
    cabecalho reg_cab;
    dados reg_dados;
    int rrn;


    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);

    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        
        int byteoffset = calculo_byteoffset_dados(rrn);
        fseek(bin, byteoffset, SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1') {
            continue;
        }
        if (reg_dados.tamNomeLinha > 0) {
            inserir_hash(tabela, reg_dados.nomeLinha, reg_dados.tamNomeLinha);
        }
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










/*Função SELECT WHERE :  dados os campos e os valores procurados, imprimir os registros validos */
void buscar_registros(FILE *bin, NoHash *tabela[], int m,
                      char nomesCampos[][50], char valoresCampos[][200]) {
    if (bin == NULL) {
        return;
    }

    int i, rrn;
    int achou = 0;

    cabecalho reg_cab;
    dados reg_dados;

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

    ler_cabecalho(bin, &reg_cab);
    
    if (reg_cab.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    /*ler os registros de forma sequencial
    Se o registro foi removido, ignora na busca
    Se não, verifica qual campo foi digitado para busca
    */
    for (rrn = 0; rrn < reg_cab.proxRRN; rrn++) {
        int ok = 1;
        
        // CORREÇÃO: Reposiciona e lê passando a referência da struct de dados
        fseek(bin, calculo_byteoffset_dados(rrn), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1')
            continue;

        for (i = 0; i < m && ok; i++) {

            if (strcmp(nomesCampos[i], "codEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.codEstacao != -1)
                        ok = 0;
                } else if (reg_dados.codEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "nomeEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.tamNomeEstacao != 0)
                        ok = 0;
                } else if (reg_dados.tamNomeEstacao == 0 ||
                           (int)strlen(valoresCampos[i]) != reg_dados.tamNomeEstacao ||
                           strncmp(reg_dados.nomeEstacao, valoresCampos[i], reg_dados.tamNomeEstacao) != 0) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codLinha") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.codLinha != -1)
                        ok = 0;
                } else if (reg_dados.codLinha != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "nomeLinha") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.tamNomeLinha != 0)
                        ok = 0;
                } else if (reg_dados.tamNomeLinha == 0 ||
                           (int)strlen(valoresCampos[i]) != reg_dados.tamNomeLinha ||
                           strncmp(reg_dados.nomeLinha, valoresCampos[i], reg_dados.tamNomeLinha) != 0) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codProxEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.codProxEstacao != -1)
                        ok = 0;
                } else if (reg_dados.codProxEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "distProxEstacao") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.distProxEstacao != -1)
                        ok = 0;
                } else if (reg_dados.distProxEstacao != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codLinhaIntegra") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.codLinhaIntegra != -1)
                        ok = 0;
                } else if (reg_dados.codLinhaIntegra != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }

            else if (strcmp(nomesCampos[i], "codEstIntegra") == 0) {
                if (strlen(valoresCampos[i]) == 0) {
                    if (reg_dados.codEstIntegra != -1)
                        ok = 0;
                } else if (reg_dados.codEstIntegra != atoi(valoresCampos[i])) {
                    ok = 0;
                }
            }
        }

        // Se o registro atendeu a todas as m condições concomitantes
        if (ok) {
            imprime_inteiro_ou_nulo(reg_dados.codEstacao);
            imprime_texto_ou_nulo(reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
            imprime_inteiro_ou_nulo(reg_dados.codLinha);
            imprime_texto_ou_nulo(reg_dados.nomeLinha, reg_dados.tamNomeLinha);
            imprime_inteiro_ou_nulo(reg_dados.codProxEstacao);
            imprime_inteiro_ou_nulo(reg_dados.distProxEstacao);
            imprime_inteiro_ou_nulo(reg_dados.codLinhaIntegra);
            imprime_inteiro_ou_nulo(reg_dados.codEstIntegra);
            
            achou = 1;
            printf("\n");
        }
    }
    
    if (!achou)
        printf("Registro inexistente.\n");
}













/* Função que recebe um arquivo csv, realiza o parse manual tratando campos vazios
   e retorna as informações preenchidas diretamente na struct 'dados' */
int ler_registro_csv(FILE *csv, dados *reg_dados) {
    char linha[MAX_LINHA_CSV];
    char *p;
    int valor, tem_digito, i;

    if (csv == NULL || reg_dados == NULL) return 0;
    if (fgets(linha, MAX_LINHA_CSV, csv) == NULL) return 0;

    //limpa a memoria 
    memset(reg_dados->nomeEstacao, 0, sizeof(reg_dados->nomeEstacao));
    memset(reg_dados->nomeLinha, 0, sizeof(reg_dados->nomeLinha));
    p = linha;

    // 1. codEstacao
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->codEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    // 2. nomeEstacao
    i = 0;
    while (i > 0 && (reg_dados->nomeEstacao[i-1] == ' ' || 
                     reg_dados->nomeEstacao[i-1] == '\r' || 
                     reg_dados->nomeEstacao[i-1] == '\n')) {
        i--; 
    }
    reg_dados->nomeEstacao[i] = '\0';
    reg_dados->tamNomeEstacao = i;
    if (*p == ',') p++;

    // 3. codLinha
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->codLinha = tem_digito ? valor : -1;
    if (*p == ',') p++;

    // 4. nomeLinha
    i = 0;
    while (i > 0 && (reg_dados->nomeLinha[i-1] == ' ' || 
                     reg_dados->nomeLinha[i-1] == '\r' || 
                     reg_dados->nomeLinha[i-1] == '\n')) {
        i--; 
    }
    reg_dados->nomeLinha[i] = '\0';
    reg_dados->tamNomeLinha = i;
    if (*p == ',') p++;

    // 5. codProxEstacao
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->codProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    // 6. distProxEstacao
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->distProxEstacao = tem_digito ? valor : -1;
    if (*p == ',') p++;

    // 7. codLinhaIntegra
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->codLinhaIntegra = tem_digito ? valor : -1;
    if (*p == ',') p++;

    // 8. codEstIntegra
    valor = 0; tem_digito = 0;
    while (*p != ',' && *p != '\0' && *p != '\n' && *p != '\r') {
        valor = valor * 10 + (*p - '0');
        tem_digito = 1;
        p++;
    }
    reg_dados->codEstIntegra = tem_digito ? valor : -1;

   

    return 1; 
}








void adicionar_csv_no_binario(char *arq_csv, char *arq_bin, NoHash *tabela[]) {
    char linha_cabecalho[MAX_LINHA_CSV];
    
    FILE *csv = fopen(arq_csv, "r");
    if (csv == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    cabecalho reg_cab;
    dados reg_dados;

    FILE *bin = escrever_binario(arq_bin); 
    
    if (bin == NULL) {
        // Arquivo não existe -> cria e abre para escrita
        bin = cria_escreve_binario(arq_bin); 
        if (bin == NULL) {
            printf("Falha no processamento do arquivo.\n");
            fclose(csv); // CORREÇÃO 3: Evita vazamento do descritor do CSV
            return;
        }
        // CORREÇÃO 2: Apenas atribuição, sem redeclarar o tipo 'cabecalho'
        reg_cab = cria_cabecalho();
        escreve_cabecalho(bin, &reg_cab);
    } else {
        // Arquivo já existe -> lê o cabeçalho existente
        ler_cabecalho(bin, &reg_cab);
        
        // Se o arquivo existe - mas está inconsistente
        if (reg_cab.status == '0') {
            printf("Falha no processamento do arquivo (Arquivo corrompido).\n");
            fclose(bin);
            fclose(csv);
            return;
        }

        // Se estava '1', mudamos para '0' enquanto alteramos para garantir a segurança
        reg_cab.status = '0';
        fseek(bin, 0, SEEK_SET);
        escreve_cabecalho(bin, &reg_cab);
    }
    
    // Pula a linha de cabeçalho do CSV
    if (fgets(linha_cabecalho, MAX_LINHA_CSV, csv) == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        fclose(csv);
        return;
    }
    
    int rrn_atual = reg_cab.proxRRN;
    int byteoffset = calculo_byteoffset_dados(rrn_atual);
    fseek(bin, byteoffset, SEEK_SET);
    // Garante que a struct comece limpa antes da primeira leitura
    memset(&reg_dados, 0, sizeof(dados));

    while (ler_registro_csv(csv, &reg_dados)) {

        reg_dados.removido = '0';
        reg_dados.proximo = NEGATIVO;

        // Escreve os dados estruturados no arquivo binário
        escreve_regdados(bin, &reg_dados);

        // Gerenciamento de metadados via tabela Hash usando as strings lidas
        if (reg_dados.tamNomeEstacao > 0) {
            NoHash *busca = buscar_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);

            if (busca == NULL) {
                inserir_hash(tabela, reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
                reg_cab.nroEstacoes++;
            } else {
                busca->repeticoes++;
            }
        }

        if (reg_dados.codProxEstacao != NEGATIVO) {
            reg_cab.nroParesEstacoes++;
        }
        
        rrn_atual++;

        memset(&reg_dados, 0, sizeof(dados));
    }

    reg_cab.proxRRN = rrn_atual;
    reg_cab.status = '1'; // Arquivo fechado com sucesso e estável

    fseek(bin, 0, SEEK_SET);
    escreve_cabecalho(bin, &reg_cab);

    fclose(bin);
    fclose(csv);
}



/*Função que lê todos os registros do arquivo binário e retorna essas informações de forma sequencial.
*/
void mostrar_binario_sequencial(FILE *bin){
    // Cria um registro temporário para ler os dados do arquivo
    cabecalho reg_cab ;
    dados reg_dados;
    int i;

    if(bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Posiciona a leitura no inicio do arquivo e le o registro de cabeçalho
    fseek(bin, 0, SEEK_SET);
    ler_cabecalho(bin, &reg_cab);
    if (reg_cab.proxRRN == 0 || reg_cab.nroEstacoes == 0) {
        printf("Registro inexistente.\n");
        return;
    }
    if(reg_cab.status != '1'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for(i = 0; i < reg_cab.proxRRN; i++){
        fseek(bin, calculo_byteoffset_dados(i), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if(reg_dados.removido == '1') continue;

        imprime_inteiro_ou_nulo(reg_dados.codEstacao);
        imprime_texto_ou_nulo(reg_dados.nomeEstacao, reg_dados.tamNomeEstacao);
        imprime_inteiro_ou_nulo(reg_dados.codLinha);
        imprime_texto_ou_nulo(reg_dados.nomeLinha, reg_dados.tamNomeLinha);
        imprime_inteiro_ou_nulo(reg_dados.codProxEstacao);
        imprime_inteiro_ou_nulo(reg_dados.distProxEstacao);
        imprime_inteiro_ou_nulo(reg_dados.codLinhaIntegra);
        imprime_inteiro_ou_nulo(reg_dados.codEstIntegra);
        printf("\n");
    }
}