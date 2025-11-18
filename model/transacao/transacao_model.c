#include "model/transacao/transacao_model.h"
#include <stdio.h>
#include <stdlib.h>
#include "model/config_armazenamento/config_armazenamento.h"

// Nomes dos arquivos
#define ARQUIVO_TRANSACOES_BIN "data/transacoes.dat"
#define ARQUIVO_TRANSACOES_TXT "data/transacoes.txt"
#define ARQUIVO_CAIXA_BIN "data/caixa.dat"
#define ARQUIVO_CAIXA_TXT "data/caixa.txt"

void salvarTransacoes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    // 1. Salvar a lista de transacoes
    FILE *f = fopen(modo == ARQUIVO_BINARIO ? ARQUIVO_TRANSACOES_BIN : ARQUIVO_TRANSACOES_TXT, 
                    modo == ARQUIVO_BINARIO ? "wb" : "w");
    
    if (f != NULL) {
        if (modo == ARQUIVO_BINARIO) {
            fwrite(&sistema->num_transacoes, sizeof(int), 1, f);
            if (sistema->num_transacoes > 0) {
                fwrite(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
            }
        } else {
            fprintf(f, "%d\n", sistema->num_transacoes);
            for(int i=0; i < sistema->num_transacoes; i++) {
                Transacao *t = &sistema->lista_transacoes[i];
                // Salva tudo numa linha ou varias, formato simples
                fprintf(f, "%d\n%d\n%d\n%.2f\n%s\n%s\n%s\n", 
                        t->codigo, t->tipo, t->status, t->valor, 
                        t->descricao, t->data_vencimento, t->data_pagamento);
            }
        }
        fclose(f);
    }

    // 2. Salvar o saldo do caixa separadamente para facilitar
    salvarCaixa(sistema);
}

void carregarTransacoes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *f = fopen(modo == ARQUIVO_BINARIO ? ARQUIVO_TRANSACOES_BIN : ARQUIVO_TRANSACOES_TXT, 
                    modo == ARQUIVO_BINARIO ? "rb" : "r");
    
    if (f != NULL) {
        if (modo == ARQUIVO_BINARIO) {
            fread(&sistema->num_transacoes, sizeof(int), 1, f);
            if (sistema->num_transacoes > 0) {
                sistema->lista_transacoes = realloc(sistema->lista_transacoes, sistema->num_transacoes * sizeof(Transacao));
                fread(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
                sistema->capacidade_transacoes = sistema->num_transacoes;
            }
        } else {
            fscanf(f, "%d\n", &sistema->num_transacoes);
            if (sistema->num_transacoes > 0) {
                sistema->lista_transacoes = realloc(sistema->lista_transacoes, sistema->num_transacoes * sizeof(Transacao));
                sistema->capacidade_transacoes = sistema->num_transacoes;
                
                for(int i=0; i < sistema->num_transacoes; i++) {
                    Transacao *t = &sistema->lista_transacoes[i];
                    int tipo, status;
                    fscanf(f, "%d\n%d\n%d\n%f\n", &t->codigo, &tipo, &status, &t->valor);
                    t->tipo = tipo;
                    t->status = status;
                    // Ler strings (limpa o \n do final)
                    fgets(t->descricao, sizeof(t->descricao), f); 
                    t->descricao[strcspn(t->descricao, "\n")] = 0;
                    
                    fgets(t->data_vencimento, sizeof(t->data_vencimento), f);
                    t->data_vencimento[strcspn(t->data_vencimento, "\n")] = 0;
                    
                    fgets(t->data_pagamento, sizeof(t->data_pagamento), f);
                    t->data_pagamento[strcspn(t->data_pagamento, "\n")] = 0;
                }
            }
        }
        fclose(f);
    }

    carregarCaixa(sistema);
}

void salvarCaixa(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    FILE *f = fopen(modo == ARQUIVO_BINARIO ? ARQUIVO_CAIXA_BIN : ARQUIVO_CAIXA_TXT, 
                    modo == ARQUIVO_BINARIO ? "wb" : "w");
    if(f) {
        if(modo == ARQUIVO_BINARIO) fwrite(&sistema->saldo_caixa, sizeof(float), 1, f);
        else fprintf(f, "%.2f\n", sistema->saldo_caixa);
        fclose(f);
    }
}

void carregarCaixa(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    FILE *f = fopen(modo == ARQUIVO_BINARIO ? ARQUIVO_CAIXA_BIN : ARQUIVO_CAIXA_TXT, 
                    modo == ARQUIVO_BINARIO ? "rb" : "r");
    if(f) {
        if(modo == ARQUIVO_BINARIO) fread(&sistema->saldo_caixa, sizeof(float), 1, f);
        else fscanf(f, "%f\n", &sistema->saldo_caixa);
        fclose(f);
    } else {
        sistema->saldo_caixa = 0.0;
    }
}

void liberarMemoriaTransacoes(Sistema *sistema) {
    if (sistema->lista_transacoes) {
        free(sistema->lista_transacoes);
        sistema->lista_transacoes = NULL;
    }
}