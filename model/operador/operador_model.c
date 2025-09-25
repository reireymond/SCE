#include "model/operador/operador_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPERADORES_DATA_FILE "data/operadores.dat"
#define OPERADORES_TEXT_FILE "data/operadores.txt"

void salvarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(OPERADORES_DATA_FILE, "wb");
        if (arquivo) {
            fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
            fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
        }
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(OPERADORES_TEXT_FILE, "w");
        if (arquivo) {
            fprintf(arquivo, "%d\n", sistema->num_operadores);
            for (int i = 0; i < sistema->num_operadores; i++) {
                fprintf(arquivo, "%d\n%s\n%s\n%s\n", // A senha também será salva
                        sistema->lista_operadores[i].codigo,
                        sistema->lista_operadores[i].nome,
                        sistema->lista_operadores[i].usuario,
                        sistema->lista_operadores[i].senha);
            }
        }
    }
    if (arquivo) fclose(arquivo);
    else perror("Nao foi possivel abrir o arquivo de operadores");
}

void carregarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    const char *nomeArquivo = (modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE;
    const char *modoAbertura = (modo == ARQUIVO_BINARIO) ? "rb" : "r";
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_operadores, sizeof(int), 1, arquivo);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            if (sistema->lista_operadores) {
                fread(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
                sistema->capacidade_operadores = sistema->num_operadores;
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &sistema->num_operadores);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            for (int i = 0; i < sistema->num_operadores; i++) {
                Operador *op = &sistema->lista_operadores[i];
                fscanf(arquivo, "%d\n", &op->codigo);
                fgets(op->nome, sizeof(op->nome), arquivo); op->nome[strcspn(op->nome, "\n")] = 0;
                fgets(op->usuario, sizeof(op->usuario), arquivo); op->usuario[strcspn(op->usuario, "\n")] = 0;
                fgets(op->senha, sizeof(op->senha), arquivo); op->senha[strcspn(op->senha, "\n")] = 0;
            }
        }
    }
    fclose(arquivo);
}