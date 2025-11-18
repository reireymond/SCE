#include "operador_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define OPERADORES_DATA_FILE "data/operadores.dat"
#define OPERADORES_TEXT_FILE "data/operadores.txt"

void salvarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
    } else {
        fprintf(arquivo, "%d\n", sistema->num_operadores);
        for (int i = 0; i < sistema->num_operadores; i++) {
            fprintf(arquivo, "%d\n%s\n%s\n%s\n",
                    sistema->lista_operadores[i].codigo,
                    sistema->lista_operadores[i].nome,
                    sistema->lista_operadores[i].usuario,
                    sistema->lista_operadores[i].senha);
        }
    }
    fclose(arquivo);
}

void carregarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_operadores, sizeof(int), 1, arquivo);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            fread(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
        }
    } else {
        fscanf(arquivo, "%d\n", &sistema->num_operadores);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            for (int i = 0; i < sistema->num_operadores; i++) {
                Operador *op = &sistema->lista_operadores[i];
                fscanf(arquivo, "%d\n", &op->codigo);
                fgets(op->nome, 100, arquivo); op->nome[strcspn(op->nome, "\n")] = 0;
                fgets(op->usuario, 50, arquivo); op->usuario[strcspn(op->usuario, "\n")] = 0;
                fgets(op->senha, 50, arquivo); op->senha[strcspn(op->senha, "\n")] = 0;
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaOperadores(Sistema *sistema) {
    if (sistema->lista_operadores) {
        free(sistema->lista_operadores);
        sistema->lista_operadores = NULL;
        sistema->num_operadores = 0;
        sistema->capacidade_operadores = 0;
    }
}