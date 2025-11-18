// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef TRANSACAO_CONTROLLER_H
#define TRANSACAO_CONTROLLER_H

#include "model/sistema.h"

// Função auxiliar exposta para outros controllers usarem para salvar transações
void registrarTransacao(Sistema *sistema, Transacao nova_transacao);

// Funções do menu de transações
void lancarAquisicaoController(Sistema *sistema);
void gerenciarContasReceberController(Sistema *sistema);
void gerenciarContasPagarController(Sistema *sistema);
void verSaldoCaixaController(Sistema *sistema);

#endif