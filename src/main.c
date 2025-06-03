#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "registro.h"
#include "arquivo_utils.h"
#include "sequencial_indexado.h"
#include "arvore_binaria.h"
#include "arvore_b.h"

// Variáveis globais para métricas
int transferencias = 0;
int comparacoes = 0;
clock_t inicio_exec;

void iniciar_contagem() {
    transferencias = 0;
    comparacoes = 0;
    inicio_exec = clock();
}

void exibir_metricas() {
    double tempo = (double)(clock() - inicio_exec) / CLOCKS_PER_SEC;
    printf("Transferências: %d\n", transferencias);
    printf("Comparações: %d\n", comparacoes);
    printf("Tempo: %.6f s\n", tempo);
}

void exibir_registro(Registro* r) {
    if (r) {
        printf("Chave: %d\n", r->chave);
        printf("Dado1: %ld\n", r->dado1);
        printf("Dado2: %.10s...\n", r->dado2);
        printf("Dado3: %.10s...\n", r->dado3);
    } else {
        printf("Registro não encontrado\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Uso: %s <método> <quantidade> <situação> <chave> [-P]\n", argv[0]);
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int exibir_chaves = (argc > 5 && strcmp(argv[5], "-P") == 0);

    // Preparar ambiente
    criar_diretorio("dados");
    char nome_arquivo[100];
    sprintf(nome_arquivo, "dados/%d_%d.bin", quantidade, situacao);
    
    // Criar arquivo se necessário
    FILE* arq = fopen(nome_arquivo, "rb");
    if (!arq) {
        gerar_arquivo(nome_arquivo, quantidade, situacao);
        arq = fopen(nome_arquivo, "rb");
    }

    iniciar_contagem();
    Registro* resultado = NULL;

    switch (metodo) {
        case 1: { // Acesso sequencial indexado
            int total_paginas;
            IndicePagina* indice = criar_indice(arq, quantidade, &total_paginas);
            resultado = pesquisar_indexado(arq, indice, total_paginas, chave, &transferencias, &comparacoes);
            free(indice);
            break;
        }
        
        case 2: { // Árvore binária
            No* raiz = NULL;
            Registro reg;
            long pos = 0;
            
            for (int i = 0; i < quantidade; i++) {
                fread(&reg, sizeof(Registro), 1, arq);
                raiz = inserir_no(raiz, reg.chave, pos);
                pos += sizeof(Registro);
            }
            rewind(arq);
            resultado = pesquisar_arvore(arq, raiz, chave, &transferencias, &comparacoes);
            // Liberar memória da árvore (não implementado)
            break;
        }
        
        case 3: { // Árvore B
            // Implementação básica (n fiz praticamente nada ainda)
            NoB* raiz = criar_no_b(1);
            Registro reg;
            long pos = 0;
            
            for (int i = 0; i < quantidade; i++) {
                fread(&reg, sizeof(Registro), 1, arq);
                if (raiz->num_chaves < 2 * ORDEM - 1) {
                    raiz->chaves[raiz->num_chaves] = reg.chave;
                    raiz->posicoes[raiz->num_chaves] = pos;
                    raiz->num_chaves++;
                }
                pos += sizeof(Registro);
            }
            rewind(arq);
            resultado = pesquisar_arvore_b(arq, raiz, chave, &transferencias, &comparacoes);
            break;
        }
        
        case 4: // Árvore B* (não fiz nada)
            printf("Árvore B* não implementada\n");
            break;
            
        default:
            printf("Método inválido\n");
    }

    // Resultados
    printf("\n--- RESULTADO ---\n");
    exibir_registro(resultado);
    printf("\n--- MÉTRICAS ---\n");
    exibir_metricas();

    if (exibir_chaves) {
        printf("\n--- CHAVES ---\n");
        rewind(arq);
        Registro r;
        for (int i = 0; i < quantidade; i++) {
            fread(&r, sizeof(Registro), 1, arq);
            printf("%d ", r.chave);
        }
        printf("\n");
    }

    fclose(arq);
    free(resultado);
    return 0;
}
