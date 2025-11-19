#include "operador_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/operador/operador_controller.h"

void menuOperadoresView(Sistema *sistema) {
    int opcao;
    do {
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
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarOperadorController(sistema); break;
            case 2: alterarOperadorController(sistema); break;
            case 3: listarOperadoresView(sistema); break;
            case 4: excluirOperadorController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarOperadoresView(Sistema *sistema) {
    if (sistema->num_operadores == 0) {
        printf("\nNenhum operador cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Operadores ---\n");
    for (int i = 0; i < sistema->num_operadores; i++) {
        printf("ID: %d | Nome: %s | Usuario: %s\n", 
               sistema->lista_operadores[i].codigo,
               sistema->lista_operadores[i].nome,
               sistema->lista_operadores[i].usuario);
    }
}