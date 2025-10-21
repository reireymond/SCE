// Inclui o cabeçalho da view de cliente, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "cliente_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela', 'limpar_buffer' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller de cliente para poder chamar as funções que executam as ações
// (adicionarClienteController, alterarClienteController, etc.).
#include "controller/cliente/cliente_controller.h"

// Implementação da função que exibe e gerencia o menu de clientes.
void menuClientesView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' garante que o menu seja exibido pelo menos uma vez e continue
    // sendo exibido até que o usuário escolha a opção 0 para sair.
    do {
        // Limpa a tela antes de exibir o menu para uma interface mais limpa.
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




        // Lê a opção do usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para remover o '\n' deixado pelo scanf.
        limpar_buffer();

        // Um 'switch' é usado para tratar a opção escolhida pelo usuário.
        switch (opcao) {
            // Cada caso chama a função do controller correspondente à ação desejada.
            case 1: adicionarClienteController(sistema); break;
            case 2: alterarClienteController(sistema); break;
            case 3: listarClientesView(sistema); break;
            case 4: excluirClienteController(sistema); break;
            case 0: break; // A opção 0 simplesmente sai do loop.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), pausa a tela para que o usuário possa ver
        // o resultado da operação antes de o menu ser exibido novamente.
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Implementação da função que exibe a lista de clientes.
void listarClientesView(Sistema *sistema) {
    // Verifica se existem clientes cadastrados. Se a lista estiver vazia, exibe uma mensagem.
    if (sistema->num_clientes == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return; // Retorna para a função anterior.
    }
    // Imprime o cabeçalho da lista.
    printf("+=====================================================================+\n");
    printf("|                      Lista de Clientes                              |\n");
    // Percorre o array de clientes do início ao fim.
    for (int i = 0; i < sistema->num_clientes; i++) {
        // Para cada cliente, imprime seus dados de forma formatada e legível.
        printf("+=====================================================================+\n");
        printf("| Codigo: %d\n", sistema->lista_clientes[i].codigo);
        printf("| Nome / Razao Social: %s\n", sistema->lista_clientes[i].razao_social);
        printf("| CPF / CNPJ: %s\n", sistema->lista_clientes[i].cnpj);
        printf("| Endereco: %s\n", sistema->lista_clientes[i].endereco);
        printf("| Telefone: %s\n", sistema->lista_clientes[i].telefone);
        printf("| E-mail: %s\n", sistema->lista_clientes[i].email);
        printf("| Contato: %s\n", sistema->lista_clientes[i].nome_do_contato);
    }
        printf("+=====================================================================\n");

    // Imprime uma linha final para fechar a formatação da lista.
   
}