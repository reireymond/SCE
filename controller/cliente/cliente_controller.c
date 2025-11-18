#include "cliente_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/cliente/cliente_model.h"
#include "view/cliente/cliente_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Função que adiciona um novo cliente ao sistema
void adicionarClienteController(Sistema *sistema) {
    // Expande a lista se atingir a capacidade máxima
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

    // Posição do novo cliente
    Cliente *novo_cliente = &sistema->lista_clientes[sistema->num_clientes];
    novo_cliente->codigo = sistema->num_clientes + 1;

    printf("\n--- Cadastro de Novo Cliente (Codigo: %d) ---\n", novo_cliente->codigo);

    // Leitura dos dados com validação
    printf("Nome / Razao Social: ");
    ler_texto_valido(novo_cliente->razao_social, sizeof(novo_cliente->razao_social), VALIDAR_NAO_VAZIO);

    printf("CPF / CNPJ: ");
    ler_texto_valido(novo_cliente->cnpj, sizeof(novo_cliente->cnpj), VALIDAR_CPF_CNPJ);

    printf("Endereco Completo: ");
    ler_texto_valido(novo_cliente->endereco, sizeof(novo_cliente->endereco), VALIDAR_NAO_VAZIO);

    printf("Telefone: ");
    ler_texto_valido(novo_cliente->telefone, sizeof(novo_cliente->telefone), VALIDAR_TELEFONE);

    printf("E-mail: ");
    ler_texto_valido(novo_cliente->email, sizeof(novo_cliente->email), VALIDAR_EMAIL);

    printf("Nome do Contato: ");
    ler_texto_valido(novo_cliente->nome_do_contato, sizeof(novo_cliente->nome_do_contato), VALIDAR_NOME);

    // Incrementa e salva
    sistema->num_clientes++;
    salvarClientes(sistema);
    printf("\nCliente '%s' cadastrado com sucesso!\n", novo_cliente->razao_social);
}

// Função para alterar os dados de um cliente existente
void alterarClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo, indice_cliente = -1;
    printf("\nDigite o codigo do cliente que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o cliente pelo código
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

    // Menu de alteração
    do {
        limpar_tela();
        printf("--- Alterando Cliente: %s ---\n\n", cliente->razao_social);
        printf("1. Nome / Razao Social\n2. CPF / CNPJ\n3. Endereco\n4. Telefone\n5. E-mail\n6. Nome do Contato\n");
        printf("0. Salvar e Voltar\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 6);

        switch (opcao) {
            case 1:
                printf("Novo Nome / Razao Social: ");
                ler_texto_valido(cliente->razao_social, sizeof(cliente->razao_social), VALIDAR_NAO_VAZIO);
                break;
            case 2:
                printf("Novo CPF / CNPJ: ");
                ler_texto_valido(cliente->cnpj, sizeof(cliente->cnpj), VALIDAR_CPF_CNPJ);
                break;
            case 3:
                printf("Novo Endereco: ");
                ler_texto_valido(cliente->endereco, sizeof(cliente->endereco), VALIDAR_NAO_VAZIO);
                break;
            case 4:
                printf("Novo Telefone: ");
                ler_texto_valido(cliente->telefone, sizeof(cliente->telefone), VALIDAR_TELEFONE);
                break;
            case 5:
                printf("Novo E-mail: ");
                ler_texto_valido(cliente->email, sizeof(cliente->email), VALIDAR_EMAIL);
                break;
            case 6:
                printf("Novo Nome do Contato: ");
                ler_texto_valido(cliente->nome_do_contato, sizeof(cliente->nome_do_contato), VALIDAR_NOME);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    salvarClientes(sistema);
}

// Função para excluir um cliente
void excluirClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo;
    printf("\nDigite o codigo do cliente que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Busca o cliente pelo código
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            char confirmacao;
            printf("Tem certeza que deseja excluir o cliente %s? (s/n): ", sistema->lista_clientes[i].razao_social);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Substitui o cliente pelo último e reduz o total
                sistema->lista_clientes[i] = sistema->lista_clientes[sistema->num_clientes - 1];
                sistema->num_clientes--;
                salvarClientes(sistema);
                printf("\nCliente excluido com sucesso!\n");
            } else {
                printf("\nExclusao cancelada.\n");
            }
            return;
        }
    }

    printf("\nCliente com codigo %d nao encontrado.\n", codigo);
}
