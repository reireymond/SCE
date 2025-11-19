#include "fornecedor_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/fornecedor/fornecedor_controller.h"

void menuFornecedoresView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           MENU FORNECEDORES E PARCEIROS             |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Fornecedor                       |\n");
        printf("| [2] Alterar Fornecedor Existente                    |\n");
        printf("| [3] Listar Todos os Fornecedores                    |\n");
        printf("| [4] Excluir Fornecedor                              |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarFornecedorController(sistema); break;
            case 2: alterarFornecedorController(sistema); break;
            case 3: listarFornecedoresView(sistema); break;
            case 4: excluirFornecedorController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarFornecedoresView(Sistema *sistema) {
    if (sistema->num_fornecedores == 0) {
        printf("\nNenhum fornecedor cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Fornecedores ---\n");
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        printf("ID: %d | %s | Servico: %s\n", 
               sistema->lista_fornecedores[i].codigo,
               sistema->lista_fornecedores[i].nome_fantasia,
               sistema->lista_fornecedores[i].tipo_servico);
    }
}