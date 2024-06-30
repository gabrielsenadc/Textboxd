#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compress.h"

typedef struct cellType cellType;
typedef struct listType listType;

struct treeType{
    treeType *left;
    treeType *right;
    int qtt;
    char c;
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

listType *createList(int * vet){
    listType *list = malloc(sizeof(listType));
    list->first = list->last = NULL;

    for(int i = 0; i < 256; i++){
        if(vet[i] != 0) insertTree(list, createTree(vet[i], (char)i, NULL, NULL));
    }

    return list;
}

void insertTree(listType *list, treeType *tree){
    cellType *cell = malloc(sizeof(cellType));

    cell->tree = tree;

    cellType *aux = NULL;
    int i = 0;
    for(aux = list->first; aux; aux = aux->next){
        i++;
        if(aux->tree->qtt > cell->tree->qtt){
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

treeType * createTree(int qtt, char c, treeType *left, treeType *right){
    treeType *tree = malloc(sizeof(treeType));

    tree->qtt = qtt;
    tree->c = c;
    tree->right = right;
    tree->left = left;

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
    char character = -1;

    while(fscanf(file, "%c", &character) == 1) {
        counter[(int)character]++;
    }

    rewind(file);

return counter;
}

