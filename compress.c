#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "compress.h"

typedef struct cellType cellType;
typedef struct listType listType;

struct treeType{
    treeType *left;
    treeType *right;
    int qtt;
    char c;
    int stop;
    int leaf;
};

struct cellType{
    cellType *next;
    cellType *prior;

    treeType *tree;
};

struct listType{
    cellType *first;
    cellType *last;
};

treeType * createStop(){
    treeType *tree = malloc(sizeof(treeType));

    tree->qtt = 0;
    tree->c = 3;
    tree->right = NULL;
    tree->left = NULL;
    tree->stop = 1;
    tree->leaf = 1;

    return tree;
}

listType *createList(int * vet){
    listType *list = malloc(sizeof(listType));
    list->first = list->last = NULL;

    for(int i = 0; i < 256; i++){
        if(vet[i] != 0) insertTree(list, createTree(vet[i], (char)i, NULL, NULL, 1));
    }

    insertTree(list, createStop());

    return list;
}

void insertTree(listType *list, treeType *tree){
    cellType *cell = malloc(sizeof(cellType));

    cell->tree = tree;

    cellType *aux = NULL;
    int i = 0;
    for(aux = list->first; aux; aux = aux->next){
        i++;
        if(aux->tree->qtt >= cell->tree->qtt){
            if(i == 1) list->first = cell;

            cell->prior = aux->prior;
            if(aux->prior) aux->prior->next = cell;

            cell->next = aux;
            aux->prior = cell;

            break;
        }
    }

    if(list->last == NULL) list->last = cell;

    if(aux == NULL && i == 0){
        cell->prior = cell->next = NULL;
        list->first = cell;
    } 

    if(aux == NULL && i != 0){
        cell->prior = list->last;
        cell->next = NULL;
        list->last->next = cell;
        list->last = cell;
    } 


}

treeType *removeFirstTree(listType *list){
    cellType *aux = list->first;
    treeType *tree = aux->tree;

    list->first = aux->next;
    if(aux->next) aux->next->prior = NULL;

    free(aux);
    return tree; 
}


void printList(listType *list){
    for(cellType *cell = list->first; cell; cell = cell->next){
        printTree(cell->tree);
    }
}

void freeList(listType *list){
    cellType *aux = NULL, *cell = list->first;

    while(cell){
        aux = cell;
        cell = cell->next;
        freeTree(aux->tree);
        free(aux);
    }

    free(list);
}

void setLeaf(treeType *tree){
    tree->leaf = 1;
}

treeType * createEmptyTree() {

    treeType *tree = malloc(sizeof(treeType));

    tree->qtt = 0;
    tree->c = '\0';
    tree->right = NULL;
    tree->left = NULL;
    tree->stop = 0;
    tree->leaf = 0;

    return tree;

}

treeType * createTree(int qtt, char c, treeType *left, treeType *right, int leaf){
    treeType *tree = malloc(sizeof(treeType));

    tree->qtt = qtt;
    tree->c = c;
    tree->right = right;
    tree->left = left;
    tree->stop = 0;
    tree->leaf = leaf;

    return tree;
}

void printTree(treeType *tree){
    if(tree == NULL) return;

    printf(" <");
    printf("%c:%d", tree->c, tree->qtt);
    printTree(tree->left);
    printTree(tree->right);
    printf(">");
}

void freeTree(treeType *tree){
    if(tree == NULL) return;

    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}

int * countCharacters(FILE * file) {

    rewind(file);

    int * counter = (int *) calloc(256, sizeof(int));
    unsigned char character = 0;

    while(fread(&character, sizeof(unsigned char), 1, file) == 1) {
        counter[character]++;
    }

    rewind(file);

return counter;
}

int sizeList(listType *list){
    int i = 0;
    for(cellType *cell = list->first; cell; cell = cell->next) i++;

    return i;
}

treeType *createBinaryTree(listType *list){
    if(sizeList(list) <= 1) return removeFirstTree(list);

    treeType *left = removeFirstTree(list);
    treeType *right = removeFirstTree(list);
    insertTree(list, createTree(left->qtt + right->qtt, '\0', left, right, 0));

    return createBinaryTree(list);

}


//OBS: retorna o valor ao contrário, tratamento correto feito em createBitMapContent
bitmap * returnCodedValue(treeType *tree, char c, int stop){
    if(tree == NULL) return NULL;
    if((c == tree->c && !stop) || (tree->stop && stop)) return bitmapInit(100);
    bitmap * bitsLeft = returnCodedValue(tree->left, c, stop);
    if(bitsLeft != NULL){
        bitmapAppendLeastSignificantBit(bitsLeft, 0);
        return bitsLeft;
    }
    
    bitmap * bitsRight = returnCodedValue(tree->right, c, stop);
    if(bitsRight != NULL){
        bitmapAppendLeastSignificantBit(bitsRight, 1);
        return bitsRight;
    }

    return NULL;
}

bitmap * createBitMapContent(treeType *tree, FILE *file){
    bitmap * bm = bitmapInit(1000000);
    unsigned char c = '\0';

    while(fread(&c, sizeof(unsigned char), 1, file) == 1){
        bitmap * coded = returnCodedValue(tree, c, 0);

        for(int i = bitmapGetLength(coded) - 1; i >= 0; i--){
            bitmapAppendLeastSignificantBit(bm, bitmapGetBit(coded, i));
        }

        bitmapLibera(coded);
    }

    //Adiciona char de parada
    bitmap * coded = returnCodedValue(tree, '\0', 1);
    for(int i = bitmapGetLength(coded) - 1; i >= 0; i--){
        bitmapAppendLeastSignificantBit(bm, bitmapGetBit(coded, i));
    }
    bitmapLibera(coded);

    return bm;
}

void appendCodedCharacter(bitmap * bm, unsigned char c) {

    int value = (int)c;
    char binary[8];

    for(int i = 7, j = 0; i >= 0; i--, j++) {

        int div = pow(2, i);

        if(value / div == 1) {
            value -= div;
            binary[j] = '1';
            bitmapAppendLeastSignificantBit(bm, 1);
        }
        else {
            binary[j] = '0';
            bitmapAppendLeastSignificantBit(bm, 0);
        }

    }

}

void * insertPath(treeType * tree, bitmap * treeBitmap) {

    if(tree != NULL) {
        if(tree -> c == '\0') {
            bitmapAppendLeastSignificantBit(treeBitmap, 0);
            insertPath(tree -> left, treeBitmap);
            insertPath(tree -> right, treeBitmap);
        }
        else {
            bitmapAppendLeastSignificantBit(treeBitmap, 1);
            appendCodedCharacter(treeBitmap, tree -> c);
        }
    }

}

bitmap * createTreeBitmap(treeType * tree) {
    
    bitmap * treeBitmap = bitmapInit(1000000);

    insertPath(tree, treeBitmap);

return treeBitmap;
}

void compress(FILE * file, char * file_name) {

    int * counter = countCharacters(file);

    listType * list = createList(counter);
    treeType * tree = createBinaryTree(list);
    bitmap * bmTree = createTreeBitmap(tree);
    bitmap * bmFile = createBitMapContent(tree, file);

    char compressed_file_name[1000];
    sprintf(compressed_file_name, "./%s.comp", file_name);

    FILE * compressed_file = NULL;
    compressed_file = fopen(compressed_file_name, "wb");

    unsigned char * contentsTree = bitmapGetContents(bmTree);
    short int bmTreeSize = bitmapGetLength(bmTree);
    fwrite(&bmTreeSize, sizeof(short int), 1, compressed_file);
    fwrite(contentsTree, sizeof(unsigned char), (bitmapGetLength(bmTree)/8)+ 1, compressed_file);

    unsigned char * contentsFile = bitmapGetContents(bmFile);
    fwrite(contentsFile, sizeof(unsigned char), (bitmapGetLength(bmFile)/8)+ 1, compressed_file);

    fclose(compressed_file);

    printTree(tree);
    FILE * file2 = fopen("figura.png", "wb");

    if(file2 == NULL) exit(1);

    int index = 0, flag = 1;

    while(flag){
        flag = decode_print(bmFile, &index, tree, file2);
    }


    freeTree(tree);
    freeList(list);
    free(counter);

    bitmapLibera(bmTree);
    bitmapLibera(bmFile);

}


