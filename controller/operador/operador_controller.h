// Evita múltiplas inclusões do arquivo, prevenindo redefinições de funções
#ifndef OPERADOR_CONTROLLER_H
#define OPERADOR_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e a lista de operadores

// Funções do controller para gerenciar operadores
void adicionarOperadorController(Sistema *sistema);
void alterarOperadorController(Sistema *sistema); 
void excluirOperadorController(Sistema *sistema);

#endif 
