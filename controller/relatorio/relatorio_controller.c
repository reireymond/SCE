#include "relatorio_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view/relatorio/relatorio_view.h"
#include "utils/utils.h"

static long long data_num(char *data) {
    if(strcmp(data, "0") == 0) return 0;
    int d, m, a;
    sscanf(data, "%d/%d/%d", &d, &m, &a);
    return (long long)a*10000 + m*100 + d;
}
void relatorio_clientes(Sistema *s) {
    int min, max, ordem;
    form_filtro_cliente(&min, &max, &ordem);
    int tipo = perguntarTipoSaida();

    FILE *f = NULL;
    if(tipo == 2) {
        f = fopen("relatorio_clientes.csv", "w");
        if(!f) { msg_erro_arquivo(); return; }
        fprintf(f, "%-6s| %-30s| %-18s| %-15s| %s\n", "CODIGO", "NOME", "CNPJ", "TELEFONE", "EMAIL"); //CSV
    } else {
        cabecalho_cliente();
    }
    Cliente *lista = malloc(s->num_clientes * sizeof(Cliente));
    memcpy(lista, s->lista_clientes, s->num_clientes * sizeof(Cliente));

    if(ordem == 1) {
        for(int i=0; i < s->num_clientes-1; i++) {
            for(int j=0; j < s->num_clientes-i-1; j++) {
                if(strcmp(lista[j].razao_social, lista[j+1].razao_social) > 0) {
                    Cliente temp = lista[j];
                    lista[j] = lista[j+1];
                    lista[j+1] = temp;
                }
            }
        }
    }

    for(int i=0; i < s->num_clientes; i++) {
        Cliente *c = &lista[i];
        
        // filtros
        if(min > 0 && c->codigo < min) continue;
        if(max > 0 && c->codigo > max) continue;

        if(tipo == 2) {
            fprintf(f, "%-6d| %-30s| %-18s| %-15s| %s\n", c->codigo, c->razao_social, c->cnpj, c->telefone, c->email);
        } else {
            linha_cliente(c);
        }
    }

    free(lista);
    if(tipo == 2) { fclose(f); msg_arquivo_salvo("relatorio_clientes.csv"); }
    else msg_relatorio_concluido();
}

void relatorio_eventos(Sistema *s) {
    int cod_cli, status_op;
    char ini[15], fim[15];
    form_filtro_eventos(&cod_cli, ini, fim, &status_op);
    int tipo = perguntarTipoSaida();

    FILE *f = NULL;
    if(tipo == 2) {
        f = fopen("relatorio_eventos.csv", "w");
        if(!f) { msg_erro_arquivo(); return; }
        fprintf(f, "%-5s| %-25s| %-8s| %-12s| %-12s| %-12s| %s\n", "ID", "EVENTO", "CLIENTE", "INICIO", "FIM", "STATUS", "VALOR");
    } else {
        cabecalho_evento();
    }

    long long d_ini = data_num(ini);
    long long d_fim = data_num(fim);

    for(int i=0; i < s->num_eventos; i++) {
        Evento *e = &s->lista_eventos[i];

        // Filtros
        if(cod_cli > 0 && e->codigo_cliente != cod_cli) continue;
        
        if(status_op > 0) {
            if(status_op == 1 && e->status != ORCAMENTO) continue;
            if(status_op == 2 && e->status != APROVADO) continue;
            if(status_op == 3 && e->status != FINALIZADO) continue;
        }

        if(d_ini > 0 || d_fim > 0) {
            long long d_evento = data_num(e->data_inicio);
            if(d_ini > 0 && d_evento < d_ini) continue;
            if(d_fim > 0 && d_evento > d_fim) continue;
        }

        char texto_status[20];
        if(e->status == ORCAMENTO) strcpy(texto_status, "Orcamento");
        else if(e->status == APROVADO) strcpy(texto_status, "Aprovado");
        else if(e->status == FINALIZADO) strcpy(texto_status, "Finalizado");
        else strcpy(texto_status, "Cancelado");

        if(tipo == 2) {
            fprintf(f, "%-5d| %-25s| %-8d| %-12s| %-12s| %-12s| %.2f\n", e->codigo, e->nome_evento, e->codigo_cliente, e->data_inicio, e->data_fim, texto_status, e->custo_total_previsto);
        } else {
            linha_evento(e);
        }
    }

    if(tipo == 2) { fclose(f); msg_arquivo_salvo("relatorio_eventos.csv"); }
    else msg_relatorio_concluido();
}

