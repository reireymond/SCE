#include "evento_controller.h"
#include <stdlib.h>
#include <string.h>
#include "model/evento/evento_model.h"
#include "view/evento/evento_view.h"
#include "view/recurso/recurso_view.h"
#include "view/cliente/cliente_view.h"
void adicionarEventoController(Sistema *sistema) {

    if (sistema->num_clientes == 0) {
        printf("Sem clientes.\n"); return; 
    }
    if (sistema->num_eventos == sistema->capacidade_eventos) {
        
    }

    Evento *e = &sistema->lista_eventos[sistema->num_eventos];
    memset(e, 0, sizeof(Evento));
    e->codigo = sistema->num_eventos + 1;
    e->status = ORCAMENTO;

    listarClientesView(sistema);
    formulario_novo_evento_basico(e);

    while (perguntar_se_adiciona_fornecedor()) {
        listarFornecedoresView(sistema);
        
        int cod;
        formulario_adicionar_fornecedor(&cod);

        Fornecedor *f = NULL;
        for(int i=0; i < sistema->num_fornecedores; i++) {
            if(sistema->lista_fornecedores[i].codigo == cod) {
                f = &sistema->lista_fornecedores[i];
                break;
            }
        }

        if(f) {
            // AUTOMÁTICO: Pega o valor do cadastro do fornecedor
            float custo_automatico = f->valor_servico; 

            e->lista_fornecedores_alocados = realloc(e->lista_fornecedores_alocados, (e->num_fornecedores_alocados + 1) * sizeof(ItemFornecedorEvento));
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].codigo_fornecedor = f->codigo;
            e->lista_fornecedores_alocados[e->num_fornecedores_alocados].valor_cobrado = custo_automatico;
            e->num_fornecedores_alocados++;
            e->custo_total_previsto += custo_automatico;
            msg_fornecedor_adicionado_sucesso(f->nome_fantasia, custo_automatico);
        } else {
            msg_recurso_nao_encontrado();
        }
    }
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int id = pedir_id_evento("APROVAR");
}