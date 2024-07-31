#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "compress.h"

typedef struct cellType cellType;
typedef struct listType listType;
typedef struct treeType treeType;

#define MAXTAM 1000000

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

// Tree Functions

/**
 * Cria uma folha que representa a codificação do caractere de parada.
 * @return a árvore de parada.
 */
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

/**
 * Cria uma árvore vazia.
 * @return a árvore.
 */
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

/**
 * Cria uma árvore a partir dos parâmetros.
 * @param qtt quantidade de vezes que o caractere aparece no texto.
 * @param c o caractere.
 * @param left a árvore da esquerda.
 * @param right a árvore da direita.
 * @param leaf se a árvore criada é uma folha.
 * @return a árvore.
 */
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

/**
 * Insere uma árvore na Lista em ordem crescente.
 * @param list a Lista para adicionar a árvore.
 * @param tree a árvore.
 */
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

/**
 * Remove a primeira árvore da Lista, 
 * ou seja, a árvore com menor quantidade de aparições no texto.
 * @param list Lista a ser analisada.
 * @return a primeira árvore da Lista.
 */
treeType *removeFirstTree(listType *list){
    cellType *aux = list->first;
    treeType *tree = aux->tree;

    list->first = aux->next;
    if(aux->next) aux->next->prior = NULL;

    free(aux);
    return tree; 
}

/**
 * Imprime a árvore;
 * @param tree a árvore a ser impressa.
 */
void printTree(treeType *tree){
    if(tree == NULL) return;

    printf(" <");
    printf("%c:%d", tree->c, tree->qtt);
    printTree(tree->left);
    printTree(tree->right);
    printf(">");
}

/**
 * Libera a árvore recursivamente
 * @param tree árvore a ser liberada.
 */
void freeTree(treeType *tree){
    if(tree == NULL) return;

    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}

// List Functions

/**
 * Cria a Lista de arvores em ordem crescente de aparições de cada caractere para a codificação de HuffMan.
 * @param vet o vetor da quantidade de aparições de cada caractere.
 * @return a lista criada.
 */
listType *createList(int * vet){
    listType *list = malloc(sizeof(listType));
    list->first = list->last = NULL;

    for(int i = 0; i < 256; i++){
        if(vet[i] != 0) insertTree(list, createTree(vet[i], (char)i, NULL, NULL, 1));
    }

    insertTree(list, createStop());

    return list;
}

/**
 * Retorna o tamanho da lista.
 * @param list a lista.
 * @return tamanho da lista.
 */
int sizeList(listType *list){
    int i = 0;
    for(cellType *cell = list->first; cell; cell = cell->next) i++;

    return i;
}

/**
 * Libera a memória dinâmica alocada para uma lista.
 * @param list a lista.
 */
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

// Compress Functions

/**
 * Conta a quantidade de aparições de um determinado byte (em forma de caractere)
 * e retorna o vetor com a contagem
 * @param file arquivo que contém os bytes.
 * @return vetor de 256 posições com a contagem de frequência dos bytes.
*/
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

/**
 * Cria a árvore binária de codificação de HuffMan.
 * @param list Lista a ser execurado o processo de para a codificação
 * @return a árvore.
 */
treeType *createBinaryTree(listType *list){
    if(sizeList(list) <= 1) return removeFirstTree(list);

    treeType *left = removeFirstTree(list);
    treeType *right = removeFirstTree(list);
    insertTree(list, createTree(left->qtt + right->qtt, '\0', left, right, 0));

    return createBinaryTree(list);

}


//OBS: retorna o valor ao contrário, tratamento correto feito em createBitMapContent
/**
 * Codifica um caractere por meio de uma árvore de codificação binária.
 * @param tree árvore que será realizada a busca do caractere.
 * @param c caractere a ser codificado.
 * @param stop se o caractere a ser codificado é de parada ou não
 * @return bitmap da codificação ao contrário.
 */
