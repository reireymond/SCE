#include "recurso_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/recurso/recurso_controller.h"

void menuRecursosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("\n--- Menu Recursos e Equipamentos ---\n");
        printf("1. Adicionar Novo Recurso\n");
        printf("2. Alterar Recurso Existente\n");
        printf("3. Listar Todos os Recursos\n");
        printf("4. Excluir Recurso\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarRecursoController(sistema); break;
            case 2: alterarRecursoController(sistema); break;
            case 3: listarRecursosView(sistema); break;
            case 4: excluirRecursoController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarRecursosView(Sistema *sistema) {
    if (sistema->num_recursos == 0) {
        printf("\nNenhum recurso cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Recursos e Equipamentos ---\n");
    for (int i = 0; i < sistema->num_recursos; i++) {
        printf("----------------------------------\n");
        printf("Codigo: %d\n", sistema->lista_recursos[i].codigo);
        printf("Descricao: %s\n", sistema->lista_recursos[i].descricao);
        printf("Categoria: %s\n", sistema->lista_recursos[i].categoria);
        printf("Estoque: %d unid.\n", sistema->lista_recursos[i].quantidade_estoque);
        printf("Preco de Custo: R$ %.2f\n", sistema->lista_recursos[i].preco_custo);
        printf("Valor Locacao (diaria): R$ %.2f\n", sistema->lista_recursos[i].valor_locacao);
    }
    printf("----------------------------------\n");
}