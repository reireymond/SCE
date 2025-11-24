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

// Função auxiliar para converter data/hora em número para comparação
long long transformarDataHoraEmNumero(char* data, char* hora) {
    int d, m, a, hh, mm;
    sscanf(data, "%d/%d/%d", &d, &m, &a);
    sscanf(hora, "%d:%d", &hh, &mm);
    return (long long)a*100000000 + m*1000000 + d*10000 + hh*100 + mm;
}

void adicionarEventoController(Sistema *sistema) {
    // Validações iniciais
    if (sistema->num_clientes == 0) { 
        printf("\nSem clientes cadastrados. Cadastre um primeiro.\n"); 
        return; 
    }

    // Expande memória se precisar
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        sistema->capacidade_eventos = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, sistema->capacidade_eventos * sizeof(Evento));
    }

    // Cria o evento zerado
    Evento *e = &sistema->lista_eventos[sistema->num_eventos];
    memset(e, 0, sizeof(Evento));
    
    e->codigo = sistema->num_eventos + 1;
    e->status = ORCAMENTO;

    printf("\n--- Novo Evento (ID: %d) ---\n", e->codigo);
    
    printf("Nome: "); ler_texto_valido(e->nome_evento, 150, VALIDAR_NAO_VAZIO);
    
    listarClientesView(sistema);
    printf("Cod. Cliente: "); ler_inteiro_valido(&e->codigo_cliente, 1, 99999);
    
    printf("Data Inicio (DD/MM/AAAA): "); ler_texto_valido(e->data_inicio, 12, VALIDAR_DATA);
    printf("Hora Inicio (HH:MM): "); ler_texto_valido(e->hora_inicio, 10, VALIDAR_NAO_VAZIO);
    printf("Data Fim (DD/MM/AAAA): "); ler_texto_valido(e->data_fim, 12, VALIDAR_DATA);
    printf("Hora Fim (HH:MM): "); ler_texto_valido(e->hora_fim, 10, VALIDAR_NAO_VAZIO);
    printf("Local: "); ler_texto_valido(e->local, 150, VALIDAR_NAO_VAZIO);

    int op = 0;

    // ADICIONAR EQUIPAMENTOS
    printf("\nAdd Equipamentos? (1-Sim, 0-Nao): "); 
    scanf("%d", &op); limpar_buffer();

    while(op == 1) {
        if(sistema->num_recursos == 0) { printf("Sem equipamentos no sistema.\n"); break; }
        
        listarRecursosView(sistema);
        int cod, qtd;
        printf("Cod Equipamento: "); scanf("%d", &cod); limpar_buffer();
        
        Recurso *r = NULL;
        for(int i=0; i < sistema->num_recursos; i++) {
            if(sistema->lista_recursos[i].codigo == cod) r = &sistema->lista_recursos[i];
        }

        if(r) {
            printf("Qtd: "); scanf("%d", &qtd); limpar_buffer();
            
            e->lista_recursos_alocados = realloc(e->lista_recursos_alocados, (e->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            e->lista_recursos_alocados[e->num_recursos_alocados].codigo_recurso = r->codigo;
            e->lista_recursos_alocados[e->num_recursos_alocados].quantidade = qtd;
            e->lista_recursos_alocados[e->num_recursos_alocados].custo_locacao_momento = r->valor_locacao;
            
            e->num_recursos_alocados++;
            e->custo_total_previsto += (r->valor_locacao * qtd);
            printf("Adicionado!\n");
        } else {
            printf("Nao achou.\n");
        }
        printf("Add mais? (1-Sim, 0-Nao): "); scanf("%d", &op); limpar_buffer();
    }

    // ADICIONAR EQUIPE
    printf("\nAdd Equipe? (1-Sim, 0-Nao): "); 
    scanf("%d", &op); limpar_buffer();

    while(op == 1) {
        if(sistema->num_equipe == 0) { printf("Sem equipe cadastrada.\n"); break; }

        listarEquipeInternaView(sistema);
        int cod;
        printf("Cod Funcionario: "); scanf("%d", &cod); limpar_buffer();
        
        EquipeInterna *m = NULL;
        for(int i=0; i < sistema->num_equipe; i++) {
            if(sistema->lista_equipe[i].codigo == cod) m = &sistema->lista_equipe[i];
        }

        if(m) {
            e->lista_equipe_alocada = realloc(e->lista_equipe_alocada, (e->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            e->lista_equipe_alocada[e->num_equipe_alocada].codigo_equipe = m->codigo;
            e->lista_equipe_alocada[e->num_equipe_alocada].custo_diaria_momento = m->valor_diaria;
            
            e->num_equipe_alocada++;
            e->custo_total_previsto += m->valor_diaria;
            printf("Adicionado!\n");
        } else {
            printf("Nao achou.\n");
        }
        printf("Add mais? (1-Sim, 0-Nao): "); scanf("%d", &op); limpar_buffer();
    }

    // ADICIONAR FORNECEDORES (SERVIÇOS)
    printf("\nAdd Servico Fornecedor? (1-Sim, 0-Nao): "); 
    scanf("%d", &op); limpar_buffer();

    while(op == 1) {
        if(sistema->num_fornecedores == 0) { printf("Sem fornecedores cadastrados.\n"); break; }

        listarFornecedoresView(sistema);
        int cod;
        printf("Cod Fornecedor: "); scanf("%d", &cod); limpar_buffer();

        Fornecedor *f = NULL;
        for(int i=0; i < sistema->num_fornecedores; i++) {
            if(sistema->lista_fornecedores[i].codigo == cod) f = &sistema->lista_fornecedores[i];
        }

        if(f) {
            float val;
            printf("Valor Cobrado: R$ "); ler_float_positivo(&val);

            e->lista_fornecedores_alocados = realloc(e->lista_fornecedores_alocados, (e->num_fornecedores_alocados + 1) * sizeof(ItemFornecedorEvento));
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].codigo_fornecedor = f->codigo;
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].valor_cobrado = val;

            e->num_fornecedores_alocados++;
            e->custo_total_previsto += val;
            printf("Adicionado!\n");
        } else {
            printf("Nao achou.\n");
        }
        printf("Add mais? (1-Sim, 0-Nao): "); scanf("%d", &op); limpar_buffer();
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    printf("\nEvento criado! Previsto: R$ %.2f\n", e->custo_total_previsto);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int id;
    printf("\nID para APROVAR: "); scanf("%d", &id); limpar_buffer();

    Evento *e = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == id) e = &sistema->lista_eventos[i];
    }

    if (!e || e->status != ORCAMENTO) { 
        printf("Erro: Nao existe ou ja aprovado.\n"); return; 
    }

    printf("Verificando estoque...\n");
    
    long long ini = transformarDataHoraEmNumero(e->data_inicio, e->hora_inicio);
    long long fim = transformarDataHoraEmNumero(e->data_fim, e->hora_fim);
    int conflito = 0;

    // Verifica estoque de cada recurso pedido
    for(int i=0; i < e->num_recursos_alocados; i++) {
        int cod = e->lista_recursos_alocados[i].codigo_recurso;
        int qtd_quer = e->lista_recursos_alocados[i].quantidade;
        int total = 0;

        // Acha o total do estoque
        for(int k=0; k < sistema->num_recursos; k++) {
            if(sistema->lista_recursos[k].codigo == cod) total = sistema->lista_recursos[k].quantidade_estoque;
        }

        // Calcula quanto ja ta usado nesse horario
        int usado = 0;
        for(int j=0; j < sistema->num_eventos; j++) {
            Evento *out = &sistema->lista_eventos[j];
            if(out->status == APROVADO && out->codigo != e->codigo) {
                long long i2 = transformarDataHoraEmNumero(out->data_inicio, out->hora_inicio);
                long long f2 = transformarDataHoraEmNumero(out->data_fim, out->hora_fim);
                
                // Se as datas batem
                if(ini <= f2 && fim >= i2) {
                    for(int r=0; r < out->num_recursos_alocados; r++) {
                        if(out->lista_recursos_alocados[r].codigo_recurso == cod) 
                            usado += out->lista_recursos_alocados[r].quantidade;
                    }
                }
            }
        }

        if (qtd_quer > (total - usado)) {
            printf("Conflito! Recurso %d sem estoque (Total: %d, Usado: %d).\n", cod, total, usado);
            conflito = 1; break;
        }
    }

    if(!conflito) {
        e->status = APROVADO;
        salvarEventos(sistema);
        printf("Aprovado!\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int id;
    printf("\nID para FINALIZAR: "); scanf("%d", &id); limpar_buffer();
    
    Evento *e = NULL;
    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == id) e = &sistema->lista_eventos[i];
    }

    if (!e || e->status != APROVADO) { printf("Erro: Nao esta aprovado.\n"); return; }

    printf("Previsto: R$ %.2f\n", e->custo_total_previsto);
    printf("Valor Final a Cobrar: R$ "); ler_float_positivo(&e->valor_final_faturado);

    Transacao t;
    memset(&t, 0, sizeof(Transacao));
    t.tipo = CONTA_A_RECEBER;
    t.status = PENDENTE;
    t.valor = e->valor_final_faturado;
    sprintf(t.descricao, "Fatura Evento %d", e->codigo);
    printf("Vencimento (DD/MM/AAAA): "); ler_texto_valido(t.data_vencimento, 12, VALIDAR_DATA);

    registrarTransacao(sistema, t);
    
    e->status = FINALIZADO;
    salvarEventos(sistema);
    printf("Finalizado e conta gerada.\n");
}