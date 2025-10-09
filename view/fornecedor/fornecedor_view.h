// Previne a inclusão múltipla deste arquivo de cabeçalho, uma salvaguarda
// para evitar erros de compilação por definições duplicadas de funções.
#ifndef FORNECEDOR_VIEW_H
#define FORNECEDOR_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem exibir os dados contidos nela,
// como a lista de fornecedores.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento de fornecedores.
// Este menu apresenta ao usuário as opções como Adicionar, Alterar, Listar e Excluir,
// e direciona o fluxo para a função do controller correspondente.
void menuFornecedoresView(Sistema *sistema);

// Declaração da função que exibe a lista formatada de todos os fornecedores cadastrados.
// Esta função percorre a lista de fornecedores no sistema e imprime os dados de cada
// um na tela de forma organizada.
void listarFornecedoresView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif