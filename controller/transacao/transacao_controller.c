#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"
#include "model/transacao/transacao_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "utils/validation.h"

// Funcao auxiliar para criar e salvar uma transacao
void adicionarTransacao(Sistema *s, TipoTransacao tipo, char *desc, float valor, char *venc) {
    if (s->num_transacoes == s->capacidade_transacoes) {
        int nova_cap = (s->capacidade_transacoes == 0) ? 10 : s->capacidade_transacoes * 2;
        s->lista_transacoes = realloc(s->lista_transacoes, nova_cap * sizeof(Transacao));
        s->capacidade_transacoes = nova_cap;
    }

    Transacao *t = &s->lista_transacoes[s->num_transacoes];
    t->codigo = s->num_transacoes + 1;
    t->tipo = tipo;
    t->status = PENDENTE;
    t->valor = valor;
    strcpy(t->descricao, desc);
    strcpy(t->data_vencimento, venc);
    strcpy(t->data_pagamento, ""); // Ainda nao pago
    
    s->num_transacoes++;
    salvarTransacoes(s);
}

void lancarAquisicaoController(Sistema *sistema) {
    limpar_tela();
    printf("=== REGISTAR COMPRA (NOTA FISCAL) ===\n");

    float frete_total, imposto_total;
    int total_itens_nota;
    float valor_total_produtos = 0; // Acumulador para a divida

    printf("Valor TOTAL do Frete (R$): ");
    ler_float_positivo(&frete_total);

    printf("Valor TOTAL dos Impostos (R$): ");
    ler_float_positivo(&imposto_total);

    printf("Quantidade TOTAL de itens comprados: ");
    ler_int_valido(&total_itens_nota, 1, 10000);

    // Calculo do rateio (quanto de frete/imposto cada unidade paga)
    float frete_por_unidade = frete_total / total_itens_nota;
    float imposto_por_unidade = imposto_total / total_itens_nota;

    int continuar = 1;
    while (continuar == 1) {
        // Aumentar memoria de recursos se preciso
        if (sistema->num_recursos == sistema->capacidade_recursos) {
            int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
            Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
            if (!temp) { printf("Erro memoria!\n"); return; }
            sistema->lista_recursos = temp;
            sistema->capacidade_recursos = nova_cap;
        }

        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("\n--- Novo Item da Nota --- \n");
        printf("Descricao: ");
        ler_string_valida(novo->descricao, sizeof(novo->descricao), VALIDATE_NAO_VAZIA);
        printf("Categoria: ");
        ler_string_valida(novo->categoria, sizeof(novo->categoria), VALIDATE_NAO_VAZIA);
        printf("Quantidade: ");
        ler_int_valido(&novo->quantidade_estoque, 1, 1000);
        printf("Custo Unitario (R$): ");
        ler_float_positivo(&novo->preco_custo);

        // Soma ao total para criar a conta a pagar depois
        valor_total_produtos += (novo->preco_custo * novo->quantidade_estoque);

        // CALCULO DO PRECO DE LOCACAO (Regra do PDF)
        float custo_final = novo->preco_custo + frete_por_unidade + imposto_por_unidade;
        float margem = (sistema->dados_produtora != NULL) ? sistema->dados_produtora->margem_lucro : 20.0;
        
        novo->valor_locacao = custo_final * (1 + (margem / 100.0));

        printf("--> Valor Locacao Calculado: R$ %.2f\n", novo->valor_locacao);

        sistema->num_recursos++;
        salvarRecursos(sistema); 

        printf("\nMais itens na nota? (1-Sim, 0-Nao): ");
        scanf("%d", &continuar);
        limpar_buffer();
    }

    // Gera a conta a pagar total
    float total_final = valor_total_produtos + frete_total + imposto_total;
    adicionarTransacao(sistema, CONTA_A_PAGAR, "Compra Equipamentos (Nota Fiscal)", total_final, "30/12/2025");
    
    printf("\nTransacao de R$ %.2f registada em Contas a Pagar!\n", total_final);
    printf("Estoque atualizado.\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    limpar_tela();
    printf("=== FLUXO DE CAIXA ===\n");
    printf("Saldo Disponivel: R$ %.2f\n", sistema->saldo_caixa);
    pausar();
}

void gerenciarContasReceberController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A RECEBER ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_RECEBER) {
            printf("#%d | %s | R$ %.2f | %s\n", t->codigo, t->descricao, t->valor, 
                   t->status == PAGA ? "PAGO" : "PENDENTE");
        }
    }
    
    int cod;
    printf("\nCodigo para RECEBER (0 voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_RECEBER) {
            if(sistema->lista_transacoes[i].status == PENDENTE) {
                sistema->lista_transacoes[i].status = PAGA;
                sistema->saldo_caixa += sistema->lista_transacoes[i].valor;
                salvarTransacoes(sistema);
                printf("Recebido! Saldo atual: %.2f\n", sistema->saldo_caixa);
            } else {
                printf("Ja estava pago.\n");
            }
            return;
        }
    }
    printf("Nao encontrado.\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    limpar_tela();
    printf("=== CONTAS A PAGAR ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->tipo == CONTA_A_PAGAR) {
            printf("#%d | %s | R$ %.2f | %s\n", t->codigo, t->descricao, t->valor, 
                   t->status == PAGA ? "PAGO" : "PENDENTE");
        }
    }

    int cod;
    printf("\nCodigo para PAGAR (0 voltar): ");
    scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod && sistema->lista_transacoes[i].tipo == CONTA_A_PAGAR) {
            if(sistema->lista_transacoes[i].status == PENDENTE) {
                if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                    sistema->lista_transacoes[i].status = PAGA;
                    sistema->saldo_caixa -= sistema->lista_transacoes[i].valor;
                    salvarTransacoes(sistema);
                    printf("Pago! Saldo atual: %.2f\n", sistema->saldo_caixa);
                } else {
                    printf("Erro: Sem saldo suficiente!\n");
                }
            } else {
                printf("Ja estava pago.\n");
            }
            return;
        }
    }
    printf("Nao encontrado.\n");
}