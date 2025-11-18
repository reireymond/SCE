#include "cliente_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/cliente/cliente_model.h"
#include "view/cliente/cliente_view.h"
#include "utils/utils.h"
#include "utils/validation.h"

// Auxiliar para evitar IDs duplicados
static int obterProximoIdCliente(Sistema *sistema) {
    int max = 0;
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo > max) max = sistema->lista_clientes[i].codigo;
    }
    return max + 1;
}

void adicionarClienteController(Sistema *sistema) {
    if (sistema->num_clientes == sistema->capacidade_clientes) {
        int nova_capacidade = (sistema->capacidade_clientes == 0) ? 10 : sistema->capacidade_clientes * 2;
        Cliente *temp = realloc(sistema->lista_clientes, nova_capacidade * sizeof(Cliente));
        if (!temp) { printf("\nErro de memoria!\n"); return; }
        sistema->lista_clientes = temp;
        sistema->capacidade_clientes = nova_capacidade;
    }

    Cliente *novo_cliente = &sistema->lista_clientes[sistema->num_clientes];
    novo_cliente->codigo = obterProximoIdCliente(sistema); // CORREÇÃO DE ID

    printf("\n--- Cadastro de Novo Cliente (ID: %d) ---\n", novo_cliente->codigo);
    printf("Nome / Razao Social: "); ler_texto_valido(novo_cliente->razao_social, sizeof(novo_cliente->razao_social), VALIDAR_NAO_VAZIO);
    printf("CPF / CNPJ: "); ler_texto_valido(novo_cliente->cnpj, sizeof(novo_cliente->cnpj), VALIDAR_CPF_CNPJ);
    printf("Endereco Completo: "); ler_texto_valido(novo_cliente->endereco, sizeof(novo_cliente->endereco), VALIDAR_NAO_VAZIO);
    printf("Telefone: "); ler_texto_valido(novo_cliente->telefone, sizeof(novo_cliente->telefone), VALIDAR_TELEFONE);
    printf("E-mail: "); ler_texto_valido(novo_cliente->email, sizeof(novo_cliente->email), VALIDAR_EMAIL);
    printf("Nome do Contato: "); ler_texto_valido(novo_cliente->nome_do_contato, sizeof(novo_cliente->nome_do_contato), VALIDAR_NOME);

    sistema->num_clientes++;
    salvarClientes(sistema);
    printf("\nCliente cadastrado com sucesso!\n");
}

void alterarClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o ID do cliente para alterar: "); scanf("%d", &codigo); limpar_buffer();

    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { printf("\nCliente nao encontrado.\n"); return; }

    Cliente *c = &sistema->lista_clientes[indice];
    int opcao;
    do {
        limpar_tela();
        printf("--- Alterando Cliente: %s ---\n", c->razao_social);
        printf("1. Nome\n2. CPF/CNPJ\n3. Endereco\n4. Telefone\n5. Email\n6. Contato\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 6);

        switch (opcao) {
            case 1: printf("Novo Nome: "); ler_texto_valido(c->razao_social, sizeof(c->razao_social), VALIDAR_NAO_VAZIO); break;
            case 2: printf("Novo CPF/CNPJ: "); ler_texto_valido(c->cnpj, sizeof(c->cnpj), VALIDAR_CPF_CNPJ); break;
            case 3: printf("Novo Endereco: "); ler_texto_valido(c->endereco, sizeof(c->endereco), VALIDAR_NAO_VAZIO); break;
            case 4: printf("Novo Telefone: "); ler_texto_valido(c->telefone, sizeof(c->telefone), VALIDAR_TELEFONE); break;
            case 5: printf("Novo Email: "); ler_texto_valido(c->email, sizeof(c->email), VALIDAR_EMAIL); break;
            case 6: printf("Novo Contato: "); ler_texto_valido(c->nome_do_contato, sizeof(c->nome_do_contato), VALIDAR_NOME); break;
            case 0: printf("\nSalvo!\n"); break;
        }
    } while (opcao != 0);
    salvarClientes(sistema);
}

void excluirClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo;
    printf("\nDigite o ID do cliente para excluir: "); scanf("%d", &codigo); limpar_buffer();

    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            char conf;
            printf("Excluir %s? (s/n): ", sistema->lista_clientes[i].razao_social);
            scanf(" %c", &conf); limpar_buffer();
            if (conf == 's' || conf == 'S') {
                sistema->lista_clientes[i] = sistema->lista_clientes[sistema->num_clientes - 1];
                sistema->num_clientes--;
                salvarClientes(sistema);
                printf("\nCliente excluido!\n");
            } else { printf("\nCancelado.\n"); }
            return;
        }
    }
    printf("\nNao encontrado.\n");
}