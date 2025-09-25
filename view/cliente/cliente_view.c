#include "cliente_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/cliente/cliente_controller.h"

void menuClientesView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- Menu Clientes ---\n");
        printf("1. Adicionar Novo Cliente\n");
        printf("2. Alterar Cliente Existente\n");
        printf("3. Listar Todos os Clientes\n");
        printf("4. Excluir Cliente\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
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
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_clientes[i].codigo);
        printf("Nome / Razao Social: %s\n", sistema->lista_clientes[i].razao_social);
        printf("CPF / CNPJ: %s\n", sistema->lista_clientes[i].cnpj);
        printf("Endereco: %s\n", sistema->lista_clientes[i].endereco);
        printf("Telefone: %s\n", sistema->lista_clientes[i].telefone);
        printf("E-mail: %s\n", sistema->lista_clientes[i].email);
        printf("Contato: %s\n", sistema->lista_clientes[i].nome_do_contato);
    }
    printf("----------------------------------\n");
}