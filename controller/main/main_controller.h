#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "model/sistema.h"

// Funções para a configuração inicial e o ciclo de vida dos dados do sistema.

/**
 * @brief Exibe o menu para o usuário escolher o modo de armazenamento 
 * e configura o sistema de acordo.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void configuracaoInicialController(Sistema *sistema);

/**
 * @brief Carrega todos os dados dos arquivos de persistência para a memória,
 * de acordo com o modo de armazenamento configurado.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void carregarTodosOsDadosController(Sistema *sistema);

/**
 * @brief Libera toda a memória alocada dinamicamente pelas listas do sistema.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void liberarTodaMemoriaController(Sistema *sistema);

#endif // MAIN_CONTROLLER_H
