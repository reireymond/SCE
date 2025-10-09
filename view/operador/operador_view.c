// Inclui o cabeçalho da view de operador, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "operador_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller de operador para poder chamar as funções que
// executam as ações (adicionarOperadorController, etc.).
#include "controller/operador/operador_controller.h"

// Implementação da função que exibe e gerencia o menu de operadores.
void menuOperadoresView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' garante que o menu seja exibido continuamente até que o
    // usuário escolha a opção 0 para voltar ao menu anterior.
    do {
        // Limpa a tela antes de exibir o menu para uma interface mais limpa.
        limpar_tela();
        printf("\n--- Menu Operadores do Sistema ---\n");
        printf("1. Adicionar Novo Operador\n");
        printf("2. Alterar Operador Existente\n");
        printf("3. Listar Todos os Operadores\n");
        printf("4. Excluir Operador\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
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
        // ler o resultado da operação antes de o menu ser exibido novamente.
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
    printf("\n--- Lista de Operadores ---\n");
    // Percorre o array de operadores.
    for (int i = 0; i < sistema->num_operadores; i++) {
        // Para cada operador, imprime seus dados de forma formatada.
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_operadores[i].codigo);
        printf("Nome: %s\n", sistema->lista_operadores[i].nome);
        printf("Usuario: %s\n", sistema->lista_operadores[i].usuario);
        // Por uma questão de segurança, a senha do operador não é exibida na listagem.
    }
    // Imprime uma linha final para fechar a formatação da lista.
    printf("----------------------------------\n");
}