#include "cliente_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/cliente/cliente_controller.h"

void menuClientesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                   MENU CLIENTES                     |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Cliente                          |\n");
        printf("| [2] Alterar Cliente Existente                       |\n");
        printf("| [3] Listar Todos os Clientes                        |\n");
        printf("| [4] Excluir Cliente                                 |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarClienteController(sistema); break;
            case 2: alterarClienteController(sistema); break;
            case 3: listarClientesView(sistema); break;
            case 4: excluirClienteController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarClientesView(Sistema *sistema) {
    if (sistema->num_clientes == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Clientes ---\n");
    for (int i = 0; i < sistema->num_clientes; i++) {
        printf("ID: %d | Nome: %s | Tel: %s\n", 
               sistema->lista_clientes[i].codigo, 
               sistema->lista_clientes[i].razao_social, 
               sistema->lista_clientes[i].telefone);
    }
}