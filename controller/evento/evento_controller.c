// Inclui o cabeçalho do controller, com as declarações das funções deste módulo
#include "evento_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/evento/evento_model.h"
#include "view/evento/evento_view.h"
#include "utils/utils.h"
#include "utils/validation.h"
#include "model/sistema.h"
#include "view/cliente/cliente_view.h"


// Adiciona um novo evento (equivalente a "Criar Orçamento")
void adicionarEventoController(Sistema *sistema) {
    // Expande o vetor de eventos, se necessário
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_capacidade = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        Evento *temp = realloc(sistema->lista_eventos, nova_capacidade * sizeof(Evento));

        if (temp == NULL) {
            printf("\nErro de alocacao de memoria!\n");
            return;
        }
        sistema->lista_eventos = temp;
        sistema->capacidade_eventos = nova_capacidade;
    }

    // Ponteiro para o novo evento
    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    
    memset(novo, 0, sizeof(Evento)); 

    novo->codigo = sistema->num_eventos + 1;

    printf("\n--- Criacao de Novo Evento (Orcamento Codigo: %d) ---\n", novo->codigo);

    printf("Nome do Evento: ");
    ler_string_valida(novo->nome_evento, sizeof(novo->nome_evento), VALIDATE_NAO_VAZIA);

    // Lista clientes para o usuário escolher
    listarClientesView(sistema);
    if(sistema->num_clientes > 0) {
        printf("Digite o Codigo do Cliente: ");
        ler_int_valido(&novo->codigo_cliente, 1, 99999); // Assume um ID máximo
    } else {
        printf("\nNenhum cliente cadastrado. Cancele e cadastre um cliente primeiro.\n");
        // Remove o evento que estava sendo criado (voltando o contador)
        // (Ou poderia ter uma validação antes de começar)
        return; // Cancela a adição
    }

    printf("Data de Inicio (DD/MM/AAAA): ");
    ler_string_valida(novo->data_inicio, sizeof(novo->data_inicio), VALIDATE_DATA);

    printf("Data de Fim (DD/MM/AAAA): ");
    ler_string_valida(novo->data_fim, sizeof(novo->data_fim), VALIDATE_DATA);

    printf("Local do Evento: ");
    ler_string_valida(novo->local, sizeof(novo->local), VALIDATE_NAO_VAZIA);

    // Ambos começam zerados.
    novo->custo_total_previsto = 0.0;
    novo->valor_final_faturado = 0.0;

    // Status inicial do evento
    novo->status = ORCAMENTO;

    // (Falta implementar a adição de itens ao orçamento aqui)

    sistema->num_eventos++;
    salvarEventos(sistema); // (Função definida em evento_model.h)

    printf("\nOrcamento para '%s' criado com sucesso!\n", novo->nome_evento);
}

// Altera o status do evento (equivalente a "Aprovar")
void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema); // (Função definida em evento_view.h)
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do evento (orcamento) que deseja aprovar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nEvento com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Evento *evento = &sistema->lista_eventos[indice];

    printf("\nStatus atual: ");
    switch(evento->status) {
        case ORCAMENTO: printf("Orcamento\n"); break;
        case APROVADO: printf("Aprovado\n"); break;
        case FINALIZADO: printf("Finalizado\n"); break;
    }

    if (evento->status == ORCAMENTO) {
        printf("1. Aprovar Orcamento\n");
        printf("0. Cancelar\n");
        printf("Escolha: ");
        int opcao;
        ler_int_valido(&opcao, 0, 1);

        if (opcao == 1) {
            // (Falta lógica de verificação de estoque e alocação de recursos)
            evento->status = APROVADO;
            salvarEventos(sistema);
            printf("\nEvento APROVADO com sucesso!\n");
        } else {
            printf("\nAlteracao cancelada.\n");
        }
    } else if (evento->status == APROVADO) {
         printf("\nEste evento ja esta APROVADO.\n");
         printf("Use a opcao 'Finalizar Evento' (3) para encerra-lo.\n");
    } else { // FINALIZADO
         printf("\nEste evento esta FINALIZADO e nao pode ter seu status alterado.\n");
    }
}

// Finaliza um evento (equivalente a "Finalizar e Faturar")
void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do evento que deseja finalizar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nEvento com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Evento *evento = &sistema->lista_eventos[indice];

    if (evento->status != APROVADO) {
        printf("\nSomente eventos APROVADOS podem ser finalizados!\n");
        if(evento->status == ORCAMENTO) printf("Este evento ainda e um orcamento.\n");
        if(evento->status == FINALIZADO) printf("Este evento ja esta finalizado.\n");
        return;
    }

    printf("\nCusto Total Previsto do Evento: R$ %.2f\n", evento->custo_total_previsto);
    printf("Digite o valor final faturado (R$): ");
    ler_float_positivo(&evento->valor_final_faturado); 

    evento->status = FINALIZADO;

    // (Falta lógica de devolução de recursos ao estoque)

    salvarEventos(sistema);
    printf("\nEvento '%s' finalizado com sucesso!\n", evento->nome_evento);
}

// Detalha um evento (equivalente a "Ver Itens de um Evento")
void detalharEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do evento para ver detalhes: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nEvento com codigo %d nao encontrado.\n", codigo);
        return;
    }

    Evento *evento = &sistema->lista_eventos[indice];

    printf("\n--- Detalhes do Evento #%d ---\n", evento->codigo);
    printf("Nome do Evento: %s\n", evento->nome_evento);
    
    // (Idealmente, buscaria o nome do cliente pelo código)
    printf("Cliente (Codigo): %d\n", evento->codigo_cliente);
    
    printf("Local: %s\n", evento->local);
    printf("Data Inicio: %s\n", evento->data_inicio);
    printf("Data Fim: %s\n", evento->data_fim);

    printf("Status: ");
    switch(evento->status) {
        case ORCAMENTO: printf("Orcamento\n"); break;
        case APROVADO: printf("Aprovado\n"); break;
        case FINALIZADO: printf("Finalizado\n"); break;
    }

    printf("Custo Total Previsto: R$ %.2f\n", evento->custo_total_previsto);

    if (evento->status == FINALIZADO) {
        printf("Valor Final Faturado: R$ %.2f\n", evento->valor_final_faturado);
    }
    
    // (Falta implementar a listagem dos itens, equipe e serviços)
    printf("\n(Detalhes de itens, equipe e servicos ainda nao implementados.)\n");
}