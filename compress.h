#ifndef compress_h
#define compress_h

#include "bitmap.h"

typedef struct treeType treeType;
typedef struct listType listType;

//These functions need to be removed from here... (pov: the client doesn't need to know)

treeType * createStop();

listType * createList(int * vet);

void insertTree(listType *list, treeType *tree);

treeType * removeFirstTree(listType *list);

void freeList(listType *list);

treeType * createTree(int qtt, char c, treeType *left, treeType *right, int leaf);

void printTree(treeType *tree);

void freeTree(treeType *tree);

int * countCharacters(FILE * file);

treeType * createBinaryTree(listType *list);

bitmap * createBitMapContent(treeType *tree, FILE *file, FILE *compressed_file);

bitmap * createTreeBitmap(treeType * tree);

int decode_print(bitmap ** bm, int * index, treeType * tree, FILE * file, FILE *compressed_file);

void setStop(bitmap *bm, int * index, treeType *tree);

//Ends here

/**
 * Compacta um arquivo, gerando um arquivo .comp com as informações compactadas.
 * @param file arquivo a ser comprimido.
 * @param file_name nome do arquivo a ser comprimido.
 */
void compress(FILE * file, char * file_name);

/**
 * Descompacta um arquivo .comp, gerando o arquivo normal
 * @param file_name nome do arquivo a ser descompactado.
 */
void decompress(char * file_name);

#endif