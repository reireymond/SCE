#include "transacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso_model.h"
#include "model/transacao/transacao_model.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Registra transação e salva no disco
void registrarTransacao(Sistema *sistema, Transacao t) {
    if (sistema->num_transacoes == sistema->capacidade_transacoes) {
        int nova_cap = (sistema->capacidade_transacoes == 0) ? 10 : sistema->capacidade_transacoes * 2;
        sistema->lista_transacoes = realloc(sistema->lista_transacoes, nova_cap * sizeof(Transacao));
        sistema->capacidade_transacoes = nova_cap;
    }
    t.codigo = sistema->num_transacoes + 1;
    sistema->lista_transacoes[sistema->num_transacoes++] = t;
    salvarTransacoes(sistema);
}

// Lista transações filtradas por tipo e retorna se achou alguma
int listarTransacoesPorTipo(Sistema *sistema, TipoTransacao tipo, const char* titulo) {
    printf("\n=== %s ===\n", titulo);
    int achou = 0;
    for(int i = 0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == tipo) {
             Transacao *t = &sistema->lista_transacoes[i];
             printf("ID: %d | R$ %8.2f | Vence: %-10s | %s | %s\n", 
                t->codigo, t->valor, t->data_vencimento, 
                (t->status == PAGA ? "PAGA" : "PENDENTE"), t->descricao);
             achou = 1;
        }
    }
    if(!achou) printf("Nenhum registro encontrado.\n");
    return achou;
}

// --- CONTROLLERS ---

void lancarAquisicaoController(Sistema *sistema) {
    if (!sistema->dados_produtora) { printf("Erro: Cadastre a Produtora primeiro.\n"); return; }

    printf("\n=== NOVA COMPRA (NOTA FISCAL) ===\n");
    float frete, imposto, total_prod = 0;
    int qtd_itens;

    printf("Frete Total: R$ "); ler_float_positivo(&frete);
    printf("Impostos Total: R$ "); ler_float_positivo(&imposto);
    printf("Qtd. Itens Diferentes: "); ler_inteiro_valido(&qtd_itens, 1, 100);

    //  Aloca memória para TODOS os itens de uma vez só
    if (sistema->num_recursos + qtd_itens > sistema->capacidade_recursos) {
        sistema->capacidade_recursos += (qtd_itens + 10);
        sistema->lista_recursos = realloc(sistema->lista_recursos, sistema->capacidade_recursos * sizeof(Recurso));
    }

    float taxas_por_item = (frete + imposto) / qtd_itens;

    for(int i = 0; i < qtd_itens; i++) {
        Recurso *r = &sistema->lista_recursos[sistema->num_recursos];
        r->codigo = sistema->num_recursos + 1;

        printf("\n-- Item %d --\n", i+1);
        printf("Descricao: "); ler_texto_valido(r->descricao, 150, VALIDAR_NAO_VAZIO);
        printf("Categoria: "); ler_texto_valido(r->categoria, 50, VALIDAR_NAO_VAZIO);
        printf("Qtd: "); ler_inteiro_valido(&r->quantidade_estoque, 1, 10000);
        printf("Custo Unit.: R$ "); ler_float_positivo(&r->preco_custo);

        total_prod += (r->preco_custo * r->quantidade_estoque);
        
        // Custo final e Preço Locação
        float custo_final = r->preco_custo + (taxas_por_item / r->quantidade_estoque);
        r->valor_locacao = custo_final * (1.0 + (sistema->dados_produtora->margem_lucro / 100.0));
        
        sistema->num_recursos++;
    }
    salvarRecursos(sistema);

    float total_nota = total_prod + frete + imposto;
    printf("\nTOTAL NOTA: R$ %.2f\n1. A Vista\n2. A Prazo\nOpcao: ", total_nota);
    int op; ler_inteiro_valido(&op, 1, 2);

    // Lógica de Pagamento
    float entrada = 0;
    if (op == 2) { // A Prazo
        printf("Valor da Entrada (0 se nao houver): R$ ");
        scanf("%f", &entrada); limpar_buffer();
    } else { // A Vista
        entrada = total_nota; // Trata tudo como entrada imediata
    }

    // Processa Pagamento Imediato (Entrada ou Total à vista)
    if (entrada > 0) {
        if (sistema->saldo_caixa >= entrada) {
            sistema->saldo_caixa -= entrada;
            Transacao t = {0};
            t.tipo = CONTA_A_PAGAR; t.status = PAGA; t.valor = entrada;
            strcpy(t.descricao, op == 1 ? "Compra A Vista" : "Entrada Compra");
            printf("Data Pagamento: "); ler_texto_valido(t.data_pagamento, 12, VALIDAR_DATA);
            strcpy(t.data_vencimento, t.data_pagamento);
            registrarTransacao(sistema, t);
            printf("Debitado R$ %.2f do Caixa.\n", entrada);
        } else {
            printf("Erro: Saldo insuficiente (Tem: %.2f) para pagar R$ %.2f agora.\n", sistema->saldo_caixa, entrada);
            if(op == 1) return; // Se era a vista, cancela. Se prazo, continua parcelas.
        }
    }

    // Processa Parcelas (Restante)
    float restante = total_nota - entrada;
    if (restante > 0) {
        int parcelas;
        printf("Qtd Parcelas para R$ %.2f: ", restante); ler_inteiro_valido(&parcelas, 1, 48);
        char vencto[15];
        printf("1o Vencimento: "); ler_texto_valido(vencto, 12, VALIDAR_DATA);

        float valor_p = restante / parcelas;
        for(int p=1; p<=parcelas; p++) {
            Transacao t = {0};
            t.tipo = CONTA_A_PAGAR; t.status = PENDENTE; t.valor = valor_p;
            sprintf(t.descricao, "Parc %d/%d Compra", p, parcelas);
            strcpy(t.data_vencimento, (p==1 ? vencto : "Ver Detalhes"));
            registrarTransacao(sistema, t);
        }
        printf("%d parcelas geradas.\n", parcelas);
    }
}

