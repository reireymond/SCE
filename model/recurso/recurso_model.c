#include "model/recurso/recurso_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECURSOS_DATA_FILE "data/recursos.dat"
#define RECURSOS_TEXT_FILE "data/recursos.txt"

void salvarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(RECURSOS_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&sistema->num_recursos, sizeof(int), 1, arquivo);
            fwrite(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
        }
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(RECURSOS_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, "%d\n", sistema->num_recursos);
            for (int i = 0; i < sistema->num_recursos; i++) {
                fprintf(arquivo, "%d\n%s\n%s\n%d\n%.2f\n%.2f\n",
                        sistema->lista_recursos[i].codigo,
                        sistema->lista_recursos[i].descricao,
                        sistema->lista_recursos[i].categoria,
                        sistema->lista_recursos[i].quantidade_estoque,
                        sistema->lista_recursos[i].preco_custo,
                        sistema->lista_recursos[i].valor_locacao);
            }
        }
    }

    if (arquivo) fclose(arquivo);
    else perror("Nao foi possivel abrir o arquivo de recursos");
}

void carregarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? RECURSOS_DATA_FILE : RECURSOS_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_recursos, sizeof(int), 1, arquivo);
        if (sistema->num_recursos > 0) {
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            if (sistema->lista_recursos) {
                fread(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
                sistema->capacidade_recursos = sistema->num_recursos;
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &sistema->num_recursos);
        if (sistema->num_recursos > 0) {
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            sistema->capacidade_recursos = sistema->num_recursos;
            for (int i = 0; i < sistema->num_recursos; i++) {
                Recurso *r = &sistema->lista_recursos[i];
                fscanf(arquivo, "%d\n", &r->codigo);
                fgets(r->descricao, sizeof(r->descricao), arquivo); r->descricao[strcspn(r->descricao, "\n")] = 0;
                fgets(r->categoria, sizeof(r->categoria), arquivo); r->categoria[strcspn(r->categoria, "\n")] = 0;
                fscanf(arquivo, "%d\n", &r->quantidade_estoque);
                fscanf(arquivo, "%f\n", &r->preco_custo);
                fscanf(arquivo, "%f\n", &r->valor_locacao);
            }
        }
    }
    fclose(arquivo);
}