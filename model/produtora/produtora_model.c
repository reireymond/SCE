#include "model/produtora/produtora_model.h"
#include <stdio.h>
#include <stdlib.h>

#define PRODUTORA_DATA_FILE "data/produtora.dat"
#define PRODUTORA_TEXT_FILE "data/produtora.txt"

void salvarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "wb");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo da produtora");
        return;
    }

    int produtora_existe = (sistema->dados_produtora != NULL);
    fwrite(&produtora_existe, sizeof(int), 1, arquivo);
    if (produtora_existe) {
        fwrite(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
    }
    fclose(arquivo);
}

void carregarProdutora(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen(PRODUTORA_DATA_FILE, "rb");
    if (arquivo) {
        int produtora_existe = 0;
        fread(&produtora_existe, sizeof(int), 1, arquivo);
        if (produtora_existe) {
            sistema->dados_produtora = (Produtora *)malloc(sizeof(Produtora));
            if (sistema->dados_produtora) {
                fread(sistema->dados_produtora, sizeof(Produtora), 1, arquivo);
            }
        }
        fclose(arquivo);
    }
}

void liberarMemoriaProdutora(Sistema *sistema) {
    if (sistema->dados_produtora) {
        free(sistema->dados_produtora);
        sistema->dados_produtora = NULL;
    }
}