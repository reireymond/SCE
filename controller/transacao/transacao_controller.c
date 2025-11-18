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
        sistema->lista_transacoes = realloc(sistema->lista_transacoes, nova * sizeof(Transacao));
        sistema->capacidade_transacoes = nova;
    }
    t.codigo = sistema->num_transacoes + 1;
    sistema->lista_transacoes[sistema->num_transacoes] = t;
    sistema->num_transacoes++;
    salvarTransacoes(sistema);
}

void lancarAquisicaoController(Sistema *sistema) {
    if (!sistema->dados_produtora) {
        printf("Erro: Cadastre a Produtora antes.\n"); return;
    }

    printf("\n=== REGISTRAR COMPRA (NOTA FISCAL) ===\n");
    float frete, imposto;
    int itens;

    printf("Total Frete: R$ "); ler_float_positivo(&frete);
    printf("Total Imposto: R$ "); ler_float_positivo(&imposto);
    printf("Qtd Itens diferentes: "); ler_inteiro_valido(&itens, 1, 100);

    float frete_item = frete / itens;
    float imp_item = imposto / itens;

    for(int i=0; i < itens; i++) {
        printf("\n--- Item %d ---\n", i+1);
        if (sistema->num_recursos == sistema->capacidade_recursos) {
             sistema->lista_recursos = realloc(sistema->lista_recursos, (sistema->num_recursos + 10) * sizeof(Recurso));
             sistema->capacidade_recursos += 10;
        }
        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("Descricao: "); ler_texto_valido(novo->descricao, 150, VALIDAR_NAO_VAZIO);
        printf("Categoria: "); ler_texto_valido(novo->categoria, 50, VALIDAR_NAO_VAZIO);
        printf("Qtd: "); ler_inteiro_valido(&novo->quantidade_estoque, 1, 1000);
        printf("Custo Unitario: R$ "); ler_float_positivo(&novo->preco_custo);

        // Calculo do valor de locacao (Custo + Rateio + Lucro)
        float custo_final = novo->preco_custo + (frete_item/novo->quantidade_estoque) + (imp_item/novo->quantidade_estoque);
        novo->valor_locacao = custo_final * (1 + (sistema->dados_produtora->margem_lucro / 100.0));

        printf("-> Valor Locacao Calculado: %.2f\n", novo->valor_locacao);
        sistema->num_recursos++;
    }
    salvarRecursos(sistema);
    
    // Conta a Pagar da compra
    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_PAGAR;
    t.status = PENDENTE;
    printf("\nValor Total da Nota a Pagar: R$ "); ler_float_positivo(&t.valor);
    printf("Descricao: "); ler_texto_valido(t.descricao, 100, VALIDAR_NAO_VAZIO);
    printf("Vencimento: "); ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);
    
    registrarTransacao(sistema, t);
    printf("Compra registrada!\n");
}

void gerenciarContasReceberController(Sistema *sistema) {
    printf("=== A RECEBER ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == CONTA_A_RECEBER)
             printf("ID: %d | %s | R$ %.2f | Pago: %d\n", sistema->lista_transacoes[i].codigo, sistema->lista_transacoes[i].descricao, sistema->lista_transacoes[i].valor, sistema->lista_transacoes[i].status);
    }
    
    int cod;
    printf("ID para RECEBER (0 sair): "); scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
            sistema->lista_transacoes[i].status = PAGA;
            sistema->saldo_caixa += sistema->lista_transacoes[i].valor;
            salvarTransacoes(sistema);
            printf("Recebido! Caixa atualizado.\n");
            return;
        }
    }
}

void gerenciarContasPagarController(Sistema *sistema) {
    printf("=== A PAGAR ===\n");
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == CONTA_A_PAGAR)
             printf("ID: %d | %s | R$ %.2f | Pago: %d\n", sistema->lista_transacoes[i].codigo, sistema->lista_transacoes[i].descricao, sistema->lista_transacoes[i].valor, sistema->lista_transacoes[i].status);
    }
    
    int cod;
    printf("ID para PAGAR (0 sair): "); scanf("%d", &cod);
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
            if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                sistema->lista_transacoes[i].status = PAGA;
                sistema->saldo_caixa -= sistema->lista_transacoes[i].valor;
                salvarTransacoes(sistema);
                printf("Pago! Caixa atualizado.\n");
            } else printf("Sem saldo!\n");
            return;
        }
    }
}

void verSaldoCaixaController(Sistema *sistema) {
    printf("Caixa: R$ %.2f\n", sistema->saldo_caixa);
}