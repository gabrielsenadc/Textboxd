//Essa main é um teste para o contador de arquivos 

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

int main(int argc, char ** argv) {

    if(argc <= 1) {
        printf("ERRO: Diretório de arquivo não informado!");
        exit(1);
    }

    decompress(argv[1]);

return 0;
}