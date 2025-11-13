// Previne múltiplas inclusões do cabeçalho, evitando redefinições
#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "model/sistema.h" // Para acessar a estrutura 'Sistema' e gerenciar os dados do sistema

// Controller para gerenciar a inicialização e o ciclo de vida dos dados do sistema

void configuracaoInicialController(Sistema *sistema);
void carregarTodosOsDadosController(Sistema *sistema);
void liberarTodaMemoriaController(Sistema *sistema);

#endif
