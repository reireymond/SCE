// Previne a inclusão múltipla deste arquivo de cabeçalho. Este é um mecanismo
// de guarda para evitar erros de compilação por definições duplicadas.
#ifndef FORNECEDOR_CONTROLLER_H
#define FORNECEDOR_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções do controller
// precisam de um ponteiro para 'Sistema' para poderem acessar e modificar os dados
// da aplicação, como a lista de fornecedores.
#include "model/sistema.h"

// Declaração da função do controller para adicionar um novo fornecedor.
// Esta função irá orquestrar o processo de obter os dados do novo fornecedor
// do usuário e adicioná-lo à lista de fornecedores no sistema.
void adicionarFornecedorController(Sistema *sistema);

// Declaração da função do controller para alterar os dados de um fornecedor existente.
// Esta função lida com a lógica de encontrar o fornecedor, solicitar as novas
// informações ao usuário e atualizar o registro correspondente.
void alterarFornecedorController(Sistema *sistema);

// Declaração da função do controller para excluir um fornecedor do sistema.
// Esta função será responsável por encontrar um fornecedor na lista e removê-lo
// de forma segura.
void excluirFornecedorController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif