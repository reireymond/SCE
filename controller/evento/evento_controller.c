#include "evento_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/evento/evento_model.h"
#include "model/transacao/transacao_model.h"
#include "view/evento/evento_view.h"
#include "view/recurso/recurso_view.h"
#include "view/cliente/cliente_view.h"
#include "view/fornecedor/fornecedor_view.h"
#include "controller/transacao/transacao_controller.h"
#include "utils/utils.h"
void adicionarEventoController(Sistema *sistema) {
    if (sistema->num_clientes == 0) {
        mensagem_erro("Sem clientes cadastrados. Cadastre um primeiro."); 
        return; 
    }
    
    // Expansão de memória se necessário
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_cap = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        Evento *temp = realloc(sistema->lista_eventos, nova_cap * sizeof(Evento));
        if(!temp) { mensagem_erro("Erro de memoria"); return; }
        sistema->lista_eventos = temp;
        sistema->capacidade_eventos = nova_cap;
    }

    Evento *e = &sistema->lista_eventos[sistema->num_eventos];
    memset(e, 0, sizeof(Evento));
    e->codigo = sistema->num_eventos + 1;
    e->status = ORCAMENTO;

    listarClientesView(sistema);
    formulario_novo_evento_basico(e);

    //recursos
    while (perguntar_se_adiciona_recurso()) {
        listarRecursosView(sistema);
        int cod, qtd;
        formulario_adicionar_recurso(&cod, &qtd);

        Recurso *r = NULL;
        for(int i=0; i < sistema->num_recursos; i++)
            if(sistema->lista_recursos[i].codigo == cod) r = &sistema->lista_recursos[i];

        if(r) {
            e->lista_recursos_alocados = realloc(e->lista_recursos_alocados, (e->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            e->lista_recursos_alocados[e->num_recursos_alocados].codigo_recurso = r->codigo;
            e->lista_recursos_alocados[e->num_recursos_alocados].quantidade = qtd;
            e->lista_recursos_alocados[e->num_recursos_alocados].custo_locacao_momento = r->valor_locacao;
            
            e->num_recursos_alocados++;
            e->custo_total_previsto += (r->valor_locacao * qtd);
            msg_recurso_adicionado();
        } else {
            msg_recurso_nao_encontrado();
        }
    }

    //fornecedor
    while (perguntar_se_adiciona_fornecedor()) {
        listarFornecedoresView(sistema);
        int cod;
        formulario_adicionar_fornecedor(&cod); 

        Fornecedor *f = NULL;
        for(int i=0; i < sistema->num_fornecedores; i++) {
            if(sistema->lista_fornecedores[i].codigo == cod) { f = &sistema->lista_fornecedores[i]; break; }
        }

        if(f) {
            float custo = f->valor_servico;
            e->lista_fornecedores_alocados = realloc(e->lista_fornecedores_alocados, (e->num_fornecedores_alocados + 1) * sizeof(ItemFornecedorEvento));
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].codigo_fornecedor = f->codigo;
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].valor_cobrado = custo;

            e->num_fornecedores_alocados++;
            e->custo_total_previsto += custo;
           
            printf(">> Fornecedor adicionado. Custo: %.2f\n", custo); 
        } else {
            msg_recurso_nao_encontrado();
        }
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    mensagem_sucesso("Evento criado com sucesso!");
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int id = pedir_id_evento("APROVAR");
    
    for(int i=0; i<sistema->num_eventos; i++) {
        if(sistema->lista_eventos[i].codigo == id) {
            if(sistema->lista_eventos[i].status == ORCAMENTO) {
                sistema->lista_eventos[i].status = APROVADO;
                salvarEventos(sistema);
                msg_evento_aprovado();
            } else {
                mensagem_erro("Evento nao esta em orcamento ou nao encontrado.");
            }
            return;
        }
    }
    mensagem_erro("Nao encontrado.");
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int cod = pedir_id_evento("FINALIZAR");
    if (cod == 0) return;

    for (int i = 0; i < sistema->num_eventos; i++) {
        Evento *e = &sistema->lista_eventos[i];
        if (e->codigo == cod) {
            
            if (e->status != APROVADO) {
                mensagem_erro("O evento deve estar APROVADO para ser finalizado.");
                return;
            }
            
            float valor_final_faturado = e->custo_total_previsto; 
            
            e->status = FINALIZADO;
            e->valor_final_faturado = valor_final_faturado;
            
            // Cria a transação
            Transacao t;
            memset(&t, 0, sizeof(Transacao));
            t.tipo = CONTA_A_RECEBER;
            t.status = PENDENTE;
            t.valor = valor_final_faturado;
            t.codigo_evento_ref = e->codigo;
            t.codigo_cliente_ref = e->codigo_cliente;
            sprintf(t.descricao, "Faturamento Evento: %s", e->nome_evento);
            strcpy(t.data_vencimento, e->data_fim); 

            registrarTransacao(sistema, t);
            
            salvarEventos(sistema);

            printf("\nEvento finalizado! Valor faturado: %.2f\n", valor_final_faturado);
            pausar();
            return;
        }
    }
    mensagem_erro("Evento nao encontrado.");
}