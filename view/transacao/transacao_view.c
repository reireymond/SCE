#include "transacao_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/transacao/transacao_controller.h"

void menuTransacoesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|               MENU FINANCEIRO / COMPRAS             |\n");
        printf("+=====================================================+\n");
        printf("| [1] Registrar Compra (Nota Fiscal)                  |\n");
        printf("| [2] Gerenciar Contas a Receber                      |\n");
        printf("| [3] Gerenciar Contas a Pagar                        |\n");
        printf("| [4] Ver Saldo do Caixa                              |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: lancarAquisicaoController(sistema); break;
            case 2: gerenciarContasReceberController(sistema); break;
            case 3: gerenciarContasPagarController(sistema); break;
            case 4: verSaldoCaixaController(sistema); break;
            case 0: break;
            default: printf("Opcao invalida.\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}