#include "model/equipe_interna/equipe_interna_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define EQUIPE_DATA_FILE "data/equipe.dat"
#define EQUIPE_TEXT_FILE "data/equipe.txt" 

void salvarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(EQUIPE_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&sistema->num_equipe, sizeof(int), 1, arquivo);
            fwrite(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
        }
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(EQUIPE_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, "%d\n", sistema->num_equipe);
            for (int i = 0; i < sistema->num_equipe; i++) {
                fprintf(arquivo, "%d\n%s\n%s\n%s\n%.2f\n",
                        sistema->lista_equipe[i].codigo,
                        sistema->lista_equipe[i].nome,
                        sistema->lista_equipe[i].cpf,
                        sistema->lista_equipe[i].funcao,
                        sistema->lista_equipe[i].valor_diaria);
            }
        }
    }

    if (arquivo) fclose(arquivo);
    else perror("Nao foi possivel abrir o arquivo da equipe");
}

void carregarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? EQUIPE_DATA_FILE : EQUIPE_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_equipe, sizeof(int), 1, arquivo);
        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            if (sistema->lista_equipe) {
                fread(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
                sistema->capacidade_equipe = sistema->num_equipe;
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &sistema->num_equipe);
        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            sistema->capacidade_equipe = sistema->num_equipe;
            for (int i = 0; i < sistema->num_equipe; i++) {
                EquipeInterna *e = &sistema->lista_equipe[i];
                fscanf(arquivo, "%d\n", &e->codigo);
                fgets(e->nome, sizeof(e->nome), arquivo); e->nome[strcspn(e->nome, "\n")] = 0;
                fgets(e->cpf, sizeof(e->cpf), arquivo); e->cpf[strcspn(e->cpf, "\n")] = 0;
                fgets(e->funcao, sizeof(e->funcao), arquivo); e->funcao[strcspn(e->funcao, "\n")] = 0;
                fscanf(arquivo, "%f\n", &e->valor_diaria);
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaEquipe(Sistema *sistema) {
    if (sistema->lista_equipe != NULL) {
        free(sistema->lista_equipe);
        sistema->lista_equipe = NULL;
        sistema->num_equipe = 0;
        sistema->capacidade_equipe = 0;
    }
}