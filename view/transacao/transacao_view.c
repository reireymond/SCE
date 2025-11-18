#include "transacao_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/transacao/transacao_controller.h"

void menuTransacoesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=============================+\n");
        printf("|      MENU FINANCEIRO        |\n");
        printf("+=============================+\n");
        printf("| [1] Registrar Compra        |\n");
        printf("| [2] Contas a Receber        |\n");
        printf("| [3] Contas a Pagar          |\n");
        printf("| [4] Saldo do Caixa          |\n");
        printf("| [0] Voltar                  |\n");
        printf("+=============================+\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: lancarAquisicaoController(sistema); break;
            case 2: gerenciarContasReceberController(sistema); break;
            case 3: gerenciarContasPagarController(sistema); break;
            case 4: verSaldoCaixaController(sistema); break;
            case 0: break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}