#ifndef FUNCOES_H
#define FUNCOES_H


typedef struct ArquivoAberto {
    char nome[100];
    struct ArquivoAberto *prox;
} ArquivoAberto;




void mostrar_binario_sequencial(FILE *bin);


int verificar_criterios(int m, dados* reg_dados);
void buscar_registros(FILE *bin, NoHash *tabela[], int m,char nomesCampos[][50], char valoresCampos[][200]);


int arquivo_ja_processado(ArquivoAberto *lista, char *nome_bin);
void adicionar_arquivo_processado(ArquivoAberto **lista, char *nome_bin);
void liberar_lista_arquivos(ArquivoAberto *lista);

#endif

