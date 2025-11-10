// Garante que este arquivo seja incluído apenas uma vez, evitando redefinições
#ifndef CLIENTE_CONTROLLER_H
#define CLIENTE_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e a lista de clientes

// Funções do controller para gerenciar clientes
void adicionarClienteController(Sistema *sistema);
void alterarClienteController(Sistema *sistema);
void excluirClienteController(Sistema *sistema);
#endif
