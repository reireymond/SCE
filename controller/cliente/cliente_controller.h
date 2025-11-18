// Impede de incluir o arquivo duas vezes
#ifndef CLIENTE_CONTROLLER_H
#define CLIENTE_CONTROLLER_H

#include "model/sistema.h"

// Funcoes pra gerenciar clientes
void adicionarClienteController(Sistema *sistema);
void alterarClienteController(Sistema *sistema);
void excluirClienteController(Sistema *sistema);
#endif
