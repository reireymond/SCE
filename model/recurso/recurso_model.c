#include "recurso.h"
#include <stdio.h>
#include <stdlib.h>

#define RECURSOS_DATA_FILE "data/recursos.dat"

void salvarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(RECURSOS_DATA_FILE, "wb");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo de recursos");
        return;
    }
    fwrite(&sistema->num_recursos, sizeof(int), 1, arquivo);
    fwrite(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
    fclose(arquivo);
}

void carregarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(RECURSOS_DATA_FILE, "rb");
    if (arquivo != NULL) {
        fread(&sistema->num_recursos, sizeof(int), 1, arquivo);
        if (sistema->num_recursos > 0) {
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            if (sistema->lista_recursos != NULL) {
                fread(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
                sistema->capacidade_recursos = sistema->num_recursos;
            }
        }
        fclose(arquivo);
    }
}

void liberarMemoriaRecursos(Sistema *sistema) {
    if (sistema->lista_recursos != NULL) {
        free(sistema->lista_recursos);
        sistema->lista_recursos = NULL;
        sistema->num_recursos = 0;
        sistema->capacidade_recursos = 0;
    }
}