#ifndef ARVORE_B_H
#define ARVORE_B_H

#include <stdio.h>
#include <stdlib.h>
#include "registro.h"

#define ORDEM 5  // Ordem m da árvore B (conforme slides)

typedef struct NoB {
    int num_chaves;                     // Número de chaves no nó
    int chaves[2 * ORDEM - 1];          // Chaves
    long posicoes[2 * ORDEM - 1];       // Posições no arquivo
    struct NoB* filhos[2 * ORDEM];      // Ponteiros para filhos
    int folha;                          // Indica se é folha
} NoB;

// Cria novo nó
NoB* criar_no_b(int folha) {
    NoB* no = malloc(sizeof(NoB));
    no->num_chaves = 0;
    no->folha = folha;
    for (int i = 0; i < 2 * ORDEM; i++)
        no->filhos[i] = NULL;
    return no;
}

// Pesquisa em um nó específico
Registro* pesquisar_no(FILE* arq, NoB* no, int chave, int* transf, int* comp) {
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i]) {
        (*comp)++;
        i++;
    }
    
    if (i < no->num_chaves && chave == no->chaves[i]) {
        fseek(arq, no->posicoes[i], SEEK_SET);
        Registro* reg = malloc(sizeof(Registro));
        fread(reg, sizeof(Registro), 1, arq);
        (*transf)++;
        return reg;
    }
    
    return no->folha ? NULL : pesquisar_no(arq, no->filhos[i], chave, transf, comp);
}

// Pesquisa na árvore B
Registro* pesquisar_arvore_b(FILE* arq, NoB* raiz, int chave, int* transf, int* comp) {
    return raiz ? pesquisar_no(arq, raiz, chave, transf, comp) : NULL;
}

#endif
