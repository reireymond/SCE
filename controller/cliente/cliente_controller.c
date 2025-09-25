#include "cliente_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/cliente/cliente.h"
#include "view/cliente/cliente_view.h"
#include "utils/utils.h"

void adicionarClienteController(Sistema *sistema) {
    if (sistema->num_clientes == sistema->capacidade_clientes) {
        int nova_capacidade = (sistema->capacidade_clientes == 0) ? 10 : sistema->capacidade_clientes * 2;
        Cliente *temp = realloc(sistema->lista_clientes, nova_capacidade * sizeof(Cliente));
        if (!temp) {
            printf("\nErro de alocacao de memoria!\n");
            return;
        }
        sistema->lista_clientes = temp;
        sistema->capacidade_clientes = nova_capacidade;
    }

    Cliente novo_cliente;
    novo_cliente.codigo = sistema->num_clientes + 1;

    printf("\n--- Cadastro de Novo Cliente (Codigo: %d) ---\n", novo_cliente.codigo);
    printf("Nome / Razao Social: ");
    fgets(novo_cliente.razao_social, 100, stdin);
    novo_cliente.razao_social[strcspn(novo_cliente.razao_social, "\n")] = 0;
    // ... complete com os outros fgets para os campos restantes ...

    sistema->lista_clientes[sistema->num_clientes] = novo_cliente;
    sistema->num_clientes++;

    salvarClientes(sistema);
    printf("\nCliente '%s' cadastrado com sucesso!\n", novo_cliente.razao_social);
}

void alterarClienteController(Sistema *sistema) {
    // Implemente a lógica de alteração aqui, usando listarClientesView para mostrar as opções
    printf("\nFuncao de alterar cliente nao implementada.\n");
}

void excluirClienteController(Sistema *sistema) {
    // Implemente a lógica de exclusão aqui
    printf("\nFuncao de excluir cliente nao implementada.\n");
}