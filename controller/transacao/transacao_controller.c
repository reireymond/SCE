#include "transacao_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include "model/transacao/transacao_model.h"
#include "view/transacao/transacao_view.h"

#include "view/cliente/cliente_view.h"
void registrarTransacao(Sistema *sistema, Transacao t) {
    if (sistema->num_transacoes == sistema->capacidade_transacoes) {
        int nova = (sistema->capacidade_transacoes == 0) ? 10 : sistema->capacidade_transacoes * 2;
        Transacao *temp = realloc(sistema->lista_transacoes, nova * sizeof(Transacao));
        
        if (temp == NULL) {
            mensagem_erro("Erro de alocacao de memoria para transacoes!");
            return;
        }
        sistema->lista_transacoes = temp;
        sistema->capacidade_transacoes = nova;
    }
    t.codigo = sistema->num_transacoes + 1;

    sistema->lista_transacoes[sistema->num_transacoes] = t;
    sistema->num_transacoes++;
    salvarTransacoes(sistema);
}

void gerenciarContasReceberController(Sistema *sistema) {

    listar_contas_filtro(sistema, CONTA_A_RECEBER);
    
    int cod = pedir_id_pagamento();
    if(cod == 0) return; 

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
        
            if(sistema->lista_transacoes[i].tipo != CONTA_A_RECEBER) {
                mensagem_erro("Este ID nao pertence a uma conta a receber.");
                return;
            }
            if(sistema->lista_transacoes[i].status == PAGA) {
                mensagem_aviso("Esta conta ja foi paga anteriormente.");
                return;
            }
            sistema->lista_transacoes[i].status = PAGA;
            sistema->saldo_caixa += sistema->lista_transacoes[i].valor;

            salvarTransacoes(sistema);
            mensagem_sucesso("Recebimento registrado! Caixa atualizado.");
            return;
        }
    }
    mensagem_erro("Conta nao encontrada.");
}

void gerenciarContasPagarController(Sistema *sistema) {
    listar_contas_filtro(sistema, CONTA_A_PAGAR);

    int cod = pedir_id_pagamento();
    if(cod == 0) return;

    for(int i=0; i < sistema->num_transacoes; i++) {
        if(sistema->lista_transacoes[i].codigo == cod) {
            
           
            if(sistema->lista_transacoes[i].tipo != CONTA_A_PAGAR) {
                mensagem_erro("Este ID nao pertence a uma conta a pagar.");
                return;
            }
             if(sistema->lista_transacoes[i].status == PAGA) {
                mensagem_aviso("Esta conta ja foi paga anteriormente.");
                return;
            }
            
            // Verifica se tem dinheiro no caixa
            if(sistema->saldo_caixa >= sistema->lista_transacoes[i].valor) {
                sistema->lista_transacoes[i].status = PAGA;
                sistema->saldo_caixa -= sistema->lista_transacoes[i].valor;
                
                salvarTransacoes(sistema);
                mensagem_sucesso("Pagamento realizado! Valor descontado do caixa.");
            } else {
                msg_sem_saldo(sistema->saldo_caixa);
            }
            return;
        }
    }
    mensagem_erro("Conta nao encontrada.");
}

void verSaldoCaixaController(Sistema *sistema) {
    //para exibir o dado
    mostrar_saldo(sistema->saldo_caixa);
}