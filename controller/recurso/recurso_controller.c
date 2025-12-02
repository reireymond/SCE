#include "recurso_controller.h"
#include <stdlib.h>
#include "model/recurso/recurso_model.h"
#include "view/recurso/recurso_view.h"

static int obterProximoIdRecurso(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_recursos; i++) {
        if(sistema->lista_recursos[i].codigo > max) max = sistema->lista_recursos[i].codigo;
    }
    return max + 1;
}

void adicionarRecursoController(Sistema *sistema) {
    if (sistema->num_recursos == sistema->capacidade_recursos) {
        int nova_cap = (sistema->capacidade_recursos == 0) ? 10 : sistema->capacidade_recursos * 2;
        Recurso *temp = realloc(sistema->lista_recursos, nova_cap * sizeof(Recurso));
        if (!temp) { mensagem_erro("Erro memoria"); return; }
        sistema->lista_recursos = temp;
        sistema->capacidade_recursos = nova_cap;
    }

    Recurso *novo = &sistema->lista_recursos[sistema->num_recursos];
    
    novo->codigo = obterProximoIdRecurso(sistema);
    formulario_novo_recurso(novo);

    sistema->num_recursos++;
    salvarRecursos(sistema);
    mensagem_sucesso("Recurso salvo!");
}

void alterarRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo = pedir_id_recurso("alterar");
    int indice = -1;

    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { mensagem_erro("Recurso nao encontrado."); return; }

    int op;
    do {
        op = menu_alterar_recurso(&sistema->lista_recursos[indice]);
    } while(op != 0);

    salvarRecursos(sistema);
}

void excluirRecursoController(Sistema *sistema) {
    listarRecursosView(sistema);
    if (sistema->num_recursos == 0) return;

    int codigo = pedir_id_recurso("excluir");
    
    for (int i = 0; i < sistema->num_recursos; i++) {
        if (sistema->lista_recursos[i].codigo == codigo) {
            
            if (confirmar_exclusao_recurso(sistema->lista_recursos[i].descricao)) {
                sistema->lista_recursos[i] = sistema->lista_recursos[sistema->num_recursos - 1];
                sistema->num_recursos--;
                salvarRecursos(sistema);
                mensagem_sucesso("Excluido.");
            }
            return;
        }
    }
    mensagem_erro("Nao encontrado.");
}