void gerenciarContasReceberController(Sistema *sistema) {
    if(!listarTransacoesPorTipo(sistema, CONTA_A_RECEBER, "CONTAS A RECEBER")) return;
    
    int id; printf("\nID para RECEBER (0 sair): "); scanf("%d", &id); limpar_buffer();
    if(id == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->codigo == id && t->tipo == CONTA_A_RECEBER && t->status == PENDENTE) {
            t->status = PAGA;
            printf("Data Receb.: "); ler_texto_valido(t->data_pagamento, 12, VALIDAR_DATA);
            sistema->saldo_caixa += t->valor;
            salvarTransacoes(sistema);
            printf("Recebido! Saldo Atual: %.2f\n", sistema->saldo_caixa);
            return;
        }
    }
    printf("ID invalido ou ja pago.\n");
}

void gerenciarContasPagarController(Sistema *sistema) {
    if(!listarTransacoesPorTipo(sistema, CONTA_A_PAGAR, "CONTAS A PAGAR")) return;

    int id; printf("\nID para PAGAR (0 sair): "); scanf("%d", &id); limpar_buffer();
    if(id == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        Transacao *t = &sistema->lista_transacoes[i];
        if(t->codigo == id && t->tipo == CONTA_A_PAGAR && t->status == PENDENTE) {
            if(sistema->saldo_caixa >= t->valor) {
                t->status = PAGA;
                printf("Data Pagto: "); ler_texto_valido(t->data_pagamento, 12, VALIDAR_DATA);
                sistema->saldo_caixa -= t->valor;
                salvarTransacoes(sistema);
                printf("Pago! Saldo Atual: %.2f\n", sistema->saldo_caixa);
            } else {
                printf("Sem saldo! Precisa: %.2f | Tem: %.2f\n", t->valor, sistema->saldo_caixa);
            }
            return;
        }
    }
    printf("ID invalido ou ja pago.\n");
}

void verSaldoCaixaController(Sistema *sistema) {
    printf("\n=== CAIXA: R$ %.2f ===\n", sistema->saldo_caixa);
}