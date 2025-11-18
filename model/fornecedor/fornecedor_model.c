#include "fornecedor_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define FORNECEDORES_DATA_FILE "data/fornecedores.dat"
#define FORNECEDORES_TEXT_FILE "data/fornecedores.txt"

void salvarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
    } else {
        fprintf(arquivo, "%d\n", sistema->num_fornecedores);
        for (int i = 0; i < sistema->num_fornecedores; i++) {
            fprintf(arquivo, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n",
                    sistema->lista_fornecedores[i].codigo,
                    sistema->lista_fornecedores[i].nome_fantasia,
                    sistema->lista_fornecedores[i].razao_social,
                    sistema->lista_fornecedores[i].cnpj,
                    sistema->lista_fornecedores[i].endereco,
                    sistema->lista_fornecedores[i].telefone,
                    sistema->lista_fornecedores[i].tipo_servico);
        }
    }
    fclose(arquivo);
}

void carregarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;
            fread(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
        }
    } else {
        fscanf(arquivo, "%d\n", &sistema->num_fornecedores);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;
            for (int i = 0; i < sistema->num_fornecedores; i++) {
                Fornecedor *f = &sistema->lista_fornecedores[i];
                fscanf(arquivo, "%d\n", &f->codigo);
                fgets(f->nome_fantasia, 100, arquivo); f->nome_fantasia[strcspn(f->nome_fantasia, "\n")] = 0;
                fgets(f->razao_social, 100, arquivo); f->razao_social[strcspn(f->razao_social, "\n")] = 0;
                fgets(f->cnpj, 20, arquivo); f->cnpj[strcspn(f->cnpj, "\n")] = 0;
                fgets(f->endereco, 150, arquivo); f->endereco[strcspn(f->endereco, "\n")] = 0;
                fgets(f->telefone, 20, arquivo); f->telefone[strcspn(f->telefone, "\n")] = 0;
                fgets(f->tipo_servico, 100, arquivo); f->tipo_servico[strcspn(f->tipo_servico, "\n")] = 0;
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaFornecedores(Sistema *sistema) {
    if (sistema->lista_fornecedores) {
        free(sistema->lista_fornecedores);
        sistema->lista_fornecedores = NULL;
        sistema->num_fornecedores = 0;
        sistema->capacidade_fornecedores = 0;
    }
}