//Decompress functions

bitmap * readBinaryFileContent(FILE *file){
    bitmap * bm = bitmapInit(1000000);

    unsigned char c;

    while(fread(&c, sizeof(unsigned char), 1, file) == 1)
        for(int i = 0; i < 8; i++) bitmapAppendLeastSignificantBit(bm, (c >> (7-(i%8))) & 0x01);
    
    return bm;
}

int decode_print(bitmap * bm, int * index, treeType * tree, FILE * file){
    if(tree == NULL) return 0;
    if(tree->stop) return 0;
    if(tree->leaf == 1){
        printf("%c", tree->c);
        fwrite(&(tree -> c), sizeof(unsigned char), 1, file);
        return 1;
    }

    (*index)++;
    if(bitmapGetBit(bm, *index - 1) == 1) return decode_print(bm, index, tree->right, file);
    if(bitmapGetBit(bm, *index - 1) == 0) return decode_print(bm, index, tree->left, file);

}

void decode(FILE * file, treeType *tree, FILE * decompressed_file){
    int index = 0, flag = 1;
    bitmap * bm = readBinaryFileContent(file);

    while(flag){
        flag = decode_print(bm, &index, tree, decompressed_file);
    }

    bitmapLibera(bm);
}

char decodeChar(char * binary) {

    int value = 0;

    for(int i = 7, j = 0; i >= 0; i--, j++) {

        int sum = pow(2, i);
        if(binary[j]) value += sum;

    }

return (char)value;
}

treeType * recoverTreeBitmap(bitmap * bmTree, int * index) {
    
    treeType * dTree = createEmptyTree();


    if(bitmapGetBit(bmTree, *index)) {
        char binaryChar[8];
        for(int i = 0; i < 8; i++) {
            (*index)++;
            binaryChar[i] = bitmapGetBit(bmTree, *index);
        }
        unsigned char c = decodeChar(binaryChar);
        (*index)++;
        if(c == 3) return createStop();
        return createTree(0, c, NULL, NULL, 1);
    }
    else {
        (*index)++;
        dTree -> left = recoverTreeBitmap(bmTree, index);
        dTree -> right = recoverTreeBitmap(bmTree, index);
    }

return dTree;
}

void printAll(FILE * file) {

    unsigned char c;
    bitmap * bm = bitmapInit(1000000);

    printf("PRINTANDO O ARQUIVO INTEIRO: \n");
    while(fread(&c, sizeof(unsigned char), 1, file) == 1) {
        for(int i = 0; i < 8; i++) bitmapAppendLeastSignificantBit(bm, (c >> (7-(i%8))) & 0x01);
    }

    for(int i = 0; i < bitmapGetLength(bm); i++) {
        printf("%d", bitmapGetBit(bm, i));
    }
    printf("\n");

}

void decompress(char * file_name) {

    FILE * file = fopen(file_name, "rb");
    if(!file) {
        printf("Não foi possível localizar o arquivo.\n");
        return;
    }

    file_name[strlen(file_name) - 4] = '\0';
    FILE * decompressed_file = fopen(file_name, "wb");

    bitmap * bmTree = bitmapInit(1000000);
    unsigned char * contentsTree;
    unsigned char c;

    short int lenght = 0;

    fread(&lenght, sizeof(short int), 1, file);
    for(int i = 0; i < (lenght/8) + 1; i++) {
        fread(&c, sizeof(unsigned char), 1, file);
        for(int i = 0; i < 8; i++) bitmapAppendLeastSignificantBit(bmTree, (c >> (7-(i%8))) & 0x01);
    }
    int index = 0;
    printf("%d\n", bitmapGetLength(bmTree));

    treeType * dTree = recoverTreeBitmap(bmTree, &index);

    decode(file, dTree, decompressed_file);

    fclose(file);

    freeTree(dTree);
    bitmapLibera(bmTree);    

}