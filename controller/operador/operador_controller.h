// Previne a inclusão múltipla deste arquivo de cabeçalho, evitando erros
// de compilação por definições duplicadas de funções.
#ifndef OPERADOR_CONTROLLER_H
#define OPERADOR_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções do controller
// precisam de um ponteiro para esta estrutura para acessar e manipular os dados
// da aplicação, como a lista de operadores.
#include "model/sistema.h"

// Declaração da função do controller para adicionar um novo operador.
// Esta função irá gerenciar o processo de coletar os dados do novo operador
// e inseri-lo na lista de operadores do sistema.
void adicionarOperadorController(Sistema *sistema);

// Declaração da função do controller para alterar os dados de um operador existente.
// Responsável pela lógica de selecionar um operador, solicitar as novas informações
// e atualizar o registro correspondente.
void alterarOperadorController(Sistema *sistema);

// Declaração da função do controller para excluir um operador.
// Gerencia a localização do operador na lista e sua remoção.
void excluirOperadorController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif