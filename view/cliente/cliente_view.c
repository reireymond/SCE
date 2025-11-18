#include "cliente_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/cliente/cliente_controller.h"

// Menu de clientes
void menuClientesView(Sistema *sistema) {
    int opcao;
   
    do {
        // Limpa a tela antes
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




        // Le a opcao
        scanf("%d", &opcao);
        // Limpa o buffer
        limpar_buffer();

        // Escolhe o que fazer
        switch (opcao) {
            // Chama a funcao correspondente
            case 1: adicionarClienteController(sistema); break;
            case 2: alterarClienteController(sistema); break;
            case 3: listarClientesView(sistema); break;
            case 4: excluirClienteController(sistema); break;
            case 0: break; // Sai do menu
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se nao saiu, pausa pra ver o resultado
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Lista os clientes
void listarClientesView(Sistema *sistema) {
    // Verifica se tem algum cliente
    if (sistema->num_clientes == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }
    // Mostra o cabecalho
    printf("+=====================================================================+\n");
    printf("|                      Lista de Clientes                              |\n");
    // Mostra cada cliente
    for (int i = 0; i < sistema->num_clientes; i++) {
        // Mostra os dados de cada cliente
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

   
}