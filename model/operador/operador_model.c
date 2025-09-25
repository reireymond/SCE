#include "operador.h"
#include <stdio.h>
#include <stdlib.h>

#define OPERADORES_DATA_FILE "data/operadores.dat"
#define RECURSOS_TEXT_FILE "data/recursos.txt"

void salvarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(OPERADORES_DATA_FILE, "wb");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo de operadores");
        return;
    }
    fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
    fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
    fclose(arquivo);
}

void carregarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(OPERADORES_DATA_FILE, "rb");
    if (arquivo != NULL) {
        fread(&sistema->num_operadores, sizeof(int), 1, arquivo);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            if (sistema->lista_operadores != NULL) {
                fread(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
                sistema->capacidade_operadores = sistema->num_operadores;
            }
        }
        fclose(arquivo);
    }
}

void liberarMemoriaOperadores(Sistema *sistema) {
    if (sistema->lista_operadores != NULL) {
        free(sistema->lista_operadores);
        sistema->lista_operadores = NULL;
        sistema->num_operadores = 0;
        sistema->capacidade_operadores = 0;
    }
}