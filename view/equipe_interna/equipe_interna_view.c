#include "equipe_interna_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/equipe_interna/equipe_interna_controller.h"

void menuEquipeInternaView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|                MENU EQUIPE INTERNA                  |\n");
        printf("+=====================================================+\n");
        printf("| [1] Adicionar Novo Membro                           |\n");
        printf("| [2] Alterar Membro Existente                        |\n");
        printf("| [3] Listar Todos os Membros                         |\n");
        printf("| [4] Excluir Membro                                  |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarEquipeController(sistema); break;
            case 2: alterarEquipeController(sistema); break;
            case 3: listarEquipeInternaView(sistema); break;
            case 4: excluirEquipeController(sistema); break;
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarEquipeInternaView(Sistema *sistema) {
    if (sistema->num_equipe == 0) {
        printf("\nNenhum membro cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Equipe ---\n");
    for (int i = 0; i < sistema->num_equipe; i++) {
        printf("ID: %d | Nome: %s | Funcao: %s | Diaria: R$ %.2f\n", 
               sistema->lista_equipe[i].codigo,
               sistema->lista_equipe[i].nome,
               sistema->lista_equipe[i].funcao,
               sistema->lista_equipe[i].valor_diaria);
    }
}