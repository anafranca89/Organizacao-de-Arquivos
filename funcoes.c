/* Função para ler o arquivo binário, e remover lógicamente os registros de dados
Delete from --- where*/

void delete (char* binario, int n){
    FILE *file = fopen(binario, "r+b");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int proxRRN = file -> proxRRN;
 

}