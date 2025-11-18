#ifndef TRANSACAO_CONTROLLER_H
#define TRANSACAO_CONTROLLER_H
#include "model/sistema.h"

void registrarTransacao(Sistema *sistema, Transacao t);
void lancarAquisicaoController(Sistema *sistema);
void gerenciarContasReceberController(Sistema *sistema);
void gerenciarContasPagarController(Sistema *sistema);
void verSaldoCaixaController(Sistema *sistema);

#endif