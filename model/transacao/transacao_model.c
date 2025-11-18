#include "model/transacao/transacao_model.h"
#include <stdio.h>
#include <stdlib.h>

#define TRANSACAO_DATA "data/transacoes.dat"
#define CAIXA_DATA "data/caixa.dat"

void salvarTransacoes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // Salva a lista de transações
    FILE *f = fopen(TRANSACAO_DATA, "wb");
    if (f) {
        fwrite(&sistema->num_transacoes, sizeof(int), 1, f);
        if (sistema->num_transacoes > 0)
            fwrite(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
        fclose(f);
    }

    // Salva o saldo do caixa separadamente
    FILE *fc = fopen(CAIXA_DATA, "wb");
    if (fc) {
        fwrite(&sistema->saldo_caixa, sizeof(float), 1, fc);
        fclose(fc);
    }
}

void carregarTransacoes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // Carrega transações
    FILE *f = fopen(TRANSACAO_DATA, "rb");
    if (f) {
        fread(&sistema->num_transacoes, sizeof(int), 1, f);
        if (sistema->num_transacoes > 0) {
            sistema->lista_transacoes = realloc(sistema->lista_transacoes, sistema->num_transacoes * sizeof(Transacao));
            fread(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
            sistema->capacidade_transacoes = sistema->num_transacoes;
        }
        fclose(f);
    }

    // Carrega saldo do caixa
    FILE *fc = fopen(CAIXA_DATA, "rb");
    if (fc) {
        fread(&sistema->saldo_caixa, sizeof(float), 1, fc);
        fclose(fc);
    }
}

// (Adicionar função liberarMemoriaTransacoes aqui também)