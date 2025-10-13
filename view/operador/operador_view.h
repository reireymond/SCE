// Previne a inclusão múltipla deste arquivo de cabeçalho, evitando erros
// de compilação por definições duplicadas de funções.
#ifndef OPERADOR_VIEW_H
#define OPERADOR_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem ler e exibir os dados,
// como a lista de operadores.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento de operadores.
// Este menu apresenta ao usuário as opções de Adicionar, Alterar, Listar e Excluir,
// e direciona o fluxo para a função do controller correspondente.
void menuOperadoresView(Sistema *sistema);

// Declaração da função que exibe a lista formatada de todos os operadores cadastrados.
// Esta função percorre a lista de operadores no sistema e imprime os dados
// de cada um na tela de forma organizada.
void listarOperadoresView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif