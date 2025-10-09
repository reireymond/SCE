// Previne a inclusão múltipla deste arquivo de cabeçalho, uma prática de segurança
// para evitar erros de compilação por redefinições de funções.
#ifndef CLIENTE_VIEW_H
#define CLIENTE_VIEW_H

// Inclui a definição da estrutura principal 'Sistema'. As funções da view precisam
// de um ponteiro para 'Sistema' para poderem exibir os dados contidos nela,
// como a lista de clientes.
#include "model/sistema.h"

// Declaração da função que exibe o menu de gerenciamento de clientes.
// Este menu apresenta as opções como Adicionar, Alterar, Listar e Excluir,
// e direciona o fluxo para a função do controller correspondente.
void menuClientesView(Sistema *sistema);

// Declaração da função que exibe a lista formatada de todos os clientes cadastrados.
// Esta função percorre a lista de clientes no sistema e imprime os dados de cada
// um na tela de forma organizada.
void listarClientesView(Sistema *sistema);

// Fim da diretiva de inclusão condicional.
#endif