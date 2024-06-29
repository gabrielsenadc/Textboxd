//Essa main é um teste para o contador de arquivos 

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

int main(int argc, char ** argv) {

    /*
    if(argc <= 1) {
        printf("ERRO: Diretório de arquivo não informado!");
    }

    FILE * file = fopen(argv[1], "r");
    if(!file) {
        printf("ERRO: Não foi possível abrir o arquivo %s", argv[1]);
    }
    */

   char bombom[20];
   sprintf(bombom, "%s./bombom.txt", "");

    FILE * file = fopen(bombom, "r");
    if(!file) {
        printf("ERRO: Não foi possível abrir o arquivo %s", bombom);
        return 1;
    }

    int * counter = countCharacters(file);

    fclose(file);

return 0;
}