#include "evento_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/evento/evento_controller.h"

void menuEventosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=====================================================+\n");
        printf("|           GESTAO DE EVENTOS E ORCAMENTOS            |\n");
        printf("+=====================================================+\n");
        printf("| [1] Criar Novo Orcamento                            |\n");
        printf("| [2] Aprovar Evento (Verificar Estoque)              |\n");
        printf("| [3] Finalizar Evento (Faturamento)                  |\n");
        printf("| [4] Listar Eventos                                  |\n");
        printf("+-----------------------------------------------------+\n");
        printf("| [0] Voltar                                          |\n");
        printf("+=====================================================+\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarEventoController(sistema); break;
            case 2: alterarStatusEventoController(sistema); break;
            case 3: finalizarEventoController(sistema); break;
            case 4: listarEventosView(sistema); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarEventosView(Sistema *sistema) {
    if (sistema->num_eventos == 0) {
        printf("Nenhum evento cadastrado.\n"); 
        return;
    }
    printf("\n--- Lista de Eventos ---\n");
    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        char status_texto[20];
        if(e->status == 0) sprintf(status_texto, "Orcamento");
        else if(e->status == 1) sprintf(status_texto, "Aprovado");
        else sprintf(status_texto, "Finalizado");
        
        printf("#%d | %s | Inicio: %s | Status: %s\n", 
               e->codigo, e->nome_evento, e->data_inicio, status_texto);
    }
}