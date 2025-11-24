#include "transacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/recurso/recurso_model.h"
#include "model/transacao/transacao_model.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Funcao interna pra salvar uma transacao nova
void registrarTransacao(Sistema *sistema, Transacao t) {
    // Aumenta lista se precisar
    if (sistema->num_transacoes == sistema->capacidade_transacoes) {
        int nova = (sistema->capacidade_transacoes == 0) ? 10 : sistema->capacidade_transacoes * 2;
        sistema->lista_transacoes = realloc(sistema->lista_transacoes, nova * sizeof(Transacao));
        sistema->capacidade_transacoes = nova;
    }
    // Define ID
    t.codigo = sistema->num_transacoes + 1;
    // Guarda na lista
    sistema->lista_transacoes[sistema->num_transacoes] = t;
    sistema->num_transacoes++;
    salvarTransacoes(sistema);
}

void lancarAquisicaoController(Sistema *sistema) {
    // Precisa da produtora cadastrada pra pegar a margem de lucro
    if (!sistema->dados_produtora) {
        printf("Erro: Cadastre a Produtora primeiro pra saber a margem de lucro.\n"); 
        return;
    }

    printf("\n=== REGISTRAR COMPRA (NOTA FISCAL) ===\n");
    float frete_total, imposto_total;
    int qtd_itens_diferentes;

    printf("Valor Total do Frete na Nota: R$ "); 
    ler_float_positivo(&frete_total);
    
    printf("Valor Total de Impostos: R$ "); 
    ler_float_positivo(&imposto_total);
    
    printf("Quantos produtos diferentes tem na nota? "); 
    ler_inteiro_valido(&qtd_itens_diferentes, 1, 100);

    // Divide frete e imposto igual pra todos (rateio simples)
    float frete_por_item = frete_total / qtd_itens_diferentes;
    float imposto_por_item = imposto_total / qtd_itens_diferentes;

    // Loop pra cadastrar cada item da nota
    for(int i=0; i < qtd_itens_diferentes; i++) {
        printf("\n--- Produto %d ---\n", i+1);
        
        // Aumenta vetor de recursos se precisar
        if (sistema->num_recursos == sistema->capacidade_recursos) {
             sistema->lista_recursos = realloc(sistema->lista_recursos, (sistema->num_recursos + 10) * sizeof(Recurso));
             sistema->capacidade_recursos += 10;
        }
        
        Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
        novo->codigo = sistema->num_recursos + 1;

        printf("Descricao do Equipamento: "); 
        ler_texto_valido(novo->descricao, 150, VALIDAR_NAO_VAZIO);
        
        printf("Categoria (Ex: Som, Luz): "); 
        ler_texto_valido(novo->categoria, 50, VALIDAR_NAO_VAZIO);
        
        printf("Quantidade comprada: "); 
        ler_inteiro_valido(&novo->quantidade_estoque, 1, 1000);
        
        printf("Preco de Custo (Unitario): R$ "); 
        ler_float_positivo(&novo->preco_custo);

        // CALCULO DO PRECO DE ALUGUEL (Corrigido conforme requisito)
        // Custo final = Custo + (Frete rateado / qtd) + (Imposto rateado / qtd)
        float custo_unitario_final = novo->preco_custo + (frete_por_item / novo->quantidade_estoque) + (imposto_por_item / novo->quantidade_estoque);
        
        // Antes tava errado somando 1. Agora calcula so a porcentagem (ex: 10% do valor)
        novo->valor_locacao = custo_unitario_final * (sistema->dados_produtora->margem_lucro / 100.0);

        printf("-> Valor de Locacao Calculado (Baseado na margem): R$ %.2f\n", novo->valor_locacao);
        
        sistema->num_recursos++;
    }
    salvarRecursos(sistema);
    
    // Pergunta como vai pagar a compra dos equipamentos
    int tipo_pagamento;
    printf("\nComo deseja pagar essa compra?\n");
    printf("1. A Vista (Sai do Caixa agora)\n");
    printf("2. A Prazo (Gera Conta a Pagar)\n");
    printf("Opcao: ");
    ler_inteiro_valido(&tipo_pagamento, 1, 2);

    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_PAGAR;
    
    printf("\nValor Total da Nota para Pagar: R$ "); 
    ler_float_positivo(&t.valor);
    
    printf("Descricao da conta: "); 
    ler_texto_valido(t.descricao, 100, VALIDAR_NAO_VAZIO);
    
    if (tipo_pagamento == 1) {
        // Se for a vista, verifica se tem dinheiro
        if (sistema->saldo_caixa >= t.valor) {
            sistema->saldo_caixa -= t.valor;
            t.status = PAGA; // Ja nasce paga
            printf("Data do Pagamento (Hoje): ");
            ler_texto_valido(t.data_pagamento, 12, VALIDAR_DATA);
            strcpy(t.data_vencimento, t.data_pagamento); // Vencimento igual pagamento
            printf("Pagamento realizado com sucesso! Saiu do caixa.\n");
        } else {
            printf("ERRO: Sem dinheiro no caixa! Vai ficar como PENDENTE.\n");
            t.status = PENDENTE;
            printf("Data de Vencimento: "); 
            ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);
        }
    } else {
        // A prazo
        t.status = PENDENTE;
        printf("Data de Vencimento: "); 
        ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);
    }
    
    registrarTransacao(sistema, t);
    printf("Compra registrada e estoque atualizado!\n");
}

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
                printf("Isso nao e conta a receber.\n"); 
                return;
            }
            sistema->lista_transacoes[i].status = PAGA;
            // Entra dinheiro no caixa
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
            // Verifica se tem dinheiro
            if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                sistema->lista_transacoes[i].status = PAGA;
                // Tira dinheiro do caixa
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