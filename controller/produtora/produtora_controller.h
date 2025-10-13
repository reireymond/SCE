// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef PRODUTORA_CONTROLLER_H
#define PRODUTORA_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e os dados da produtora

// Funções do controller para gerenciar a produtora
void adicionarProdutoraController(Sistema *sistema); // Adiciona/cadastra dados da produtora
void alterarProdutoraController(Sistema *sistema);  // Altera dados existentes da produtora
void excluirProdutoraController(Sistema *sistema);  // Exclui os dados da produtora

#endif // PRODUTORA_CONTROLLER_H
