#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "model/sistema.h"

// Funções para exibir os menus principais e de navegação do sistema.

/**
 * @brief Exibe o menu para o usuário escolher o modo de armazenamento de dados.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void menuEscolherArmazenamentoView(Sistema *sistema);

/**
 * @brief Exibe o menu principal do sistema e controla a navegação inicial.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void menuPrincipalView(Sistema *sistema);

/**
 * @brief Exibe o menu de gestão de dados (cadastros) e controla a navegação
 * para os submódulos de clientes, fornecedores, etc.
 * @param sistema Ponteiro para a estrutura principal do sistema.
 */
void menuGestaoDeDadosView(Sistema *sistema);

#endif // MAIN_VIEW_H
