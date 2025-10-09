// Inclui o cabeçalho do controller de cliente, que contém as declarações das funções
// implementadas neste arquivo.
#include "cliente_controller.h"
// Inclui cabeçalhos padrão do C para entrada/saída (stdio), alocação de memória (stdlib)
// e manipulação de strings (string).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Inclui o cabeçalho do model de cliente para poder chamar a função 'salvarClientes'.
#include "model/cliente/cliente_model.h"
// Inclui o cabeçalho da view de cliente para poder chamar a função 'listarClientesView'.
#include "view/cliente/cliente_view.h"
// Inclui o cabeçalho de utilitários para funções como 'limpar_buffer' e 'limpar_tela'.
#include "utils/utils.h"
// Inclui o cabeçalho de validação para funções como 'ler_string_valida'.
#include "utils/validation.h"

// Função para adicionar um novo cliente ao sistema.
void adicionarClienteController(Sistema *sistema) {
    // Verifica se o número de clientes atingiu a capacidade máxima do array.
    if (sistema->num_clientes == sistema->capacidade_clientes) {
        // Se a capacidade for 0, define uma capacidade inicial; senão, dobra a capacidade atual.
        int nova_capacidade = (sistema->capacidade_clientes == 0) ? 10 : sistema->capacidade_clientes * 2;
        // Tenta realocar a memória para o novo tamanho. realloc preserva os dados existentes.
        Cliente *temp = realloc(sistema->lista_clientes, nova_capacidade * sizeof(Cliente));
        // Se a realocação falhar (retornar NULL), informa o erro e encerra a função.
        if (!temp) {
            printf("\nErro de alocacao de memoria!\n");
            return;
        }
        // Atualiza o ponteiro da lista e a capacidade no sistema.
        sistema->lista_clientes = temp;
        sistema->capacidade_clientes = nova_capacidade;
    }

    // Pega o endereço da próxima posição livre na lista para o novo cliente.
    Cliente *novo_cliente = &sistema->lista_clientes[sistema->num_clientes];
    // Define o código do novo cliente com base na sua posição (incrementando em 1).
    novo_cliente->codigo = sistema->num_clientes + 1;

    // Exibe o cabeçalho para o cadastro do novo cliente.
    printf("\n--- Cadastro de Novo Cliente (Codigo: %d) ---\n", novo_cliente->codigo);

    // Solicita e lê cada informação do cliente, usando as funções de validação.
    // Isso garante que os dados inseridos pelo usuário sejam consistentes e corretos.
    printf("Nome / Razao Social: ");
    ler_string_valida(novo_cliente->razao_social, sizeof(novo_cliente->razao_social), VALIDATE_NOT_EMPTY);

    printf("CPF / CNPJ: ");
    ler_string_valida(novo_cliente->cnpj, sizeof(novo_cliente->cnpj), VALIDATE_CPF_CNPJ);

    printf("Endereco Completo: ");
    ler_string_valida(novo_cliente->endereco, sizeof(novo_cliente->endereco), VALIDATE_NOT_EMPTY);

    printf("Telefone: ");
    ler_string_valida(novo_cliente->telefone, sizeof(novo_cliente->telefone), VALIDATE_PHONE);

    printf("E-mail: ");
    ler_string_valida(novo_cliente->email, sizeof(novo_cliente->email), VALIDATE_EMAIL);
    
    printf("Nome do Contato: ");
    ler_string_valida(novo_cliente->nome_do_contato, sizeof(novo_cliente->nome_do_contato), VALIDATE_NAME);

    // Incrementa o contador de clientes no sistema.
    sistema->num_clientes++;

    // Salva a lista de clientes (agora com o novo cliente) no arquivo.
    salvarClientes(sistema);
    printf("\nCliente '%s' cadastrado com sucesso!\n", novo_cliente->razao_social);
}

