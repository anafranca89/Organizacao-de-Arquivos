#include <stdio.h>
#include "arvb.h"  





/* lê o cabeçalho no arquivo de indices e retorna p/ a struct de parametro
Ponteiro deve estar no começo do arquivo !
Parametros : arquivo binário para escrita e a struct para escrita do cabecalho
*/
void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fread(&cab->noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->topo, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->nroNos, sizeof(int), 1, ponteiro_arquivo);
}
/* Escreve o cabeçalho no arquivo de indice, com as informações da struct de parametro.
Parametros : arquivo inário para escrita e a struct em RAM do cabecalho
*/
void escreve_ind_cabecalho(FILE *ponteiro_arquivo, cab_indice* cab){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&cab->noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->topo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&cab->nroNos, sizeof(int), 1, ponteiro_arquivo);
}

/* Lê a struct de indices no arquivo. 
Ponteiro deve estar no byteoffset correto!
Parametros: ponteiro p/arquivo de indice, ponteiro p/ struct onde serão salvas as informações
*/

void ler_indice(FILE* ponteiro_arquivo, indice* reg_indice ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fread(&reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fread(&reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}



/* Escreve a struct de indices no arquivo. 
Ponteiro deve estar no byteoffset correto!
Parametros: ponteiro p/arquivo de indice, a struct a ser salva.
*/
void escreve_indice(FILE *ponteiro_arquivo, indice* reg_indice){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fwrite(&reg_indice->removido, sizeof(char), 1, ponteiro_arquivo);
    fwrite(&reg_indice->proximo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->tipoNo, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->nroChaves, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->C3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->Pr3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv1, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv2, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv3, sizeof(int), 1, ponteiro_arquivo);
    fwrite(&reg_indice->arv4, sizeof(int), 1, ponteiro_arquivo);
}



/*Construtor de um Cabeçalho --> Arvore Vazia*/
cab_indice new_cab_indice(){
    cab_indice aux;
    aux.status ='0';
    aux.noRaiz =NEGATIVO;
    aux.topo =NEGATIVO;
    aux.proxRRN = 0;
    aux.nroNos =0;
    return aux;
}


/*Construtor Base de Um índice--> Valores são criados com o valor  padrão
removido =0, proximo = -1, chaves e ponteiros -1, nrChaves =0*/
indice new_indice(){
    indice aux; 
    aux.removido = '0';
    aux.proximo= NEGATIVO;
    aux.tipoNo= NEGATIVO;
    aux.nroChaves= 0;
    aux.C1= NEGATIVO;
    aux.Pr1= NEGATIVO;
    aux.C2= NEGATIVO;
    aux.Pr2= NEGATIVO;
    aux.C3= NEGATIVO;
    aux.Pr3= NEGATIVO;
    aux.arv1= NEGATIVO; 
    aux.arv2= NEGATIVO; 
    aux.arv3= NEGATIVO; 
    aux.arv4= NEGATIVO; 
    return aux;
}

/*Função Auxiliar:
Retorna o byteoffset de um RRN do arquivo de indices da arvore B
*/
int calculo_byteoffset_indice(int RRN){
    return RRN*TAM_REG_IND +TAM_CAB_IND;
}



/*Algoritmo Driver: Cria a lista de nós da árvore.
Caso1: Arquivo/arvore n existe--> cria um arquivo p/leitura e escrita
Caso2: Arquivo existe --> apenas abre arquivo p/escrita 
    - Enquanto existe registros --> count é menor que proxRRN
        - verifica se n é logicamente removido, senão vai pro proximo registro
        - removido == 0, então tenta inserir na raiz
        - segue a função de inserção
            - retornou PROMOTION => raiz sofreu split
            - cria novo nó
        - Atualiza cabeçalho da arvore
        - fecha arquivo
*/
void cria_arvore(FILE* arq_dados, char* arq_index) {

    FILE* indexes = cria_escreve_binario(arq_index);
    if (indexes == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    cab_indice index_cab = new_cab_indice();

    index_cab.status = '0';
    fseek(indexes, 0, SEEK_SET);
    escreve_ind_cabecalho(indexes, &index_cab);

    cabecalho reg_cab_dados = cria_cabecalho();

    fseek(arq_dados, 0, SEEK_SET);
    ler_cabecalho(arq_dados, &reg_cab_dados);

    if (reg_cab_dados.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(indexes);
        return;
    }

    dados reg_dados = cria_dados();

    for (int i = 0; i < reg_cab_dados.proxRRN; i++) {

        int byteoffset_dados = calculo_byteoffset_dados(i);

        fseek(arq_dados, byteoffset_dados, SEEK_SET);
        ler_regdados(arq_dados, &reg_dados);

        if (reg_dados.removido == '1') {
            continue;
        }

        insere_recebendo_chave_e_byteoffset(
            indexes,
            &index_cab,
            reg_dados.codEstacao,
            byteoffset_dados
        );
    }

    index_cab.status = '1';
    fseek(indexes, 0, SEEK_SET);
    escreve_ind_cabecalho(indexes, &index_cab);

    fclose(indexes);
}

void insere_recebendo_chave_e_byteoffset(FILE *indexes, cab_indice *index_cab, int chave, int byteoffset_dados) {
    int pos_chave_no = NEGATIVO;

    busca_arvore(indexes, index_cab->noRaiz, chave, &pos_chave_no);

    if (pos_chave_no != NEGATIVO) {
        return;
    }

    int filho_promovido;
    int chave_promovida;
    int byte_dados_promovido;

    int retorno_prom = insere_arvore(
        indexes,
        index_cab,
        index_cab->noRaiz,
        chave,
        byteoffset_dados,
        &filho_promovido,
        &chave_promovida,
        &byte_dados_promovido
    );

    if (retorno_prom == PROMOTION) {

        int rrn_raiz_antiga = index_cab->noRaiz;

        if (rrn_raiz_antiga != NEGATIVO) {

            int byte_raiz_antiga = calculo_byteoffset_indice(rrn_raiz_antiga);

            indice raiz_antiga;

            fseek(indexes, byte_raiz_antiga, SEEK_SET);
            ler_indice(indexes, &raiz_antiga);

            if (raiz_antiga.arv1 == NEGATIVO) {
                raiz_antiga.tipoNo = NEGATIVO;
            } else {
                raiz_antiga.tipoNo = 1;
            }

            fseek(indexes, byte_raiz_antiga, SEEK_SET);
            escreve_indice(indexes, &raiz_antiga);
        }

        indice nova_raiz = new_indice();

        if (rrn_raiz_antiga == NEGATIVO) {
            nova_raiz.tipoNo = NEGATIVO;
        } else {
            nova_raiz.tipoNo = 0;
        }

        nova_raiz.nroChaves = 1;

        nova_raiz.C1 = chave_promovida;
        nova_raiz.Pr1 = byte_dados_promovido;

        nova_raiz.arv1 = rrn_raiz_antiga;
        nova_raiz.arv2 = filho_promovido;

        int rrn_nova_raiz = index_cab->proxRRN;

        fseek(indexes, calculo_byteoffset_indice(rrn_nova_raiz), SEEK_SET);
        escreve_indice(indexes, &nova_raiz);

        index_cab->noRaiz = rrn_nova_raiz;
        index_cab->proxRRN++;
        index_cab->nroNos++;
    }
}

/*Assumindo q existe o arquivo de indice existe e é != NULL.
Busca uma chave única int - codEstacao-  num nó da árvore.
Caso 1: Arvore com elementos --> rrn da raiz >=0
Caso 2: Arvore vazia --> rrn da raiz == -1
Parametros: Arquivo de indices, rrn do no onde faz a busca, a chave para busca, 
        e a posicao (1, 2, 3) da chave. posicao == -1 se a chave n existe
        ------util para inserção
Retorna : RRN do nó onde DEVERIA ESTAR ou ESTÁ.

->Chama a função no nó raiz
    - arvore existe? 
        - Se não - retorna -1
        - Se sim - continua
    - No atual foi removido?
        - retorna -1, ou seja, não encontrou a chave
    - Senão - Lê nó (posiciona o ponteiro para ler o no exato)
        - chave está no nó ? 
            - se sim, acaba a busca, retorna o rrn e atualiza a pos_chava_no
            - se não, verifica se é nó intermediário
                - se sim, continua a busca recursivamente
                - se não, termina a busca. E no folha - logo sem descendentes p/buscar.

*/
int busca_arvore(FILE *arquivo, int rrn_no, int chave_unica, int *pos_chave_no){
    if(rrn_no==NEGATIVO){
        if (pos_chave_no != NULL) *pos_chave_no = NEGATIVO;
        return NEGATIVO;
    } 
    indice aux;
    int bytecorrente = calculo_byteoffset_indice(rrn_no);
    //posiciona no lugar de leitura dos dados
    fseek(arquivo, bytecorrente, SEEK_SET);
    ler_indice(arquivo, &aux);

    if(aux.removido == '1'){
        if (pos_chave_no != NULL) *pos_chave_no = NEGATIVO;
        return NEGATIVO;
    }

    // no não removido 
    if (chave_unica == aux.C1) {
        if (pos_chave_no != NULL) *pos_chave_no = 1;
        return rrn_no;
    } 
    else if (aux.nroChaves >= 2 && chave_unica == aux.C2) {
        if (pos_chave_no != NULL) *pos_chave_no = 2;
        return rrn_no;
    } 
    else if (aux.nroChaves == 3 && chave_unica == aux.C3) {
        if (pos_chave_no != NULL) *pos_chave_no = 3;
        return rrn_no;
    }
    //é no folha?
    if (aux.tipoNo == NEGATIVO) { 
        if (pos_chave_no != NULL) *pos_chave_no = NEGATIVO; 
        return rrn_no; 
    }


    //Continua busca recursiva
    if (chave_unica < aux.C1) {
        //busca subarvore 1
        return busca_arvore(arquivo, aux.arv1, chave_unica, pos_chave_no);
    } 
    else if (aux.nroChaves == 1 || (aux.nroChaves >= 2 && chave_unica < aux.C2)) {
        //busca subarvore 2
        return busca_arvore(arquivo, aux.arv2, chave_unica, pos_chave_no);
    } 
    else if (aux.nroChaves == 2 || (aux.nroChaves == 3 && chave_unica < aux.C3)) {
        //busca subarvore 3
        return busca_arvore(arquivo, aux.arv3, chave_unica, pos_chave_no);
    } 
    else {
        //busca subarvore 4
        return busca_arvore(arquivo, aux.arv4, chave_unica, pos_chave_no);
    }
}










/*Parametros: arquivo index arvore, rrn do nó atual a ser inserido, 
                chave a ser inserida,ponteiro da chave,  chave da promoção,
                 nó filho promovido.
Retorna INT : 1 se houve PROMOTION
              -1 se houve ERRO
              0 Padrão - No PROMOTION
Caso base : rrn-atual = -1, promove a chave
    OBS:Inserção apenas em No Folha  E começa por uma busca(na raiz!:))
    - busca a posição esperada da chave
        - encontrou a chave? -> retorna erro de chaveduplicada -> quando pos_chave_no != de -1 
        - SENÃO- temos o rrn esperado daquela chave.
            - lê o nó
     - encontra a posicao esperada da chave no nó - 1, 2, 3
    - ret_value = insere recursivo
        -  NO_PROMOTION ou ERRO na pilha de recursão  ?
            - retorna NO PROMOTION  OU NEGATIVO
        - Senão tenta inserção no nó
            - nó tem espaço?
                - insere ordenado
            - nó não tem espaço?
                - split-> cria um novo no, ordena as chaves, escreve o no_esq
                    escreve o no_dir, promove a chave mais a esquerda do no_dir, 
                    com filho nó_dir.
                retorna PROMOTION

*/
int insere_arvore(FILE* arq_index, cab_indice *cabecalho, int rrn_no , int chave, 
                    int byte_dados_chave , int *filho_promovido, int*chave_promovida, int *byte_dados_promovido){

    int valor_retorno, pos_chave_no;
    if (rrn_no == NEGATIVO){
        *chave_promovida = chave;
        *filho_promovido = NEGATIVO;
        *byte_dados_promovido = byte_dados_chave; 
        return PROMOTION; 
    }
    if(rrn_no == cabecalho->noRaiz){
        pos_chave_no = NEGATIVO;
        busca_arvore(arq_index, cabecalho->noRaiz, chave, &pos_chave_no);
        if(pos_chave_no != NEGATIVO){
            return NEGATIVO; //erro: chave duplicada
        }
    }
    
    
    indice no_aux; 
    int byteoffset = calculo_byteoffset_indice(rrn_no);
    fseek(arq_index, byteoffset, SEEK_SET);
    ler_indice(arq_index, &no_aux);
    

    int prox_rrn = NEGATIVO;
    if (chave < no_aux.C1) {
        prox_rrn = no_aux.arv1;
    } else if (no_aux.nroChaves == 1 || 
            (no_aux.nroChaves >= 2 && chave < no_aux.C2)) {
        prox_rrn = no_aux.arv2;
    } else if (no_aux.nroChaves == 2 || 
            (no_aux.nroChaves == 3 && chave < no_aux.C3)) {
        prox_rrn = no_aux.arv3;
    } else {
        prox_rrn = no_aux.arv4;
    }



    valor_retorno = insere_arvore(arq_index, cabecalho, prox_rrn, chave, byte_dados_chave,
                                    filho_promovido, chave_promovida, byte_dados_promovido);

    

    if (valor_retorno == NO_PROMOTION || valor_retorno == NEGATIVO){
        return valor_retorno;
    }

    //seta fseek na posição correta
    fseek(arq_index, byteoffset, SEEK_SET);
    ler_indice(arq_index, &no_aux);

    //No tem espaço?
    if (no_aux.nroChaves<3){
        insere_ordenado_no(&no_aux, *chave_promovida, *filho_promovido, *byte_dados_promovido);
        fseek(arq_index, byteoffset, SEEK_SET);
        escreve_indice(arq_index, &no_aux);
        return NO_PROMOTION;
    }else{
        int nova_chave_promovida, novo_byte_promovido, novo_filho_promovido;

        split(arq_index, cabecalho, rrn_no, &no_aux, 
            *chave_promovida, *byte_dados_promovido, *filho_promovido,
            &nova_chave_promovida, &novo_byte_promovido, &novo_filho_promovido);

        *chave_promovida = nova_chave_promovida;
        *byte_dados_promovido = novo_byte_promovido;
        *filho_promovido = novo_filho_promovido;
     return PROMOTION;
    } 


}




/*Sobreescreve o no de indice, para ordenar as chaves dentro dele.
 Como ele insere no nó, é assumido que nroChaves<3.
 Parametros: no p/sobrescerver, chave, filho dessa chave, e o ponteiro p/arquivo de dados.
    - Se qtdChaves == 0 - insere no primeiro espaço 
    - se qtdchaves == 1 --> 2 casos     
                        - insere no espaço 2 
                        - ou shifta e insere no inicio
    - se qtdchaves ==2 --> 3 casos
                        - insere no espaço 3
                        - shifta tudo e insere no inicio
                        - shifta o espaço 2 e insere no meio.
*/
void insere_ordenado_no(indice *no, int chave, int filho_promovido,  int byte_ponteiro){
    if(no->nroChaves == 0){
        //apenas insere no inicio
        no->C1 = chave;
        no->Pr1 = byte_ponteiro;
        no->arv2 = filho_promovido;
        no->nroChaves = 1;
    }else if (no->nroChaves == 1) {
        if(chave > no->C1){
            no->C2 = chave;
            no->Pr2 = byte_ponteiro;
            no->arv3 = filho_promovido;
        }else{
            //shifta e insere no começo do no
            no->C2 = no->C1;
            no->Pr2 = no->Pr1;
            no->arv3 = no->arv2;
            no->arv2 = filho_promovido;
            
            no->C1 = chave;
            no->Pr1 = byte_ponteiro;
        }
        no->nroChaves = 2;
    }else if (no->nroChaves == 2) {
        if(chave >no->C2){
            no->C3 = chave;
            no->Pr3 = byte_ponteiro;
            no->arv4= filho_promovido;
        }else if (chave > no->C1){
            // shifta 
            no->C3 = no->C2;
            no->Pr3= no->Pr2;
            no->arv4 = no->arv3;
            //insere no meio
            no->C2 = chave;
            no->Pr2 = byte_ponteiro;
            no->arv3 = filho_promovido;
        }else{
            //shifta tudo e insere no inicio
            no->C3 = no->C2;
            no->Pr3 = no->Pr2;
            no->arv4 = no->arv3;

            no->C2 = no->C1;
            no->Pr2 = no->Pr1;
            no->arv3 = no->arv2;
            no->arv2 = filho_promovido;

            //insere no inicio
            no->C1 = chave;
            no->Pr1 = byte_ponteiro;
        }
        no->nroChaves = 3;
    }
    
}



/* Cria um novo no, promove uma chave e a sub arvore, copiando os valores anteriores.
Parametros: arquivo de indezx, cabeçalho do index, rrn do no, o proprio no, 
            chave q deve ser inserida, o rrn filho, e o byte_ponteiro.
            a chave q é realmente promovida, o seu byte ponteiro e seu filho
    - Cria uma estrutura de comparação, com espaço para m+1 ->4 chaves, 4 ponteiro, 5 subarvores.
    - copia as chaves da pag atual e compara com a chave.
    - ordena as chaves
    - aloca um novo no no index
    - atualiza a chave a ser promovida e o filho a ser promovido 
    - organiza as chaves nos 2 nós e retorna a novapagina.
*/

int alocar_rrn_no_arvore(FILE *arq_index, cab_indice *cab_ind) {
    int rrn_alocado;

    if (cab_ind->topo != NEGATIVO) {
        indice no_removido;

        rrn_alocado = cab_ind->topo;

        fseek(arq_index, calculo_byteoffset_indice(rrn_alocado), SEEK_SET);
        ler_indice(arq_index, &no_removido);

        cab_ind->topo = no_removido.proximo;
    } else {
        rrn_alocado = cab_ind->proxRRN;
        cab_ind->proxRRN++;
    }

    cab_ind->nroNos++;

    return rrn_alocado;
}

void split(
    FILE *arq_index,
    cab_indice *cab_ind,
    int rrn_no_ant,
    indice *no_ant,
    int chave,
    int byte_chave,
    int filho_chave,
    int *chave_promovida,
    int *byte_promovido,
    int *filho_promovido
) {
    int chaves_temp[4];
    int byte_temp[4];
    int arv_temp[5];

    chaves_temp[0] = no_ant->C1;
    byte_temp[0] = no_ant->Pr1;

    chaves_temp[1] = no_ant->C2;
    byte_temp[1] = no_ant->Pr2;

    chaves_temp[2] = no_ant->C3;
    byte_temp[2] = no_ant->Pr3;

    arv_temp[0] = no_ant->arv1;
    arv_temp[1] = no_ant->arv2;
    arv_temp[2] = no_ant->arv3;
    arv_temp[3] = no_ant->arv4;

    int i = 2;

    while (i >= 0 && chave < chaves_temp[i]) {
        chaves_temp[i + 1] = chaves_temp[i];
        byte_temp[i + 1] = byte_temp[i];

        arv_temp[i + 2] = arv_temp[i + 1];

        i--;
    }

    chaves_temp[i + 1] = chave;
    byte_temp[i + 1] = byte_chave;
    arv_temp[i + 2] = filho_chave;

    indice novo_no = new_indice();

    if (no_ant->tipoNo == 0) {
        if (no_ant->arv1 == NEGATIVO) {
            no_ant->tipoNo = NEGATIVO;
            novo_no.tipoNo = NEGATIVO;
        } else {
            no_ant->tipoNo = 1;
            novo_no.tipoNo = 1;
        }
    } else {
        novo_no.tipoNo = no_ant->tipoNo;
    }

    int rrn_novo_no = alocar_rrn_no_arvore(arq_index, cab_ind);

    no_ant->C1 = chaves_temp[0];
    no_ant->C2 = chaves_temp[1];
    no_ant->C3 = NEGATIVO;

    novo_no.C1 = chaves_temp[3];
    novo_no.C2 = NEGATIVO;
    novo_no.C3 = NEGATIVO;

    no_ant->Pr1 = byte_temp[0];
    no_ant->Pr2 = byte_temp[1];
    no_ant->Pr3 = NEGATIVO;

    novo_no.Pr1 = byte_temp[3];
    novo_no.Pr2 = NEGATIVO;
    novo_no.Pr3 = NEGATIVO;

    no_ant->arv1 = arv_temp[0];
    no_ant->arv2 = arv_temp[1];
    no_ant->arv3 = arv_temp[2];
    no_ant->arv4 = NEGATIVO;

    novo_no.arv1 = arv_temp[3];
    novo_no.arv2 = arv_temp[4];
    novo_no.arv3 = NEGATIVO;
    novo_no.arv4 = NEGATIVO;

    no_ant->nroChaves = 2;
    novo_no.nroChaves = 1;

    novo_no.removido = '0';
    novo_no.proximo = NEGATIVO;

    *chave_promovida = chaves_temp[2];
    *byte_promovido = byte_temp[2];
    *filho_promovido = rrn_novo_no;

    fseek(arq_index, calculo_byteoffset_indice(rrn_no_ant), SEEK_SET);
    escreve_indice(arq_index, no_ant);

    fseek(arq_index, calculo_byteoffset_indice(rrn_novo_no), SEEK_SET);
    escreve_indice(arq_index, &novo_no);
}

void ler_no_arvore(FILE *index, int rrn, indice *no) {
    fseek(index, calculo_byteoffset_indice(rrn), SEEK_SET);
    ler_indice(index, no);
}

void escrever_no_arvore(FILE *index, int rrn, indice *no) {
    fseek(index, calculo_byteoffset_indice(rrn), SEEK_SET);
    escreve_indice(index, no);
}

int no_eh_folha(indice *no) {
    return no->arv1 == NEGATIVO;
}

void inicializar_vetor_com_negativo(int *vet, int tam) {
    int i;

    for (i = 0; i < tam; i++) {
        vet[i] = NEGATIVO;
    }
}

void no_para_vetores(indice *no, int chaves[], int prs[], int filhos[]) {
    chaves[0] = no->C1;
    chaves[1] = no->C2;
    chaves[2] = no->C3;

    prs[0] = no->Pr1;
    prs[1] = no->Pr2;
    prs[2] = no->Pr3;

    filhos[0] = no->arv1;
    filhos[1] = no->arv2;
    filhos[2] = no->arv3;
    filhos[3] = no->arv4;
}

void vetores_para_no(indice *no, int nroChaves, int chaves[], int prs[], int filhos[]) {
    no->nroChaves = nroChaves;

    no->C1 = NEGATIVO;
    no->C2 = NEGATIVO;
    no->C3 = NEGATIVO;
    no->Pr1 = NEGATIVO;
    no->Pr2 = NEGATIVO;
    no->Pr3 = NEGATIVO;
    no->arv1 = NEGATIVO;
    no->arv2 = NEGATIVO;
    no->arv3 = NEGATIVO;
    no->arv4 = NEGATIVO;

    if (nroChaves >= 1) {
        no->C1 = chaves[0];
        no->Pr1 = prs[0];
    }

    if (nroChaves >= 2) {
        no->C2 = chaves[1];
        no->Pr2 = prs[1];
    }

    if (nroChaves >= 3) {
        no->C3 = chaves[2];
        no->Pr3 = prs[2];
    }

    no->arv1 = filhos[0];

    if (nroChaves >= 1) no->arv2 = filhos[1];
    if (nroChaves >= 2) no->arv3 = filhos[2];

    if (nroChaves >= 3) no->arv4 = filhos[3];
}

void atualizar_tipo_no(indice *no, cab_indice *cab_index, int rrn) {
    if (no_eh_folha(no)) {
        no->tipoNo = NEGATIVO;
    } else if (rrn == cab_index->noRaiz) {
        no->tipoNo = 0;
    } else {
        no->tipoNo = 1;
    }
}

int posicao_chave_no(indice *no, int chave) {
    if (no->nroChaves >= 1 && no->C1 == chave) return 0;

    if (no->nroChaves >= 2 && no->C2 == chave) return 1;

    if (no->nroChaves >= 3 && no->C3 == chave) return 2;

    return NEGATIVO;
}

int posicao_filho_para_chave(indice *no, int chave) {
    if (no->nroChaves == 0) return 0;

    if (chave < no->C1) return 0;
    if (no->nroChaves == 1 || chave < no->C2) return 1;
    if (no->nroChaves == 2 || chave < no->C3) return 2;
    return 3;
}

int obter_filho(indice *no, int pos) {
    if (pos == 0) return no->arv1;
    if (pos == 1) return no->arv2;
    if (pos == 2) return no->arv3;
    return no->arv4;
}

void remover_chave_da_pagina(indice *no, int pos) {
    int chaves[3];
    int prs[3];
    int filhos[4];
    int i;

    no_para_vetores(no, chaves, prs, filhos);

    for (i = pos; i < no->nroChaves - 1; i++) {
        chaves[i] = chaves[i + 1];
        prs[i] = prs[i + 1];
    }

    chaves[no->nroChaves - 1] = NEGATIVO;
    prs[no->nroChaves - 1] = NEGATIVO;

    vetores_para_no(no, no->nroChaves - 1, chaves, prs, filhos);
}

void remover_chave_e_filho_direito(indice *pai, int pos_chave) {
    int chaves[3];
    int prs[3];
    int filhos[4];
    int i;
    int n;

    no_para_vetores(pai, chaves, prs, filhos);

    n = pai->nroChaves;

    for (i = pos_chave; i < n - 1; i++) {
        chaves[i] = chaves[i + 1];
        prs[i] = prs[i + 1];
    }

    chaves[n - 1] = NEGATIVO;
    prs[n - 1] = NEGATIVO;

    for (i = pos_chave + 1; i < n; i++) {
        filhos[i] = filhos[i + 1];
    }

    filhos[n] = NEGATIVO;

    vetores_para_no(pai, n - 1, chaves, prs, filhos);
}

void setar_chave_no_pai(indice *pai, int pos, int chave, int pr) {
    if (pos == 0) {
        pai->C1 = chave;
        pai->Pr1 = pr;
    } else if (pos == 1) {
        pai->C2 = chave;
        pai->Pr2 = pr;
    } else {
        pai->C3 = chave;
        pai->Pr3 = pr;
    }
}

int get_chave(indice *no, int pos) {
    if (pos == 0) return no->C1;
    if (pos == 1) return no->C2;
    return no->C3;
}

int get_pr(indice *no, int pos) {
    if (pos == 0) return no->Pr1;
    if (pos == 1) return no->Pr2;
    return no->Pr3;
}

void empilhar_no_removido_arvore(FILE *index, cab_indice *cab_index, int rrn_removido) {
    indice no_removido;

    if (rrn_removido == NEGATIVO) {
        return;
    }

    ler_no_arvore(index, rrn_removido, &no_removido);

    no_removido.removido = '1';
    no_removido.proximo = cab_index->topo;

    escrever_no_arvore(index, rrn_removido, &no_removido);

    cab_index->topo = rrn_removido;

    if (cab_index->nroNos > 0) {
        cab_index->nroNos--;
    }
}

// função para encontrar menor chave da subárvore
void encontrar_sucessor(FILE *index, int rrn, int *chave_sucessora, int *pr_sucessor) {
    indice no_atual;
    int rrn_atual;

    rrn_atual = rrn;

    while (rrn_atual != NEGATIVO) {
        ler_no_arvore(index, rrn_atual, &no_atual);

        if (no_eh_folha(&no_atual)) {
            *chave_sucessora = no_atual.C1;
            *pr_sucessor = no_atual.Pr1;
            return;
        }

        rrn_atual = no_atual.arv1;
    }

    *chave_sucessora = NEGATIVO;
    *pr_sucessor = NEGATIVO;
}

void redistribuir_entre_nos(FILE *index,cab_indice *cab_index,int rrn_esq,int rrn_pai,int pos_chave_pai,int rrn_dir) {
    indice esq;
    indice dir;
    indice pai;

    int chaves_esq[3], prs_esq[3], filhos_esq[4];
    int chaves_dir[3], prs_dir[3], filhos_dir[4];

    int todas_chaves[7];
    int todos_prs[7];
    int todos_filhos[8];

    int novas_chaves_esq[3];
    int novos_prs_esq[3];
    int novos_filhos_esq[4];

    int novas_chaves_dir[3];
    int novos_prs_dir[3];
    int novos_filhos_dir[4];

    int total_chaves;
    int qtd_esq;
    int qtd_dir;
    int i;
    int k;
    int folha;

    ler_no_arvore(index, rrn_esq, &esq);
    ler_no_arvore(index, rrn_dir, &dir);
    ler_no_arvore(index, rrn_pai, &pai);

    no_para_vetores(&esq, chaves_esq, prs_esq, filhos_esq);
    no_para_vetores(&dir, chaves_dir, prs_dir, filhos_dir);

    inicializar_vetor_com_negativo(todas_chaves, 7);
    inicializar_vetor_com_negativo(todos_prs, 7);
    inicializar_vetor_com_negativo(todos_filhos, 8);
    inicializar_vetor_com_negativo(novas_chaves_esq, 3);
    inicializar_vetor_com_negativo(novos_prs_esq, 3);
    inicializar_vetor_com_negativo(novos_filhos_esq, 4);
    inicializar_vetor_com_negativo(novas_chaves_dir, 3);
    inicializar_vetor_com_negativo(novos_prs_dir, 3);
    inicializar_vetor_com_negativo(novos_filhos_dir, 4);

    k = 0;

    for (i = 0; i < esq.nroChaves; i++) {
        todas_chaves[k] = chaves_esq[i];
        todos_prs[k] = prs_esq[i];
        k++;
    }

    todas_chaves[k] = get_chave(&pai, pos_chave_pai);
    todos_prs[k] = get_pr(&pai, pos_chave_pai);
    k++;

    for (i = 0; i < dir.nroChaves; i++) {
        todas_chaves[k] = chaves_dir[i];
        todos_prs[k] = prs_dir[i];
        k++;
    }

    total_chaves = k;

    folha = no_eh_folha(&esq) && no_eh_folha(&dir);

    if (!folha) {
        k = 0;

        for (i = 0; i <= esq.nroChaves; i++) {
            todos_filhos[k] = filhos_esq[i];
            k++;
        }

        for (i = 0; i <= dir.nroChaves; i++) {
            todos_filhos[k] = filhos_dir[i];
            k++;
        }
    }

    qtd_esq = total_chaves / 2;
    qtd_dir = total_chaves - qtd_esq - 1;

    for (i = 0; i < qtd_esq; i++) {
        novas_chaves_esq[i] = todas_chaves[i];
        novos_prs_esq[i] = todos_prs[i];
    }

    setar_chave_no_pai(
        &pai,
        pos_chave_pai,
        todas_chaves[qtd_esq],
        todos_prs[qtd_esq]
    );

    for (i = 0; i < qtd_dir; i++) {
        novas_chaves_dir[i] = todas_chaves[qtd_esq + 1 + i];
        novos_prs_dir[i] = todos_prs[qtd_esq + 1 + i];
    }

    if (!folha) {
        for (i = 0; i <= qtd_esq; i++) {
            novos_filhos_esq[i] = todos_filhos[i];
        }

        for (i = 0; i <= qtd_dir; i++) {
            novos_filhos_dir[i] = todos_filhos[qtd_esq + 1 + i];
        }
    }

    vetores_para_no(&esq, qtd_esq, novas_chaves_esq, novos_prs_esq, novos_filhos_esq);
    vetores_para_no(&dir, qtd_dir, novas_chaves_dir, novos_prs_dir, novos_filhos_dir);
    atualizar_tipo_no(&esq, cab_index, rrn_esq);
    atualizar_tipo_no(&dir, cab_index, rrn_dir);
    atualizar_tipo_no(&pai, cab_index, rrn_pai);
    escrever_no_arvore(index, rrn_esq, &esq);
    escrever_no_arvore(index, rrn_dir, &dir);
    escrever_no_arvore(index, rrn_pai, &pai);
}

void concatenar_nos(FILE *index,cab_indice *cab_index,int rrn_esq,int rrn_pai,int pos_chave_pai,int rrn_dir) {
    indice esq = new_indice();
    indice dir = new_indice();
    indice pai = new_indice();

    int chaves_esq[3], prs_esq[3], filhos_esq[4];
    int chaves_dir[3], prs_dir[3], filhos_dir[4];

    int novas_chaves[3];
    int novos_prs[3];
    int novos_filhos[4];

    int i;
    int k;
    int folha;

    ler_no_arvore(index, rrn_esq, &esq);
    ler_no_arvore(index, rrn_dir, &dir);
    ler_no_arvore(index, rrn_pai, &pai);

    no_para_vetores(&esq, chaves_esq, prs_esq, filhos_esq);
    no_para_vetores(&dir, chaves_dir, prs_dir, filhos_dir);

    inicializar_vetor_com_negativo(novas_chaves, 3);
    inicializar_vetor_com_negativo(novos_prs, 3);
    inicializar_vetor_com_negativo(novos_filhos, 4);

    k = 0;

    for (i = 0; i < esq.nroChaves; i++) {
        novas_chaves[k] = chaves_esq[i];
        novos_prs[k] = prs_esq[i];
        k++;
    }

    novas_chaves[k] = get_chave(&pai, pos_chave_pai);
    novos_prs[k] = get_pr(&pai, pos_chave_pai);
    k++;

    for (i = 0; i < dir.nroChaves; i++) {
        novas_chaves[k] = chaves_dir[i];
        novos_prs[k] = prs_dir[i];
        k++;
    }

    folha = no_eh_folha(&esq) && no_eh_folha(&dir);

    if (!folha) {
        k = 0;

        for (i = 0; i <= esq.nroChaves; i++) {
            novos_filhos[k] = filhos_esq[i];
            k++;
        }

        for (i = 0; i <= dir.nroChaves; i++) {
            novos_filhos[k] = filhos_dir[i];
            k++;
        }
    }

    vetores_para_no(&esq, esq.nroChaves + 1 + dir.nroChaves, novas_chaves, novos_prs, novos_filhos);

    atualizar_tipo_no(&esq, cab_index, rrn_esq);

    remover_chave_e_filho_direito(&pai, pos_chave_pai);
    atualizar_tipo_no(&pai, cab_index, rrn_pai);

    escrever_no_arvore(index, rrn_esq, &esq);
    escrever_no_arvore(index, rrn_pai, &pai);

    // a página destruída é sempre a da direita
    empilhar_no_removido_arvore(index, cab_index, rrn_dir);
}






// função para corrigir underflow no nó filho(isto é, quando tem menos chaves que o mínimo), aqui damos um exemplos possíveis
/* Primeiro caso de correção de underflow, pegando do filho a direita
     [3, 4]
[1, 2]  []  [5, 6]

corrigi para 

     [3, 5]
[1, 2]  [4]  [6]


Segundo caso de correção de underflow, pegando do filho a direita
corrigi para 
    [3, 4]
[1, 2]  []  [5]

Corrigi para 
    [2, 4]
[1]  [3]  [5]


Terceiro caso de correção de underflow, concatenando com o filha a esquerda
    [2, 3]
[1]  []  [4]

corrigi para 

    [3]
[1, 2]  [4]


Quarto  caso de correção de underflow, pegando do filho a direita
    [1, 3]
[]  [2]  [4]

corrigi para:

    [3]
[1, 2]  [4]

Mas e se ter underflow do nó de cima? aí entra a lógica recursiva!
    [1]
[]  [2]  [4]

vai para:
        [5, 6]
    []          [7, 8]
[1, 2] [4]

vai para:
        [6, 7]
    [5]          [8]
[1, 2] [4]
*/

// função para corrigir underflow no nó filho(isto é, quando tem menos chaves que o mínimo)
void corrigir_underflow_filho(FILE *index, cab_indice *cab_index, int rrn_pai,int pos_filho) {
    indice pai;
    indice irmao;
    int filhos[4];
    int chaves[3];
    int prs[3];
    int rrn_filho;
    int rrn_irmao_dir;
    int rrn_irmao_esq;

    ler_no_arvore(index, rrn_pai, &pai);
    no_para_vetores(&pai, chaves, prs, filhos);

    rrn_filho = filhos[pos_filho];

    // tentamos redistribuir com a página a direita
    if (pos_filho < pai.nroChaves) {
        rrn_irmao_dir = filhos[pos_filho + 1];

        if (rrn_irmao_dir != NEGATIVO) {
            ler_no_arvore(index, rrn_irmao_dir, &irmao);

            if (irmao.nroChaves > MIN_CHAVES_B) {
                redistribuir_entre_nos(index,cab_index,rrn_filho,rrn_pai,pos_filho,rrn_irmao_dir);

                return;
            }
        }
    }
    // tentamos redistribuir com a página a esquerda
    if (pos_filho > 0) {
        rrn_irmao_esq = filhos[pos_filho - 1];

        if (rrn_irmao_esq != NEGATIVO) {
            ler_no_arvore(index, rrn_irmao_esq, &irmao);

            if (irmao.nroChaves > MIN_CHAVES_B) {
                redistribuir_entre_nos(index,cab_index,rrn_irmao_esq,rrn_pai,pos_filho - 1,rrn_filho);

                return;
            }
        }
    }
    // senão der certo, concatena com a esquerda
    if (pos_filho > 0) {
        rrn_irmao_esq = filhos[pos_filho - 1];

        concatenar_nos(
            index,
            cab_index,
            rrn_irmao_esq,
            rrn_pai,
            pos_filho - 1,
            rrn_filho
        );

        return;
    }

    // senão der certo, concatena com a direira
    if (pos_filho < pai.nroChaves) {
        rrn_irmao_dir = filhos[pos_filho + 1];

        concatenar_nos(index,cab_index,rrn_filho,rrn_pai,pos_filho,rrn_irmao_dir);
    }
}

// Função principal para a remoção, ela desce até as folhas e depois volta corrigindo
// o retorno da função indica se houve underflow
int remover_recursivo_arvore(FILE *index, cab_indice *cab_index, int rrn_atual, int chave,int eh_raiz) {
    indice no_atual;
    int pos_chave;
    int pos_filho;
    int rrn_filho;
    int chave_sucessora;
    int pr_sucessor;
    int houve_underflow;
    // caso base da árvore, já chegamos no final (indicado pello rnn negativo), então acabou a recursão
    if (rrn_atual == NEGATIVO) {
        return 0;
    }
    // começa lendo o no dá arvore
    ler_no_arvore(index, rrn_atual, &no_atual);
    // se ele está removido não precisamo mexer nele
    if (no_atual.removido == '1') {
        return 0;
    }

    pos_chave = posicao_chave_no(&no_atual, chave);
    // primeiro caso, chava está no nó atual
    if (pos_chave != NEGATIVO) {
        // Caso A, caso o nó for folha
        if (no_eh_folha(&no_atual)) {
            // Removemos o nó, verificando se deu underflow ou não
            remover_chave_da_pagina(&no_atual, pos_chave);
            atualizar_tipo_no(&no_atual, cab_index, rrn_atual);
            escrever_no_arvore(index, rrn_atual, &no_atual);

            if (!eh_raiz && no_atual.nroChaves < MIN_CHAVES_B) return 1;
            return 0;
        }

        // Caso B, a chave não está na raiz
        rrn_filho = obter_filho(&no_atual, pos_chave + 1);
        // encontra o sucessor na subárvore dada
        encontrar_sucessor(index,rrn_filho,&chave_sucessora,&pr_sucessor);

        if (chave_sucessora == NEGATIVO) return 0;
        setar_chave_no_pai(&no_atual, pos_chave, chave_sucessora, pr_sucessor);
        escrever_no_arvore(index, rrn_atual, &no_atual);

        houve_underflow = remover_recursivo_arvore(
            index,
            cab_index,
            rrn_filho,
            chave_sucessora,
            0
        );

        if (houve_underflow) {
            corrigir_underflow_filho(
                index,
                cab_index,
                rrn_atual,
                pos_chave + 1
            );
        }

        ler_no_arvore(index, rrn_atual, &no_atual);

        if (!eh_raiz && no_atual.nroChaves < MIN_CHAVES_B) return 1;
        return 0;
    }

    // segundo caso, chave não está no nó atual, então deveremos decidir para onde descer

    // o nó é folha, então não precisamos mais descer
    if (no_eh_folha(&no_atual)) {
        return 0;
    }

    pos_filho = posicao_filho_para_chave(&no_atual, chave);
    rrn_filho = obter_filho(&no_atual, pos_filho);

    if (rrn_filho == NEGATIVO) {
        return 0;
    }
    
    houve_underflow = remover_recursivo_arvore(
        index,
        cab_index,
        rrn_filho,
        chave,
        0
    );

    if (houve_underflow) {
        corrigir_underflow_filho(
            index,
            cab_index,
            rrn_atual,
            pos_filho
        );
    }

    ler_no_arvore(index, rrn_atual, &no_atual);

    if (!eh_raiz && no_atual.nroChaves < MIN_CHAVES_B) {
        return 1;
    }

    return 0;
}

void ajustar_raiz_apos_remocao(FILE *index, cab_indice *cab_index) {
    indice raiz;
    indice nova_raiz;
    int rrn_raiz_antiga;
    int rrn_nova_raiz;

    if (cab_index->noRaiz == NEGATIVO) {
        return;
    }

    rrn_raiz_antiga = cab_index->noRaiz;

    ler_no_arvore(index, rrn_raiz_antiga, &raiz);

    if (raiz.nroChaves > 0) {
        atualizar_tipo_no(&raiz, cab_index, rrn_raiz_antiga);
        escrever_no_arvore(index, rrn_raiz_antiga, &raiz);
        return;
    }

     
    // Raiz ficou vazia e era folha: árvore vazia.
    if (no_eh_folha(&raiz)) {
        empilhar_no_removido_arvore(index, cab_index, rrn_raiz_antiga);
        cab_index->noRaiz = NEGATIVO;
        return;
    }

    // Raiz ficou vazia, mas tem um filho.
    // Esse filho vira a nova raiz.
    rrn_nova_raiz = raiz.arv1;

    empilhar_no_removido_arvore(index, cab_index, rrn_raiz_antiga);

    cab_index->noRaiz = rrn_nova_raiz;

    if (rrn_nova_raiz != NEGATIVO) {
        ler_no_arvore(index, rrn_nova_raiz, &nova_raiz);
        atualizar_tipo_no(&nova_raiz, cab_index, rrn_nova_raiz);
        escrever_no_arvore(index, rrn_nova_raiz, &nova_raiz);
    }
}

void remove_chave_arvore(FILE *index,cab_indice *cab_index,int chave) {
    int pos_chave;

    if (index == NULL || cab_index == NULL) return;
        
    if (cab_index->noRaiz == NEGATIVO) return;

    pos_chave = NEGATIVO;
    // busca na árvore e verifica se a chave existe para chamar a raiz
    busca_arvore(
        index,
        cab_index->noRaiz,
        chave,
        &pos_chave
    );
    if (pos_chave == NEGATIVO) return;
    // função principal da remoção 
    remover_recursivo_arvore(index,cab_index,cab_index->noRaiz,chave,1);
    // precisamos cuidar da raiz, já que ela é um caso especial (não tem pai) e não vai ser tratado no código recursivo
    ajustar_raiz_apos_remocao(index, cab_index);
}

// função para remover registros dinãmico usando a árvore
void remover_registros_dinamico_com_arvore(FILE *bin,FILE *index,NoHash *tabela[],cabecalho *reg_cab,cab_indice *cab_index,int m,char nomesCampos[][50],char valoresCampos[][200]) {
    int rrn_atual;
    dados reg_dados;

    if (bin == NULL || index == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    for (rrn_atual = 0; rrn_atual < reg_cab->proxRRN; rrn_atual++) {
        int ok;

        reg_dados = cria_dados();

        fseek(bin, calculo_byteoffset_dados(rrn_atual), SEEK_SET);
        ler_regdados(bin, &reg_dados);

        if (reg_dados.removido == '1') {
            continue;
        }

        ok = verificar_criterios(
            m,
            nomesCampos,
            valoresCampos,
            &reg_dados
        );

        if (ok) {
            int topo_antigo = reg_cab->topo;

            // primeiro remove da árvore
            if (reg_dados.codEstacao != -1) {
                remove_chave_arvore(
                    index,
                    cab_index,
                    reg_dados.codEstacao
                );
            }

            reg_dados.removido = '1';
            reg_dados.proximo = topo_antigo;

            fseek(bin, calculo_byteoffset_dados(rrn_atual), SEEK_SET);
            escreve_regdados(bin, &reg_dados);

            reg_cab->topo = rrn_atual;
            // trata o hashing
            if (reg_dados.tamNomeEstacao > 0) {
                NoHash *h = buscar_hash(
                    tabela,
                    reg_dados.nomeEstacao,
                    reg_dados.tamNomeEstacao
                );

                if (h != NULL && h->repeticoes == 1) {
                    reg_cab->nroEstacoes--;
                }

                decrementar_hash(
                    tabela,
                    reg_dados.nomeEstacao,
                    reg_dados.tamNomeEstacao
                );
            }

            if (reg_dados.codProxEstacao != -1) {
                reg_cab->nroParesEstacoes--;
            }
        }
    }
}
