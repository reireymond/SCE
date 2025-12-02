#ifndef TRANSACAO_VIEW_H
#define TRANSACAO_VIEW_H
#include "model/sistema.h"

void menuTransacoesView(Sistema *sistema);
void formularioRegistrarCompraView(Sistema *sistema);

void listar_contas_filtro(Sistema *sistema, TipoTransacao tipo); 
int pedir_id_pagamento();
void mostrar_saldo(float saldo);
void msg_conta_paga_sucesso();
void msg_sem_saldo(float saldo_atual);

#endif