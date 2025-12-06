#include "equipe_interna_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h" 

#define EQUIPE_DATA_FILE "data/equipe.dat"
#define EQUIPE_TEXT_FILE "data/equipe.txt"

void salvarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? EQUIPE_DATA_FILE : EQUIPE_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_equipe, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
    } else {
        fprintf(arquivo, "Numero de equipe cadastradas: %d\n", sistema->num_equipe);
        for (int i = 0; i < sistema->num_equipe; i++) {
            fprintf(arquivo, "Codigo: %d\nNome: %s\nCPF: %s\nFuncao: %s\n Valor da diaria: %.2f\n",
                    sistema->lista_equipe[i].codigo,
                    sistema->lista_equipe[i].nome,
                    sistema->lista_equipe[i].cpf,
                    sistema->lista_equipe[i].funcao,
                    sistema->lista_equipe[i].valor_diaria);
        }
    }
    fclose(arquivo);
}

void carregarEquipeInterna(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? EQUIPE_DATA_FILE : EQUIPE_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_equipe, sizeof(int), 1, arquivo);
        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            sistema->capacidade_equipe = sistema->num_equipe;
            fread(sistema->lista_equipe, sizeof(EquipeInterna), sistema->num_equipe, arquivo);
        }
    } else {
        fscanf(arquivo, "%d\n", &sistema->num_equipe);
        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            sistema->capacidade_equipe = sistema->num_equipe;
            for (int i = 0; i < sistema->num_equipe; i++) {
                EquipeInterna *e = &sistema->lista_equipe[i];
                fscanf(arquivo, "%d\n", &e->codigo);
                fgets(e->nome, 100, arquivo); e->nome[strcspn(e->nome, "\n")] = 0;
                fgets(e->cpf, 15, arquivo); e->cpf[strcspn(e->cpf, "\n")] = 0;
                fgets(e->funcao, 50, arquivo); e->funcao[strcspn(e->funcao, "\n")] = 0;
                fscanf(arquivo, "%f\n", &e->valor_diaria);
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaEquipe(Sistema *sistema) {
    if (sistema->lista_equipe) {
        free(sistema->lista_equipe);
        sistema->lista_equipe = NULL;
        sistema->num_equipe = 0;
        sistema->capacidade_equipe = 0;
    }
}