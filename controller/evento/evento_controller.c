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
#include "view/recurso/recurso_view.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "view/fornecedor/fornecedor_view.h"
#include "controller/transacao/transacao_controller.h"

static int obterProximoIdEvento(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_eventos; i++) {
        if(sistema->lista_eventos[i].codigo > max) max = sistema->lista_eventos[i].codigo;
    }
    return max + 1;
}

int dataParaInteiro(char* data) {
    int dia, mes, ano;
    if(sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;
    return (ano * 10000) + (mes * 100) + dia;
}

void adicionarEventoController(Sistema *sistema) {
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_cap = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova_cap * sizeof(Evento));
        sistema->capacidade_eventos = nova_cap;
    }

    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    memset(novo, 0, sizeof(Evento));
    novo->codigo = obterProximoIdEvento(sistema); // CORREÇÃO DE ID
    novo->status = ORCAMENTO;

    printf("\n--- Novo Orcamento (ID: %d) ---\n", novo->codigo);
    printf("Nome do Evento: "); ler_texto_valido(novo->nome_evento, sizeof(novo->nome_evento), VALIDAR_NAO_VAZIO);
    listarClientesView(sistema);
    printf("Codigo do Cliente: "); ler_inteiro_valido(&novo->codigo_cliente, 1, 99999);
    printf("Data Inicio (DD/MM/AAAA): "); ler_texto_valido(novo->data_inicio, sizeof(novo->data_inicio), VALIDAR_DATA);
    printf("Data Fim (DD/MM/AAAA): "); ler_texto_valido(novo->data_fim, sizeof(novo->data_fim), VALIDAR_DATA);
    printf("Local: "); ler_texto_valido(novo->local, sizeof(novo->local), VALIDAR_NAO_VAZIO);

    // Recursos
    int op = 0;
    printf("\nAdicionar Recursos? (1-Sim, 0-Nao): "); scanf("%d", &op);
    while(op == 1) {
        listarRecursosView(sistema);
        int cod, qtd;
        printf("Codigo do Recurso: "); scanf("%d", &cod);
        Recurso *r = NULL;
        for(int i=0; i<sistema->num_recursos; i++) 
            if(sistema->lista_recursos[i].codigo == cod) r = &sistema->lista_recursos[i];

        if(r) {
            printf("Quantidade: "); scanf("%d", &qtd);
            novo->lista_recursos_alocados = realloc(novo->lista_recursos_alocados, (novo->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            ItemRecursoEvento *item = &novo->lista_recursos_alocados[novo->num_recursos_alocados++];
            item->codigo_recurso = r->codigo;
            item->quantidade = qtd;
            item->custo_locacao_momento = r->valor_locacao;
            novo->custo_total_previsto += (r->valor_locacao * qtd);
        } else printf("Nao encontrado.\n");
        printf("Mais recursos? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    // Equipe
    printf("\nAdicionar Equipe? (1-Sim, 0-Nao): "); scanf("%d", &op);
    while(op == 1) {
        listarEquipeInternaView(sistema);
        int cod; printf("Codigo do Membro: "); scanf("%d", &cod);
        EquipeInterna *e = NULL;
        for(int i=0; i<sistema->num_equipe; i++) 
            if(sistema->lista_equipe[i].codigo == cod) e = &sistema->lista_equipe[i];

        if(e) {
            novo->lista_equipe_alocada = realloc(novo->lista_equipe_alocada, (novo->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            ItemEquipeEvento *item = &novo->lista_equipe_alocada[novo->num_equipe_alocada++];
            item->codigo_equipe = e->codigo;
            item->custo_diaria_momento = e->valor_diaria;
            novo->custo_total_previsto += e->valor_diaria;
        } else printf("Nao encontrado.\n");
        printf("Mais equipe? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    // Serviços
    printf("\nAdicionar Servicos (Fornecedores)? (1-Sim, 0-Nao): "); scanf("%d", &op);
    while(op == 1) {
        listarFornecedoresView(sistema);
        int cod; float valor; char desc[100];
        printf("Codigo do Fornecedor: "); scanf("%d", &cod); limpar_buffer();
        printf("Descricao do Servico: "); ler_texto_valido(desc, 100, VALIDAR_NAO_VAZIO);
        printf("Valor contratado: "); ler_float_positivo(&valor);

        novo->lista_servicos_contratados = realloc(novo->lista_servicos_contratados, (novo->num_servicos_contratados + 1) * sizeof(ItemFornecedorEvento));
        ItemFornecedorEvento *item = &novo->lista_servicos_contratados[novo->num_servicos_contratados++];
        item->codigo_fornecedor = cod;
        item->custo_contratado = valor;
        strcpy(item->descricao_servico, desc);
        novo->custo_total_previsto += valor;
        printf("Mais servicos? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    printf("\nOrcamento Criado! Total Previsto: %.2f\n", novo->custo_total_previsto);
}

void editarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;
    int codigo, indice = -1;
    printf("\nID para editar: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_eventos; i++) if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }
    
    Evento *e = &sistema->lista_eventos[indice];
    if (e->status != ORCAMENTO) { printf("Apenas orcamentos podem ser editados.\n"); return; }

    int opcao;
    do {
        limpar_tela();
        printf("--- Editando: %s ---\n", e->nome_evento);
        printf("1. Data Inicio\n2. Data Fim\n3. Local\n4. Limpar Recursos\n0. Sair\nEscolha: ");
        ler_inteiro_valido(&opcao, 0, 4);
        switch(opcao) {
            case 1: printf("Nova Data Inicio: "); ler_texto_valido(e->data_inicio, 11, VALIDAR_DATA); break;
            case 2: printf("Nova Data Fim: "); ler_texto_valido(e->data_fim, 11, VALIDAR_DATA); break;
            case 3: printf("Novo Local: "); ler_texto_valido(e->local, 150, VALIDAR_NAO_VAZIO); break;
            case 4:
                e->custo_total_previsto = 0;
                for(int i=0; i<e->num_equipe_alocada; i++) e->custo_total_previsto += e->lista_equipe_alocada[i].custo_diaria_momento;
                for(int i=0; i<e->num_servicos_contratados; i++) e->custo_total_previsto += e->lista_servicos_contratados[i].custo_contratado;
                free(e->lista_recursos_alocados);
                e->lista_recursos_alocados = NULL;
                e->num_recursos_alocados = 0;
                printf("Recursos limpos.\n"); pausar();
                break;
            case 0: break;
        }
    } while(opcao != 0);
    salvarEventos(sistema);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;
    int codigo, indice = -1;
    printf("\nID para aprovar: "); scanf("%d", &codigo);
    for (int i = 0; i < sistema->num_eventos; i++) if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Evento *evento = &sistema->lista_eventos[indice];
    if (evento->status != ORCAMENTO) { printf("Status invalido para aprovacao.\n"); return; }

    printf("Verificando estoque...\n");
    int conflito = 0;
    int ini1 = dataParaInteiro(evento->data_inicio);
    int fim1 = dataParaInteiro(evento->data_fim);

    for(int i=0; i<evento->num_recursos_alocados; i++) {
        int codRec = evento->lista_recursos_alocados[i].codigo_recurso;
        int qtdPedida = evento->lista_recursos_alocados[i].quantidade;
        int estoqueTotal = 0;
        for(int k=0; k<sistema->num_recursos; k++)
            if(sistema->lista_recursos[k].codigo == codRec) estoqueTotal = sistema->lista_recursos[k].quantidade_estoque;
        
        int qtdOcupada = 0;
        for(int j=0; j<sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            if(outro->status == APROVADO && outro->codigo != evento->codigo) {
                int ini2 = dataParaInteiro(outro->data_inicio);
                int fim2 = dataParaInteiro(outro->data_fim);
                if(ini1 <= fim2 && fim1 >= ini2) {
                    for(int r=0; r<outro->num_recursos_alocados; r++)
                        if(outro->lista_recursos_alocados[r].codigo_recurso == codRec) qtdOcupada += outro->lista_recursos_alocados[r].quantidade;
                }
            }
        }
        if (qtdPedida > (estoqueTotal - qtdOcupada)) {
            printf("ERRO: Recurso ID %d sem estoque suficiente na data!\n", codRec);
            conflito = 1; break;
        }
    }

    if(conflito == 0) {
        evento->status = APROVADO;
        salvarEventos(sistema);
        printf("Evento APROVADO!\n");
    } else {
        printf("Nao foi possivel aprovar por falta de estoque.\n");
    }
}

// NOVA FUNÇÃO: Cancelar Evento
void cancelarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;
    int codigo, indice = -1;
    printf("\nID para CANCELAR: "); scanf("%d", &codigo);
    for (int i = 0; i < sistema->num_eventos; i++) if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Evento *e = &sistema->lista_eventos[indice];
    if(e->status == FINALIZADO) {
        printf("Erro: Evento ja finalizado/faturado nao pode ser cancelado.\n");
        return;
    }
    
    char conf;
    printf("Tem certeza que deseja CANCELAR o evento '%s'? (s/n): ", e->nome_evento);
    scanf(" %c", &conf); limpar_buffer();
    if(conf == 's' || conf == 'S') {
        e->status = CANCELADO;
        salvarEventos(sistema);
        printf("Evento cancelado. Recursos liberados.\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;
    int codigo, indice = -1;
    printf("\nID para finalizar: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_eventos; i++) if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    if (indice == -1) return;

    Evento *e = &sistema->lista_eventos[indice];
    // SEGURANÇA: Prevenir dupla finalização
    if (e->status == FINALIZADO) {
        printf("Erro: Evento ja foi finalizado anteriormente!\n");
        return;
    }
    if (e->status != APROVADO) {
        printf("Erro: Apenas eventos APROVADOS podem ser finalizados.\n");
        return;
    }

    printf("\n=== FECHAMENTO FINANCEIRO ===\n");
    printf("Custo Previsto: R$ %.2f\n", e->custo_total_previsto);
    printf("Valor Final a Cobrar: R$ "); ler_float_positivo(&e->valor_final_faturado);

    printf("Pagamento:\n1. A Vista (Caixa)\n2. A Prazo (Conta a Receber)\nOpcao: ");
    int forma; ler_inteiro_valido(&forma, 1, 2);

    Transacao t_receber;
    memset(&t_receber, 0, sizeof(Transacao));
    t_receber.valor = e->valor_final_faturado;
    t_receber.codigo_cliente_ref = e->codigo_cliente;
    t_receber.codigo_evento_ref = e->codigo;
    sprintf(t_receber.descricao, "Receita Evento #%d", e->codigo);

    if (forma == 1) {
        t_receber.tipo = MOVIMENTACAO_CAIXA;
        t_receber.status = PAGA;
        strcpy(t_receber.data_pagamento, "HOJE"); 
        sistema->saldo_caixa += e->valor_final_faturado;
        printf("Recebido no Caixa!\n");
    } else {
        t_receber.tipo = CONTA_A_RECEBER;
        t_receber.status = PENDENTE;
        printf("Vencimento (DD/MM/AAAA): "); ler_texto_valido(t_receber.data_vencimento, 12, VALIDAR_DATA);
        printf("Conta a Receber gerada!\n");
    }
    registrarTransacao(sistema, t_receber);

    // Gerar Contas a Pagar
    if (e->num_servicos_contratados > 0) {
        printf("\nGerando contas a pagar (Fornecedores)...\n");
        for (int i = 0; i < e->num_servicos_contratados; i++) {
            ItemFornecedorEvento *item = &e->lista_servicos_contratados[i];
            Transacao t_pagar;
            memset(&t_pagar, 0, sizeof(Transacao));
            t_pagar.tipo = CONTA_A_PAGAR;
            t_pagar.status = PENDENTE;
            t_pagar.valor = item->custo_contratado;
            t_pagar.codigo_fornecedor_ref = item->codigo_fornecedor;
            t_pagar.codigo_evento_ref = e->codigo;
            sprintf(t_pagar.descricao, "Servico Evt #%d: %s", e->codigo, item->descricao_servico);
            printf("Vencimento forn. %d (R$ %.2f): ", item->codigo_fornecedor, item->custo_contratado);
            ler_texto_valido(t_pagar.data_vencimento, 12, VALIDAR_DATA);
            registrarTransacao(sistema, t_pagar);
        }
    }

    e->status = FINALIZADO;
    salvarEventos(sistema);
    printf("\nEvento finalizado com sucesso!\n");
}

void detalharEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int codigo, indice = -1;
    printf("\nID para detalhes: "); scanf("%d", &codigo); limpar_buffer();
    for (int i = 0; i < sistema->num_eventos; i++) if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Evento *e = &sistema->lista_eventos[indice];
    limpar_tela();
    printf("=== EVENTO #%d ===\n", e->codigo);
    printf("Nome: %s\n", e->nome_evento);
    printf("Status: %d (0-Orcamento, 1-Aprovado, 2-Finalizado, 3-Cancelado)\n", e->status);
printf("Cliente ID: %d\n", e->codigo_cliente);
    printf("Periodo: %s ate %s\n", e->data_inicio, e->data_fim);
    printf("Status: %d (0-Orcamento, 1-Aprovado, 2-Finalizado)\n", e->status);
    printf("\n--- RECURSOS ---\n");
    for(int i=0; i<e->num_recursos_alocados; i++) {
        printf("- Recurso ID %d | Qtd: %d | Custo un: %.2f\n", 
               e->lista_recursos_alocados[i].codigo_recurso,
               e->lista_recursos_alocados[i].quantidade,
               e->lista_recursos_alocados[i].custo_locacao_momento);
    }

    printf("\n--- EQUIPE ---\n");
    for(int i=0; i<e->num_equipe_alocada; i++) {
        printf("- Membro ID %d | Custo: %.2f\n", 
               e->lista_equipe_alocada[i].codigo_equipe,
               e->lista_equipe_alocada[i].custo_diaria_momento);
    }

    printf("\n--- SERVICOS TERCEIRIZADOS ---\n");
    for(int i=0; i<e->num_servicos_contratados; i++) {
        printf("- Fornecedor ID %d | %s | Custo: %.2f\n", 
               e->lista_servicos_contratados[i].codigo_fornecedor,
               e->lista_servicos_contratados[i].descricao_servico,
               e->lista_servicos_contratados[i].custo_contratado);
    }
    
    printf("\nTotal Previsto: %.2f\n", e->custo_total_previsto);
    if(e->status == FINALIZADO) {
        printf("Valor Faturado Final: %.2f\n", e->valor_final_faturado);
    }
    pausar();
}