#include "operador_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define OPERADORES_DATA_FILE "data/operadores.dat"
#define OPERADORES_TEXT_FILE "data/operadores.txt"

void remover_quebra_linha_operador(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void salvarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_operadores, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
    } else {
        fprintf(arquivo, "Numero de operadores cadastrados: %d\n", sistema->num_operadores);
        for (int i = 0; i < sistema->num_operadores; i++) {
            fprintf(arquivo, " Codigo: %d\n Nome: %s\n Usuario: %s\n Senha: %s\n",
                    sistema->lista_operadores[i].codigo,
                    sistema->lista_operadores[i].nome,
                    sistema->lista_operadores[i].usuario,
                    sistema->lista_operadores[i].senha);
        }
    }
    fclose(arquivo);
}

void carregarOperadores(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? OPERADORES_DATA_FILE : OPERADORES_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_operadores, sizeof(int), 1, arquivo);
        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            fread(sistema->lista_operadores, sizeof(Operador), sistema->num_operadores, arquivo);
        }
    } else {
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) sistema->num_operadores = atoi(valor + 1);
        }

        if (sistema->num_operadores > 0) {
            sistema->lista_operadores = malloc(sistema->num_operadores * sizeof(Operador));
            sistema->capacidade_operadores = sistema->num_operadores;
            for (int i = 0; i < sistema->num_operadores; i++) {
                Operador *op = &sistema->lista_operadores[i];
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) op->codigo = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(op->nome, valor + 2); remover_quebra_linha_operador(op->nome); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(op->usuario, valor + 2); remover_quebra_linha_operador(op->usuario); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(op->senha, valor + 2); remover_quebra_linha_operador(op->senha); }
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaOperadores(Sistema *sistema) {
    if (sistema->lista_operadores) {
        free(sistema->lista_operadores);
        sistema->lista_operadores = NULL;
        sistema->num_operadores = 0;
        sistema->capacidade_operadores = 0;
    }
}