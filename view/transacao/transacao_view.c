#include "transacao_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "utils/validation.h"
#include "controller/transacao/transacao_controller.h"
#include "model/recurso/recurso_model.h"

void menuTransacoesView(Sistema *sistema) {
 
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|               MENU FINANCEIRO / COMPRAS             |\n");
        printf("| [1] Registrar Compra (Nota Fiscal)                  |\n");
        printf("| [2] Gerenciar Contas a Receber                      |\n");
        printf("| [3] Gerenciar Contas a Pagar                        |\n");
        printf("| [4] Ver Saldo do Caixa                              |\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: formularioRegistrarCompraView(sistema); break;
            case 2: gerenciarContasReceberController(sistema); break;
            case 3: gerenciarContasPagarController(sistema); break;
            case 4: verSaldoCaixaController(sistema); break;
            case 0: break;
            default: printf("Opcao invalida.\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// ... (Mantenha formularioRegistrarCompraView se estiver lá) ...
void formularioRegistrarCompraView(Sistema *sistema) {
    // ... (Seu código existente de registrar compra) ...
    // Se não tiver o código, avise que eu mando, mas parece que já tinha no upload anterior.
    // Lembre-se: NÃO implemente gerenciarContas... aqui embaixo!
}

// --- IMPLEMENTAÇÕES QUE FALTAVAM ---

void listar_contas_filtro(Sistema *sistema, TipoTransacao tipo) {
    printf("\n--- Lista de Contas ---\n");
    int achou = 0;
    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].tipo == tipo) {
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
    if(!achou) printf("Nenhuma conta encontrada neste filtro.\n");
}

int pedir_id_pagamento() {
    int id;
    printf("\nDigite o ID da conta (ou 0 para voltar): ");
    ler_inteiro_valido(&id, 0, 999999);
    return id;
}

void mostrar_saldo(float saldo) {
    printf("\n==================================\n");
    printf(" SALDO ATUAL DO CAIXA: R$ %.2f\n", saldo);
    printf("==================================\n");
}

void msg_conta_paga_sucesso() {
    mensagem_sucesso("Transacao realizada com sucesso!");
}

void msg_sem_saldo(float saldo_atual) {
    printf("\n[ERRO] Saldo insuficiente! Voce tem apenas R$ %.2f\n", saldo_atual);
    pausar();
}