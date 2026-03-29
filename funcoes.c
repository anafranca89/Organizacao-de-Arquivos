#include <stdio.h>
#include "registros.c"





/* Função que lê os m nomeCampo e dadoCampo. Busca no arquivo binário os registros que tem esses campos e valores.
Retorna a lista de rrn com esses valores .*/

int* busca_registro(int m){
    int *rrns = (int*)malloc(m * sizeof(int));
    for (int i=0; i<m; i++){
        char nomeCampo[50];
        char valorCampo[50];
        scanf("%s %s", nomeCampo, valorCampo);
        //busca o campo e o valor no arquivo binário. Se encontrar, retorna o RRN do registro.
        rrns = //rrn da busca;
    }
    

}







/* Função para ler o arquivo binário, e remover lógicamente os registros de dados
Delete from --- where*/

void delete (char* binario, int n){
    //chama função pra ler o arquivo binário.
    cabecalho regcab = ler_cabecalho(binario);

    for(int i=0; i<n; i++){
        //ler os n reg a serem removidos
        //m  nomeCampo1 valorCampo1 ... nomeCampom valorCampom
        // ler a qtd m de campos a buscar
        /*Usa uma função pra buscar os registros com esses campos.
        Retorna o rrn dos registros encontrados.
        */
        
        //se o registro é encontrado, 
        if(RRN< regcab.proxRRN){
            int byteoffset = RRN *80;
            fseek(binario,  byteoffset, SEEK_SET);
            dados registro = ler_regdados(binario, byteoffset);
            if(registro.removido == '0'){
                /*registro encontrado e n está removido
                Então: marca como removido == 1; atualiza o topo e o proximo registro removido*/
                registro.removido = '1';
                registro.proximo = regcab.topo;
                regcab.topo = RRN;
                escreve_cabecalho(binario, regcab);
                escreve_regdados(binario, registro, byteoffset);

            }
        } else{
            //registro nao encontrado
            printf("Registro nao encontrado\n");
        }
    
    }

    BinarionaTela(binario);
}




/* Função Atualização UPDATE.

*/


void update (char* binario, int n){
    //chama função pra ler o arquivo binário.
    cabecalho regcab = ler_cabecalho(binario);

    for(int i=0; i<n; i++){
        int byteoffset;
        //ler os n reg a serem atualizados
        //m  nomeCampo1 valorCampo1 ... nomeCampom valorCampom
        // ler a qtd m de campos a buscar
        /*Usa uma função pra buscar os registros com esses campos.
        Retorna o rrn dos registros encontrados.
        */
       // ler o p nomecampo valor campo a ser atualizado.
        
        //se o registro é encontrado, 
        if(regcab.topo == -1){
            byteoffset = regcab.proxRRN *80;
            fseek(binario,  byteoffset, SEEK_SET);
            dados registro = ler_regdados(binario, byteoffset);
            /*
            atualiza os campos necessários
            */
            escreve_regdados(binario, registro, byteoffset);
        } else{
            byteoffset = regcab.topo * 80;
            fseek(binario,  byteoffset, SEEK_SET);
            regcab.topo = RRN;
            escreve_cabecalho(binario, regcab);
            dados registro = ler_regdados(binario, byteoffset);
            /*
            atualiza os campos necessários
            */
            escreve_regdados(binario, registro, byteoffset);
        }

    
    }

    BinarionaTela(binario);
}
