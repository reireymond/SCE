#include "fornecedor_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/fornecedor/fornecedor_controller.h"

void menuFornecedoresView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- Menu Fornecedores e Parceiros ---\n");
        printf("1. Adicionar Novo Fornecedor\n");
        printf("2. Alterar Fornecedor Existente\n");
        printf("3. Listar Todos os Fornecedores\n");
        printf("4. Excluir Fornecedor\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
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

    printf("\n--- Lista de Fornecedores e Parceiros ---\n");
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_fornecedores[i].codigo);
        printf("Nome Fantasia: %s\n", sistema->lista_fornecedores[i].nome_fantasia);
        printf("Razao Social: %s\n", sistema->lista_fornecedores[i].razao_social);
        printf("CNPJ / CPF: %s\n", sistema->lista_fornecedores[i].cnpj);
        printf("Endereco: %s\n", sistema->lista_fornecedores[i].endereco);
        printf("Telefone: %s\n", sistema->lista_fornecedores[i].telefone);
        printf("Tipo de Servico: %s\n", sistema->lista_fornecedores[i].tipo_servico);
    }
    printf("----------------------------------\n");
}