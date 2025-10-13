// Inclui o cabeçalho da view de recurso, que contém as declarações das funções
// que serão implementadas neste arquivo.
#include "recurso_view.h"
// Inclui a biblioteca padrão de entrada/saída para usar funções como 'printf' e 'scanf'.
#include <stdio.h>
// Inclui o cabeçalho de utilitários para usar funções como 'limpar_tela' e 'pausar'.
#include "utils/utils.h"
// Inclui o cabeçalho do controller de recurso para poder chamar as funções que executam
// as ações (adicionarRecursoController, etc.).
#include "controller/recurso/recurso_controller.h"

// Implementação da função que exibe e gerencia o menu de recursos e equipamentos.
void menuRecursosView(Sistema *sistema) {
    int opcao;
    // O loop 'do-while' garante que o menu seja exibido continuamente até que o
    // usuário escolha a opção 0 para voltar ao menu anterior.
    do {
        // Limpa a tela antes de cada exibição do menu para uma interface mais limpa.
        limpar_tela();
        printf("\n--- Menu Recursos e Equipamentos ---\n");
        printf("1. Adicionar Novo Recurso\n");
        printf("2. Alterar Recurso Existente\n");
        printf("3. Listar Todos os Recursos\n");
        printf("4. Excluir Recurso\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        // Lê a opção digitada pelo usuário.
        scanf("%d", &opcao);
        // Limpa o buffer de entrada para remover o '\n' deixado pelo scanf.
        limpar_buffer();

        // Um 'switch' direciona o fluxo do programa com base na opção do usuário.
        switch (opcao) {
            // Cada caso chama a função do controller correspondente para executar a ação.
            case 1: adicionarRecursoController(sistema); break;
            case 2: alterarRecursoController(sistema); break;
            case 3: listarRecursosView(sistema); break;
            case 4: excluirRecursoController(sistema); break;
            case 0: break; // A opção 0 simplesmente encerra o loop.
            default: printf("\nOpcao invalida!\n"); break;
        }
        // Se a opção não for 0 (sair), o programa pausa, aguardando que o usuário
        // pressione Enter para continuar.
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// Implementação da função que exibe a lista de recursos e equipamentos.
void listarRecursosView(Sistema *sistema) {
    // Verifica se há recursos cadastrados. Se não houver, exibe uma mensagem.
    if (sistema->num_recursos == 0) {
        printf("\nNenhum recurso cadastrado.\n");
        return; // Retorna para a função anterior (o menu).
    }
    // Imprime o cabeçalho da lista.
    printf("\n--- Lista de Recursos e Equipamentos ---\n");
    // Percorre o array de recursos.
    for (int i = 0; i < sistema->num_recursos; i++) {
        // Para cada recurso, imprime seus dados de forma formatada.
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_recursos[i].codigo);
        printf("Descricao: %s\n", sistema->lista_recursos[i].descricao);
        printf("Categoria: %s\n", sistema->lista_recursos[i].categoria);
        printf("Estoque: %d unid.\n", sistema->lista_recursos[i].quantidade_estoque);
        // Para os valores float, '%.2f' formata o número para exibir duas casas decimais.
        printf("Preco de Custo: R$ %.2f\n", sistema->lista_recursos[i].preco_custo);
        printf("Valor Locacao (diaria): R$ %.2f\n", sistema->lista_recursos[i].valor_locacao);
    }

    printf("----------------------------------\n");
}