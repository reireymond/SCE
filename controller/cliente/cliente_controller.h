// Garante que este arquivo de cabeçalho seja incluído apenas uma vez
// durante a compilação de um arquivo, evitando erros de redefinição de funções.
#ifndef CLIENTE_CONTROLLER_H
#define CLIENTE_CONTROLLER_H

// Inclui a definição da estrutura principal do sistema. Todas as funções do controller
// precisam de um ponteiro para a estrutura 'Sistema' para acessar e modificar
// os dados da aplicação (como a lista de clientes).
#include "model/sistema.h"

// Declaração da função do controller para adicionar um novo cliente.
// Esta função irá orquestrar o processo de coletar os dados do novo cliente
// e adicioná-lo à lista de clientes no sistema.
void adicionarClienteController(Sistema *sistema);

// Declaração da função do controller para alterar os dados de um cliente existente.
// Esta função irá lidar com a lógica de encontrar o cliente, solicitar as novas
// informações e atualizar o registro correspondente.
void alterarClienteController(Sistema *sistema);

// Declaração da função do controller para excluir um cliente.
// Esta função será responsável por encontrar um cliente na lista e removê-lo.
void excluirClienteController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif