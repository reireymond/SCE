#ifndef TRANSACAO_MODEL_H
#define TRANSACAO_MODEL_H
#include "model/sistema.h"
void salvarTransacoes(Sistema *sistema);
void carregarTransacoes(Sistema *sistema);
void liberarMemoriaTransacoes(Sistema *sistema);
void salvarCaixa(Sistema *sistema); // Para salvar o float saldo_caixa
void carregarCaixa(Sistema *sistema); // Para carregar o float saldo_caixa
#endif