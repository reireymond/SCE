#include "model/fornecedor/fornecedor_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORNECEDORES_DATA_FILE "data/fornecedores.dat"
#define FORNECEDORES_TEXT_FILE "data/fornecedores.txt"

void salvarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(FORNECEDORES_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
            fwrite(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
        }
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(FORNECEDORES_TEXT_FILE, "w");
        if (arquivo) {
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
    }

    if (arquivo) fclose(arquivo);
    else perror("Nao foi possivel abrir o arquivo de fornecedores");
}

void carregarFornecedores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? FORNECEDORES_DATA_FILE : FORNECEDORES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_fornecedores, sizeof(int), 1, arquivo);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            if (sistema->lista_fornecedores) {
                fread(sistema->lista_fornecedores, sizeof(Fornecedor), sistema->num_fornecedores, arquivo);
                sistema->capacidade_fornecedores = sistema->num_fornecedores;
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &sistema->num_fornecedores);
        if (sistema->num_fornecedores > 0) {
            sistema->lista_fornecedores = malloc(sistema->num_fornecedores * sizeof(Fornecedor));
            sistema->capacidade_fornecedores = sistema->num_fornecedores;
            for (int i = 0; i < sistema->num_fornecedores; i++) {
                Fornecedor *f = &sistema->lista_fornecedores[i];
                fscanf(arquivo, "%d\n", &f->codigo);
                fgets(f->nome_fantasia, sizeof(f->nome_fantasia), arquivo); f->nome_fantasia[strcspn(f->nome_fantasia, "\n")] = 0;
                fgets(f->razao_social, sizeof(f->razao_social), arquivo); f->razao_social[strcspn(f->razao_social, "\n")] = 0;
                fgets(f->cnpj, sizeof(f->cnpj), arquivo); f->cnpj[strcspn(f->cnpj, "\n")] = 0;
                fgets(f->endereco, sizeof(f->endereco), arquivo); f->endereco[strcspn(f->endereco, "\n")] = 0;
                fgets(f->telefone, sizeof(f->telefone), arquivo); f->telefone[strcspn(f->telefone, "\n")] = 0;
                fgets(f->tipo_servico, sizeof(f->tipo_servico), arquivo); f->tipo_servico[strcspn(f->tipo_servico, "\n")] = 0;
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaFornecedores(Sistema *sistema) {
    if (sistema->lista_fornecedores != NULL) {
        free(sistema->lista_fornecedores);
        sistema->lista_fornecedores = NULL;
        sistema->num_fornecedores = 0;
        sistema->capacidade_fornecedores = 0;
    }
}