##Anotações Aulão 3

    Separar contextos no sistema computacional.
    Tabelas - Arquivos - diferentes para diferentes contextos. 
    Chae key que une os dois arquivos.
    Problema: como juntar esses dados.
    
    No trabalho, juntar baseado no codEstacao,proxEstacao, nomeExtacao.


    TODOS os codEstacoes são unicos e não repetidos, e é garantido que existem no arquivo.

#Trabalho
    - 1° abordagem: NESTED JOIN mais ineficiente -loop de for aninhado. N^2 



        O algoritmo da junção de loop aninhado é definido como segue. Para cada
        registro presente no arquivo arquivoA (loop externo), recupere cada registro do
        arquivo arquivoB (loop interno) e teste se os dois registros satisfazem à condição de
        junção Acampo1 = Bcampo1. Ou seja:
        para cada registro em estacao1 faça // loop externo
            para cada registro em estacao2 faça // loop interno
                se estacao1.codProxEstacao = estacao2.codEstacao
                então mostre os campos desejados de estacao1 e estacao2
                fim-se
            fim-para
        fim-para
    - 2° abordagem: JUNCTION JOIN o mesmo, mas usando um registro de indice ARVB dado como parametro.
    - 3: ORDENAR o arquivo : faz uma lista 
        - cuidados - o arquivo n tem a qtd de registros q vamos ler.
                    é proibido ler o arquivo todo p/ ver o tamanho dele.
                    OU faz uma lista de 300 registros+-.
                    OU faz realloc inteligente de 50 em 50 registros.
                    PODE funcionar usando o proxRRN 
        Essa func tem que ser genérica !!!!
        -Pra receber ou o codEstacao ou o codProxEstacao
        A func recebe o arq entrada/ arq saida / comparaçao e so retorna o arquivo de saida.
    -4° DEPENDE DA ORDENAÇÃO ANTERIOR
        MERGE SORT JOIN. 
        Só faz o join num arquivo já ordenado. Faz um merge.
        Abre 2 arquivos, ordena com a função modularizada.

        algoritmo :
        le ambos os arquivos ao mesmo tempo. 
        se a key de A < key de B
            avança A
        se a key de A > key de B
            avança B
        se a key de A  =key de B
            deu match -> print
            avança A e B
        - TEM UM ERRO O INDICE NÃO É PARAMETRO!!!!!!
        


         
