#ifndef compress_h
#define compress_h

typedef struct treeType treeType;

treeType * createTree(int qtt, char c);

int * countCharacters(FILE * file);

#endif