#include "fornecedor.h"
#include <stdio.h>
#include <stdlib.h>

#define FORNECEDORES_DATA_FILE "data/fornecedores.dat"

void salvarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(FORNECEDORES_DATA_FILE, "wb");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo de fornecedores");
        return;
    }
    fwrite(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
    fwrite(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
    fclose(arquivo);
}

void carregarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(FORNECEDORES_DATA_FILE, "rb");
    if (arquivo != NULL) {
        fread(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            if (sistema->lista_fornecedores != NULL) {
                fread(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
                sistema->capacidade_fornecedores = sistema->num_fornecedores;
            }
        }
        fclose(arquivo);
    }
}

void liberarMemoriaFornecedores(Sistema *sistema) {
    if (sistema->lista_fornecedores != NULL) {
        free(sistema->lista_fornecedores);
        sistema->lista_fornecedores = NULL;
        sistema->num_fornecedores = 0;
        sistema->capacidade_fornecedores = 0;
    }
}