bitmap * returnCodedValue(treeType *tree, char c, int stop){
    if(tree == NULL) return NULL;
    if(tree->leaf) if((c == tree->c && !stop) || (tree->stop && stop)) {
        return bitmapInit(100);
    }
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

/**
 * Cria o bitmap que contém o texto coddificado (content),
 * a medida que o bitmap de content chega no Tamanho Máximo,
 * o bitmap é escrito no arquivo comprimido e cria um bitmap novo.
 * @param tree arvore de codificação binária a ser usada.
 * @param file arquivo a ser comprimido.
 * @param compressed_file arquivo comprimido, onde o content será adicionado.
 * @return o último bitmap de content a ser codificado.
 */
bitmap * createBitMapContent(treeType *tree, FILE *file, FILE *compressed_file){
    bitmap * bm = bitmapInit(MAXTAM);
    unsigned char c = '\0';

    while(fread(&c, sizeof(unsigned char), 1, file) == 1){
        bitmap * coded = returnCodedValue(tree, c, 0);

        for(int i = bitmapGetLength(coded) - 1; i >= 0; i--){
            if(bitmapGetLength(bm) == MAXTAM) {
                unsigned char * contentsFile = bitmapGetContents(bm);
                fwrite(contentsFile, sizeof(unsigned char), (MAXTAM/8), compressed_file);
                bitmapLibera(bm);
                bm = bitmapInit(MAXTAM);
            }
            bitmapAppendLeastSignificantBit(bm, bitmapGetBit(coded, i));
        }

        bitmapLibera(coded);
    }

    //Adiciona char de parada
    bitmap * coded = returnCodedValue(tree, '\0', 1);
    for(int i = bitmapGetLength(coded) - 1; i >= 0; i--){
        if(bitmapGetLength(bm) == MAXTAM) {
                unsigned char * contentsFile = bitmapGetContents(bm);
                fwrite(contentsFile, sizeof(unsigned char), (MAXTAM/8), compressed_file);
                bitmapLibera(bm);
                bm = bitmapInit(MAXTAM);
        }
        bitmapAppendLeastSignificantBit(bm, bitmapGetBit(coded, i));
    }
    bitmapLibera(coded);

    return bm;
}


/**
 * Insere no bitmap o valor binário do byte/caractere
 * @param bm bitmap
 * @param c byte/caractere
*/
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

/**
 * Codifica uma árvore binária para um bitmap
 * Os nós são representados como 0 e as folhas como 1
 * A cada folha encontrada, o bitmap recebe um byte/caractere
 * @param tree árvore a ser codificada
 * @param treeBitmap bitmap onde será adicionada a árvore
*/
void insertPath(treeType * tree, bitmap * treeBitmap) {

    if(tree != NULL) {
        if(!tree->leaf) {
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

/**
 * Inicializa o bitmap da árvore e insere os respectivos
 * dados no bitmap
 * @param tree árvore a ser codificada para bitmap
 * @return bitmap codificado da árvore
*/
bitmap * createTreeBitmap(treeType * tree) {
    
    bitmap * treeBitmap = bitmapInit(1000000);

    insertPath(tree, treeBitmap);

return treeBitmap;
}

/**
 * Compacta um arquivo atráves da codificação de Huffman
 * @param file arquivo que se deseja compactar
 * @param file_name nome do arquivo
*/
void compress(FILE * file, char * file_name) {

    int * counter = countCharacters(file);

    listType * list = createList(counter);
    treeType * tree = createBinaryTree(list);
    bitmap * bmTree = createTreeBitmap(tree);

    char compressed_file_name[1000];
    sprintf(compressed_file_name, "./%s.comp", file_name);

    FILE * compressed_file = NULL;
    compressed_file = fopen(compressed_file_name, "wb");

    unsigned char * contentsTree = bitmapGetContents(bmTree);
    short int bmTreeSize = bitmapGetLength(bmTree);
    fwrite(&bmTreeSize, sizeof(short int), 1, compressed_file);
    fwrite(contentsTree, sizeof(unsigned char), (bitmapGetLength(bmTree)/8)+ 1, compressed_file);

    bitmap * coded = returnCodedValue(tree, 3, 1);
    bitmap * stop = bitmapInit(100);
    for(int i = bitmapGetLength(coded) - 1; i >= 0; i--){
        bitmapAppendLeastSignificantBit(stop, bitmapGetBit(coded, i));
    }
    unsigned char * contentStop = bitmapGetContents(stop);
    short int size_stop = (bitmapGetLength(stop)/8) + 1;
    fwrite(&size_stop, sizeof(short int), 1, compressed_file);
    fwrite(contentStop, sizeof(unsigned char), size_stop, compressed_file);

    bitmapLibera(stop);
    bitmapLibera(coded);


    bitmap * bmFile = createBitMapContent(tree, file, compressed_file);

    unsigned char * contentsFile = bitmapGetContents(bmFile);
    fwrite(contentsFile, sizeof(unsigned char), (bitmapGetLength(bmFile)/8)+ 1, compressed_file);

    fclose(compressed_file);



    freeTree(tree);
    freeList(list);
    free(counter);

    bitmapLibera(bmTree);
    bitmapLibera(bmFile);

}



//Decompress functions

/**
 * Lê, no arquivo a ser descompactado, até o Tamanho Máximo do bitmap
 * @param file arquivo a ser lido.
 * @return bitmap com o content lido.
 */
bitmap * readBinaryFileContent(FILE *file){
    bitmap * bm = bitmapInit(MAXTAM);

    unsigned char c;

    while(fread(&c, sizeof(unsigned char), 1, file) == 1){
        for(int i = 0; i < 8; i++) bitmapAppendLeastSignificantBit(bm, (c >> (7-(i%8))) & 0x01);
        if(bitmapGetLength(bm) == MAXTAM) return bm;
    }
    return bm;
}

/**
 * Decodifica e printa um caractere.
 * @param bm um ponteiro de ponteiro do bitmap com o content que está sendo usado na decodificação no momento.
 * @param index um ponteiro do numero que representa a posição no bitmap em que está sendo realizada a tradução.
 * @param tree árvore usada para a decodificação.
 * @param file arquivo descompactado.
 * @param compressed_file arquivo a ser descompactado.
 * @return 1 se printou um caractere e 0 se achou o caractere de parada.
 */
int decode_print(bitmap ** bm, int * index, treeType * tree, FILE * file, FILE *compressed_file){
    if(tree == NULL) return 0;
    if(tree->stop) return 0;
    
    if(tree->leaf == 1){
        fwrite(&(tree -> c), sizeof(unsigned char), 1, file);
        return 1;
    }

    if(*index >= MAXTAM){
        bitmapLibera(*bm);
        bitmap * content = readBinaryFileContent(compressed_file);
        *bm = content;
        *index = 0;
    }

    (*index)++;
    if(bitmapGetBit(*bm, *index - 1) == 1) return decode_print(bm, index, tree->right, file, compressed_file);
    if(bitmapGetBit(*bm, *index - 1) == 0) return decode_print(bm, index, tree->left, file, compressed_file);

}

/**
 * Decodifica um arquivo compactado, gerando o novo arquivo descompactado.
 * @param file arquivo compactado.
 * @param tree árvore binária de codificação a ser usada para decodificar.
 * @param decompressed_file arquivo descompactado.
 */
void decode(FILE * file, treeType *tree, FILE * decompressed_file){
    int index = 0, flag = 1;
    bitmap * content = readBinaryFileContent(file);
    bitmap ** bm = &content;


    while(flag){
        flag = decode_print(bm, &index, tree, decompressed_file, file);
    }

    bitmapLibera(*bm);
}

/**
 * Recupera o byte/caractere dado em um vetor de caracteres
 * formada por caracteres 0 e 1
 * @param binary vetor de caracteres com os valores
 * @return byte/caractere decodificado
*/
char decodeChar(char * binary) {

    int value = 0;

    for(int i = 7, j = 0; i >= 0; i--, j++) {

        int sum = pow(2, i);
        if(binary[j]) value += sum;

    }

return (char)value;
}

/**
 * Recupera uma árvore binária a partir
 * de um bitmap codificado
 * @param bmTree bitmap com os dados
 * @param index posição do vetor que avança no bitmap
 * @return árvore binária reconstruída
*/
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
        freeTree(dTree);
        return createTree(0, c, NULL, NULL, 1);
    }
    else {
        (*index)++;
        dTree -> left = recoverTreeBitmap(bmTree, index);
        dTree -> right = recoverTreeBitmap(bmTree, index);
    }

return dTree;
}

/**
 * Imprime o arquivo binário INTEIRO no terminal
 * Utilizada para debug
 * @param file arquivo binário
*/
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

/**
 * Atribui uma das folhas da árvore de codificação a flag de parada.
 * @param bm bitmap que possui a codificação do caractere de parada.
 * @param index ponteiro do número que representa a posição atual do bitmpa
 * @param tree árvore de codificação binária.
 */
void setStop(bitmap *bm, int * index, treeType *tree){
    if(tree == NULL) return;
    if(tree->leaf == 1){
        tree->stop = 1;
        return;
    }

    (*index)++;
    if(bitmapGetBit(bm, *index - 1) == 1) return setStop(bm, index, tree->right);
    if(bitmapGetBit(bm, *index - 1) == 0) return setStop(bm, index, tree->left);
}

/**
 * Descompacta um arquivo compactado do tipo ".comp"
 * @param file_name nome do arquivo a ser descompactado
*/
void decompress(char * file_name) {

    FILE * file = fopen(file_name, "rb");
    if(!file) {
        printf("Não foi possível localizar o arquivo.\n");
        return;
    }

    file_name[strlen(file_name) - 5] = '\0';
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

    treeType * dTree = recoverTreeBitmap(bmTree, &index);

    bitmap * stop = bitmapInit(100);
    int stop_size = 0;
    fread(&stop_size, sizeof(short int), 1, file);
    for(int i = 0; i < stop_size; i++){
        fread(&c, sizeof(unsigned char), 1, file);
        for(int i = 0; i < 8; i++) bitmapAppendLeastSignificantBit(stop, (c >> (7-(i%8))) & 0x01);
    }


    int i = 0;
    setStop(stop, &i, dTree);
    bitmapLibera(stop);

    decode(file, dTree, decompressed_file);

    fclose(file);
    fclose(decompressed_file);

    freeTree(dTree);
    bitmapLibera(bmTree);  
}