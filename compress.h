#ifndef compress_h
#define compress_h

#include "bitmap.h"

typedef struct treeType treeType;
typedef struct listType listType;

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

#endif