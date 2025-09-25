#include "equipe_interna.h"
#include <stdio.h>
#include <stdlib.h>

#define EQUIPE_DATA_FILE "data/equipe.dat"

void salvarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // Por enquanto, o enunciado pede apenas binário para este módulo
    FILE *arquivo = fopen(EQUIPE_DATA_FILE, "wb");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo da equipe");
        return;
    }
    fwrite(&sistema->num_equipe, sizeof(int), 1, arquivo);
    fwrite(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
    fclose(arquivo);
}

void carregarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(EQUIPE_DATA_FILE, "rb");
    if (arquivo != NULL) {
        fread(&sistema->num_equipe, sizeof(int), 1, arquivo);
        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            if (sistema->lista_equipe != NULL) {
                fread(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
                sistema->capacidade_equipe = sistema->num_equipe;
            }
        }
        fclose(arquivo);
    }
}

void liberarMemoriaEquipe(Sistema *sistema) {
    if (sistema->lista_equipe != NULL) {
        free(sistema->lista_equipe);
        sistema->lista_equipe = NULL;
        sistema->num_equipe = 0;
        sistema->capacidade_equipe = 0;
    }
}