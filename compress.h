#ifndef compress_h
#define compress_h

#include "bitmap.h"

typedef struct treeType treeType;
typedef struct listType listType;

//These functions need to be removed from here... (pov: the client doesn't need to know)

listType * createList(int * vet);

void insertTree(listType *list, treeType *tree);

treeType * removeFirstTree(listType *list);

void printList(listType *list);

treeType * createTree(int qtt, char c, treeType *left, treeType *right);

void printTree(treeType *tree);

void freeTree(treeType *tree);

void freeList(listType *list);

int * countCharacters(FILE * file);

treeType * createBinaryTree(listType *list);

bitmap * createBitMapContent(treeType *tree, FILE *file);

bitmap * createTreeBitmap(treeType * tree);

int decode_print(bitmap * bm, int * index, treeType * tree, FILE * file);

//Ends here

void compress(FILE * file, char * file_name);

void decompress(char * file_name);

#endif