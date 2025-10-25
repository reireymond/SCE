#ifndef FORNECEDOR_CONTROLLER_H
#define FORNECEDOR_CONTROLLER_H

#include "model/sistema.h"  // Acesso à estrutura principal do sistema

// Adiciona um novo fornecedor ao sistema
void adicionarFornecedorController(Sistema *sistema);

// Altera os dados de um fornecedor existente
void alterarFornecedorController(Sistema *sistema);

// Exclui um fornecedor do sistema
void excluirFornecedorController(Sistema *sistema);

#endif