// Função para alterar os dados de um cliente já existente.
void alterarClienteController(Sistema *sistema) {
    // Primeiro, exibe a lista de todos os clientes para que o usuário saiba qual código escolher.
    listarClientesView(sistema);
    // Se não houver clientes cadastrados, exibe a mensagem e retorna.
    if (sistema->num_clientes == 0) return;

    int codigo, indice_cliente = -1;
    printf("\nDigite o codigo do cliente que deseja alterar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o cliente na lista pelo código informado.
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            indice_cliente = i; // Armazena o índice do cliente encontrado.
            break; // Interrompe o loop, pois o cliente já foi encontrado.
        }
    }

    // Se o cliente não foi encontrado (índice continua -1), informa o usuário e retorna.
    if (indice_cliente == -1) {
        printf("\nCliente com codigo %d nao encontrado.\n", codigo);
        return;
    }

    // Pega o endereço do cliente que será alterado para facilitar a manipulação.
    Cliente *cliente = &sistema->lista_clientes[indice_cliente];
    int opcao;
    // Loop do menu de alteração.
    do {
        limpar_tela();
        printf("--- Alterando Cliente: %s ---\n\n", cliente->razao_social);
        printf("Qual campo deseja alterar?\n");
        printf("1. Nome / Razao Social\n2. CPF / CNPJ\n3. Endereco\n4. Telefone\n5. E-mail\n6. Nome do Contato\n");
        printf("0. Salvar e Voltar\n");
        printf("Escolha: ");
        // Lê a opção do usuário, garantindo que seja um número válido entre 0 e 6.
        ler_int_valido(&opcao, 0, 6);

        // Processa a opção escolhida.
        switch (opcao) {
            case 1:
                printf("Novo Nome / Razao Social: ");
                ler_string_valida(cliente->razao_social, sizeof(cliente->razao_social), VALIDATE_NOT_EMPTY);
                break;
            case 2:
                printf("Novo CPF / CNPJ: ");
                ler_string_valida(cliente->cnpj, sizeof(cliente->cnpj), VALIDATE_CPF_CNPJ);
                break;
            case 3:
                printf("Novo Endereco Completo: ");
                ler_string_valida(cliente->endereco, sizeof(cliente->endereco), VALIDATE_NOT_EMPTY);
                break;
            case 4:
                printf("Novo Telefone: ");
                ler_string_valida(cliente->telefone, sizeof(cliente->telefone), VALIDATE_PHONE);
                break;
            case 5:
                printf("Novo E-mail: ");
                ler_string_valida(cliente->email, sizeof(cliente->email), VALIDATE_EMAIL);
                break;
            case 6:
                printf("Novo Nome do Contato: ");
                ler_string_valida(cliente->nome_do_contato, sizeof(cliente->nome_do_contato), VALIDATE_NAME);
                break;
            case 0:
                printf("\nAlteracoes salvas!\n");
                break;
        }
    } while (opcao != 0);

    // Após o usuário sair do menu, salva todas as alterações no arquivo.
    salvarClientes(sistema);
}

// Função para excluir um cliente do sistema.
void excluirClienteController(Sistema *sistema) {
    // Exibe a lista de clientes para o usuário.
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo;
    printf("\nDigite o codigo do cliente que deseja excluir: ");
    scanf("%d", &codigo);
    limpar_buffer();

    // Procura o cliente pelo código.
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            char confirmacao;
            // Pede confirmação para evitar exclusões acidentais.
            printf("Tem certeza que deseja excluir o cliente %s? (s/n): ", sistema->lista_clientes[i].razao_social);
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                // Para remover o cliente, sobrescrevemos seus dados com os dados do último cliente da lista.
                // Esta é uma estratégia eficiente para remoção em arrays dinâmicos, pois evita ter que deslocar todos os elementos.
                sistema->lista_clientes[i] = sistema->lista_clientes[sistema->num_clientes - 1];
                // Decrementa o número total de clientes.
                sistema->num_clientes--;
                printf("\nCliente excluido com sucesso!\n");
                // Salva o estado atual da lista no arquivo.
                salvarClientes(sistema);
            } else {
                printf("\nExclusao cancelada.\n");
            }
            // Retorna da função, pois a operação (exclusão ou cancelamento) foi concluída.
            return;
        }
    }
    // Se o loop terminar sem encontrar o cliente, informa o usuário.
    printf("\nCliente com codigo %d nao encontrado.\n", codigo);
}