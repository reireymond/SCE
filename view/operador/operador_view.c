// Inclui o cabeçalho da view de operador, que contém as declarações das funções
#include "operador_view.h"
#include <stdio.h>
#include "utils/utils.h"
// Inclui o cabeçalho do controller de operador para poder chamar as funções que
#include "controller/operador/operador_controller.h"

// Implementação da função que exibe e gerencia o menu de operadores.
void menuOperadoresView(Sistema *sistema) {
    int opcao;
   
    do {
        // Limpa a tela antes de exibir o menu para uma interface mais limpa.
       limpar_tela();
        printf("+=====================================================+\n");
        printf("|             MENU OPERADORES DO SISTEMA              |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Operador                         |\n");
        printf("| [2] Alterar Operador Existente                      |\n");
        printf("| [3] Listar Todos os Operadores                      |\n");
        printf("| [4] Excluir Operador                                |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        // Lê a opção digitada pelo usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para remover o '\n' deixado pelo scanf.
        limpar_buffer();

        // Um 'switch' direciona o fluxo do programa com base na opção do usuário.
        switch (opcao) {
            // Cada caso chama a função do controller correspondente para executar a ação.
            case 1: adicionarOperadorController(sistema); break;
            case 2: alterarOperadorController(sistema); break;
            case 3: listarOperadoresView(sistema); break;
            case 4: excluirOperadorController(sistema); break;
            case 0: break; // A opção 0 simplesmente encerra o loop.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), pausa a execução para que o usuário possa
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Implementação da função que exibe a lista de operadores.
void listarOperadoresView(Sistema *sistema) {
    // Verifica se há operadores cadastrados. Se não houver, exibe uma mensagem.
    if (sistema->num_operadores == 0) {
        printf("\nNenhum operador cadastrado.\n");
        return; // Retorna para a função anterior (o menu).
    }
    // Imprime o cabeçalho da lista.
printf("+=====================================================+\n");
printf("|               LISTA DE OPERADORES                   |\n");
printf("+=====================================================+\n");

if (sistema->num_operadores == 0) {
    printf("| Nenhum operador cadastrado.                        |\n");
} else {
    for (int i = 0; i < sistema->num_operadores; i++) {
        printf("+-----------------------------------------------------+\n");
        printf("| Codigo : %-42d \n", sistema->lista_operadores[i].codigo);
        printf("| Nome   : %-42s \n", sistema->lista_operadores[i].nome);
        printf("| Usuario: %-42s \n", sistema->lista_operadores[i].usuario);
    }
}

        printf("+=====================================================+\n");

}