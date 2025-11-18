#include "view/evento/evento_view.h"
#include <stdio.h>
#include "utils/utils.h"
// Inclui o controller para chamar as ações 
#include "controller/evento/evento_controller.h"

void menuEventosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|          MENU ORCAMENTOS E GESTAO DE EVENTOS        |\n");
        printf("+=====================================================+\n");
        printf("| [1] Criar Novo Orcamento                            |\n");
        printf("| [2] Aprovar Orcamento (Iniciar Evento)              |\n");
        printf("| [3] Finalizar e Faturar Evento                      |\n");
        printf("| [4] Ver Detalhes do Evento (Itens/Custos)           |\n");
        printf("| [5] Listar Todos os Eventos                         |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: 
                adicionarEventoController(sistema); 
                break;
            case 2: 
                alterarStatusEventoController(sistema); 
                break;
            case 3: 
                finalizarEventoController(sistema); 
                break;
            case 4: 
                detalharEventoController(sistema); 
                break;
            case 5: 
                listarEventosView(sistema); 
                break;
            case 0: 
                break;
            default: 
                printf("\nOpcao invalida!\n"); 
                break;
        }

        if (opcao != 0) pausar();
        
    } while (opcao != 0);
}

void listarEventosView(Sistema *sistema) {
    if (sistema->num_eventos == 0) {
        printf("\nNenhum evento ou orcamento cadastrado.\n");
        return;
    }

    printf("+==========================================================================================+\n");
    printf("|                                   LISTA DE EVENTOS                                       |\n");
    printf("+==========================================================================================+\n");
    printf("| Cod  | Nome do Evento                 | Data Ini.  | Status       | Cliente (ID)         |\n");
    printf("+------+--------------------------------+------------+--------------+----------------------+\n");

    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        
        // Converte o Enum de status para uma string legível
        char statusStr[20];
        switch(e->status) {
            case ORCAMENTO: sprintf(statusStr, "Orcamento"); break;
            case APROVADO: sprintf(statusStr, "Aprovado"); break;
            case FINALIZADO: sprintf(statusStr, "Finalizado"); break;
            default: sprintf(statusStr, "Desconhecido"); break;
        }

        // Exibe os dados formatados em colunas
      
        printf("| %03d  | %-30s | %-10s | %-12s | %-20d |\n", 
               e->codigo, 
               e->nome_evento, 
               e->data_inicio, 
               statusStr, 
               e->codigo_cliente);
    }

    printf("+==========================================================================================+\n");
}