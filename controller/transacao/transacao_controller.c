#include "transacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso_model.h"
#include "model/transacao/transacao_model.h"
#include "utils/utils.h"
#include "utils/validation.h"

void registrarTransacao(Sistema *sistema, Transacao t) {
    if (sistema->num_transacoes == sistema->capacidade_transacoes) {
        int nova = (sistema->capacidade_transacoes == 0) ? 10 : sistema->capacidade_transacoes * 2;
        Transacao *temp = realloc(sistema->lista_transacoes, nova * sizeof(Transacao));
        if (temp == NULL) {
            printf("Erro de alocacao de memoria para transacoes!\n");
            return;
        }
        sistema->lista_transacoes = temp;
        sistema->capacidade_transacoes = nova;
    }
    t.codigo = sistema->num_transacoes + 1;
    sistema->lista_transacoes[sistema->num_transacoes] = t;
    sistema->num_transacoes++;
    salvarTransacoes(sistema);
}

// Mantivemos essas funcoes aqui pois sao mistas de logica e exibicao simples
// num refactor mais pesado, deveriam ir para view tambem.

void gerenciarContasReceberController(Sistema *sistema) {
    printf("\n=== CONTAS A RECEBER ===\n");
    int achou = 0;
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == CONTA_A_RECEBER) {
             char *status_txt = (sistema->lista_transacoes[i].status == PAGA) ? "PAGA" : "PENDENTE";
             printf("ID: %d | %s | R$ %.2f | Vence: %s | Status: %s\n", 
                sistema->lista_transacoes[i].codigo, 
                sistema->lista_transacoes[i].descricao, 
                sistema->lista_transacoes[i].valor, 
                sistema->lista_transacoes[i].data_vencimento,
                status_txt);
             achou = 1;
        }
    }
    
    if(achou == 0) {
        printf("Nenhuma conta a receber.\n");
        return;
    }
    
    int cod;
    printf("\nDigite o ID para RECEBER (ou 0 para sair): "); 
    scanf("%d", &cod);
    limpar_buffer();
    
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
            if(sistema->lista_transacoes[i].tipo != CONTA_A_RECEBER) {
                printf("Isso nao e conta a receber.\n"); return;
            }
            if(sistema->lista_transacoes[i].status == PAGA) {
                printf("Esta conta ja foi paga.\n"); return;
            }
            sistema->lista_transacoes[i].status = PAGA;
            sistema->saldo_caixa += sistema->lista_transacoes[i].valor;
            salvarTransacoes(sistema);
            printf("Recebido com sucesso! Caixa atualizado.\n");
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    printf("\n=== CONTAS A PAGAR ===\n");
    int achou = 0;
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == CONTA_A_PAGAR) {
             char *status_txt = (sistema->lista_transacoes[i].status == PAGA) ? "PAGA" : "PENDENTE";
             printf("ID: %d | %s | R$ %.2f | Vence: %s | Status: %s\n", 
                sistema->lista_transacoes[i].codigo, 
                sistema->lista_transacoes[i].descricao, 
                sistema->lista_transacoes[i].valor, 
                sistema->lista_transacoes[i].data_vencimento,
                status_txt);
             achou = 1;
        }
    }

    if(achou == 0) {
        printf("Nenhuma conta a pagar.\n");
        return;
    }
    
    int cod;
    printf("\nDigite o ID para PAGAR (ou 0 para sair): "); 
    scanf("%d", &cod);
    limpar_buffer();
    
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
            if(sistema->lista_transacoes[i].tipo != CONTA_A_PAGAR) {
                printf("Isso nao e conta a pagar.\n"); return;
            }
             if(sistema->lista_transacoes[i].status == PAGA) {
                printf("Esta conta ja foi paga.\n"); return;
            }
            if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                sistema->lista_transacoes[i].status = PAGA;
                sistema->saldo_caixa -= sistema->lista_transacoes[i].valor;
                salvarTransacoes(sistema);
                printf("Conta paga! Caixa atualizado.\n");
            } else {
                printf("Erro: Saldo insuficiente no caixa! Voce tem R$ %.2f\n", sistema->saldo_caixa);
            }
            return;
        }
    }
    printf("Conta nao encontrada.\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    printf("\n==================================\n");
    printf(" SALDO ATUAL DO CAIXA: R$ %.2f\n", sistema->saldo_caixa);
    printf("==================================\n");
}
