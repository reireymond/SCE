#include "transacao_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/config_armazenamento/config_armazenamento.h"

#define ARQUIVO_TRANSACOES_BIN "data/transacoes.dat"
#define ARQUIVO_TRANSACOES_TXT "data/transacoes.txt"
#define ARQUIVO_CAIXA_BIN "data/caixa.dat"
#define ARQUIVO_CAIXA_TXT "data/caixa.txt"

void remover_quebra_linha_transacao(char *str) {
    str[strcspn(str, "\n")] = 0;
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

void salvarTransacoes(Sistema *sistema) {
    TipoArmazenamento modo = obterModoDeArmazenamento(sistema);
    if (modo == MEMORIA) return;

    FILE *f = fopen(modo == ARQUIVO_BINARIO ? ARQUIVO_TRANSACOES_BIN : ARQUIVO_TRANSACOES_TXT, 
                    modo == ARQUIVO_BINARIO ? "wb" : "w");
    
    if (f != NULL) {
        if (modo == ARQUIVO_BINARIO) {
            fwrite(&sistema->num_transacoes, sizeof(int), 1, f);
            if (sistema->num_transacoes > 0) {
                fwrite(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
            }
        } else {
            fprintf(f, "Numero de trasacoes: %d\n", sistema->num_transacoes);
            for(int i=0; i < sistema->num_transacoes; i++) {
                Transacao *t = &sistema->lista_transacoes[i];
                fprintf(f, "Codigo: %d\n Tipo: %d\n Status: %d\n Valor: %.2f\n Descricao: %s\nData de vencimento: %s\n Data de pagamento: %s\n", 
                        t->codigo, t->tipo, t->status, t->valor, 
                        t->descricao, t->data_vencimento, t->data_pagamento);
            }
        }
        fclose(f);
    }
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
                sistema->lista_transacoes = malloc(sistema->num_transacoes * sizeof(Transacao));
                fread(sistema->lista_transacoes, sizeof(Transacao), sistema->num_transacoes, f);
                sistema->capacidade_transacoes = sistema->num_transacoes;
            }
        } else {
            char linha[256];
            char *valor;

            if(fgets(linha, sizeof(linha), f)) {
                valor = strchr(linha, ':');
                if(valor) sistema->num_transacoes = atoi(valor + 1);
            }

            if (sistema->num_transacoes > 0) {
                sistema->lista_transacoes = malloc(sistema->num_transacoes * sizeof(Transacao));
                sistema->capacidade_transacoes = sistema->num_transacoes;
                
                for(int i=0; i < sistema->num_transacoes; i++) {
                    Transacao *t = &sistema->lista_transacoes[i];
                    
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) t->codigo = atoi(valor + 1);
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) t->tipo = atoi(valor + 1);
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) t->status = atoi(valor + 1);
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); if(valor) t->valor = atof(valor + 1);
                    
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); 
                    if(valor) { strcpy(t->descricao, valor + 2); remover_quebra_linha_transacao(t->descricao); }
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); 
                    if(valor) { strcpy(t->data_vencimento, valor + 2); remover_quebra_linha_transacao(t->data_vencimento); }
                    fgets(linha, sizeof(linha), f); valor = strchr(linha, ':'); 
                    if(valor) { strcpy(t->data_pagamento, valor + 2); remover_quebra_linha_transacao(t->data_pagamento); }
                }
            }
        }
        fclose(f);
    }
    carregarCaixa(sistema);
}

void liberarMemoriaTransacoes(Sistema *sistema) {
    if (sistema->lista_transacoes) {
        free(sistema->lista_transacoes);
        sistema->lista_transacoes = NULL;
        sistema->num_transacoes = 0;
        sistema->capacidade_transacoes = 0;
    }
}