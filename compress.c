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

listType *createList(){
    listType *list = malloc(sizeof(listType));

    list->first = list->last = NULL;

    return list;
}

void insertTree(listType *list, treeType *tree){
    cellType *cell = malloc(sizeof(cellType));

    cell->tree = tree;

    cellType *aux = NULL;
    for(aux = list->first; aux; aux = aux->next){
        if(aux->tree->qtt < cell->tree->qtt) break;
    }

    if(list->last == NULL) list->last = cell;

    if(aux == NULL){
        cell->prior = cell->next = NULL;
        list->first = cell;
    } else {
        cell->prior = aux->prior;
        if(aux->prior) aux->prior->next = cell;

        cell->next = aux;
        aux->prior = cell;
    }



}


treeType * createTree(int qtt, char c){
    treeType *tree = malloc(sizeof(treeType));

    tree->qtt = qtt;
    tree->c = c;
    tree->right = tree->left = NULL;

    return tree;
}

