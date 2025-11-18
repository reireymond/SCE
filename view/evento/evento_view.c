#include "evento_view.h"
#include <stdio.h>
#include "utils/utils.h"
#include "controller/evento/evento_controller.h"

void menuEventosView(Sistema *sistema) {
    int opcao;
    do {
        limpar_tela();
        printf("+=========================================+\n");
        printf("|       GESTAO DE EVENTOS                 |\n");
        printf("+=========================================+\n");
        printf("| [1] Criar Novo Orcamento                |\n");
        printf("| [2] Aprovar Evento                      |\n");
        printf("| [3] Finalizar Evento                    |\n");
        printf("| [4] Listar Eventos                      |\n");
        printf("| [0] Voltar                              |\n");
        printf("+=========================================+\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionarEventoController(sistema); break;
            case 2: alterarStatusEventoController(sistema); break;
            case 3: finalizarEventoController(sistema); break;
            case 4: listarEventosView(sistema); break;
            case 0: break;
            default: printf("Invalido!\n"); break;
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

void listarEventosView(Sistema *sistema) {
    if (sistema->num_eventos == 0) {
        printf("Nenhum evento.\n"); return;
    }
    printf("\n--- Lista de Eventos ---\n");
    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        char st[20];
        if(e->status == 0) sprintf(st, "Orcamento");
        else if(e->status == 1) sprintf(st, "Aprovado");
        else sprintf(st, "Finalizado");
        
        printf("#%d | %s | %s | %s\n", e->codigo, e->nome_evento, e->data_inicio, st);
    }
}