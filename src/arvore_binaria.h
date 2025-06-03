#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "registro.h"

typedef struct No {
    int chave;
    long posicao;
    struct No* esq;
    struct No* dir;
} No;

// Insere nó na árvore não balanceada
No* inserir_no(No* raiz, int chave, long posicao) {
    if (!raiz) {
        No* novo = malloc(sizeof(No));
        novo->chave = chave;
        novo->posicao = posicao;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    
    if (chave < raiz->chave) 
        raiz->esq = inserir_no(raiz->esq, chave, posicao);
    else 
        raiz->dir = inserir_no(raiz->dir, chave, posicao);
    
    return raiz;
}

// Pesquisa na árvore binária
Registro* pesquisar_arvore(FILE* arq, No* raiz, int chave, int* transf, int* comp) {
    No* atual = raiz;
    while (atual) {
        (*comp)++;
        if (chave == atual->chave) {
            fseek(arq, atual->posicao, SEEK_SET);
            Registro* reg = malloc(sizeof(Registro));
            fread(reg, sizeof(Registro), 1, arq);
            (*transf)++;
            return reg;
        }
        atual = (chave < atual->chave) ? atual->esq : atual->dir;
    }
    return NULL;
}

#endif
