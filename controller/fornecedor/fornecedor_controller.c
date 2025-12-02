#include "fornecedor_controller.h"
#include <stdlib.h>
#include "model/fornecedor/fornecedor_model.h"
#include "view/fornecedor/fornecedor_view.h"
#include "view/cliente/cliente_view.h"

static int obterProximoIdFornecedor(Sistema *sistema) {
    int max = 0;
    for(int i=0; i<sistema->num_fornecedores; i++) {
        if(sistema->lista_fornecedores[i].codigo > max) max = sistema->lista_fornecedores[i].codigo;
    }
    return max + 1;
}

void adicionarFornecedorController(Sistema *sistema) {
    if (sistema->num_fornecedores == sistema->capacidade_fornecedores) {
        int nova_cap = (sistema->capacidade_fornecedores == 0) ? 10 : sistema->capacidade_fornecedores * 2;
        Fornecedor *temp = realloc(sistema->lista_fornecedores, nova_cap * sizeof(Fornecedor));
        if (!temp) { mensagem_erro("Memoria insuficiente"); return; }
        sistema->lista_fornecedores = temp;
        sistema->capacidade_fornecedores = nova_cap;
    }

    Fornecedor *novo = &sistema->lista_fornecedores[sistema->num_fornecedores];
    novo->codigo = obterProximoIdFornecedor(sistema);

    formulario_novo_fornecedor(novo);

    sistema->num_fornecedores++;
    salvarFornecedores(sistema);
    mensagem_sucesso("Fornecedor cadastrado!");
}

void alterarFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo = pedir_id_fornecedor("alterar");
    int indice = -1;

    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { mensagem_erro("Nao encontrado."); return; }

    int op;
    do {
        op = menu_alterar_fornecedor(&sistema->lista_fornecedores[indice]);
    } while(op != 0);
    
    salvarFornecedores(sistema);
}

void excluirFornecedorController(Sistema *sistema) {
    listarFornecedoresView(sistema);
    if (sistema->num_fornecedores == 0) return;

    int codigo = pedir_id_fornecedor("excluir");
    for (int i = 0; i < sistema->num_fornecedores; i++) {
        if (sistema->lista_fornecedores[i].codigo == codigo) {
            if(confirmar_exclusao_fornecedor(sistema->lista_fornecedores[i].nome_fantasia)) {
                sistema->lista_fornecedores[i] = sistema->lista_fornecedores[sistema->num_fornecedores - 1];
                sistema->num_fornecedores--;
                salvarFornecedores(sistema);
                mensagem_sucesso("Excluido!");
            }
            return;
        }
    }
    mensagem_erro("Nao encontrado.");
}