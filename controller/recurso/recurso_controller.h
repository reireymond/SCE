// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef RECURSO_CONTROLLER_H
#define RECURSO_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e a lista de recursos

// Funções do controller para gerenciar recursos (equipamentos)
void adicionarRecursoController(Sistema *sistema);
void alterarRecursoController(Sistema *sistema); 
void excluirRecursoController(Sistema *sistema);  

#endif 
