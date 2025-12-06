#include "equipe_interna_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h" 

#define EQUIPE_DATA_FILE "data/equipe.dat"
#define EQUIPE_TEXT_FILE "data/equipe.txt"

void remover_quebra_linha_equipe(char *str) {
    str[strcspn(str, "\n")] = 0;
}

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
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) sistema->num_equipe = atoi(valor + 1);
        }

        if (sistema->num_equipe > 0) {
            sistema->lista_equipe = malloc(sistema->num_equipe * sizeof(EquipeInterna));
            sistema->capacidade_equipe = sistema->num_equipe;

            for (int i = 0; i < sistema->num_equipe; i++) {
                EquipeInterna *e = &sistema->lista_equipe[i];
                
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) e->codigo = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(e->nome, valor + 2); remover_quebra_linha_equipe(e->nome); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(e->cpf, valor + 2); remover_quebra_linha_equipe(e->cpf); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(e->funcao, valor + 2); remover_quebra_linha_equipe(e->funcao); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) e->valor_diaria = atof(valor + 1);
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