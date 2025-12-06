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

//transformar data e hora num numero grande
long long converteData(char *data, char *hora) {
    int d, m, a, h, min;
    sscanf(data, "%d/%d/%d", &d, &m, &a);
    sscanf(hora, "%d:%d", &h, &min);
    
    // Matemática basica pra juntar tudo
    long long resultado = (long long)a * 100000000; 
    resultado += m * 1000000;
    resultado += d * 10000;
    resultado += h * 100;
    resultado += min;
    
    return resultado;
}

int verificar_conflitos(Sistema *sistema, Evento *e_atual) {
    // Pega o tempo do evento atual
    long long inicio_atual = converteData(e_atual->data_inicio, e_atual->hora_inicio);
    long long fim_atual = converteData(e_atual->data_fim, e_atual->hora_fim);

    // verificar se tem recurso
    for (int i = 0; i < e_atual->num_recursos_alocados; i++) {
        int id_rec = e_atual->lista_recursos_alocados[i].codigo_recurso;
        int qtd_precisa = e_atual->lista_recursos_alocados[i].quantidade;
        int qtd_usada_outros = 0;
        
        // estoque total do recurso
        int estoque = 0;
        for(int k=0; k < sistema->num_recursos; k++) {
            if(sistema->lista_recursos[k].codigo == id_rec) {
                estoque = sistema->lista_recursos[k].quantidade_estoque;
            }
        }

        // olha todos os outros eventos pra ver se batem o horario
        for (int j = 0; j < sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            
            // Pula se for o mesmo ou nao tiver aprovado
            if (outro->codigo == e_atual->codigo) continue;
            if (outro->status != APROVADO) continue;

            long long inicio_outro = converteData(outro->data_inicio, outro->hora_inicio);
            long long fim_outro = converteData(outro->data_fim, outro->hora_fim);

            //se um comeca antes do outro terminar
            if (inicio_atual < fim_outro && inicio_outro < fim_atual) {
                // se bater verifica a disponibilidadae de recursos
                for(int x=0; x < outro->num_recursos_alocados; x++) {
                    if(outro->lista_recursos_alocados[x].codigo_recurso == id_rec) {
                        qtd_usada_outros = qtd_usada_outros + outro->lista_recursos_alocados[x].quantidade;
                    }
                }
            }
        }

        if (qtd_precisa + qtd_usada_outros > estoque) {
            msg_conflito_estoque(id_rec, estoque, qtd_usada_outros);
            return 0;
        }
    }

    //faz o mesmo processo para equipe
    for (int i = 0; i < e_atual->num_equipe_alocada; i++) {
        int id_func = e_atual->lista_equipe_alocada[i].codigo_equipe;

        for (int j = 0; j < sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            if (outro->codigo == e_atual->codigo) continue;
            if (outro->status != APROVADO) continue;

            long long inicio_outro = converteData(outro->data_inicio, outro->hora_inicio);
            long long fim_outro = converteData(outro->data_fim, outro->hora_fim);

            if (inicio_atual < fim_outro && inicio_outro < fim_atual) {
                // olha se o funcionario ta nesse evento
                for(int x=0; x < outro->num_equipe_alocada; x++) {
                    if(outro->lista_equipe_alocada[x].codigo_equipe == id_func) {
                        msg_conflito_equipe(id_func, outro->nome_evento);
                        return 0;
                    }
                }
            }
        }
    }
    // faz o mesmo processo para o fornecedor
    for (int i = 0; i < e_atual->num_fornecedores_alocados; i++) {
        int id_forn = e_atual->lista_fornecedores_alocados[i].codigo_fornecedor;

        for (int j = 0; j < sistema->num_eventos; j++) {
            Evento *outro = &sistema->lista_eventos[j];
            if (outro->codigo == e_atual->codigo) continue;
            if (outro->status != APROVADO) continue;

            long long inicio_outro = converteData(outro->data_inicio, outro->hora_inicio);
            long long fim_outro = converteData(outro->data_fim, outro->hora_fim);

            if (inicio_atual < fim_outro && inicio_outro < fim_atual) {
                // Verifica se o fornecedor ta nesse evento
                for(int x=0; x < outro->num_fornecedores_alocados; x++) {
                    if(outro->lista_fornecedores_alocados[x].codigo_fornecedor == id_forn) {
                        msg_conflito_fornecedor(id_forn, outro->nome_evento);
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

void adicionarEventoController(Sistema *sistema) {
    if (sistema->num_clientes == 0) {
        mensagem_erro("Sem clientes cadastrados."); 
        return; 
    }
    
    // Aumenta memoria caso precise
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova * sizeof(Evento));
        sistema->capacidade_eventos = nova;
    }

    Evento *e = &sistema->lista_eventos[sistema->num_eventos];
    memset(e, 0, sizeof(Evento));
    e->codigo = sistema->num_eventos + 1;
    e->status = ORCAMENTO;

    listarClientesView(sistema);
    formulario_novo_evento_basico(e);

    // recursos
    while (perguntar_se_adiciona_recurso()) {
        listarRecursosView(sistema);
        int cod, qtd;
        formulario_adicionar_recurso(&cod, &qtd);

        // busca manual
        Recurso *r = NULL;
        for(int i=0; i < sistema->num_recursos; i++) {
            if(sistema->lista_recursos[i].codigo == cod) r = &sistema->lista_recursos[i];
        }

        if(r) {
            e->lista_recursos_alocados = realloc(e->lista_recursos_alocados, (e->num_recursos_alocados + 1) * sizeof(ItemRecursoEvento));
            e->lista_recursos_alocados[e->num_recursos_alocados].codigo_recurso = r->codigo;
            e->lista_recursos_alocados[e->num_recursos_alocados].quantidade = qtd;
            e->lista_recursos_alocados[e->num_recursos_alocados].custo_locacao_momento = r->valor_locacao;
            
            e->num_recursos_alocados++;
            e->custo_total_previsto = e->custo_total_previsto + (r->valor_locacao * qtd);
            msg_recurso_adicionado();
        } else {
            msg_recurso_nao_encontrado();
        }
    }

    //eEquipe
    while (perguntar_se_adiciona_equipe()) {
        int cod;
        listarEquipeInternaView(sistema);//lista a equipe cadastrada
        formulario_adicionar_equipe(&cod); 

        EquipeInterna *eq = NULL;
        for(int i=0; i < sistema->num_equipe; i++) {
            if(sistema->lista_equipe[i].codigo == cod) eq = &sistema->lista_equipe[i];
        }

        if(eq) {
            e->lista_equipe_alocada = realloc(e->lista_equipe_alocada, (e->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            e->lista_equipe_alocada[e->num_equipe_alocada].codigo_equipe = eq->codigo;
            e->lista_equipe_alocada[e->num_equipe_alocada].custo_diaria_momento = eq->valor_diaria;
            
            e->num_equipe_alocada++;
            e->custo_total_previsto = e->custo_total_previsto + eq->valor_diaria;
            msg_membro_equipe_adicionado();
        } else {
            msg_membro_nao_encontrado();
        }
    }

    //fornecedores
    while (perguntar_se_adiciona_fornecedor()) {
        listarFornecedoresView(sistema);
        int cod;
        formulario_adicionar_fornecedor(&cod); 

        Fornecedor *f = NULL;
        for(int i=0; i < sistema->num_fornecedores; i++) {
            if(sistema->lista_fornecedores[i].codigo == cod) f = &sistema->lista_fornecedores[i];
        }

        if(f) {
            
            float custo = f->valor_servico;
            e->lista_fornecedores_alocados = realloc(e->lista_fornecedores_alocados, (e->num_fornecedores_alocados + 1) * sizeof(ItemFornecedorEvento));
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].codigo_fornecedor = f->codigo;
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].valor_cobrado = custo;

            e->num_fornecedores_alocados++;
            e->custo_total_previsto = e->custo_total_previsto + custo;
           
            msg_fornecedor_adicionado_sucesso(f->nome_fantasia, custo);
        } else {
            msg_recurso_nao_encontrado();
        }
    }

    sistema->num_eventos++;
    salvarEventos(sistema);
    mensagem_sucesso("Orcamento criado!");
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int id = pedir_id_evento("APROVAR");
    
    for(int i=0; i<sistema->num_eventos; i++) {
        if(sistema->lista_eventos[i].codigo == id) {
            
            if(sistema->lista_eventos[i].status == ORCAMENTO) {
                msg_verificando_disponibilidade();
                
                // funcao que verifica tudo de uma vez
                if (verificar_conflitos(sistema, &sistema->lista_eventos[i])) {
                    sistema->lista_eventos[i].status = APROVADO;
                    salvarEventos(sistema);
                    msg_evento_aprovado();
                } else {
                    msg_erro_aprovacao_conflito();
                }

            } else {
                mensagem_erro("Evento nao esta em orcamento.");
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
                mensagem_erro("Tem que estar APROVADO pra finalizar.");
                return;
            }
            
            //valor previsto
            float valor_final = e->custo_total_previsto;
            
            e->status = FINALIZADO;
            e->valor_final_faturado = valor_final;
            
            // cria a conta a receber
            Transacao t;
            memset(&t, 0, sizeof(Transacao));
            t.tipo = CONTA_A_RECEBER;
            t.status = PENDENTE;
            t.valor = valor_final;
            t.codigo_evento_ref = e->codigo;
            t.codigo_cliente_ref = e->codigo_cliente;
            sprintf(t.descricao, "Faturamento Evento: %s", e->nome_evento);
            strcpy(t.data_vencimento, e->data_fim); 

            registrarTransacao(sistema, t);
            salvarEventos(sistema);

            msg_evento_finalizado_sucesso(valor_final);
            return;
        }
    }
    mensagem_erro("Evento nao achado.");
}
