// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef RECURSO_CONTROLLER_H
#define RECURSO_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e a lista de recursos

// Funções do controller para gerenciar recursos (equipamentos)
void adicionarRecursoController(Sistema *sistema); // Adiciona um novo recurso
void alterarRecursoController(Sistema *sistema);  // Altera dados de um recurso existente
void excluirRecursoController(Sistema *sistema);  // Remove um recurso do sistema

#endif // RECURSO_CONTROLLER_H
