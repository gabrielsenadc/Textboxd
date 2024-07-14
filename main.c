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

    listType *list = createList(counter);
    treeType *tree = createBinaryTree(list);

    printTree(tree);

    printf("\n");
    bitmap * bm = createBitMapContent(tree, file);
    for(int i = 0; i < bitmapGetLength(bm); i++){
        printf("%d", bitmapGetBit(bm, i));
    }
    printf("\n");

    printf("Orig Bitmap: ");
    bitmap * bmTree = createTreeBitmap(tree);
    printf("\nTree Bitmap Lenght: %d\n", bitmapGetLength(bmTree));
    printf("Tree Bitmap: ");
    for(int i = 0; i < bitmapGetLength(bmTree); i++) {
        printf("%d", bitmapGetBit(bmTree, i));
    }
    
    printf("\n");
    freeTree(tree);
    freeList(list);
    free(counter);

    fclose(file);
    bitmapLibera(bm);

return 0;
}