void relatorio_cronograma(Sistema *s) {
    int cod_rec;
    char ini[15], fim[15];
    form_filtro_cronograma(&cod_rec, ini, fim);
    int tipo = perguntarTipoSaida();

    FILE *f = NULL;
    if(tipo == 2) {
        f = fopen("cronograma.csv", "w");
        if(!f) { msg_erro_arquivo(); return; }
        fprintf(f, "%-25s| %-12s| %-8s| %s\n", "RECURSO", "DATA", "HORA", "EVENTO");
    } else {
        cabecalho_cronograma();
    }

    long long d_ini = data_num(ini);
    long long d_fim = data_num(fim);

    // percorre eventos e seus recursos alocados
    for(int i=0; i < s->num_eventos; i++) {
        Evento *e = &s->lista_eventos[i];
        
        // filtra data do evento
        long long d_evento = data_num(e->data_inicio);
        if(d_ini > 0 && d_evento < d_ini) continue;
        if(d_fim > 0 && d_evento > d_fim) continue;

        for(int j=0; j < e->num_recursos_alocados; j++) {
            int r_id = e->lista_recursos_alocados[j].codigo_recurso;
            
            if(cod_rec > 0 && r_id != cod_rec) continue;

            // busca nome do recurso
            char nome_rec[100]; strcpy(nome_rec, "Desconhecido");
            for(int k=0; k<s->num_recursos; k++) {
                if(s->lista_recursos[k].codigo == r_id) strcpy(nome_rec, s->lista_recursos[k].descricao);
            }

            if(tipo == 2) {
                fprintf(f, "%-25s| %-12s| %-8s| %s\n", nome_rec, e->data_inicio, e->hora_inicio, e->nome_evento);
            } else {
                linha_cronograma(nome_rec, e->data_inicio, e->hora_inicio, e->nome_evento);
            }
        }
    }

    if(tipo == 2) { fclose(f); msg_arquivo_salvo("cronograma.csv"); }
    else msg_relatorio_concluido();
}

void relatorio_equipamentos(Sistema *s) {
    int min, max;
    char cat[50];
    form_filtro_equipamentos(&min, &max, cat);
    int tipo = perguntarTipoSaida();

    FILE *f = NULL;
    if(tipo == 2) {
        f = fopen("relatorio_equipamentos.csv", "w");
        if(!f) { msg_erro_arquivo(); return; }
        fprintf(f, "%-5s| %-30s| %-20s| %-10s| %s\n", "ID", "DESCRICAO", "CATEGORIA", "ESTOQUE", "VALOR");
    } else {
        cabecalho_equipamento();
    }

    for(int i=0; i < s->num_recursos; i++) {
        Recurso *r = &s->lista_recursos[i];

        if(min > 0 && r->codigo < min) continue;
        if(max > 0 && r->codigo > max) continue;
        if(strcmp(cat, "0") != 0 && strcmp(cat, r->categoria) != 0) continue;

        if(tipo == 2) {
            fprintf(f, "%-5d| %-30s| %-20s| %-10d| %.2f\n", r->codigo, r->descricao, r->categoria, r->quantidade_estoque, r->valor_locacao);
        } else {
            linha_equipamento(r);
        }
    }

    if(tipo == 2) { fclose(f); msg_arquivo_salvo("relatorio_equipamentos.csv"); }
    else msg_relatorio_concluido();
}

void relatorio_financeiro(Sistema *s, TipoTransacao tipo_filtro) {
    int cod_ent;
    char ini[15], fim[15];
    form_filtro_financeiro(&cod_ent, ini, fim);
    int tipo = perguntarTipoSaida();

    FILE *f = NULL;
    char nome_arq[50];
    if(tipo_filtro == CONTA_A_RECEBER) strcpy(nome_arq, "contas_receber.csv");
    else if(tipo_filtro == CONTA_A_PAGAR) strcpy(nome_arq, "contas_pagar.csv");
    else strcpy(nome_arq, "caixa.csv");

    if(tipo == 2) {
        f = fopen(nome_arq, "w");
        if(!f) { msg_erro_arquivo(); return; }
        fprintf(f, "%-5s| %-12s| %-12s| %-10s| %s\n", "ID", "VENCIMENTO", "VALOR", "STATUS", "DESCRICAO");
    } else {
        cabecalho_financeiro();
    }

    long long d_ini = data_num(ini);
    long long d_fim = data_num(fim);

    for(int i=0; i < s->num_transacoes; i++) {
        Transacao *t = &s->lista_transacoes[i];

        if(tipo_filtro != MOVIMENTACAO_CAIXA && t->tipo != tipo_filtro) continue;

        if(cod_ent > 0 && t->codigo_cliente_ref != cod_ent) continue;

        long long d_venc = data_num(t->data_vencimento);
        if(d_ini > 0 && d_venc < d_ini) continue;
        if(d_fim > 0 && d_venc > d_fim) continue;

        char st[20];
        if(t->status == PAGA) strcpy(st, "PAGO");
        else strcpy(st, "PENDENTE");

        if(tipo == 2) {
            fprintf(f, "%-5d| %-12s| %-12.2f| %-10s| %s\n", t->codigo, t->data_vencimento, t->valor, st, t->descricao);
        } else {
            linha_financeiro(t);
        }
    }

    if(tipo == 2) { fclose(f); msg_arquivo_salvo(nome_arq); }
    else msg_relatorio_concluido();
}


void menuRelatoriosController(Sistema *sistema) {
    int op;
    do {
        op = menuRelatoriosView();

        switch(op) {
            case 1: relatorio_clientes(sistema); break;
            case 2: relatorio_eventos(sistema); break;
            case 3: relatorio_cronograma(sistema); break;
            case 4: relatorio_equipamentos(sistema); break;
            case 5: relatorio_financeiro(sistema, CONTA_A_RECEBER); break;
            case 6: relatorio_financeiro(sistema, CONTA_A_PAGAR); break;
            case 7: relatorio_financeiro(sistema, MOVIMENTACAO_CAIXA); break;
            case 0: break;
            default: msg_opcao_invalida(); break;
        }
    } while(op != 0);
}