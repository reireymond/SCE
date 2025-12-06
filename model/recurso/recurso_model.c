#include "recurso_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define RECURSOS_DATA_FILE "data/recursos.dat"
#define RECURSOS_TEXT_FILE "data/recursos.txt"

void remover_quebra_linha_recurso(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void salvarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? RECURSOS_DATA_FILE : RECURSOS_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "wb" : "w");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fwrite(&sistema->num_recursos, sizeof(int), 1, arquivo);
        fwrite(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
    } else {
        fprintf(arquivo, "Numero de recursos cadastrados: %d\n", sistema->num_recursos);
        for (int i = 0; i < sistema->num_recursos; i++) {
            fprintf(arquivo, "Codigo: %d\nDescricao: %s\n Categoria: %s\nQuantidade em estoque: %d\nPreco de custo: %.2f\n Valor da locacao: %.2f\n",
                    sistema->lista_recursos[i].codigo,
                    sistema->lista_recursos[i].descricao,
                    sistema->lista_recursos[i].categoria,
                    sistema->lista_recursos[i].quantidade_estoque,
                    sistema->lista_recursos[i].preco_custo,
                    sistema->lista_recursos[i].valor_locacao);
        }
    }
    fclose(arquivo);
}

void carregarRecursos(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *arquivo = fopen((modo == ARQUIVO_BINARIO) ? RECURSOS_DATA_FILE : RECURSOS_TEXT_FILE, 
                          (modo == ARQUIVO_BINARIO) ? "rb" : "r");
    if (!arquivo) return;

    if (modo == ARQUIVO_BINARIO) {
        fread(&sistema->num_recursos, sizeof(int), 1, arquivo);
        if (sistema->num_recursos > 0) {
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            sistema->capacidade_recursos = sistema->num_recursos;
            fread(sistema->lista_recursos, sizeof(Recurso), sistema->num_recursos, arquivo);
        }
    } else {
        char linha[256];
        char *valor;

        if(fgets(linha, sizeof(linha), arquivo)) {
            valor = strchr(linha, ':');
            if(valor) sistema->num_recursos = atoi(valor + 1);
        }

        if (sistema->num_recursos > 0) {
            sistema->lista_recursos = malloc(sistema->num_recursos * sizeof(Recurso));
            sistema->capacidade_recursos = sistema->num_recursos;
            
            for (int i = 0; i < sistema->num_recursos; i++) {
                Recurso *r = &sistema->lista_recursos[i];
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) r->codigo = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(r->descricao, valor + 2); remover_quebra_linha_recurso(r->descricao); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) { strcpy(r->categoria, valor + 2); remover_quebra_linha_recurso(r->categoria); }
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) r->quantidade_estoque = atoi(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) r->preco_custo = atof(valor + 1);
                fgets(linha, sizeof(linha), arquivo);
                valor = strchr(linha, ':'); if(valor) r->valor_locacao = atof(valor + 1);
            }
        }
    }
    fclose(arquivo);
}

void liberarMemoriaRecursos(Sistema *sistema) {
    if (sistema->lista_recursos) {
        free(sistema->lista_recursos);
        sistema->lista_recursos = NULL;
        sistema->num_recursos = 0;
        sistema->capacidade_recursos = 0;
    }
}