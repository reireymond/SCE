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

// Transforma data string em inteiro para comparar
int dataParaInteiro(char* data) {
    int dia, mes, ano;
    // Pega dia, mes e ano
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    return (ano * 10000) + (mes * 100) + dia;
}

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

    // Adicionar recursos ao orcamento
    int quer_recurso = 0;
    printf("\nDeseja adicionar equipamentos? (1-Sim, 0-Nao): ");
    scanf("%d", &quer_recurso);
    limpar_buffer();

    while (quer_recurso == 1) {
        // Lista os recursos
        listarRecursosView(sistema);
        
        int cod_rec, qtd;
        printf("Digite o CODIGO do recurso: ");
        scanf("%d", &cod_rec);
        limpar_buffer();

        // Procura o recurso
        Recurso *rec_encontrado = NULL;
        for (int i = 0; i < sistema->num_recursos; i++) {
            if (sistema->lista_recursos[i].codigo == cod_rec) {
                rec_encontrado = &sistema->lista_recursos[i];
                break;
            }
        }

        if (rec_encontrado == NULL) {
            printf("Erro: Recurso nao encontrado.\n");
        } else {
            printf("Quantidade (Estoque: %d): ", rec_encontrado->quantidade_estoque);
            scanf("%d", &qtd);
            limpar_buffer();

            // Aumenta memoria da lista do evento
            int nova_qtd = novo->num_recursos_alocados + 1;
            ItemRecursoEvento *temp = realloc(novo->lista_recursos_alocados, nova_qtd * sizeof(ItemRecursoEvento));
            
            if (temp != NULL) {
                novo->lista_recursos_alocados = temp;
                
                // Adiciona o item
                int pos = novo->num_recursos_alocados;
                novo->lista_recursos_alocados[pos].codigo_recurso = rec_encontrado->codigo;
                novo->lista_recursos_alocados[pos].quantidade = qtd;
                novo->lista_recursos_alocados[pos].custo_locacao_momento = rec_encontrado->valor_locacao;

                novo->num_recursos_alocados++;
                
                // Atualiza o custo total
                float custo_item = rec_encontrado->valor_locacao * qtd;
                novo->custo_total_previsto = novo->custo_total_previsto + custo_item;
                
                printf("Item adicionado! + R$ %.2f\n", custo_item);
            } else {
                printf("Erro de memoria.\n");
            }
        }

        printf("\nMais recursos? (1-Sim, 0-Nao): ");
        scanf("%d", &quer_recurso);
        limpar_buffer();
    }

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
            // Valida estoque e data
            printf("\nVerificando estoque...\n");
            
            int pode_aprovar = 1;
            int inicio_novo = dataParaInteiro(evento->data_inicio);
            int fim_novo = dataParaInteiro(evento->data_fim);

            // Verifica cada recurso do evento
            for (int i = 0; i < evento->num_recursos_alocados; i++) {
                int cod_rec = evento->lista_recursos_alocados[i].codigo_recurso;
                int qtd_pedida = evento->lista_recursos_alocados[i].quantidade;
                
                // Pega o estoque total do recurso
                int estoque_total = 0;
                char nome_rec[100];
                
                for(int k=0; k < sistema->num_recursos; k++) {
                    if(sistema->lista_recursos[k].codigo == cod_rec) {
                        estoque_total = sistema->lista_recursos[k].quantidade_estoque;
                        strcpy(nome_rec, sistema->lista_recursos[k].descricao);
                        break;
                    }
                }

                // Ve quanto ja esta usado em outros eventos aprovados
                int qtd_ocupada = 0;
                
                for (int j = 0; j < sistema->num_eventos; j++) {
                    Evento *outro = &sistema->lista_eventos[j];
                    
                    // Ignora o proprio evento e orcamentos
                    if (outro->status == APROVADO && outro->codigo != evento->codigo) {
                        int inicio_outro = dataParaInteiro(outro->data_inicio);
                        int fim_outro = dataParaInteiro(outro->data_fim);

                        // Verifica se as datas batem
                        if (inicio_novo <= fim_outro && fim_novo >= inicio_outro) {
                            // Se data bate, soma a quantidade usada
                            for (int r = 0; r < outro->num_recursos_alocados; r++) {
                                if (outro->lista_recursos_alocados[r].codigo_recurso == cod_rec) {
                                    qtd_ocupada += outro->lista_recursos_alocados[r].quantidade;
                                }
                            }
                        }
                    }
                }

                // Verifica se tem estoque suficiente
                int disponivel = estoque_total - qtd_ocupada;
                if (qtd_pedida > disponivel) {
                    printf("ERRO: Conflito para '%s'!\n", nome_rec);
                    printf("Estoque: %d | Ocupado: %d | Livre: %d\n", estoque_total, qtd_ocupada, disponivel);
                    printf("Pedido: %d. Impossivel aprovar.\n", qtd_pedida);
                    pode_aprovar = 0;
                    break; // Para o loop
                }
            }

            if (pode_aprovar == 1) {
                evento->status = APROVADO;
                salvarEventos(sistema);
                printf("\nEvento APROVADO!\n");
            } else {
                printf("\nFalha na aprovacao. Verifique o estoque.\n");
            }
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