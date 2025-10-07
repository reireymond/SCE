#include "cliente_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/cliente/cliente_model.h"
#include "view/cliente/cliente_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

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

    Cliente *novo_cliente = &sistema->lista_clientes[sistema->num_clientes];
    novo_cliente->codigo = sistema->num_clientes + 1;

    printf("\n--- Cadastro de Novo Cliente (Codigo: %d) ---\n", novo_cliente->codigo);
    printf("Nome / Razao Social: ");
    ler_string_valida(novo_cliente->razao_social, sizeof(novo_cliente->razao_social), VALIDATE_NOT_EMPTY);

    printf("CPF / CNPJ: ");
    ler_string_valida(novo_cliente->cnpj, sizeof(novo_cliente->cnpj), VALIDATE_CPF_CNPJ);

    printf("Endereco Completo: ");
    ler_string_valida(novo_cliente->endereco, sizeof(novo_cliente->endereco), VALIDATE_NOT_EMPTY);

    printf("Telefone: ");
    ler_string_valida(novo_cliente->telefone, sizeof(novo_cliente->telefone), VALIDATE_NOT_EMPTY);

    printf("E-mail: ");
    ler_string_valida(novo_cliente->email, sizeof(novo_cliente->email), VALIDATE_EMAIL);
    
    printf("Nome do Contato: ");
    ler_string_valida(novo_cliente->nome_do_contato, sizeof(novo_cliente->nome_do_contato), VALIDATE_NOT_EMPTY);

    sistema->num_clientes++;

    salvarClientes(sistema);
    printf("\nCliente '%s' cadastrado com sucesso!\n", novo_cliente->razao_social);
}

void alterarClienteController(Sistema *sistema) {
    listarClientesView(sistema); // A controller chama a view para exibir os dados
    if (sistema->num_clientes == 0) return;

    int codigo, indice_cliente = -1;
    printf("\nDigite o codigo do cliente que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            indice_cliente = i;
            break;
        }
    }

    if (indice_cliente == -1) {
        printf("\nCliente com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Cliente *cliente = &sistema->lista_clientes[indice_cliente];
    int opcao;
    do {
        limpar_tela();
        printf("--- Alterando Cliente: %s ---\n\n", cliente->razao_social);
        printf("Qual campo deseja alterar?\n");
        printf("1. Nome / Razao Social\n2. CPF / CNPJ\n3. Endereco\n4. Telefone\n5. E-mail\n6. Nome do Contato\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
    case 1:
        printf("Novo Nome / Razao Social: ");
        fgets(cliente->razao_social, sizeof(cliente->razao_social), stdin);
        cliente->razao_social[strcspn(cliente->razao_social, "\n")] = 0;
        break;
    case 2:
        printf("Novo CPF / CNPJ: ");
        fgets(cliente->cnpj, sizeof(cliente->cnpj), stdin);
        cliente->cnpj[strcspn(cliente->cnpj, "\n")] = 0;
        break;
    case 3:
        printf("Novo Endereco Completo: ");
        fgets(cliente->endereco, sizeof(cliente->endereco), stdin);
        cliente->endereco[strcspn(cliente->endereco, "\n")] = 0;
        break;
    case 4:
        printf("Novo Telefone: ");
        fgets(cliente->telefone, sizeof(cliente->telefone), stdin);
        cliente->telefone[strcspn(cliente->telefone, "\n")] = 0;
        break;
    case 5:
        printf("Novo E-mail: ");
        fgets(cliente->email, sizeof(cliente->email), stdin);
        cliente->email[strcspn(cliente->email, "\n")] = 0;
        break;
    case 6:
        printf("Novo Nome do Contato: ");
        fgets(cliente->nome_do_contato, sizeof(cliente->nome_do_contato), stdin);
        cliente->nome_do_contato[strcspn(cliente->nome_do_contato, "\n")] = 0;
        break;
    case 0:
        printf("\nAlteracoes salvas!\n");
        break;
    default:
        printf("\nOpcao invalida!\n");
        pausar();
        break;
}
    } while (opcao != 0);

    salvarClientes(sistema);
}

// *** FUNÇÃO COMPLETADA ***
void excluirClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo;
    printf("\nDigite o codigo do cliente que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o cliente %s? (s/n): ", sistema->lista_clientes[i].razao_social);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Move o último cliente da lista para a posição do excluído
                sistema->lista_clientes[i] = sistema->lista_clientes[sistema->num_clientes - 1];
                sistema->num_clientes--;
                printf("\nCliente excluido com sucesso!\n");
                salvarClientes(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }
    printf("\nCliente com codigo %d nao encontrado.\n", codigo);
}