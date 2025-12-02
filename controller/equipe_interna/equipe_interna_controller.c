#include "equipe_interna_controller.h"
#include <stdlib.h>
#include "model/equipe_interna/equipe_interna_model.h"
#include "view/equipe_interna/equipe_interna_view.h"
#include "view/cliente/cliente_view.h"

static int obterProximoIdEquipe(Sistema *sistema) {
    int max = 0;
    for(int i=0; i < sistema->num_equipe; i++) {
        if(sistema->lista_equipe[i].codigo > max) max = sistema->lista_equipe[i].codigo;
    }
    return max + 1;
}

void adicionarEquipeInternaController(Sistema *sistema) {
    // Adiciona um novo membro da equipe interna ao sistema.
    if (sistema->num_equipe == sistema->capacidade_equipe) {
        int nova_cap = (sistema->capacidade_equipe == 0) ? 10 : sistema->capacidade_equipe * 2;
        EquipeInterna *temp = realloc(sistema->lista_equipe, nova_cap * sizeof(EquipeInterna));
        
        if (!temp) { 
            mensagem_erro("Memoria insuficiente para cadastrar novo membro."); 
            return; 
        }
        
        sistema->lista_equipe = temp;
        sistema->capacidade_equipe = nova_cap;
    }

    EquipeInterna *novo = &sistema->lista_equipe[sistema->num_equipe];
    novo->codigo = obterProximoIdEquipe(sistema);

    formulario_novo_equipe(novo);

    sistema->num_equipe++;
    salvarEquipeInterna(sistema);
    
    mensagem_sucesso("Membro da Equipe cadastrado!");
}

void alterarEquipeInternaController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo = pedir_id_equipe("alterar");
    int indice = -1;

    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) { 
            indice = i; 
            break; 
        }
    }

    if (indice == -1) { 
        mensagem_erro("Membro da Equipe nao encontrado."); 
        return; 
    }

    int op;
    do {
        op = menu_alterar_equipe(&sistema->lista_equipe[indice]);
    } while(op != 0);
    
    salvarEquipeInterna(sistema);
    mensagem_sucesso("Dados do Membro atualizados.");
}

void excluirEquipeInternaController(Sistema *sistema) {
    listarEquipeInternaView(sistema);
    if (sistema->num_equipe == 0) return;

    int codigo = pedir_id_equipe("excluir");
    
    for (int i = 0; i < sistema->num_equipe; i++) {
        if (sistema->lista_equipe[i].codigo == codigo) {
            
            if (confirmar_exclusao_equipe(sistema->lista_equipe[i].nome)) {
                sistema->lista_equipe[i] = sistema->lista_equipe[sistema->num_equipe - 1];
                sistema->num_equipe--;
                salvarEquipeInterna(sistema);
                mensagem_sucesso("Membro da Equipe excluido.");
            }
            return;
        }
    }
    mensagem_erro("Membro da Equipe nao encontrado.");
}