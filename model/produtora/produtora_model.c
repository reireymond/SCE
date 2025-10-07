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
    
    if (modo == ARQUIVO_BINARIO) {
    // Lógica atual para binário...
} else { // ARQUIVO_TEXTO
    arquivo = fopen(PRODUTORA_TEXT_FILE, "w");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo de texto da produtora");
        return;
    }
    int produtora_existe = (sistema->dados_produtora != NULL);
    fprintf(arquivo, "%d\n", produtora_existe);
    if (produtora_existe) {
        Produtora *p = sistema->dados_produtora;
        fprintf(arquivo, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%.2f\n",
                p->nome_fantasia, p->razao_social, p->nome_do_responsavel,
                p->cnpj, p->inscricao_estadual, p->endereco, p->telefone,
                p->telefone_responsavel, p->email, p->margem_lucro);
    }
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