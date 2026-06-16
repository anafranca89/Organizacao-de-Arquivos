#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"



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
    if (bin == NULL) {
        return;
    }

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

        if (h == NULL) {
            printf("Registro inexistente.\n");
            return;
        }
    }
    fseek(bin, 0, SEEK_SET);
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
    
    if (!achou)
        printf("Registro inexistente.\n");
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
        printf("Falha no processamento do arquivo. EEE\n");
        return;
    }

    for(i = 0; i < reg_cab.proxRRN; i++){
        fseek(bin, calculo_byteoffset_dados(i), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if(reg_dados.removido == '1') continue;
        imprime_registro_dados(&reg_dados);
    }
}




 /* 
    insere um registro usando o conceito de pilha de rns dos removidos ensinado em aula,
    isto é, apenas marca como logicamente removido.
    Parâmetros:  arquivo de dados, Tabela Hash, 

*/

long ler_e_inserir_registro(FILE *bin, NoHash *tabela[], cabecalho *reg_cabecalho) {
    dados reg_dados;

    ler_registro_entrada(&reg_dados);

    return inserir_registro_dinamico(
        bin,
        tabela,
        reg_cabecalho,
        &reg_dados
    );
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

void ler_registro_entrada(dados *reg_dados) {
    char strCodEstacao[50];
    char strCodLinha[50];
    char strCodProxEstacao[50];
    char strDistProxEstacao[50];
    char strCodLinhaIntegra[50];
    char strCodEstIntegra[50];

    *reg_dados = cria_dados();

    scanf("%s", strCodEstacao);
    ScanQuoteString(reg_dados->nomeEstacao);
    scanf("%s", strCodLinha);
    ScanQuoteString(reg_dados->nomeLinha);
    scanf("%s", strCodProxEstacao);
    scanf("%s", strDistProxEstacao);
    scanf("%s", strCodLinhaIntegra);
    scanf("%s", strCodEstIntegra);

    reg_dados->codEstacao =
        (strcasecmp(strCodEstacao, "NULO") == 0) ? -1 : atoi(strCodEstacao);

    reg_dados->codLinha =
        (strcasecmp(strCodLinha, "NULO") == 0) ? -1 : atoi(strCodLinha);

    reg_dados->codProxEstacao =
        (strcasecmp(strCodProxEstacao, "NULO") == 0) ? -1 : atoi(strCodProxEstacao);

    reg_dados->distProxEstacao =
        (strcasecmp(strDistProxEstacao, "NULO") == 0) ? -1 : atoi(strDistProxEstacao);

    reg_dados->codLinhaIntegra =
        (strcasecmp(strCodLinhaIntegra, "NULO") == 0) ? -1 : atoi(strCodLinhaIntegra);

    reg_dados->codEstIntegra =
        (strcasecmp(strCodEstIntegra, "NULO") == 0) ? -1 : atoi(strCodEstIntegra);

    reg_dados->tamNomeEstacao = strlen(reg_dados->nomeEstacao);
    reg_dados->tamNomeLinha = strlen(reg_dados->nomeLinha);

    reg_dados->removido = '0';
    reg_dados->proximo = -1;
}

long inserir_registro_dinamico(
    FILE *bin,
    NoHash *tabela[],
    cabecalho *cab,
    dados *reg_dados
) {
    long offset_insercao;
    int rrn_insercao;

    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return -1;
    }

    if (cab->topo != -1) {
        rrn_insercao = cab->topo;
        offset_insercao = calculo_byteoffset_dados(rrn_insercao);

        fseek(bin, offset_insercao + 1, SEEK_SET);

        int proximo_rrn_topo;
        fread(&proximo_rrn_topo, sizeof(int), 1, bin);

        cab->topo = proximo_rrn_topo;
    } else {
        rrn_insercao = cab->proxRRN;
        offset_insercao = calculo_byteoffset_dados(rrn_insercao);
        cab->proxRRN++;
    }

    if (reg_dados->tamNomeEstacao > 0) {
        NoHash *h = buscar_hash(
            tabela,
            reg_dados->nomeEstacao,
            reg_dados->tamNomeEstacao
        );

        if (h == NULL) {
            cab->nroEstacoes++;
        }

        inserir_hash(
            tabela,
            reg_dados->nomeEstacao,
            reg_dados->tamNomeEstacao
        );
    }

    if (reg_dados->codProxEstacao != -1) {
        cab->nroParesEstacoes++;
    }

    fseek(bin, offset_insercao, SEEK_SET);
    escreve_regdados(bin, reg_dados);

    return offset_insercao;
}

// Função que verifica se arquivo foi realmente aberto, emitindo mensagem de erro e encerrando programa senão
int verifica_se_foi_aberto(FILE* arq0, FILE* arq1, int tem_dois_arquivos) {
    
    // Verifica se o primeiro arquivo não existe ou o segundo não existe e esperamos ter 2 arquivos
    if (arq0 == NULL || (tem_dois_arquivos && arq1 == NULL)) {
        
        printf("Falha no processamento do arquivo.\n");
        
        // fecha o que tiver aberto
        if (arq0 != NULL) fclose(arq0);
        if (arq1 != NULL) fclose(arq1);

        return 1;
    }
    return 0;
}
