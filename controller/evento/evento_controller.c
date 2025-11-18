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
#include "controller/transacao/transacao_controller.h"

// Transforma data texto "DD/MM/AAAA" em numero inteiro AAAAMMDD pra facilitar comparar
int dataParaInteiro(char* data) {
    int dia, mes, ano;
    if(sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;
    return (ano * 10000) + (mes * 100) + dia;
}

void adicionarEventoController(Sistema *sistema) {
    // Aumenta a lista se precisar
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_cap = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova_cap * sizeof(Evento));
        sistema->capacidade_eventos = nova_cap;
    }

    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    memset(novo, 0, sizeof(Evento));
    
    novo->codigo = sistema->num_eventos + 1;
    novo->status = ORCAMENTO;

    printf("\n--- Criar Novo Orcamento (ID: %d) ---\n", novo->codigo);
    printf("Nome do Evento: "); 
    ler_texto_valido(novo->nome_evento, 150, VALIDAR_NAO_VAZIO);
    
    listarClientesView(sistema);
    printf("Codigo do Cliente: "); 
    ler_inteiro_valido(&novo->codigo_cliente, 1, 99999);
    
    printf("Data Inicio (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_inicio, 12, VALIDAR_DATA);
    
    printf("Data Fim (DD/MM/AAAA): "); 
    ler_texto_valido(novo->data_fim, 12, VALIDAR_DATA);
    
    printf("Local: "); 
    ler_texto_valido(novo->local, 150, VALIDAR_NAO_VAZIO);

    // --- Adicionar Recursos ---
    int op = 0;
    printf("\nAdicionar equipamentos? (1-Sim, 0-Nao): "); 
    scanf("%d", &op);
    while(op == 1) {
        listarRecursosView(sistema);
        int cod, qtd;
        printf("Codigo do Recurso: "); scanf("%d", &cod);
        
        Recurso *r = NULL;
        for(int i=0; i<sistema->num_recursos; i++) {
            if(sistema->lista_recursos[i].codigo == cod) r = &sistema->lista_recursos[i];
        }

        if(r != NULL) {
            printf("Quantidade: "); scanf("%d", &qtd);
            novo->lista_recursos_alocados = realloc(novo->lista_recursos_alocados, (novo->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            ItemRecursoEvento *item = &novo->lista_recursos_alocados[novo->num_recursos_alocados];
            item->codigo_recurso = r->codigo;
            item->quantidade = qtd;
            item->custo_locacao_momento = r->valor_locacao;
            
            novo->num_recursos_alocados++;
            novo->custo_total_previsto += (r->valor_locacao * qtd);
            printf("Adicionado!\n");
        }
        printf("Mais recursos? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    // --- Adicionar Equipe ---
    printf("\nAdicionar equipe? (1-Sim, 0-Nao): "); scanf("%d", &op);
    while(op == 1) {
        listarEquipeInternaView(sistema);
        int cod; printf("Codigo do Membro: "); scanf("%d", &cod);
        
        EquipeInterna *e = NULL;
        for(int i=0; i<sistema->num_equipe; i++) {
            if(sistema->lista_equipe[i].codigo == cod) e = &sistema->lista_equipe[i];
        }

        if(e != NULL) {
            novo->lista_equipe_alocada = realloc(novo->lista_equipe_alocada, (novo->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            ItemEquipeEvento *item = &novo->lista_equipe_alocada[novo->num_equipe_alocada];
            item->codigo_equipe = e->codigo;
            item->custo_diaria_momento = e->valor_diaria;
            
            novo->num_equipe_alocada++;
            novo->custo_total_previsto += e->valor_diaria;
            printf("Adicionado!\n");
        }
        printf("Mais equipe? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    printf("\nOrcamento criado! Valor Previsto: R$ %.2f\n", novo->custo_total_previsto);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int codigo;
    printf("\nID do orcamento para APROVAR: "); scanf("%d", &codigo);

    Evento *evento = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) evento = &sistema->lista_eventos[i];
    }

    if (!evento || evento->status != ORCAMENTO) { 
        printf("Erro: Evento nao encontrado ou status invalido.\n"); return; 
    }

    printf("Verificando estoque e datas...\n");
    int ini1 = dataParaInteiro(evento->data_inicio);
    int fim1 = dataParaInteiro(evento->data_fim);
    int conflito = 0;

    for(int i=0; i<evento->num_recursos_alocados; i++) {
        int codRec = evento->lista_recursos_alocados[i].codigo_recurso;
        int qtdPedida = evento->lista_recursos_alocados[i].quantidade;
        int estoqueTotal = 0;
        
        for(int k=0; k<sistema->num_recursos; k++)
            if(sistema->lista_recursos[k].codigo == codRec) estoqueTotal = sistema->lista_recursos[k].quantidade_estoque;
        
        int qtdUsada = 0;
        for(int j=0; j<sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            if(outro->status == APROVADO && outro->codigo != evento->codigo) {
                int ini2 = dataParaInteiro(outro->data_inicio);
                int fim2 = dataParaInteiro(outro->data_fim);
                if(ini1 <= fim2 && fim1 >= ini2) {
                    for(int r=0; r<outro->num_recursos_alocados; r++)
                        if(outro->lista_recursos_alocados[r].codigo_recurso == codRec) qtdUsada += outro->lista_recursos_alocados[r].quantidade;
                }
            }
        }
        if (qtdPedida > (estoqueTotal - qtdUsada)) {
            printf("ERRO: Recurso ID %d sem estoque na data!\n", codRec);
            conflito = 1; break;
        }
    }

    if(conflito == 0) {
        evento->status = APROVADO;
        salvarEventos(sistema);
        printf("Evento APROVADO!\n");
    } else {
        printf("Falha: Conflito de estoque.\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int codigo;
    printf("\nID para FINALIZAR: "); scanf("%d", &codigo);
    
    Evento *e = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) 
        if (sistema->lista_eventos[i].codigo == codigo) e = &sistema->lista_eventos[i];

    if (!e || e->status != APROVADO) {
        printf("Erro: Evento nao pode ser finalizado.\n"); return;
    }

    printf("\n--- Fechamento ---\n");
    printf("Custo Previsto: R$ %.2f\n", e->custo_total_previsto);
    printf("Valor Final Cobrado: R$ "); ler_float_positivo(&e->valor_final_faturado);

    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_RECEBER;
    t.status = PENDENTE;
    t.valor = e->valor_final_faturado;
    sprintf(t.descricao, "Fatura Evento #%d", e->codigo);
    printf("Vencimento (DD/MM/AAAA): "); ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);

    registrarTransacao(sistema, t);
    e->status = FINALIZADO;
    salvarEventos(sistema);
    printf("Evento finalizado e conta gerada!\n");
}