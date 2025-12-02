#include "operador_controller.h"
#include <stdlib.h>
#include "model/operador/operador_model.h"
#include "view/operador/operador_view.h"
#include "view/cliente/cliente_view.h"

static int obterProximoIdOperador(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_operadores; i++) {
        if(sistema->lista_operadores[i].codigo > max) max = sistema->lista_operadores[i].codigo;
    }
    return max + 1;
}

void adicionarOperadorController(Sistema *sistema) {
    // Adiciona um novo operador ao sistema.
    if (sistema->num_operadores == sistema->capacidade_operadores) {
        int nova_cap = (sistema->capacidade_operadores == 0) ? 10 : sistema->capacidade_operadores * 2;
        Operador *temp = realloc(sistema->lista_operadores, nova_cap * sizeof(Operador));
        if (!temp) { 
            mensagem_erro("Memoria insuficiente."); 
            return; 
        }
        sistema->lista_operadores = temp;
        sistema->capacidade_operadores = nova_cap;
    }

    Operador *novo = &sistema->lista_operadores[sistema->num_operadores];
    novo->codigo = obterProximoIdOperador(sistema); 

    formulario_novo_operador(novo);

    sistema->num_operadores++;
    salvarOperadores(sistema);
    mensagem_sucesso("Operador cadastrado!");
}

void alterarOperadorController(Sistema *sistema) {
    // Altera dados de um operador existente.
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;
    
    int codigo = pedir_id_operador("alterar");
    int indice = -1;
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) { indice = i; break; }
    }
    
    if (indice == -1) { 
        mensagem_erro("Operador nao encontrado."); 
        return; 
    }

    int op;
    do {
        op = menu_alterar_operador(&sistema->lista_operadores[indice]);
    } while(op != 0);
    
    salvarOperadores(sistema);
    mensagem_sucesso("Dados atualizados.");
}

void excluirOperadorController(Sistema *sistema) {
    // Exclui um operador do sistema.
    listarOperadoresView(sistema);
    if (sistema->num_operadores == 0) return;
    
    int codigo = pedir_id_operador("excluir");
    
    for (int i = 0; i < sistema->num_operadores; i++) {
        if (sistema->lista_operadores[i].codigo == codigo) {
            if (confirmar_exclusao_operador(sistema->lista_operadores[i].usuario)) {
                sistema->lista_operadores[i] = sistema->lista_operadores[sistema->num_operadores - 1];
                sistema->num_operadores--;
                salvarOperadores(sistema);
                mensagem_sucesso("Operador excluido!");
            }
            return;
        }
    }
    mensagem_erro("Nao encontrado.");
}