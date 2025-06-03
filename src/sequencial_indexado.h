#ifndef SEQUENCIAL_INDEXADO_H
#define SEQUENCIAL_INDEXADO_H

#include <stdio.h>
#include "registro.h"

#define REG_POR_PAGINA 4  // 4 registros por página 

typedef struct {
    int chave;      // Chave do primeiro registro na página
    long posicao;   // Posição da página no arquivo
} IndicePagina;

// Cria índice para acesso sequencial indexado
IndicePagina* criar_indice(FILE* arq, int total_reg, int* total_paginas) {
    *total_paginas = total_reg / REG_POR_PAGINA;
    if (total_reg % REG_POR_PAGINA) (*total_paginas)++;
    
    IndicePagina* indice = malloc(*total_paginas * sizeof(IndicePagina));
    Registro reg;
    long pos = 0;
    
    for (int i = 0; i < *total_paginas; i++) {
        fread(&reg, sizeof(Registro), 1, arq);
        indice[i].chave = reg.chave;
        indice[i].posicao = pos;
        
        // Avança para próxima página
        fseek(arq, (REG_POR_PAGINA - 1) * sizeof(Registro), SEEK_CUR);
        pos = ftell(arq);
    }
    rewind(arq);
    return indice;
}

// Pesquisa usando acesso sequencial indexado
Registro* pesquisar_indexado(FILE* arq, IndicePagina* indice, int total_pag, int chave, int* transf, int* comp) {
    int i = 0;
    // Encontra página onde chave pode estar
    while (i < total_pag && chave >= indice[i].chave) {
        (*comp)++;
        i++;
    }
    
    if (i == 0) return NULL;
    
    // Lê página completa
    fseek(arq, indice[i-1].posicao, SEEK_SET);
    Registro* pagina = malloc(REG_POR_PAGINA * sizeof(Registro));
    int lidos = fread(pagina, sizeof(Registro), REG_POR_PAGINA, arq);
    (*transf)++;
    
    // Busca sequencial na página
    for (int j = 0; j < lidos; j++) {
        (*comp)++;
        if (pagina[j].chave == chave) {
            Registro* resultado = malloc(sizeof(Registro));
            *resultado = pagina[j];
            free(pagina);
            return resultado;
        }
    }
    
    free(pagina);
    return NULL;
}

#endif
