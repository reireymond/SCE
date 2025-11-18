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
#include "controller/transacao/transacao_controller.h" // Para cobrar no final

// Funcao auxiliar para comparar datas (AAAAMMDD)
int dataParaInteiro(char* data) {
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    return (ano * 10000) + (mes * 100) + dia;
}

void adicionarEventoController(Sistema *sistema) {
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        int nova_cap = (sistema->capacidade_eventos == 0) ? 10 : sistema->capacidade_eventos * 2;
        sistema->lista_eventos = realloc(sistema->lista_eventos, nova_cap * sizeof(Evento));
        sistema->capacidade_eventos = nova_cap;
    }

    Evento *novo = &sistema->lista_eventos[sistema->num_eventos];
    memset(novo, 0, sizeof(Evento)); // Limpa lixo de memoria
    novo->codigo = sistema->num_eventos + 1;
    novo->status = ORCAMENTO;

    printf("\n--- Novo Orcamento ---\n");
    printf("Nome do Evento: ");
    ler_texto_valido(novo->nome_evento, sizeof(novo->nome_evento), VALIDAR_NAO_VAZIO);

    listarClientesView(sistema);
    printf("Codigo do Cliente: ");
    ler_inteiro_valido(&novo->codigo_cliente, 1, 99999);

    printf("Data Inicio (DD/MM/AAAA): ");
    ler_texto_valido(novo->data_inicio, sizeof(novo->data_inicio), VALIDAR_DATA);

    printf("Data Fim (DD/MM/AAAA): ");
    ler_texto_valido(novo->data_fim, sizeof(novo->data_fim), VALIDAR_DATA);

    printf("Local: ");
    ler_texto_valido(novo->local, sizeof(novo->local), VALIDAR_NAO_VAZIO);

    // 1. Adicionar Recursos
    int op = 0;
    printf("\nAdicionar Recursos? (1-Sim, 0-Nao): ");
    scanf("%d", &op);
    while(op == 1) {
        listarRecursosView(sistema);
        int cod, qtd;
        printf("Codigo do Recurso: "); scanf("%d", &cod);
        
        // Busca recurso
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

    // 2. Adicionar Equipe
    printf("\nAdicionar Equipe? (1-Sim, 0-Nao): ");
    scanf("%d", &op);
    while(op == 1) {
        listarEquipeInternaView(sistema);
        int cod;
        printf("Codigo do Membro: "); scanf("%d", &cod);

        EquipeInterna *e = NULL;
        for(int i=0; i<sistema->num_equipe; i++) 
            if(sistema->lista_equipe[i].codigo == cod) e = &sistema->lista_equipe[i];

        if(e) {
            novo->lista_equipe_alocada = realloc(novo->lista_equipe_alocada, (novo->num_equipe_alocada + 1) * sizeof(ItemEquipeEvento));
            ItemEquipeEvento *item = &novo->lista_equipe_alocada[novo->num_equipe_alocada++];
            item->codigo_equipe = e->codigo;
            item->custo_diaria_momento = e->valor_diaria;
            
            novo->custo_total_previsto += e->valor_diaria; // Simplificado (1 diaria)
        } else printf("Nao encontrado.\n");

        printf("Mais equipe? (1-Sim, 0-Nao): "); scanf("%d", &op);
    }

    // 3. Adicionar Servicos
    printf("\nAdicionar Servicos (Fornecedores)? (1-Sim, 0-Nao): ");
    scanf("%d", &op);
    while(op == 1) {
        listarFornecedoresView(sistema);
        int cod;
        float valor;
        char desc[100];
        printf("Codigo do Fornecedor: "); scanf("%d", &cod);
        limpar_buffer();
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

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nCodigo do evento para aprovar: ");
    scanf("%d", &codigo);

    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { printf("Nao encontrado.\n"); return; }

    Evento *evento = &sistema->lista_eventos[indice];

    if (evento->status == ORCAMENTO) {
        printf("1. Aprovar Orcamento\n0. Cancelar\nEscolha: ");
        int op; scanf("%d", &op);

        if (op == 1) {
            // VALIDACAO DE DATA E ESTOQUE
            printf("Verificando estoque...\n");
            int conflito = 0;
            int ini1 = dataParaInteiro(evento->data_inicio);
            int fim1 = dataParaInteiro(evento->data_fim);

            for(int i=0; i<evento->num_recursos_alocados; i++) {
                int codRec = evento->lista_recursos_alocados[i].codigo_recurso;
                int qtdPedida = evento->lista_recursos_alocados[i].quantidade;
                
                // Buscar estoque total
                int estoqueTotal = 0;
                for(int k=0; k<sistema->num_recursos; k++)
                    if(sistema->lista_recursos[k].codigo == codRec) 
                        estoqueTotal = sistema->lista_recursos[k].quantidade_estoque;

                // Verificar ocupacao
                int qtdOcupada = 0;
                for(int j=0; j<sistema->num_eventos; j++) {
                    Evento *outro = &sistema->lista_eventos[j];
                    if(outro->status == APROVADO && outro->codigo != evento->codigo) {
                        int ini2 = dataParaInteiro(outro->data_inicio);
                        int fim2 = dataParaInteiro(outro->data_fim);
                        
                        // Se datas sobrepoem
                        if(ini1 <= fim2 && fim1 >= ini2) {
                            for(int r=0; r<outro->num_recursos_alocados; r++)
                                if(outro->lista_recursos_alocados[r].codigo_recurso == codRec)
                                    qtdOcupada += outro->lista_recursos_alocados[r].quantidade;
                        }
                    }
                }

                if (qtdPedida > (estoqueTotal - qtdOcupada)) {
                    printf("ERRO: Recurso ID %d sem estoque para a data!\n", codRec);
                    conflito = 1;
                    break;
                }
            }

            if(conflito == 0) {
                evento->status = APROVADO;
                salvarEventos(sistema);
                printf("Evento APROVADO e recursos reservados!\n");
            }
        }
    } else {
        printf("Este evento nao esta em orcamento.\n");
    }
}

void finalizarEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    if (sistema->num_eventos == 0) return;

    int codigo, indice = -1;
    printf("\nCodigo do evento para finalizar: ");
    scanf("%d", &codigo);

    for (int i = 0; i < sistema->num_eventos; i++) {
        if (sistema->lista_eventos[i].codigo == codigo) { indice = i; break; }
    }

    if (indice != -1) {
        Evento *e = &sistema->lista_eventos[indice];
        if (e->status == APROVADO) {
            printf("Custo Previsto: %.2f\n", e->custo_total_previsto);
            printf("Valor Final Faturado (Cobrar Cliente): ");
            ler_float_positivo(&e->valor_final_faturado);
            
            e->status = FINALIZADO;
            
            // Gera cobranca automatica
            adicionarTransacao(sistema, CONTA_A_RECEBER, "Fatura Evento", e->valor_final_faturado, "30/12/2025");
            
            salvarEventos(sistema);
            printf("Evento finalizado e cobranca gerada!\n");
        } else {
            printf("Evento precisa estar APROVADO para finalizar.\n");
        }
    }
}

void detalharEventoController(Sistema *sistema) {
    // Podes manter a tua versao anterior ou simplificar, 
    // esta funcao e apenas visualizacao.
    listarEventosView(sistema);
}