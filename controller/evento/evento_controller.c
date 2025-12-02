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
    sistema->num_eventos++;
    salvarEventos(sistema);
}

void alterarStatusEventoController(Sistema *sistema) {
    listarEventosView(sistema);
    int id = pedir_id_evento("APROVAR");
}