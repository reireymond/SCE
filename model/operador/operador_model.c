// Inclui o cabeçalho do model de operador
#include "model/operador/operador_model.h"
// Bibliotecas padrão para arquivos, memória e strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui a configuração do modo de armazenamento
#include "model/config_armazenamento/config_armazenamento.h"

// Nomes dos arquivos para salvar os operadores
#define OPERADORES_DATA_FILE "data/operadores.dat"
#define OPERADORES_TEXT_FILE "data/operadores.txt"

// Salva os dados dos operadores no modo configurado (memória, texto ou binário)
void salvarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = NULL;
    if (modo == ARQUIVO_BINARIO) {
        arquivo = fopen(OPERADORES_DATA_FILE, "wb");
        if (!arquivo) {
            perror("Erro ao abrir arquivo binário de operadores");
            return;
        }
        fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
        if (sistema->num_operadores > 0)
            fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
    } else { // ARQUIVO_TEXTO
        arquivo = fopen(OPERADORES_TEXT_FILE, "w");
        if (!arquivo) {
            perror("Erro ao abrir arquivo de texto de operadores");
            return;
        }
        fprintf(arquivo, "%d\n", sistema->num_operadores);
        for (int i = 0; i < sistema->num_operadores; i++) {
            fprintf(arquivo, "%d\n%s\n%s\n%s\n",
                    sistema->lista_operadores[i].codigo,
                    sistema->lista_operadores[i].nome,
                    sistema->lista_operadores[i].usuario,
                    sistema->lista_operadores[i].senha);
        }
    }
    fclose(arquivo);
}

// Carrega os dados dos operadores para a memória
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
            } else {
                printf("Erro ao alocar memória para operadores.\n");
                sistema->num_operadores = 0;
            }
        }
    } else { // ARQUIVO_TEXTO
        fscanf(arquivo, "%d\n", &sistema->num_operadores);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            if (!sistema->lista_operadores) {
                printf("Erro ao alocar memória (texto).\n");
                fclose(arquivo);
                return;
            }
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

// Libera a memória alocada para a lista de operadores
void liberarMemoriaOperadores(Sistema *sistema) {
    if (sistema->lista_operadores) {
        free(sistema->lista_operadores);
        sistema->lista_operadores = NULL;
        sistema->num_operadores = 0;
        sistema->capacidade_operadores = 0;
    }
}
