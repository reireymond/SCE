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
    novo->codigo = sistema->num_eventos + 1;

    printf("\n--- Criacao de Novo Evento (Codigo: %d) ---\n", novo->codigo);

    // Entrada e validação dos dados
    printf("Nome do Cliente: ");
    ler_string_valida(novo->cliente, sizeof(novo->cliente), VALIDATE_NOME);

    printf("Descricao do Evento: ");
    ler_string_valida(novo->descricao, sizeof(novo->descricao), VALIDATE_NAO_VAZIA);

    printf("Data do Evento (DD/MM/AAAA): ");
    ler_string_valida(novo->data, sizeof(novo->data), VALIDATE_DATA);

    printf("Valor Orçado: R$ ");
    ler_float_positivo(&novo->valor_orcado);

    // Status inicial do evento
    strcpy(novo->status, "PENDENTE");

    sistema->num_eventos++;
    salvarEventos(sistema);

    printf("\nEvento '%s' criado com sucesso!\n", novo->descricao);
}

// Altera o status do evento (equivalente a "Aprovar")
void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nDigite o codigo do evento que deseja alterar o status: ");
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

    printf("\nStatus atual: %s\n", evento->status);
    printf("1. Aprovar Evento\n");
    printf("2. Cancelar Evento\n");
    printf("Escolha: ");

    int opcao;
    ler_int_valido(&opcao, 1, 2);

    if (opcao == 1)
        strcpy(evento->status, "APROVADO");
    else
        strcpy(evento->status, "CANCELADO");

    salvarEventos(sistema);
    printf("\nStatus do evento atualizado para '%s'!\n", evento->status);
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

    if (strcmp(evento->status, "APROVADO") != 0) {
        printf("\nSomente eventos aprovados podem ser finalizados!\n");
        return;
    }

    printf("Digite o valor final faturado (R$): ");
    ler_float_positivo(&evento->valor_final);

    strcpy(evento->status, "FINALIZADO");

    salvarEventos(sistema);
    printf("\nEvento '%s' finalizado com sucesso!\n", evento->descricao);
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
    printf("Cliente: %s\n", evento->cliente);
    printf("Descricao: %s\n", evento->descricao);
    printf("Data: %s\n", evento->data);
    printf("Valor Orcado: R$ %.2f\n", evento->valor_orcado);
    printf("Status: %s\n", evento->status);

    if (strcmp(evento->status, "FINALIZADO") == 0)
        printf("Valor Final: R$ %.2f\n", evento->valor_final);
}
