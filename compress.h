#ifndef compress_h
#define compress_h

typedef struct treeType treeType;
typedef struct listType listType;

listType *createList();

void insertTree(listType *list, treeType *tree);

treeType *removeFirstTree(listType *list);

void printList(listType *list);

treeType * createTree(int qtt, char c);

void printTree(treeType *tree);

void freeTree(treeType *tree);

void freeList(listType *list);

int * countCharacters(FILE * file);

#endif