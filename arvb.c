#include <stdio.h>
#include "arvb.h"  


/*Parametros : arquivo inário para escrita e a struct em RAM do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Se não, salva campo a campo.
ATENÇÃO: ponteiros deve estar bem posicionado no começo do cabeçalho, senão dá erro*/

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


/*Parametros : arquivo binário para escrita e a struct para escrita do cabecalho
Se ñ é possível abrir o arquivo, retorna erro.
Senão,  salva campo a campo na struct fornecida.
*/
void ler_ind_cabecalho(FILE* ponteiro_arquivo, cab_indice* cab ){
    if (ponteiro_arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n"); 
        return;
    }
    fread(&cab->status, sizeof(char), 1, ponteiro_arquivo);
    fread(&cab->.noRaiz, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.topo, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.proxRRN, sizeof(int), 1, ponteiro_arquivo);
    fread(&cab->.nroNos, sizeof(int), 1, ponteiro_arquivo);
}




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

*/
void cria_arvore(FILE* arq_dados, char*arq_index ,){

    FILE* indexes = escrever_binario( arq_index);
    cab_indice index_cab = new_cab_indice();
    if(indexes == NULL){
        indexes = cria_escreve_binario(arq_index);
        escreve_ind_cabecalho(indexes, &index_cab);
    }
    ler_ind_cabecalho(indexes, &index_cab);


    //ler o arquivo de dados
    cabecalho reg_cab_dados = cria_cabecalho();
    ler_cabecalho(arq_dados, reg_cab_dados);
    if(reg_cab_dados.status == 0){
        //status inconsistente ? --> Erro 
        printf("Falha no processamento do arquivo.");
        return;
    }
    fseek(arq_dados, TAM_CABECALHO,SEEK_SET);

    dados reg_dados = cria_dados();
    //leitura dos registros e inserção na arvore
    for(int i=0; i<reg_cab_dados.proxRRN; i++){
        // é removido?
        ler_regdados(arq_dados, &reg_dados);
        if (reg_dados.removido =='1') continue; // o ponteiro do fseek já está no começo do registro i+1

        int promotion, filho_promovido, chave_promovida;

        promotion= insere_arvore(arq_dados, &index_cab, index_cab.noRaiz,  reg_dados.codEstacao,     
                     filho_promovido,  chave_promovida);
            if (PROMOTION == )
    
    }

}


/*Assumindo q existe o arquivo de indice existe e é != NULL.
Busca uma chave única int - codEstacao-  num nó da árvore.
Caso 1: Arvore com elementos --> rrn da raiz >=0
Caso 2: Arvore vazia --> rrn da raiz == -1
Parametros: Arquivo de indices, rrn do no onde faz a busca, a chave para busca, 
        e a posicao (1, 2, 3) da chave (Real ou Onde deveria estar)
        ------util para inserção
Retorna : RRN do nó onde a busca terminou --> -1 se não existe a chave

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
        if (pos_chave != NULL) *pos_chave_no = NEGATIVO;
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
        proximo_rrn = no_aux.arv1;
    } else if (no_aux.nroChaves == 1 || 
            (no_aux.nroChaves >= 2 && chave < no_aux.C2)) {
        proximo_rrn = no_aux.arv2;
    } else if (no_aux.nroChaves == 2 || 
            (no_aux.nroChaves == 3 && chave < no_aux.C3)) {
        proximo_rrn = no_aux.arv3;
    } else {
        proximo_rrn = no_aux.arv4;
    }



    valor_retorno = insere_arvore(arq_index, cabecalho, rrn_esperado, chave, byte_dados_chave,
                                    filho_promovido, chave_promovida, byte_dados_promovido);

    

    if (valor_retorno == NO_PROMOTION || valor_retorno == NEGATIVO){
        return valor_retorno;
    }

    //seta fseek na posição correta
    fseek(arq_index, byteoffset, SEEK_SET);
    ler_indice(arq_index, &no_aux);

    //No tem espaço?
    if (no_aux.nroChaves<3){
        //insere chave na pagina 
        //FAZER FUNCAO
        return NO_PROMOTION;
    }else{
            //split 
            //insere pagina no arquivo
            //insere novapagina com rrn_filho_promovido no arquivo
            return PROMOTION;
    } 


}


void cria_novoNo( ){

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
        no->C1 = chave;
        no->Pr1 = byte_ponteiro;
        no->arv2 = filho_promovido;
        no->nroChaves = 1;
    }
    if (no->nroChaves == 1) {
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
    }if (no->nroChaves == 2) {
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
        }
        no->nroChaves = 3;
    }
    
}