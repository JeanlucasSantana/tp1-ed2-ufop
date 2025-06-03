#ifndef ARQUIVO_UTIL_H
#define ARQUIVO_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "registro.h"

// Cria diretório se não existir (usa sys/stat.h)
void criar_diretorio(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

// Gera arquivo binário com registros
void gerar_arquivo(const char* nome, int n, int situacao) {
    FILE* out = fopen(nome, "wb");
    if (!out) {
        perror("Erro ao criar arquivo");
        exit(EXIT_FAILURE);
    }

    Registro* arr = malloc(n * sizeof(Registro));
    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i].chave = i;
        arr[i].dado1 = rand();
        
        // Preenche dados com valores aleatórios
        for (int j = 0; j < TAM_DADO2 - 1; j++) 
            arr[i].dado2[j] = 'A' + (rand() % 26);
        arr[i].dado2[TAM_DADO2 - 1] = '\0';
        
        for (int j = 0; j < TAM_DADO3 - 1; j++) 
            arr[i].dado3[j] = 'a' + (rand() % 26);
        arr[i].dado3[TAM_DADO3 - 1] = '\0';
    }

    // Ordenação conforme situação
    if (situacao == 2) { // Descendente
        for (int i = 0; i < n/2; i++) {
            Registro tmp = arr[i];
            arr[i] = arr[n-1-i];
            arr[n-1-i] = tmp;
        }
    } 
    else if (situacao == 3) { // Aleatório
        for (int i = n-1; i > 0; i--) {
            int j = rand() % (i+1);
            Registro tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }

    fwrite(arr, sizeof(Registro), n, out);
    free(arr);
    fclose(out);
}

#endif
