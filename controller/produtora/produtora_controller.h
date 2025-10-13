// Previne a inclusão múltipla deste arquivo de cabeçalho. É uma guarda
// para evitar erros de compilação caso este arquivo seja incluído mais de
// uma vez em uma mesma unidade de compilação.
#ifndef PRODUTORA_CONTROLLER_H
#define PRODUTORA_CONTROLLER_H

// Inclui a definição da estrutura principal 'Sistema'. Todas as funções do controller
// da produtora precisam de um ponteiro para 'Sistema' para poder acessar e
// modificar os dados da empresa.
#include "model/sistema.h"

// Declaração da função do controller para adicionar ou cadastrar os dados da produtora.
// Esta função irá orquestrar o processo de coletar os dados da empresa
// e armazená-los na estrutura do sistema.
void adicionarProdutoraController(Sistema *sistema);

// Declaração da função do controller para alterar os dados da produtora já cadastrada.
// Será responsável pela lógica de apresentar os dados atuais e solicitar
// as novas informações para cada campo que o usuário desejar modificar.
void alterarProdutoraController(Sistema *sistema);

// Declaração da função do controller para excluir os dados da produtora.
// Esta função irá gerenciar a confirmação e a remoção completa dos dados
// da empresa do sistema.
void excluirProdutoraController(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif