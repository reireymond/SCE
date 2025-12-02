#include "cliente_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include "model/cliente/cliente_model.h"
#include "view/cliente/cliente_view.h" 
#include "utils/utils.h"

// Auxiliar para evitar IDs duplicados
static int obterProximoIdCliente(Sistema *sistema) {
    int max = 0;
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo > max) max = sistema->lista_clientes[i].codigo;
    }
    return max + 1;
}

void adicionarClienteController(Sistema *sistema) {
    if (sistema->num_clientes == sistema->capacidade_clientes) {
        int nova_capacidade = (sistema->capacidade_clientes == 0) ? 10 : sistema->capacidade_clientes * 2;
        Cliente *temp = realloc(sistema->lista_clientes, nova_capacidade * sizeof(Cliente));
        
        if (!temp) { 
            mensagem_erro(" De memoria!");
            return; 
        }
        sistema->lista_clientes = temp;
        sistema->capacidade_clientes = nova_capacidade;
    }

    Cliente *novo_cliente = &sistema->lista_clientes[sistema->num_clientes];
    novo_cliente->codigo = obterProximoIdCliente(sistema); 

    formulario_novo_cliente(novo_cliente); 

    sistema->num_clientes++;
    salvarClientes(sistema);
    mensagem_sucesso(" Cliente cadastrado");
}

void alterarClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;
    int codigo = pedir_id_cliente("alterar");
    int indice = -1;
    
    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) { indice = i; break; }
    }

    if (indice == -1) { 
        mensagem_erro("Cliente nao encontrado."); 
        return; 
    }

    Cliente *c = &sistema->lista_clientes[indice];
    int opcao;
    
    do {
        opcao = menu_alterar_cliente(c);

        if (opcao != 0) {
            atualizar_campo_cliente(c, opcao);
        } else {
            mensagem_sucesso("Alteracoes concluidas!");
        }
    } while (opcao != 0);
    
    salvarClientes(sistema);
}

void excluirClienteController(Sistema *sistema) {
    listarClientesView(sistema);
    if (sistema->num_clientes == 0) return;

    int codigo = pedir_id_cliente("excluir");

    for (int i = 0; i < sistema->num_clientes; i++) {
        if (sistema->lista_clientes[i].codigo == codigo) {
            
            if (confirmar_exclusao(sistema->lista_clientes[i].razao_social)) {
                sistema->lista_clientes[i] = sistema->lista_clientes[sistema->num_clientes - 1];
                sistema->num_clientes--;
                salvarClientes(sistema);
                
                mensagem_sucesso("cliente excluido!");
            
            } else { 
                mensagem_aviso("cliente nao encontardo!"); 
            }
            return;
        }
    }
    mensagem_erro("Cliente nao encontrado.");
}