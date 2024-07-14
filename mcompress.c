//Essa main é um teste para o contador de arquivos 

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

int main(int argc, char ** argv) {

    if(argc <= 1) {
        printf("ERRO: Diretório de arquivo não informado!");
        exit(1);
    }

    FILE * bombom = fopen(argv[1], "r");
    if(!bombom) {
        printf("ERRO: Não foi possível abrir o arquivo %s", argv[1]);
    }

    compress(bombom, argv[1]);

    fclose(bombom);

return 0;
}