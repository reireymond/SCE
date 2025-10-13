// Previne a inclusão múltipla deste arquivo de cabeçalho, uma salvaguarda
// contra erros de compilação por definições duplicadas.
#ifndef RECURSO_CONTROLLER_H
#define RECURSO_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções do controller
// precisam de um ponteiro para esta estrutura para acessar e modificar os dados
// da aplicação, como a lista de recursos.
#include "model/sistema.h"

// Declaração da função do controller para adicionar um novo recurso (equipamento).
// Esta função irá orquestrar o processo de coletar os dados do novo recurso
// e adicioná-lo à lista de recursos no sistema.
void adicionarRecursoController(Sistema *sistema);

// Declaração da função do controller para alterar os dados de um recurso existente.
// Esta função lida com a lógica de encontrar o recurso, solicitar as novas
// informações ao usuário e atualizar o registro correspondente.
void alterarRecursoController(Sistema *sistema);

// Declaração da função do controller para excluir um recurso do sistema.
// Ela será responsável por encontrar um recurso na lista e removê-lo.
void excluirRecursoController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif