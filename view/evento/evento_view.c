#include "view/evento/evento_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/evento/evento_controller.h"

void menuEventosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|          MENU ORCAMENTOS E GESTAO DE EVENTOS        |\n");
        printf("+=====================================================+\n");
        printf("| [1] Criar Novo Orcamento                            |\n");
        printf("| [2] Editar Orcamento                                |\n");
        printf("| [3] Aprovar Orcamento (Iniciar Evento)              |\n");
        printf("| [4] Finalizar e Faturar Evento                      |\n");
        printf("| [5] Ver Detalhes do Evento                          |\n");
        printf("| [6] Listar Todos os Eventos                         |\n");
        printf("| [7] Cancelar Evento                                 |\n"); // NOVO
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarEventoController(sistema); break;
            case 2: editarEventoController(sistema); break;
            case 3: alterarStatusEventoController(sistema); break;
            case 4: finalizarEventoController(sistema); break;
            case 5: detalharEventoController(sistema); break;
            case 6: listarEventosView(sistema); pausar(); break;
            case 7: cancelarEventoController(sistema); break; // NOVO
            case 0: break;
            default: printf("\nOpcao invalida!\n"); break;
        }
        if (opcao != 0 && opcao != 6) pausar();
    } while (opcao != 0);
}

void listarEventosView(Sistema *sistema) {
    if (sistema->num_eventos == 0) {
        printf("\nNenhum evento cadastrado.\n");
        return;
    }
    printf("+==========================================================================================+\n");
    printf("|                                   LISTA DE EVENTOS                                       |\n");
    printf("+==========================================================================================+\n");
    printf("| Cod  | Nome do Evento                 | Data Ini.  | Status       | Cliente (ID)         |\n");
    printf("+------+--------------------------------+------------+--------------+----------------------+\n");

    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        char statusStr[20];
        switch(e->status) {
            case ORCAMENTO: sprintf(statusStr, "Orcamento"); break;
            case APROVADO: sprintf(statusStr, "Aprovado"); break;
            case FINALIZADO: sprintf(statusStr, "Finalizado"); break;
            case CANCELADO: sprintf(statusStr, "Cancelado"); break;
            default: sprintf(statusStr, "Erro"); break;
        }
        printf("| %03d  | %-30s | %-10s | %-12s | %-20d |\n", 
               e->codigo, e->nome_evento, e->data_inicio, statusStr, e->codigo_cliente);
    }
    printf("+==========================================================================================+\n");
}